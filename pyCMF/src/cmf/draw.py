from shapely.geometry import Polygon,MultiPolygon,Point,LineString,MultiLineString,MultiPoint,GeometryCollection
import pylab
import numpy
from cmf.cell_factory import geometry as geoms
import cmf.cell_factory

def drawcells(cells,colorfunction=lambda c:c.z,cmap=pylab.cm.jet,hold=True,vmin=None,vmax=None,**kwargs):
    was_interactive=pylab.isinteractive()
    if was_interactive: pylab.ioff()
    geos=[]
    maxvalue=-1e300 if vmax is None else vmax
    minvalue=1e300 if vmin is None else vmin
    
    for c in cells:
        shape=geoms[c]
        if hasattr(shape, "geoms"):
            shapes=shape.geoms
        else:
            shapes=[shape]
        value=colorfunction(c)
        if vmax is None:
            maxvalue=max(value,maxvalue)
        if vmin is None:
            minvalue=min(value,minvalue)
        for s in shapes:
            geos.append((s,value))
    print minvalue,maxvalue
    if minvalue>=maxvalue: minvalue=maxvalue-1
    for s,v in geos:
        if isinstance(s, Polygon):
            s=s.exterior
        c=cmap(float(v-minvalue)/float(maxvalue-minvalue))
        a=pylab.array(s)
        pylab.fill(a[:,0],a[:,1],fc=c,hold=hold,**kwargs)
        hold=1
    pylab.axis('equal')
    if was_interactive:
        pylab.draw() 
        pylab.ion()
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
    pylab.axis('equal')
    if was_interactive:
        pylab.draw() 
        pylab.ion()

   


def plot_timeseries(data,style='-',**kwargs):    
        pylab.plot_date(map(lambda t:(t-cmf.Time(1,1,1)).AsDays(),cmf.timerange(data.begin(),data.end(),data.step())),list(data),style,**kwargs)
    
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
    pylab.contour(Z,extent=(raster.llcorner[0],raster.llcorner[0]+raster.extent[0],raster.llcorner[1],raster.llcorner[1]+raster.extent[1]),**kwargs)

               
        

    

    