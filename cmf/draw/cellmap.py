"""
This file contains a map for cells. Does only work with shapely support
"""

from __future__ import print_function, division, absolute_import
from .. import cmf_core as cmf
from .. import draw
from matplotlib import pylab as plt
from matplotlib.cm import ScalarMappable
from matplotlib.cm import viridis as default_colormap
from matplotlib.colors import Normalize
from matplotlib.animation import FuncAnimation
import numpy
from itertools import chain


class CellMap(ScalarMappable):
    """
    Draws a map of the cell geometries. Only functional, when shapely is installed.

    A CellMap is created with cells to show and a function returning a value from a cell

    Usage example:
    >>>import cmf
    >>>p=cmf.project()
    >>>def saturated_depth(c):
    ...    return c.saturated_depth
    >>>cm = CellMap(p, saturated_depth)
    """

    def __call__(self, recalc_range=False):
        if recalc_range:
            self.maxvalue = max((self.f(c) for c in self.cells))
            self.minvalue = min((self.f(c) for c in self.cells))

        for cell, polys in self.polygons.items():
            v = self.f(cell)
            c = self.cmap((v - self.minvalue) / (self.maxvalue - self.minvalue))
            for poly in polys:
                poly.set_fc(c)
        if plt.isinteractive():
            plt.draw()

    @property
    def f(self):
        return self.__f

    @f.setter
    def f(self, funct):
        self.__f = funct
        if plt.isinteractive():
            self(True)

    @property
    def color_values(self):
        return numpy.array([self.f(c) for c in self.cells])

    @staticmethod
    def draw_shapes(shape, c, **kwargs):
        if hasattr(shape, "exterior"):
            shape = shape.exterior
        a = plt.asarray(shape)
        return plt.fill(a[:, 0], a[:, 1], fc=c, **kwargs)[0]

    def __init__(self, cells, value_function, cmap=default_colormap,
                 hold=True, vmin=None, vmax=None, **kwargs):
        """
        Creates a new map from cells

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
        self.cells = [c for c in cells if c.geometry]
        self.__f = value_function

        was_interactive = plt.isinteractive()

        if was_interactive:
            plt.ioff()

        self.polygons = {}

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
            self.minvalue = self.maxvalue - 1

        if not hold:
            plt.cla()

        for cell, shapes, v in zip(self.cells, geos, values):
            c = self.cmap(float(v - self.minvalue) / float(self.maxvalue - self.minvalue))
            self.polygons[cell] = [self.draw_shapes(s, c, **kwargs) for s in shapes]

        plt.axis('equal')
        norm = Normalize(self.minvalue, self.maxvalue)
        plt.matplotlib.cm.ScalarMappable.__init__(self, norm, cmap)

        if was_interactive:
            plt.draw()
            plt.ion()

    def get_artists(self):
        """
        Returns the list of matplotlib.patches.Polygons in the cell map.

        Useful for Animations

        :return: iterable of polygons
        """
        return chain(*self.polygons.values())

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


class FluxMap(object):
    """
    Draws for each cell an arrow indicating the direction and velocity of flow
    through the cell (in horizontal direction). Uses plt.quiver

    The FluxMap can be updated by calling it with the new timestep

    Usage:
    >>> import cmf
    >>> p = cmf.project()
    >>> solver = cmf.RKFIntegrator(p, 1e-9)
    >>> fm = FluxMap(p, cmf.Time())
    >>> for t in solver.run(solver.t, solver.t + cmf.day * 30, cmf.h):
    ...     fm(t)
    """

    def __call__(self, t=None):
        a = numpy.array
        if t:
            self.t = t
        f = cmf.cell_flux_directions(self.cells, self.t)

        self.quiver.set_UVC(a(f.X), a(f.Y))

        if plt.isinteractive():
            plt.draw()

    def __init__(self, cells, t, **kwargs):
        """
        Creates a new flux map
        :param cells: The cells to be used
        :param t: The current time step
        :param kwargs: Keyword arguments for plt.quiver
        """
        self.cells = cells
        # get position vector
        p = cmf.cell_positions(cells)
        # get flux vector
        f = cmf.cell_flux_directions(cells, t)
        self.t = t
        a = numpy.array
        self.quiver = plt.quiver(a(p.X), a(p.Y), a(f.X), a(f.Y), **kwargs)

    def __get_scale(self):
        return self.quiver.scale

    def __set_scale(self, value):
        self.quiver.scale = value
        self()

    scale = property(__get_scale, __set_scale)


class Animator:
    def __init__(self, cells, solver, start, end, step):
        """

        :param cells: A sequence of cmf Cells, can be a project
        :param solver: A cmf solver Eg. cmf.CVodeDense()
        :param start: Start time
        :param end: End time
        :param step: Time step
        """

        self.figure = plt.figure()
        self.generator = solver.run(start, end, step)
        self.cellmap = draw.CellMap(cells,
                                    lambda c: c.layers[0].wetness if c.layers else 0.0,
                                    vmin=0, vmax=1)

        self.fluxmap = draw.FluxMap(cells, start, zorder=2, color='w')

        self.title = plt.title('<time step>')
        self.animator = None

    def __call__(self, *args, **kwargs):
        self.animator = FuncAnimation(self.figure, self.draw, frames=self.generator)

    def draw(self, frame=None):
        t = frame
        # print(self.fluxmap.scale)
        self.title.set_text(str(t))
        self.cellmap()
        self.fluxmap(t)
        return [self.title, self.fluxmap.quiver] + list(self.cellmap.get_artists())
