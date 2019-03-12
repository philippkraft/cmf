

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
import numpy

from shapely import geometry as g
import pylab


class ShapeMap(object):
    """ self.fc_function: A callable taking a feature and returning a color (fillcolor)
    self.lw_function: A callable taking a feature and returning a scalar (line width)
    """

    def refresh(self):
        for i, f in enumerate(self.features):
            for s in self.shapes[i]:
                if self.fc_function and hasattr(s, 'set_fc'):
                    s.set_fc(self.fc_function(f))
                if self.lw_function and hasattr(s, 'set_lw'):
                    s.set_lw(self.lw_function(f))
        if pylab.isinteractive():
            pylab.draw()

    def __init__(self, features, **kwargs):
        self.features = features
        self.fc_function = None
        self.lw_function = None
        wasinteractive = pylab.isinteractive()
        if wasinteractive:
            pylab.ioff()
        self.shapes = []
        for i, feature in enumerate(self.features):
            if isinstance(feature.shape, g.MultiPolygon):
                self.shapes.append([])
                for shape in feature.shape.geoms:
                    x, y = numpy.asarray(shape.exterior)[:, :2].swapaxes(0, 1)
                    self.shapes[-1].append(pylab.fill(x, y, **kwargs)[0])
            if isinstance(feature.shape, g.Polygon):
                x, y = numpy.asarray(feature.shape.exterior)[:, :2].swapaxes(0, 1)
                self.shapes.append(pylab.fill(x, y, **kwargs))
            elif isinstance(feature.shape, g.MultiLineString):
                self.shapes.append([])
                for shape in feature.shape.geoms:
                    x, y = numpy.asarray(shape)[:, :2].swapaxes(0, 1)
                    self.shapes[-1].append(pylab.plot(x, y, **kwargs)[0])
            elif isinstance(feature.shape, g.LineString):
                x, y = numpy.asarray(feature.shape).swapaxes(0, 1)
                self.shapes.append(pylab.plot(x, y, **kwargs))
            elif isinstance(feature.shape, g.Point):
                x, y = feature.shape.x, feature.shape.y
                self.shapes.append(pylab.plot([x], [y], **kwargs)[0])
        pylab.axis('equal')
        if wasinteractive:
            pylab.ion()
            pylab.draw()
