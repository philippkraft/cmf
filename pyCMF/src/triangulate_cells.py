# -*- coding=utf-8 -*-
import CMFLib as cmf
import numpy as np
import triangulate as tr
def CreateSquaredCells(dem,vegMap,soilMap,soilDepthMap):
    """ Creates upslope cells from a raster with one cell per pixel """
    print "Create cells from raster"
    factory=cmf.VerticalHomogeneousCellFactory()
    upcVec=factory.RasterCellsToUpslopeVector(dem,vegMap,soilMap,soilDepthMap)
    print "%i Upslope cells created" % len(upcVec)
    __CorrectCells(upcVec)
    return upcVec
def FindImportantPoints(dem=None,max_error=None,fraction_of_points=None,max_point_number=None):
    """ Finds the important points of a dem and triangulates them """
    if not type(dem) in [cmf.DoubleRaster,cmf.SingleRaster,cmf.IntRaster]: raise RuntimeError("dem must be a cmf raster")
    # Get the difference between a focal mean with a 3x3 Matrix and the original 
    dem_error=dem.focal_mean_difference()
    # Get the 1-fraftion_of_points percentile
    dem_error_hist=dem_error.histogram()
    if max_error:
        error_threshold=max_error
    elif max_point_number:
        error_threshold=dem_error_hist.quantile(1-max_error/dem_error_hist.sum())
    elif fraction_of_points:
        error_threshold=dem_error_hist.quantile(1-max_error)
    else:
        raise RuntimeError("One value of max_error, fraction_of_points,max_point_number has to be set")
    loc=[]
    for r in range(dem.RowCount()):
        for c in range(dem.ColumnCount()):
            if dem_error.IdentifyColRow(c,r)>error_threshold:
                loc.append((dem.GetXPosition(c),dem.GetYPosition(r),dem.IdentifyColRow(c,r)))
    return np.array(loc)    
def __makeNodes(dem,fraction_of_points=0.05):
    """ Finds the important points of a dem and trriangulates them """
    loc=FindImportantPoints(dem, fraction_of_points)
    tri=tr.triangulator(loc[:,0],loc[:,1],loc[:,2])
    return tri.nodes,tri.triangles
def CreateTriangleCells(dem,vegMap,soilMap,soilDepthMap,fraction_of_points=0.05):
    print "Create triangulation"
    nodes,triangles=__makeNodes(dem, fraction_of_points)
    print "%i Nodes and %i triangles created" % (len(nodes),len(triangles))
    print
    print "Create cells from triangles"
    upcVec=cmf.UpslopeVector()
    cells={}
    for t in tri.triangles:
        x,y,z=t.centroid()
        if soilMap.Has(x,y) and vegMap.Has(x,y):
            c=cmf.VerticalHomogeneousCell(x,y,z,t.area(),soilMap.Get(x,y),vegMap.Get(x,y))
            c.Soildepth(soilDepthMap.Get(x,y))
        else:
            c=cmf.SolidRockCell(x,y,z,t.area(),vegMap.Get(x,y))
        upcVec.append(c)
        c.thisown=0
        cells[t]=c
    for t in cells:
        if t in cells:
            c=cells[t]
            for n in t.neighbors:
                if n.target in cells:
                    c.AddNeighbor(cells[n.target],n.width)
    del cells
    print "%i Upslope cells created" % len(upcVec)
    __CorrectCells(upcVec)
    return upcVec
def CreateVoronoiCells(dem,vegMap,soilMap,soilDepthMap,fraction_of_points=0.05):
    print "Create triangulation"
    nodes,triangles=__makeNodes(dem, fraction_of_points)
    print "%i Nodes and %i triangles created" % (len(nodes),len(triangles))
    print
    print "Create cells from triangles"
    upcVec=cmf.UpslopeVector()
    cells={}
    geometry={}
    
    # Create for each node a cell
    for n in nodes:
        # get the cell geometry
        vor=n.voronoi() # As array
        voronoi=cmf.Ring()
        for p in vor:
            voronoi.AddPoint(p[0],p[1])
        area=voronoi.Area()             # Area of cell
        if area>0 :
            x,y,z=n.x,n.y,n.z           # Position of cell center (node)
            # create the cell (either vertical homogeneous or solid rock)
            if soilMap.Has(x,y) and vegMap.Has(x,y):
                c=cmf.VerticalHomogeneousCell(x,y,z,area,soilMap.Get(x,y),vegMap.Get(x,y))
                c.Soildepth(soilDepthMap.Get(x,y))
            else:
                c=cmf.SolidRockCell(x,y,z,area,vegMap.Get(x,y))
            # put cell into vector
            upcVec.append(c)
            # Make cell Python persistant
            c.thisown=0
            # Dictionary to find cell at node
            cells[n]=c
            # Dictionary to out geometry out
            geometry[c]=voronoi
    
    # Connect the cells by the edges
    for n in cells:
        n.sortneighbors(nodes)
        # For each edge of the triangulation add a neighbor connection
        for e in n.edges():
            try :
                cells[n].AddNeighbor(cells[e.end],e.flowwidth)
            except: pass    
    del cells
    print "%i Upslope cells created" % len(upcVec)
    __CorrectCells(upcVec)
    return upcVec,geometry

def __CorrectCells(cells):
    cells.FillSinks()
    cells.CalcFlowAccumulation()

def CreateIrregularCells(polygons,centers,cellbuilder,slither_tolerance=0.1):
    """ Creates cells from irregular polygons
    polygons : Sequence of cmf.Ring
    heights  : Sequence of floats, representing the heights of the polygons
    cellbuilder : a cmf.CellB
    slither_tolerance : Maximum distance between vertices that are treated as one shared vertex 
    """
    cells={}
    upcVec=cmf.UpslopeVector()
    for i,p in enumerate(polygons):
        x,y,z=centers[i]
        cell=cellbuilder(float(x),float(y),float(z),p.Area())
        cell.thisown=0
        cells[p]=cell
        upcVec.append(cell)
    for p in polygons:
        for np in polygons:
            if p!=np:
                l=p.SharedBoundary(np).MaxStraight()
                if l.VertexCount()>0:
                    dist=l[0].distanceTo(l[-1])
                    if dist>slither_tolerance:           
                        c=cells[p]
                        nc=cells[np]
                        c.AddNeighbor(nc,dist)
    #__CorrectCells(upcVec)
    return upcVec
def next_in_boundary(cell,azimuthToLast):
    minanglediff=361.
    nextCell=None
    for j in range(cell.NeighborCount()):
        nc=cell.GetNeighbor(j).Cell
        angleDiff=cell.Center().azimuth(nc.Center())-azimuthToLast
        if angleDiff<=0: angleDiff+=360
        if angleDiff<minanglediff:
            minanglediff=angleDiff
            nextCell=nc
    return cell,nextCell
    
           
