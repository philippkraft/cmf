from shapely.geometry import Polygon,MultiPolygon,Point,LineString,MultiLineString,MultiPoint,GeometryCollection
import pylab
import numpy
from cmf.cell_factory import geometry as geoms
import cmf.cell_factory

def drawcells(cells,colorfunction=lambda c:c.z,cmap=pylab.cm.jet,hold=True,vmin=None,vmax=None,return_polygons=False,**kwargs):
    was_interactive=pylab.isinteractive()
    if was_interactive: pylab.ioff()
    geos=[]
    maxvalue=-1e300 if vmax is None else vmax
    minvalue=1e300 if vmin is None else vmin
    polygons=[]
    for cell in cells:
        shape=geoms[cell]
        if hasattr(shape, "geoms"):
            shapes=shape.geoms
        else:
            shapes=[shape]
        value=colorfunction(cell)
        if vmax is None:
            maxvalue=max(value,maxvalue)
        if vmin is None:
            minvalue=min(value,minvalue)
        for s in shapes:
            geos.append((cell,s,value))
    if minvalue>=maxvalue: minvalue=maxvalue-1
    for cell,s,v in geos:
        if isinstance(s, Polygon):
            s=s.exterior
        c=cmap(float(v-minvalue)/float(maxvalue-minvalue))
        a=pylab.asarray(s)
        poly=pylab.fill(a[:,0],a[:,1],fc=c,hold=hold,**kwargs)[0]
        if return_polygons:
            polygons.append((cell,poly))
        hold=1
    pylab.axis('scaled')
    if was_interactive:
        pylab.draw() 
        pylab.ion()
    return polygons,minvalue,maxvalue
def recolor_polygons(polygons,vrange,colorfunction=lambda c:c.z,cmap=pylab.cm.jet):
    vmin,vmax=vrange
    for cell,poly in polygons:
        c=cmap((colorfunction(cell)-vmin)/(vmax-vmin))
        poly.set_fc(c)
    if pylab.isinteractive():
        pylab.draw()   
def drawobjects(objects,style=None,hold=1,**kwargs):
    was_interactive=pylab.isinteractive()
    if was_interactive: pylab.ioff()
    for i,o in enumerate(objects):
        shape=geoms.get(o)
        if hasattr(shape, "geoms"):
            shapes=list(shape.geoms)
        else:
            shapes=[shape]
        for s in shapes:
            if isinstance(s, Polygon):
                s=s.exterior
            a=pylab.array(s.coords)
            if style is None:
                pylab.plot(a[:,0],a[:,1],hold=hold,**kwargs)
            else:
                pylab.plot(a[:,0],a[:,1],style,hold=hold,**kwargs)
            hold=1
    pylab.axis('scaled')
    if was_interactive:
        pylab.draw() 
        pylab.ion()

   
def cell_to_cell_fluxes(cells,t,min_d=-1,max_d=1e300,**kwargs):
    f=cmf.cell_to_cell_fluxes(t,cells,min_d,max_d)
    pylab.quiver(f.X,f.Y,f.dX*f.V,f.dY*f.V,hold=1,scale=f.V.max(),units='x',zorder=10,**kwargs)

def plot_timeseries(data,style='-',**kwargs):    
        pylab.plot_date(map(lambda t:(t-cmf.Time(1,1,1)).AsDays(),cmf.timerange(data.begin(),data.end(),data.step())),list(data),style,**kwargs)
def plot_locatables(locatables,style='kx',**kwargs):
    get_x=lambda l:l.position.x
    get_y=lambda l:l.position.y
    pylab.plot(pylab.amap(get_x,locatables),pylab.amap(get_y,locatables),style,**kwargs)
def connector_matrix(cells,size=(500,500)):
    """Returns a matrixx
    """
    allstates=[]
    for c in cells:
        allstates.extend(c.storages)
    posdict={}
    jac=pylab.zeros(size,dtype=int)
    l=len(allstates)
    for i,a in enumerate( allstates):
        posdict[a.node_id]=i
    for i,a in enumerate( allstates):
        for f,t in a.fluxes(cmf.Time()):
            j=posdict.get(t.node_id)
            if j:
                jac[i*size[0]/l,j*size[1]/l]+=1
    return jac   
        
def contour_raster(raster,**kwargs):
    Z=raster.as_array()
    Z=numpy.flipud(Z)
    extent=(raster.llcorner[0],raster.llcorner[0]+raster.extent[0],raster.llcorner[1],raster.llcorner[1]+raster.extent[1])
    C=pylab.contour(Z,extent=extent,**kwargs)
    pylab.clabel(C)
    pylab.axis('scaled')
def contourf_raster(raster,**kwargs):
    Z=raster.as_array()
    Z=numpy.flipud(Z)
    extent=(raster.llcorner[0],raster.llcorner[0]+raster.extent[0],raster.llcorner[1],raster.llcorner[1]+raster.extent[1])
    C=pylab.contourf(Z,extent=extent,**kwargs)
    pylab.clabel(C)
    pylab.axis('scaled')
    
               
