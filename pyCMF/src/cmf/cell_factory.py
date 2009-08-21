from shapely.geometry import Polygon,LineString,Point
import cmf_core as cmf
from raster import Raster
import maps
import time
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
            
def cells_from_polygons(project,features,shape_callable=lambda feat:feat.shape,id_callable=lambda f:0,center_callable=lambda feat:tuple(feat.shape.centroid),area_callable=lambda feat:feat.shape.area,no_connect=False):
    """ Adds cells from shapely features to project, and connects them topological
    project: a cmf project
    features: a sequence holding or referencing the shapely features
    shape_callable: a callable (e.g. a lambda function) returning the shapely geometry from a feature in features. Default: lambda feature:feature.shape (suitable for shapefiles)
    center_callable: a callable returning the center of the cell from a feature as a three-tuple. Default: lambda feature:tuple(feature.shape.centroid)
    area_callable: a callable returning the area of the cell from a feature: Default: lambda feature:feature.shape.area
    
    Returns: dictionary with the features as keys and the cells as values
    """
    pred=get_predicate(0.0, shape_callable)
    cells=[]
    cell_dict={}
    q_tree=maps.simple_quad_tree()
    for f in features:
        center=center_callable(f)
        assert len(center)>2
        area=area_callable(f)
        c=project.NewCell(center[0],center[1],center[2],area)
        c.Id=id_callable(f)
        cells.append(c)
        geometry[c]=shape_callable(f)
        cell_dict[f]=c
        q_tree.add_object(f,f.shape.bounds)
    print "%i cells created" % len(cells)
    print "No. of connected cells:",
    report_at=[1,5,10,50,100,500,1000,5000,10000,50000,100000,500000]
    start=time.clock()
    if not no_connect:
        con_count=0
        cmp_count=0
        for i,s in enumerate(features):
            if i in report_at:
                print i,
            candidates=q_tree.get_objects(shape_callable(s).bounds)
            cmp_count+=len(candidates)
            for c in candidates:
                if not s is c and pred(s,c):
                    shp_s=shape_callable(s)
                    shp_cmp=shape_callable(c)
                    intersect=shp_s.intersection(shp_cmp).length
                    if intersect:
                        cell_dict[s].topology.AddNeighbor(cell_dict[c],intersect)
                        con_count+=1
        print len(features),' %0.2f sec. %i comparisons' % (time.clock()-start,cmp_count)
    return cell_dict  

def cells_from_dem(project,dem):
    """ Adds square cells from a dem to the project, and meshes them.
    project : cmf.project, where the cells should be added to
    dem     : a Raster representing the heights of cells
    
    Returns: a dictionary with the tuple (column,row) as key and the cells as values 
    
    The width of the connection between cells is based on an octagon
    straight flowwidth = 1/2*cellsize[direction]
    diagonal flowwidth = 1/sqrt(2)*avg(cellsize)   
    """
    cells_dict={}
    # Create the cells
    for x,y,z,area,c,r in dem.cells:
        cell=project.NewCell(x,y,z,area)
        geometry[cell]=Polygon(((x-dem.cellsize[0]*0.5,y-dem.cellsize[1]*0.5),
                            (x-dem.cellsize[0]*0.5,y+dem.cellsize[1]*0.5),
                            (x+dem.cellsize[0]*0.5,y+dem.cellsize[1]*0.5),
                            (x+dem.cellsize[0]*0.5,y-dem.cellsize[1]*0.5),
                            (x-dem.cellsize[0]*0.5,y-dem.cellsize[1]*0.5)))
        cells_dict[c,r]=cell
    for c,r in cells_dict:
        act_cell=cells_dict[c,r]
        for nc,nr,nv,nd,dirx,diry in dem.neighbors(c, r):
            n_cell=cells_dict[nc,nr]
            if (dirx and diry): # diagonal
                flow_width=0.354*0.5*(dem.cellsize[0]+dem.cellsize[1])
            else:
                flow_width=0.5*(abs(dem.cellsize[0]*dirx)+abs(dem.cellsize[1]*diry))
            act_cell.topology.AddNeighbor(n_cell,flow_width)
    return cells_dict
        
    
        
        
        
            
        
                               
    