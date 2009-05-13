from shapely.geometry import Polygon,MultiPolygon,Point,LineString,MultiLineString,MultiPoint,GeometryCollection
import pylab
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
    if minvalue>=maxvalue: minvalue=maxvalue-1
    for s,v in geos:
        if isinstance(s, Polygon):
            s=s.exterior
        c=cmap((v-minvalue)/(maxvalue-minvalue))
        a=pylab.array(s)
        pylab.fill(a[:,0],a[:,1],fc=c,hold=hold,**kwargs)
        hold=1
    pylab.axis('equal')
    if was_interactive: pylab.show()
def drawobjects(objects,style,hold=1,**kwargs):
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
            pylab.plot(a[:,0],a[:,1],style,hold=hold,**kwargs)
            hold=1
    pylab.axis('equal')
    if was_interactive: pylab.show()
    

        

               
        

    

    