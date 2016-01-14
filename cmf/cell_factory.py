

# Copyright 2010 by Philipp Kraft
# This file is part of cmf.
#
#   cmf is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 2 of the License, or
#   (at your option) any later version.
#
#   cmf is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
#
import cmf_core as cmf
import maps
import time
from math import *
class Geometry:
    def __init__(self):
        self.__dict={}
    def __getitem__(self,o):
        try:
            return self.__dict[hash(o)][1]
        except KeyError:
            raise KeyError("No geometry stored for %s" % o)
    def __setitem__(self,key,value):
        self.__dict[hash(key)]=(key,value)
    def itervalues(self):
        for v in self.__dict.itervalues():
            yield v[1]
    def iterkeys(self):
        for v in self.__dict.itervalues():
            yield v[0]   
    def __iter__(self):
        return self.iterkeys()
    def keys(self):
        return list(self.iterkeys())
    def values(self):
        return list(self.itervalues())
    def iteritems(self):
        for v in self.__dict.itervalues():
            return v
    def items(self):
        return list(self.iteritems())
    def __repr__(self):
        return "Geometry holder for cmf objects"
    def __len__(self):
        return len(self.__dict)
    def get(self,key):
        item= self.__dict.get(hash(key))
        if item is None:
            return None
        else: return item[1]
        
geometry=Geometry()

class cell_tree(object):
    def __addbranch(self,cell):
        self.__data[cell]=[]
        for n,w in cell.neighbors:
            if n.topology.MainOutlet() == cell:
                self.__data[cell].append(n)
                self.__addbranch(n)
    def __init__(self,start_cell):
        self.start=start_cell
        self.__data={}
        self.__addbranch(start_cell)
    def __getitem__(self,cell):
        return self.__data[cell]
    def __len__(self):
        return len(self.__data)
    def __iter__(self):
        cells=[self.start]
        while cells:
            c=cells.pop(0)
            cells.extend(self[c])
            yield c
                        

def downstream_recursive(unhandled,pred,downstreams,this,order):
    try:
        unhandled.remove(this)
    except ValueError,e:
        pass
    maxorder=order
    upstream=[c for c in unhandled if pred(this,c)]
    for u in upstream:
        downstreams[u]=this
        maxorder=max(downstream_recursive(unhandled, pred, downstreams, u,order+1),maxorder)
    return maxorder
def get_predicate(tolerance,as_shape):
    if tolerance:
        return lambda shp1,shp2:as_shape(shp1).distance(as_shape(shp2))<tolerance
    else:
        return lambda shp1,shp2:as_shape(shp1).intersects(as_shape(shp2))    
def get_downstream(features,z_callable=lambda feat:feat.z,shape_callable=lambda feat:feat.shape,tolerance=0):
    """
    Creates the topological relationship of reaches from a sequence of geo features.
    The z_callable and the shape callable must be function or function like objects returning the height resp. a shapely geometry 
    tolerance allows to specify the maximum distance (in mapping units) between two features to count as connected.  
    
    Returns a dictionary of the features, mapping to the downstream feature
    """
    downstreams={} 
    # Copy the features
    unhandled=list(features)
    # Make a comparison operator to sort the parts by height
    z=z_callable
    z_cmp=lambda rec1,rec2:1 if z(rec1)>z(rec2) else (-1 if z(rec1)<z(rec2) else 0)
    unhandled.sort(z_cmp)
    print "z: %f - %f" % (z(unhandled[0]),z(unhandled[-1])) 
    # Get the tolerance predicate
    pred=get_predicate(tolerance,shape_callable)
    # as long as unhandled parts exist
    i=0
    while unhandled:
        this=unhandled[0]
        i+=1
        print 'Start (height)', z(this), 
        print downstream_recursive(unhandled, pred, downstreams, this,0), ' sub reaches'
    return downstreams                 
            
