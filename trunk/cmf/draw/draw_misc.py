

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
import pylab
import numpy
from cmf.cell_factory import geometry as geoms
import cmf.cell_factory
import os

class cell_polygon_map(pylab.matplotlib.cm.ScalarMappable):
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
    @property
    def color_values(self):
        return numpy.array([self.f(c) for c in self.cells])
    def __init__(self,cells,value_function, cmap=pylab.cm.jet,hold=True,vmin=None,vmax=None,**kwargs):
        self.cells=cells
        self.__f = value_function
        was_interactive=pylab.isinteractive()
        if was_interactive: pylab.ioff()
        geos=[]
        self.maxvalue=-1e300 if vmax is None else vmax
        self.minvalue=1e300 if vmin is None else vmin
        self.polygons={}
        # Generate image array, which is filled with generated values from self.f
        self._A = []
        for cell in cells:
            shape=geoms[cell]
            if hasattr(shape, "geoms"):
                shapes=shape.geoms
            else:
                shapes=[shape]
            value=self.f(cell)
            self._A.append(value)
            if vmax is None:
                self.maxvalue=max(value,self.maxvalue)
            if vmin is None:
                self.minvalue=min(value,self.minvalue)
            for s in shapes:
                geos.append((cell,s,value))
        if self.minvalue>=self.maxvalue: self.minvalue=self.maxvalue-1
        for cell,s,v in geos:
            if hasattr(s, "exterior"):
                s=s.exterior
            c=cmap(float(v-self.minvalue)/float(self.maxvalue-self.minvalue))
            a=pylab.asarray(s)
            self.polygons[cell]=pylab.fill(a[:,0],a[:,1],fc=c,hold=hold,**kwargs)[0]
            hold=1
        pylab.axis('equal')
        norm = pylab.matplotlib.colors.Normalize(self.minvalue, self.maxvalue)
        pylab.matplotlib.cm.ScalarMappable.__init__(self, norm, cmap)
        if was_interactive:
            pylab.draw() 
            pylab.ion()

    def autoscale_None(self):
        """
        Overwrite base class of maplotlib.cm.ScalarMappable to prevent missuse
        """
        pass
    def autoscale(self):
        """
        Overwrite base class of maplotlib.cm.ScalarMappable to prevent missuse
        """
        pass
#    def callbacksSM(self):
#        pass

        
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
            if hasattr(s, "exterior"):
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

def __x_from_ts(ts):
    return pylab.fromiter(((t-cmf.Time(1,1,1))/cmf.day for t in ts.iter_time()),dtype=numpy.float)
def plot_timeseries(data,style='-',**kwargs):  
    try:
        step = kwargs.pop('step')        
        ts = data.reduce_avg(data.begin - data.begin % step,step)
    except KeyError:
        ts=data
    
    x=__x_from_ts(ts)
    line=pylab.plot(x,pylab.asarray(ts),style,**kwargs)[0]
    ax=pylab.gca()
    ax.xaxis_date()
    return line
def bar_timeseries(data,**kwargs):
    try:
        step = kwargs.pop('step')        
        ts = data.reduce_avg(data.begin - data.begin % step,step)
    except KeyError:
        ts=data
    x=__x_from_ts(ts)
    was_inter = pylab.isinteractive()
    pylab.ioff()
    bars=pylab.bar(x,ts,ts.step/cmf.day,**kwargs)
    ax=pylab.gca()
    ax.xaxis_date()
    if was_inter:
        pylab.draw()
        pylab.ion()
    return bars
    
def plot_locatables(locatables,style='kx',**kwargs):
    get_x=lambda l:l.position.x
    get_y=lambda l:l.position.y
    pylab.plot(pylab.amap(get_x,locatables),pylab.amap(get_y,locatables),style,**kwargs)

