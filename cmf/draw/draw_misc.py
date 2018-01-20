

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
from __future__ import print_function, division, absolute_import
import pylab
import numpy
import os
from .. import cmf_core as cmf


class CellMap(pylab.matplotlib.cm.ScalarMappable):
    """
    Draws a map of the cell geometries. Only functional, when shapely is installed.

    A CellMap is created with cells to show and a function returning a value from a cell

    Usage example:
    >>>def saturated_depth(c):
    ...    return c.saturated_depth
    >>>cm = CellMap(project, saturated_depth)
    """
    def __call__(self, recalc_range=False):
        if recalc_range:
            self.maxvalue=max((self.f(c) for c in self.cells))
            self.minvalue=min((self.f(c) for c in self.cells))

        for cell,polys in self.polygons.iteritems():
            v=self.f(cell)
            c=self.cmap((v-self.minvalue)/(self.maxvalue-self.minvalue))
            for poly in polys:
                poly.set_fc(c)
        if pylab.isinteractive():
            pylab.draw()

    @property
    def f(self): return self.__f


    @f.setter
    def f(self, funct):
        self.__f=funct
        if pylab.isinteractive():
            self(True)

    @property
    def color_values(self):
        return numpy.array([self.f(c) for c in self.cells])

    @staticmethod
    def draw_shapes(shape, c, **kwargs):
        if hasattr(shape, "exterior"):
            shape = shape.exterior
        a = pylab.asarray(shape)
        return pylab.fill(a[:, 0], a[:, 1], fc=c, **kwargs)[0]

    def __init__(self, cells, value_function, cmap=pylab.cm.jet,
                 hold=True, vmin=None, vmax=None, **kwargs):
        """

        :param cells:
        :param value_function:
        :param cmap:
        :param hold:
        :param vmin:
        :param vmax:
        :param kwargs:
        """

        if not hasattr(cmf.Cell, 'geometry'):
            raise NotImplementedError('The geometry of the cells can not be used, shapely is not installed')
        self.cmap = cmap
        self.cells=[c for c in cells if c.geometry]
        self.__f = value_function

        was_interactive = pylab.isinteractive()

        if was_interactive:
            pylab.ioff()


        self.polygons={}

        def flatten_multipolygons(shape):
            if hasattr(shape, "geoms"):
                return shape.geoms
            else:
                return [shape]

        geos = [flatten_multipolygons(c.geometry) for c in self.cells]
        values = [self.f(c) for c in self.cells]
        self.maxvalue = vmax or max(values)
        self.minvalue = vmin or min(values)

        if self.minvalue >= self.maxvalue:
            self.minvalue = self.maxvalue-1

        if not hold:
            pylab.cla()

        for cell, shapes, v in zip(self.cells, geos, values):

            c = self.cmap(float(v-self.minvalue)/float(self.maxvalue-self.minvalue))
            self.polygons[cell] = [self.draw_shapes(s, c, **kwargs) for s in shapes]

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


def __x_from_ts(ts):
    return pylab.fromiter(((t-cmf.Time(1,1,1))/cmf.day for t in ts.iter_time()),dtype=numpy.float)


def plot_timeseries(data,style='-',**kwargs):
    """
    Plots a cmf.timeseries as a line using pylab.plot
    :param data: cmf.timeseries
    :param style: Style code for pylab.plot
    :param kwargs: Keyword arguments for pylab.plot
    :return: matplotlib line object
    """
    try:
        step = kwargs.pop('step')
        ts = data.reduce_avg(data.begin - data.begin % step, step)
    except KeyError:
        ts = data

    x = __x_from_ts(ts)
    line=pylab.plot(x,pylab.asarray(ts),style,**kwargs)[0]
    ax=pylab.gca()
    ax.xaxis_date()
    return line


def bar_timeseries(data,**kwargs):
    """
    Makes a bar graph from a cmf.timeseries using pylab.bar
    :param data: cmf.timeseries
    :param kwargs: Keyword arguments of pylab.bar
    :return: as pylab.bar
    """
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
    """
    Plots a sequence of objects with a position attribute

    :param locatables:
    :param style:
    :param kwargs:
    :return:
    """
    get_x=lambda l:l.position.x
    get_y=lambda l:l.position.y
    pylab.plot(pylab.amap(get_x,locatables),pylab.amap(get_y,locatables),style,**kwargs)


def connector_matrix(allstates,size=(500,500)):
    """Returns a matrix
    """
    posdict={}
    jac=numpy.zeros(size, dtype=int)
    l=len(allstates)
    for i,a in enumerate(allstates):
        posdict[a.node_id] = i
    for i,a in enumerate(allstates):
        for f,t in a.fluxes(cmf.Time()):
            j=posdict.get(t.node_id)
            if j:
                jac[i*size[0]/l, j*size[1]/l] += 1
    return jac


class FluxMap(object):
    """
    Draws for each cell an arrow indicating the direction and velocity of flow
    through the cell (in horizontal direction). Uses pylab.quiver

    The FluxMap can be updated by calling it with the new timestep

    Usage:
    >>> fm = FluxMap(project, cmf.Time())
    >>> for t in solver.run(solver.t, solver.t + cmf.day * 30, cmf.h):
    ...     fm(t)
    """
    def __call__(self, t=None):
        a = numpy.array
        if t:
            self.t = t
        f = cmf.cell_flux_directions(self.cells, self.t)

        self.quiver.set_UVC(a(f.X), a(f.Y), self.color_array)

        if pylab.isinteractive():
            pylab.draw()

    def __init__(self, cells, t, **kwargs):
        """
        Creates a new flux map
        :param cells: The cells to be used
        :param t: The current time step
        :param kwargs: Keyword arguments for pylab.quiver
        """
        self.cells = cells
        # get position vector
        p = cmf.cell_positions(cells)
        # get flux vector
        f = cmf.cell_flux_directions(cells, t)
        self.t = t
        a = numpy.array
        self.quiver = pylab.quiver(a(p.X), a(p.Y), a(f.X), a(f.Y), **kwargs)

    def __get_scale(self):
        return self.quiver.scale

    def __set_scale(self,value):
        self.quiver.scale = value
        self()

    scale = property(__get_scale, __set_scale)




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
            print("File", filename, "or worldfile", worldname, "not found")
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