def create_reaches_preintersect(project,features,outlets,cell_callable,
                   shape_callable=lambda feat:feat.shape,
                   length_callable=None, 
                   width_callable=None,width=1.0,
                   depth_callable=None,depth=0.25,
                   type_callable=None,type='T',                   
                   tolerance=0):
    """ Creates reaches for a project containing cells
    project: the cmf project
    features: a sequence holding or referencing the shapely features
    outlets: a sequence holding catchment outlets
    cell_callable: a callable finding the cell for the reach with a feature.
    z_callable: a callable returning the mean height of a reach from a feature.
    shape_callable: a callable returning a shapely geometry from a feature. 
                    Default: lambda feature:feature.shape
    width_callable: a callable returning the width of the reach described by a feature. 
                    If None (default), the constant value width (default 1.0) is used 
    depth_callable: a callable returning the depth of the reach described by a feature. 
                    If None (default), the constant value depth (default 0.25) is used
    type_callable: a callable returning the type of reach geometry used. 
                   If None (default), the constant value type (default: 'T') is used.
                   The type is described by a single character:
                   'T' (Triangular),'R' (Rectangular), 'S' (SWAT like reach), 'P' (Pipe) 
    tolerance: Maximum distance between two reaches to be considered as connected. Default: 0.0   
    """
    if length_callable is None:
        length_callable=lambda feature:shape_callable(feature).length
    if width_callable is None:
        width_callable=lambda feature:width
    if depth_callable is None:
        depth_callable=lambda feature:depth
    if type_callable is None:
        type_callable=lambda feature:type 
    z_callable=lambda feature:cell_callable(feature).z       
    reaches={}
    cells={}
    # Cycle through all features, to create the reaches
    for f in features:
        # Get the cell of the reach
        cell=cell_callable(f)
        if cell:
            cells[f]=cell
            # Add a reach to the cell
            try:
                r=cell.add_reach(length_callable(f),type_callable(f),depth_callable(f),width_callable(f))
                reaches[f]=r
                geometry[r]=shape_callable(f)
            except NotImplementedError:
                raise RuntimeError("Problem creating reach with l=%s,t=%s,d=%s,w=%s" % (length_callable(f),type_callable(f),depth_callable(f),width_callable(f)))
    print len(reaches),'reaches created, proceed with connecting...'
    
    # Connect the reaches
    
    # Create the downstream dictionary
    downstreams=get_downstream(cells.keys(), z_callable, shape_callable, tolerance)
    root_reaches=[]
    for f in features:        
        r=reaches.get(f)
        if r:
            # If a downstream exists for this feature, use it
            if f in downstreams:
                f_down=downstreams[f]
                try:
                    r_down=reaches[f_down]
                except KeyError,e:
                    print "KeyError, reach does not exist yet"
                r.set_downstream(reaches[downstreams[f]])
            # Else look for an outlet at the reach
            else:
                root_reaches.append(r)
                outlet_connection=False
                for o in outlets:
                    p=Point(tuple(o.position))
                    if p.distance(shape_callable(f))<=tolerance:
                        r.set_outlet(o)
                        print "A reach at %s is connected to outlet %s", (r.cell,o.Name)
                        outlet_connection=True
                if not outlet_connection:
                    print "found dead end reach, water flows to surface water of %s" % r.cell
    return root_reaches

def create_reaches(project,outlets,features,
                         cell_shape_callable=lambda cell:cell.geometry,
                         shape_callable=lambda feat:feat.shape,
                         width_callable=None,width=1.0,
                         depth_callable=None,depth=0.25,
                         type_callable=None,type='T',                   
                         tolerance=0):
    """ Creates reaches for a project containing cells
    project: the cmf project
    features: a sequence holding or referencing the shapely features
    outlets: a sequence holding catchment outlets
    cell_shape_callable: a callable finding the geometry for a cell in project. If the 
            cells where created by cells_from_polygon or cells_from_dem the default is ok
    shape_callable: a callable returning a shapely geometry from a feature. 
                    Default: lambda feature:feature.shape
    width_callable: a callable returning the width of the reach described by a feature. 
                    If None (default), the constant value width (default 1.0) is used.
                    If the feature has the attribute width, use: lambda f:f.width 
    depth_callable: a callable returning the depth of the reach described by a feature. 
                    If None (default), the constant value depth (default 0.25) is used
                    If the feature has the attribute depth, use: lambda f:f.depth 
    type_callable: a callable returning the type of reach geometry used. 
                   If None (default), the constant value type (default: 'T') is used.
                   The type is described by a single character:
                   'T' (Triangular),'R' (Rectangular), 'S' (SWAT like reach), 'P' (Pipe) 
    tolerance: Maximum distance between two reaches to be considered as connected. Default: 0.0   
    """

    reaches=[]
    if width_callable is None:
        width_callable=lambda feature:width
    if depth_callable is None:
        depth_callable=lambda feature:depth
    if type_callable is None:
        type_callable=lambda feature:type 
    for cell in project:
        if hasattr(cell,"geometry"):
            cell_geo = cell_shape_callable(cell)
            for f in features:
                r_geo=shape_callable(f)
                if r_geo.intersects(cell_geo):
                    cr_geo=r_geo.intersection(cell_geo)
                    reaches.append((cr_geo,cell,cr_geo.length,width_callable(f),depth_callable(f),type_callable(f)))
    return create_reaches_preintersect(project, reaches, outlets, 
                          cell_callable=lambda f:f[1],     
                          shape_callable=lambda f:f[0], 
                          length_callable=lambda f:f[2],  
                          width_callable=lambda f:f[3], 
                          depth_callable=lambda f:f[4],
                          type_callable=lambda f:f[5], 
                          tolerance=tolerance)
 


