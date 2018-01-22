

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
from itertools import chain
from .. import cmf_core as cmf


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


