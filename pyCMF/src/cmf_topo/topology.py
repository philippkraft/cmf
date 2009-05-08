from shapely.geometry import Polygon,LineString,Point
import cmf
from raster import Raster
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
def create_reaches(project,features,outlets,cell_callable,
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
    # Cycle through all features
    for f in features:
        # Get the cell of the reach
        cell=cell_callable(f)
        if not cell:
            continue # Next feature if no cell is found
        # Add a reach to the cell
        r=cell.AddReach(length_callable(f),type_callable(f),depth_callable(f),width_callable(f))
        reaches[f]=r
    print len(reaches),'reaches created, proceed with connecting...'
    # Create the downstream dictionary
    downstreams=get_downstream(features, z_callable, shape_callable, tolerance)
    root_reaches=[]
    for f in features:        
        r=reaches[f]
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
            for o in outlets:
                p=Point(tuple(o.Location))
                if get_predicate(tolerance, shape_callable)(f,p):
                    r.set_outlet(o)
                    print "A reach is connected to outlet",o.Name
        # if no outlet is found, the reach is a dead end, only connected downwards with the surface water of its cell
    return root_reaches
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
            
def cells_from_polygons(project,features,shape_callable=lambda feat:feat.shape,center_callable=lambda feat:tuple(feat.shape.centroid),area_callable=lambda feat:feat.shape.area):
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
    for f in features:
        center=center_callable(f)
        assert len(center)>2
        area=area_callable(f)
        c=project.NewCell(center[0],center[1],center[2],area)
        cells.append(c)
        cell_dict[f]=c
    print "%i cells created" % len(cells)
    print "No. of connected cells:",
    report_at=[1,5,10,50,100,500,1000,5000,10000,50000,100000,500000]
    for i,s in enumerate(features):
        if i in report_at:
            print i,
        for j in range(i+1,len(features)):
            s_cmp=features[j]
            if pred(s,s_cmp):
                shp_s=shape_callable(s)
                shp_cmp=shape_callable(s_cmp)
                intersect=shp_s.intersection(shp_cmp).length
                if intersect:
                    cells[i].topology.AddNeighbor(cells[j],intersect)
                    cells[j].topology.AddNeighbor(cells[i],intersect)
    print
    return cell_dict  

def cells_from_raster(project,raster):
    cells_dict={}
    self.raster=Raster('bla')
    for r in self.ra
        pass
    