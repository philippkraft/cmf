

# Copyright 2010 by Philipp Kraft
# This file is part of cmf.
#
#   cmf is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
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

try:
    from PIL import Image
except ImportError:
    Image = None

from .. import cmf_core as cmf


def __x_from_ts(ts):
    """
    Creates the numeric x-Axis for plots from a timeseries
    :param ts: A cmf.timeseries
    :return: an array of floats for a matplotlib date axis
    """
    return pylab.fromiter(((t - cmf.Time(1, 1, 1)) / cmf.day for t in ts.iter_time()), dtype=numpy.float)


def plot_timeseries(data, style='-', **kwargs):
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
    line = pylab.plot(x, pylab.asarray(ts), style, **kwargs)[0]
    ax = pylab.gca()
    ax.xaxis_date()
    return line


def bar_timeseries(data, **kwargs):
    """
    Makes a bar graph from a cmf.timeseries using pylab.bar
    :param data: cmf.timeseries
    :param kwargs: Keyword arguments of pylab.bar
    :return: as pylab.bar
    """
    try:
        step = kwargs.pop('step')
        ts = data.reduce_avg(data.begin - data.begin % step, step)
    except KeyError:
        ts = data
    x = __x_from_ts(ts)
    was_inter = pylab.isinteractive()
    pylab.ioff()
    bars = pylab.bar(x, ts, ts.step / cmf.day, **kwargs)
    ax = pylab.gca()
    ax.xaxis_date()
    if was_inter:
        pylab.draw()
        pylab.ion()
    return bars


def plot_image(filename, **kwargs):
    """
    Plots an image with an ESRI Worldfile as a map background. Uses matplotlib.pylab.imshow
    :param filename: Filename of the image.
    :param kwargs: Keyword arguments to imshow
    :return: Image from imshow
    """
    if not Image:
        raise NotImplementedError('To plot a background image please install Pillow')

    fname, imgext = os.path.splitext(filename)
    worldext = '{}{}w'.format(imgext[:2], imgext[-1])
    worldname = fname + worldext

    kwargs.pop('extent', None)
    kwargs.pop('origin', None)
    if os.path.exists(filename) and os.path.exists(worldname):
        image = Image.open(filename)
        world = numpy.fromfile(worldname, sep='\n')
        left, top = world[-2:]
        bottom = top + world[3] * image.size[1]
        right = left + world[0] * image.size[0]
        return pylab.imshow(image, extent=(left, right, bottom, top), origin='bottom', **kwargs)
    else:
        raise IOError("File", filename, "or worldfile", worldname, "not found")

