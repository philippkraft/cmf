

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
from __future__ import print_function, unicode_literals, absolute_import, division

from . import cmf_core as cmf
from . import maps
import time
from math import *



class Geometry:
    def __init__(self):
        self.__dict = {}

    def __getitem__(self, o):
        try:
            return self.__dict[hash(o)][1]
        except KeyError:
            raise KeyError("No geometry stored for %s" % o)

    def __setitem__(self, key, value):
        self.__dict[hash(key)] = (key, value)

    def itervalues(self):
        for v in self.__dict.values():
            yield v[1]

    def iterkeys(self):
        for v in self.__dict.values():
            yield v[0]

    def __iter__(self):
        return self.iterkeys()

    def keys(self):
        return list(self.iterkeys())

    def values(self):
        return list(self.itervalues())

    def iteritems(self):
        for v in self.__dict.values():
            return v

    def items(self):
        return list(self.iteritems())

    def __repr__(self):
        return "Geometry holder for cmf objects"

    def __len__(self):
        return len(self.__dict)

    def get(self, key):
        item = self.__dict.get(hash(key))
        if item is None:
            return None
        else:
            return item[1]


geometry = Geometry()


class cell_tree(object):

    def __addbranch(self, cell):
        self.__data[cell] = []
        for n, w in cell.neighbors:
            if n.topology.MainOutlet() == cell:
                self.__data[cell].append(n)
                self.__addbranch(n)

    def __init__(self, start_cell):
        self.start = start_cell
        self.__data = {}
        self.__addbranch(start_cell)

    def __getitem__(self, cell):
        return self.__data[cell]

    def __len__(self):
        return len(self.__data)

    def __iter__(self):
        cells = [self.start]
        while cells:
            c = cells.pop(0)
            cells.extend(self[c])
            yield c


def get_predicate(tolerance, as_shape):
    if tolerance:
        return lambda shp1, shp2: as_shape(shp1).distance(as_shape(shp2)) < tolerance
    else:
        return lambda shp1, shp2: as_shape(shp1).intersects(as_shape(shp2))


def cell_neighbors(features, shape_callable=lambda feat: feat.shape):
    res = {}
    pred = get_predicate(0.0, shape_callable)
    for f in features:
        res[f] = []
    for i, s in enumerate(features):
        for j in range(i + 1, len(features)):
            s_cmp = features[j]
            if pred(s, s_cmp):
                shp_s = shape_callable(s)
                shp_cmp = shape_callable(s_cmp)
                intersect = shp_s.intersection(shp_cmp).length
                if intersect:
                    res[s].append((s_cmp, intersect))
                    res[s_cmp].append((s, intersect))
    return res


def cells_from_polygons(project, features,
                        shape_callable=lambda feat: feat.shape,
                        id_callable=lambda f: 0,
                        center_callable=lambda feat: (
                            feat.shape.centroid.x, feat.shape.centroid.y, feat.shape.centroid.y),
                        area_callable=lambda feat: feat.shape.area,
                        no_connect=False,
                        verbose=False):
    """ Adds cells from shapely features to project, and connects them topological
    project: a cmf project
    features: a sequence holding or referencing the shapely features
    shape_callable: a callable (e.g. a lambda function) returning the shapely geometry from a feature in features. Default: lambda feature:feature.shape (suitable for shapefiles)
    center_callable: a callable returning the center of the cell from a feature as a three-tuple. Default: lambda feature:tuple(feature.shape.centroid)
    area_callable: a callable returning the area of the cell from a feature: Default: lambda feature:feature.shape.area
    report: if True, report about connecting success
    Returns: dictionary with the features as keys and the cells as values
    """

    pred = get_predicate(0.0, shape_callable)
    cells = []
    cell_dict = {}
    q_tree = maps.simple_quad_tree()

    # Create the cells from the features and build the quad tree
    for i, f in enumerate(features):
        center = center_callable(f)
        assert len(center) > 2
        area = area_callable(f)
        c = project.NewCell(center[0], center[1], center[2], area)
        c.Id = id_callable(f)
        cells.append(c)
        geometry[c] = shape_callable(f)
        cell_dict[i] = c
        q_tree.add_object(i, f.shape.bounds)

    if verbose:
        print("No. of connected cells:")
    report_at = [100, 500, 1000, 5000, 10000, 50000, 100000, 500000]
    start = time.clock()
    if not no_connect:
        con_count = 0
        cmp_count = 0
        for i, s in enumerate(features):
            if verbose and i in report_at:
                print(i, end='\r')
            candidates = q_tree.get_objects(shape_callable(s).bounds)
            cmp_count += len(candidates)
            for ic in candidates:
                c = features[ic]
                if not s is c and pred(s, c):
                    shp_s = shape_callable(s)
                    shp_cmp = shape_callable(c)
                    intersect = shp_s.intersection(shp_cmp).length
                    if intersect:
                        cell_dict[i].topology.AddNeighbor(
                            cell_dict[ic], intersect)
                        con_count += 1
        if verbose:
            print(len(features), ' %0.2f sec. %i comparisons' %
                  (time.clock() - start, cmp_count))
    # cmf.Topology.calculate_contributing_area(project)
    return cell_dict


