from shapely.geometry import Polygon,MultiPolygon,Point,LineString,MultiLineString,MultiPoint,GeometryCollection
import pylab
import numpy
from cmf.cell_factory import geometry as geoms
import cmf.cell_factory

class cell_polygon_map(object):
    def __call__(self,recalc_range=False):
        if recalc_range:
            self.maxvalue=max((self.f(c) for c in self.polygons.iterkeys()))
            self.minvalue=min((self.f(c) for c in self.polygons.iterkeys()))
        for cell,poly in self.polygons.iteritems():
            v=self.f(cell)
            c=self.cmap((v-self.minvalue)/(self.maxvalue-self.minvalue))
            poly.set_fc(c)
        if pylab.isinteractive():
            pylab.draw()   
    @property
    def f(self): return self.__f
    @f.setter
    def f(self,funct):
        self.__f=funct
        if pylab.isinteractive():
            self(True)
    def __init__(self,cells,value_function, cmap=pylab.cm.jet,hold=True,vmin=None,vmax=None,**kwargs):
        self.cells=cells
        self.__f = value_function
        self.cmap=cmap
        was_interactive=pylab.isinteractive()
        if was_interactive: pylab.ioff()
        geos=[]
        self.maxvalue=-1e300 if vmax is None else vmax
        self.minvalue=1e300 if vmin is None else vmin
        self.polygons={}
        for cell in cells:
            shape=geoms[cell]
            if hasattr(shape, "geoms"):
                shapes=shape.geoms
            else:
                shapes=[shape]
            value=self.f(cell)
            if vmax is None:
                self.maxvalue=max(value,self.maxvalue)
            if vmin is None:
                self.minvalue=min(value,self.minvalue)
            for s in shapes:
                geos.append((cell,s,value))
        if self.minvalue>=self.maxvalue: self.minvalue=self.maxvalue-1
        for cell,s,v in geos:
            if isinstance(s, Polygon):
                s=s.exterior
            c=cmap(float(v-self.minvalue)/float(self.maxvalue-self.minvalue))
            a=pylab.asarray(s)
            self.polygons[cell]=pylab.fill(a[:,0],a[:,1],fc=c,hold=hold,**kwargs)[0]
            hold=1
        pylab.axis('scaled')
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
    pylab.axis('scaled')
    if was_interactive:
        pylab.draw() 
        pylab.ion()

   
def plot_timeseries(data,style='-',**kwargs):    
    return pylab.plot_date(map(lambda t:(t-cmf.Time(1,1,1)).AsDays(),cmf.timerange(data.begin,data.end,data.step)),list(data),style,**kwargs)[0]
def plot_locatables(locatables,style='kx',**kwargs):
    get_x=lambda l:l.position.x
    get_y=lambda l:l.position.y
    pylab.plot(pylab.amap(get_x,locatables),pylab.amap(get_y,locatables),style,**kwargs)

def connector_matrix(allstates,size=(500,500)):
    """Returns a matrixx
    """
    posdict={}
    jac=numpy.zeros(size,dtype=int)
    l=len(allstates)
    for i,a in enumerate( allstates):
        posdict[a.node_id]=i
    for i,a in enumerate( allstates):
        for f,t in a.fluxes(cmf.Time()):
            j=posdict.get(t.node_id)
            if j:
                jac[i*size[0]/l,j*size[1]/l]+=1
    return jac   

def quiverXY(P,F,**kwargs):
    """ Calls the pylab.quiver command as
    quiver(P.X,P.Y,F.X,F.Y,scale=scale)
    P a cmf.point_vector, holding the position of objects
    F a cmf.point_vector, holding the flux directions of objects
    keyword arguments see pylab.quiver
    """
    X=numpy.array(P.X)
    Y=numpy.array(P.Y)
    fX=numpy.array(F.X)
    fY=numpy.array(F.Y)
    pylab.quiver(X,Y,fX,fY,**kwargs)
    
    
        
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
    
               