def __create_reach_for_cell(reaches,outlets,downstream_reach,cell,cells,reach_geometry,subsurface_connection_type,width,depth,diffusive):
    """for internal use only, do not invoke"""
    assert(cell in cells)
    # Get cells upstream of current cell
    in_cells = [n for n,w in cell.neighbors if n.main_outlet == cell and n in cells]
    # downstream length is half of the distance to the downstream reach
    length = cmf.distance(downstream_reach.position,cell) * 0.5 if downstream_reach else 0.5*sqrt(cell.area)
    # upstream length of the reach is half of sum of distance to upstream cells
    length += 0.5 * sum(cmf.distance(cell,in_cell) for in_cell in in_cells)
    # Create the reach using the calculated length and the given parameters
    r = cell.project.NewReach(cell.x,cell.y,cell.z-depth,length,reach_geometry,width,depth,diffusive)
    r.Name = "~~ C[%i]" % cell.Id
    # Connect the reach to the cell
    r.connect_to_cell(cell,width,subsurface_connection_type,diffusive)
    # Set the downstream
    if downstream_reach:
        r.set_downstream(downstream_reach)
    else:
        outlet = cell.project.NewOutlet("Outlet of %s" % r.Name, r.position.x,r.position.y,r.position.z-0.1)
        r.set_outlet(outlet)
        outlets.append(outlet)
    # Remove cell from task set
    cells.remove(cell)
    # Append reach to the results
    reaches.append(r)
    # Create reaches for each upstream cell
    for c in in_cells:
        __create_reach_for_cell(reaches,outlets,r,c,cells,reach_geometry,subsurface_connection_type,width,depth,diffusive)
def create_reaches_for_cells(cells_with_reach,reach_geometry='T',width=1.0,depth=0.25,subsurface_connection_type=None,diffusive=None):
    """Creates reaches for a sequence of cells.
    """
    assert(subsurface_connection_type is None or issubclass(subsurface_connection_type, cmf.lateral_sub_surface_flux))
    cells = set(cells_with_reach)
    reaches=[]
    outlets=[]
    while cells:
        cur = min(cells,key = lambda c: c.z)
        __create_reach_for_cell(reaches,outlets,None,cur,cells,reach_geometry,subsurface_connection_type,width,depth,diffusive)
    return cmf.node_list(reaches),cmf.node_list(outlets)
def cell_neighbors(features,shape_callable=lambda feat:feat.shape):
    res={}
    pred=get_predicate(0.0,shape_callable)
    for f in features:
        res[f]=[]
    for i,s in enumerate(features):
        if i%10 == 0: print i,
        for j in range(i+1,len(features)):
            s_cmp=features[j]
            if pred(s,s_cmp):
                shp_s=shape_callable(s)
                shp_cmp=shape_callable(s_cmp)
                intersect=shp_s.intersection(shp_cmp).length
                if intersect:
                    res[s].append((s_cmp,intersect))
                    res[s_cmp].append((s,intersect))
    return res
            