def cells_from_dem(project, dem, direction_count=8):
    """ Adds square cells from a dem to the project, and meshes them.
    project         : cmf.project, where the cells should be added to
    dem             : a Raster representing the heights of cells
    direction_count : Number of directions to connect cells
             1: Every cell gets connected with its steepest downward neighbor. 
                Flow width is cellsize for orthogonal and sqrt(2)*cellsize for
                diagonal directions
             4: Every cell gets connected to its 4 direct orthogonal neighbors
                Flow width equals the cellsize
             8: Every cell gets connected to its 8 neighbors
                Flow width is 0.5*cellsize for orthogonal and sqrt(2)/4*cellsize 
                for diagonal neighbors
    Returns: a dictionary with the tuple (column,row) as key and the cells as values 

    """
    if not direction_count in (1, 4, 8):
        raise ValueError("You can only create cells from a dem with " +
                         "1 (connection to downslope) 4 (straight connections)" +
                         " or 8 (all neighbors) directions")
    cells_dict = {}
    # Create the cells
    for x, y, z, area, c, r in dem.cells:
        cell = project.NewCell(x, y, z, area)
        cells_dict[c, r] = cell
    cellsize = 0.5 * (dem.cellsize[0] + dem.cellsize[1])
    for c, r in cells_dict:
        act_cell = cells_dict[c, r]
        if direction_count == 8:
            for nc, nr, nv, nd, dirx, diry in dem.neighbors(c, r):
                n_cell = cells_dict[nc, nr]
                if (dirx and diry):  # diagonal
                    flow_width = 0.354 * cellsize
                else:
                    flow_width = 0.5 * \
                        (abs(dem.cellsize[0] * dirx) +
                         abs(dem.cellsize[1] * diry))
                act_cell.topology.AddNeighbor(n_cell, flow_width)
        elif direction_count == 4:
            for nc, nr in ((c - 1, r), (c, r + 1), (c + 1, r), (c, r - 1)):
                n_cell = cells_dict.get((nc, nr))
                if n_cell:
                    flow_width = dem.cellsize[int(nc == c)]
                    act_cell.topology.AddNeighbor(n_cell, flow_width)
        elif direction_count == 1:
            # Get the lowest neighbor
            nc, nr, nv, nd, dirx, diry = min(dem.neighbors(
                c, r), key=lambda t: (t[2] - act_cell.z) / t[3])
            n_cell = cells_dict[nc, nr]
            if act_cell.z > n_cell.z:
                if (dirx and diry):  # diagonal
                    flow_width = sqrt(2.) * cellsize
                else:
                    flow_width = dem.cellsize[int(nc == c)]
                act_cell.topology.AddNeighbor(n_cell, flow_width)
    cmf.Topology.calculate_contributing_area(project.cells)
    return cells_dict


def project_from_dem(dem, tracer_string='', dir_count=8):
    p = cmf.project(tracer_string)
    cells_from_dem(p, dem, dir_count)
    return p