def connector_matrix(allstates,size=(500,500)):
    """Returns a matrix
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

    
class cell_quiver(object):
    def __call__(self,t=None):
        a = numpy.array
        if t: self.t = t
        f = cmf.cell_flux_directions(self.cells,self.t)
        self.quiver.set_UVC(a(f.X),a(f.Y),self.color_array)
        if pylab.isinteractive(): pylab.draw()
    def __init__(self,cells,t,**kwargs):
        self.cells = cells
        p = cmf.cell_positions(cells)
        f = cmf.cell_flux_directions(cells,t)
        self.color_array = None
        self.t = t   
        a = numpy.array
        self.quiver = pylab.quiver(a(p.X),a(p.Y),a(f.X),a(f.Y),**kwargs)
    def __get_scale(self):
        return self.quiver.scale
    def __set_scale(self,value):
        self.quiver.scale = value
        self()
    scale = property(__get_scale,__set_scale)
class quiver3d(object):
    def __init__(self,objects,t,zscale=1.0,**kwargs):
        """Invokes mlab.quiver3d. Mayavi 2 by Enthought has to be installed
        objects: Either a cmf.node_list or a sequence of cells
        t      : A cmf.Time or a datetime.datetime object
        keywords are passed to mlab.quiver3d
        """
        try:
            from enthought.mayavi import mlab
            from numpy import asarray, meshgrid
        except ImportError:
            raise NotImplementedError("Raster.draw3d needs an installation of mayavi to work")
        if isinstance(objects,cmf.node_list):
            p=objects.get_positions()
            f=objects.get_fluxes3d(t)
        elif isinstance(objects,cmf.project):
            p=cmf.cell_positions(objects.cells)
            f=cmf.cell_fluxes(objects.cells,t)
        elif isinstance(objects[0],cmf.Cell):
            p=cmf.cell_positions(objects)
            f=cmf.cell_fluxes(objects,t)
        else:
            raise ValueError("Given objects are not a nodelist or a sequence of cells")
        self.objects = objects
        self.zscale = zscale
        self.quiver = mlab.quiver3d(p.X,p.Y,p.Z,f.X,f.Y,f.Z*zscale,**kwargs)
    def __call__(self,t):
        if isinstance(self.objects,cmf.node_list):
            f=self.objects.get_fluxes3d(t)
        elif isinstance(self.objects,cmf.project):
            f=cmf.cell_fluxes(self.objects.cells,t)
        elif isinstance(self.objects[0],cmf.Cell):
            f=cmf.cell_fluxes(objects,t)
        else:
            raise ValueError("Given objects are not a nodelist or a sequence of cells")
        self.quiver.mlab_source.set(u=f.X,v=f.Y,w=f.Z*self.zscale)
        


try:
    import Image
    def plot_image(filename,**kwargs):
        fname,imgext = os.path.splitext(filename)
        worldext = imgext[:2] + imgext[-1] + 'w'
        worldname = fname + worldext
        kwargs.pop('extent',None)
        kwargs.pop('origin',None)
        if os.path.exists(filename) and os.path.exists(worldname):
            image=Image.open(filename)
            world=numpy.fromfile(worldname,sep='\n')
            left,top = world[-2:]
            bottom = top + world[3] * image.size[1]
            right = left + world[0] * image.size[0]
            return pylab.imshow(image,extent=(left,right,bottom,top),origin='bottom',**kwargs)
        else:
            print "File",filename,"or worldfile",worldname,"not found"
except:
    pass
    
    
def contour_raster(raster,**kwargs):
    Z=raster.asarray()
    Z=numpy.flipud(Z)
    extent=(raster.llcorner[0],raster.llcorner[0]+raster.extent[0],raster.llcorner[1],raster.llcorner[1]+raster.extent[1])
    C=pylab.contour(Z,extent=extent,**kwargs)
    pylab.clabel(C)
    pylab.axis('scaled')

def contourf_raster(raster,**kwargs):
    Z=raster.asarray()
    Z=numpy.flipud(Z)
    extent=(raster.llcorner[0],raster.llcorner[0]+raster.extent[0],raster.llcorner[1],raster.llcorner[1]+raster.extent[1])
    C=pylab.contourf(Z,extent=extent,**kwargs)
    pylab.clabel(C)
    pylab.axis('scaled')
    
               