def cells_from_polygons(project,features,
                        shape_callable=lambda feat:feat.shape,
                        id_callable=lambda f:0,
                        center_callable=lambda feat:tuple(feat.shape.centroid),
                        area_callable=lambda feat:feat.shape.area,
                        no_connect=False,
                        report=1,
                        minintersectlength=0.0):
    """ Adds cells from shapely features to project, and connects them topological
    project: a cmf project
    features: a sequence holding or referencing the shapely features
    shape_callable: a callable (e.g. a lambda function) returning the shapely geometry from a feature in features. Default: lambda feature:feature.shape (suitable for shapefiles)
    center_callable: a callable returning the center of the cell from a feature as a three-tuple. Default: lambda feature:tuple(feature.shape.centroid)
    area_callable: a callable returning the area of the cell from a feature: Default: lambda feature:feature.shape.area
    report: if True, report about connecting success
    Returns: dictionary with the features as keys and the cells as values
    """
    pred=get_predicate(0.0, shape_callable)
    cells=[]
    cell_dict={}
    q_tree=maps.simple_quad_tree()
    for i,f in enumerate(features):
        center=center_callable(f)
        assert len(center)>2
        area=area_callable(f)
        c=project.NewCell(center[0],center[1],center[2],area)
        c.Id=id_callable(f)
        cells.append(c)
        geometry[c]=shape_callable(f)
        cell_dict[i]=c
        q_tree.add_object(i,f.shape.bounds)
    print "No. of connected cells:",
    report_at=[100,500,1000,5000,10000,50000,100000,500000]
    start=time.clock()
    if not no_connect:
        con_count=0
        cmp_count=0
        for i,s in enumerate(features):
            if i in report_at:
                print i,
            candidates=q_tree.get_objects(shape_callable(s).bounds)
            cmp_count+=len(candidates)
            for ic in candidates:
                c = features[ic]
                if not s is c and pred(s,c):
                    shp_s=shape_callable(s)
                    shp_cmp=shape_callable(c)
                    intersect=shp_s.intersection(shp_cmp).length
                    if intersect:
                        cell_dict[i].topology.AddNeighbor(cell_dict[ic],intersect)
                        con_count+=1
        print len(features),' %0.2f sec. %i comparisons' % (time.clock()-start,cmp_count)
    #cmf.Topology.calculate_contributing_area(project)
    return cell_dict  
    
def cells_from_dem(project,dem,direction_count=8):
    """ Adds square cells from a dem to the project, and meshes them.
    project         : cmf.project, where the cells should be added to
    dem             : a Raster representing the heights of cells
    direction_count : Number of directions to connect cells
             1: Every cell gets connected with its steepest downward neighbor. 
                Flow width is cellsize for orthogonal and sqrt(2)*cellsize for
                diagonal directions
             4: Every cell gets connected to its 4 direct orthogonal neighbors
                Flow width equals the cellsize
             8: Every cell gets connected to its 8 neighbors
                Flow width is 0.5*cellsize for orthogonal and sqrt(2)/4*cellsize 
                for diagonal neighbors
    Returns: a dictionary with the tuple (column,row) as key and the cells as values 
    
    """
    if not direction_count in (1,4,8):
        raise ValueError("You can only create cells from a dem with " +
                         "1 (connection to downslope) 4 (straight connections)" +
                         " or 8 (all neighbors) directions")
    cells_dict={}
    # Create the cells
    for x,y,z,area,c,r in dem.cells:
        cell=project.NewCell(x,y,z,area)
        cells_dict[c,r]=cell
    cellsize = 0.5 * (dem.cellsize[0] + dem.cellsize[1])
    for c,r in cells_dict:
        act_cell=cells_dict[c,r]
        if direction_count == 8:
            for nc,nr,nv,nd,dirx,diry in dem.neighbors(c, r):
                n_cell=cells_dict[nc,nr]
                if (dirx and diry): # diagonal
                    flow_width=0.354*cellsize
                else:
                    flow_width=0.5*(abs(dem.cellsize[0]*dirx)+abs(dem.cellsize[1]*diry))
                act_cell.topology.AddNeighbor(n_cell,flow_width)
        elif direction_count == 4:
            for nc,nr in ((c-1,r),(c,r+1),(c+1,r),(c,r-1)):
                n_cell=cells_dict.get((nc,nr))
                if n_cell:
                    flow_width=dem.cellsize[int(nc==c)]
                    act_cell.topology.AddNeighbor(n_cell,flow_width)
        elif direction_count == 1:
            # Get the lowest neighbor
            nc,nr,nv,nd,dirx,diry = min(dem.neighbors(c,r),key=lambda t : (t[2]-act_cell.z)/t[3])
            n_cell = cells_dict[nc,nr]
            if act_cell.z>n_cell.z:
                if (dirx and diry): # diagonal
                    flow_width=sqrt(2.)*cellsize
                else:
                    flow_width=dem.cellsize[int(nc==c)]
                act_cell.topology.AddNeighbor(n_cell,flow_width)
    cmf.Topology.calculate_contributing_area(project.cells)
    return cells_dict
        
def project_from_dem(dem,tracer_string='',dir_count=8):
    p = cmf.project(tracer_string)
    cells_from_dem(p,dem,dir_count)
    return p
    
        
        
        
            
        
                               
    