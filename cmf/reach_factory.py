
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

def get_predicate(tolerance,as_shape):
    if tolerance:
        return lambda shp1,shp2:as_shape(shp1).distance(as_shape(shp2))<tolerance
    else:
        return lambda shp1,shp2:as_shape(shp1).intersects(as_shape(shp2))


def downstream_recursive(unhandled, pred, downstreams, this, order):
    try:
        unhandled.remove(this)
    except ValueError as e:
        pass
    maxorder = order
    upstream = [c for c in unhandled if pred(this, c)]
    for u in upstream:
        downstreams[u] = this
        maxorder = max(downstream_recursive(unhandled, pred, downstreams, u, order + 1), maxorder)
    return maxorder


def get_downstream(features, z_callable=lambda feat: feat.z, shape_callable=lambda feat: feat.shape, tolerance=0):
    """
    Creates the topological relationship of reaches from a sequence of geo features.
    The z_callable and the shape callable must be function or function like objects returning the height resp. a shapely geometry
    tolerance allows to specify the maximum distance (in mapping units) between two features to count as connected.

    Returns a dictionary of the features, mapping to the downstream feature
    """
    downstreams = {}
    # Copy the features
    unhandled = list(features)
    # Make a comparison operator to sort the parts by height
    z = z_callable
    z_cmp = lambda rec1, rec2: 1 if z(rec1) > z(rec2) else (-1 if z(rec1) < z(rec2) else 0)
    unhandled.sort(z_cmp)
    print("z: %f - %f" % (z(unhandled[0]), z(unhandled[-1])))
    # Get the tolerance predicate
    pred = get_predicate(tolerance, shape_callable)
    # as long as unhandled parts exist
    i = 0
    while unhandled:
        this = unhandled[0]
        i += 1
        print('Start (height)', z(this), end='')
        print(downstream_recursive(unhandled, pred, downstreams, this, 0), ' sub reaches')
    return downstreams


def create_reaches_preintersect(project, features, outlets, cell_callable,
                                shape_callable=lambda feat: feat.shape,
                                length_callable=None,
                                width_callable=None, width=1.0,
                                depth_callable=None, depth=0.25,
                                type_callable=None, type='T',
                                tolerance=0):
    """ Creates reaches for a project containing cells
    project: the cmf project
    features: a sequence holding or referencing the shapely features
    outlets: a sequence holding catchment outlets
    cell_callable: a callable finding the cell for the reach with a feature.
    z_callable: a callable returning the mean height of a reach from a feature.
    shape_callable: a callable returning a shapely geometry from a feature.
                    Default: lambda feature:feature.shape
    width_callable: a callable returning the width of the reach described by a feature.
                    If None (default), the constant value width (default 1.0) is used
    depth_callable: a callable returning the depth of the reach described by a feature.
                    If None (default), the constant value depth (default 0.25) is used
    type_callable: a callable returning the type of reach geometry used.
                   If None (default), the constant value type (default: 'T') is used.
                   The type is described by a single character:
                   'T' (Triangular),'R' (Rectangular), 'S' (SWAT like reach), 'P' (Pipe)
    tolerance: Maximum distance between two reaches to be considered as connected. Default: 0.0
    """
    if length_callable is None:
        length_callable = lambda feature: shape_callable(feature).length
    if width_callable is None:
        width_callable = lambda feature: width
    if depth_callable is None:
        depth_callable = lambda feature: depth
    if type_callable is None:
        type_callable = lambda feature: type
    z_callable = lambda feature: cell_callable(feature).z
    reaches = {}
    cells = {}
    # Cycle through all features, to create the reaches
    for f in features:
        # Get the cell of the reach
        cell = cell_callable(f)
        if cell:
            cells[f] = cell
            # Add a reach to the cell
            try:
                r = cell.add_reach(length_callable(f), type_callable(f), depth_callable(f), width_callable(f))
                reaches[f] = r
                geometry[r] = shape_callable(f)
            except NotImplementedError:
                raise RuntimeError("Problem creating reach with l=%s,t=%s,d=%s,w=%s" % (
                length_callable(f), type_callable(f), depth_callable(f), width_callable(f)))
    print(len(reaches), 'reaches created, proceed with connecting...')

    # Connect the reaches

    # Create the downstream dictionary
    downstreams = get_downstream(cells.keys(), z_callable, shape_callable, tolerance)
    root_reaches = []
    for f in features:
        r = reaches.get(f)
        if r:
            # If a downstream exists for this feature, use it
            if f in downstreams:
                f_down = downstreams[f]
                try:
                    r_down = reaches[f_down]
                except KeyError as e:
                    print("KeyError, reach does not exist yet")
                r.set_downstream(reaches[downstreams[f]])
            # Else look for an outlet at the reach
            else:
                root_reaches.append(r)
                outlet_connection = False
                for o in outlets:
                    p = Point(tuple(o.position))
                    if p.distance(shape_callable(f)) <= tolerance:
                        r.set_outlet(o)
                        print("A reach at %s is connected to outlet %s", (r.cell, o.Name))
                        outlet_connection = True
                if not outlet_connection:
                    print("found dead end reach, water flows to surface water of %s" % r.cell)
    return root_reaches


def create_reaches(project, outlets, features,
                   cell_shape_callable=lambda cell: cell.geometry,
                   shape_callable=lambda feat: feat.shape,
                   width_callable=None, width=1.0,
                   depth_callable=None, depth=0.25,
                   type_callable=None, type='T',
                   tolerance=0):
    """ Creates reaches for a project containing cells
    project: the cmf project
    features: a sequence holding or referencing the shapely features
    outlets: a sequence holding catchment outlets
    cell_shape_callable: a callable finding the geometry for a cell in project. If the
            cells where created by cells_from_polygon or cells_from_dem the default is ok
    shape_callable: a callable returning a shapely geometry from a feature.
                    Default: lambda feature:feature.shape
    width_callable: a callable returning the width of the reach described by a feature.
                    If None (default), the constant value width (default 1.0) is used.
                    If the feature has the attribute width, use: lambda f:f.width
    depth_callable: a callable returning the depth of the reach described by a feature.
                    If None (default), the constant value depth (default 0.25) is used
                    If the feature has the attribute depth, use: lambda f:f.depth
    type_callable: a callable returning the type of reach geometry used.
                   If None (default), the constant value type (default: 'T') is used.
                   The type is described by a single character:
                   'T' (Triangular),'R' (Rectangular), 'S' (SWAT like reach), 'P' (Pipe)
    tolerance: Maximum distance between two reaches to be considered as connected. Default: 0.0
    """

    reaches = []
    if width_callable is None:
        width_callable = lambda feature: width
    if depth_callable is None:
        depth_callable = lambda feature: depth
    if type_callable is None:
        type_callable = lambda feature: type
    for cell in project:
        if hasattr(cell, "geometry"):
            cell_geo = cell_shape_callable(cell)
            for f in features:
                r_geo = shape_callable(f)
                if r_geo.intersects(cell_geo):
                    cr_geo = r_geo.intersection(cell_geo)
                    reaches.append(
                        (cr_geo, cell, cr_geo.length, width_callable(f), depth_callable(f), type_callable(f)))
    return create_reaches_preintersect(project, reaches, outlets,
                                       cell_callable=lambda f: f[1],
                                       shape_callable=lambda f: f[0],
                                       length_callable=lambda f: f[2],
                                       width_callable=lambda f: f[3],
                                       depth_callable=lambda f: f[4],
                                       type_callable=lambda f: f[5],
                                       tolerance=tolerance)


def __create_reach_for_cell(reaches, outlets, downstream_reach, cell, cells, reach_geometry, subsurface_connection_type,
                            width, depth, diffusive):
    """for internal use only, do not invoke"""
    assert (cell in cells)
    # Get cells upstream of current cell
    in_cells = [n for n, w in cell.neighbors if n.main_outlet == cell and n in cells]
    # downstream length is half of the distance to the downstream reach
    length = cmf.distance(downstream_reach.position, cell) * 0.5 if downstream_reach else 0.5 * sqrt(cell.area)
    # upstream length of the reach is half of sum of distance to upstream cells
    length += 0.5 * sum(cmf.distance(cell, in_cell) for in_cell in in_cells)
    # Create the reach using the calculated length and the given parameters
    r = cell.project.NewReach(cell.x, cell.y, cell.z - depth, length, reach_geometry, width, depth, diffusive)
    r.Name = "~~ C[%i]" % cell.Id
    # Connect the reach to the cell
    r.connect_to_cell(cell, width, subsurface_connection_type, diffusive)
    # Set the downstream
    if downstream_reach:
        r.set_downstream(downstream_reach)
    else:
        outlet = cell.project.NewOutlet("Outlet of %s" % r.Name, r.position.x, r.position.y, r.position.z - 0.1)
        r.set_outlet(outlet)
        outlets.append(outlet)
    # Remove cell from task set
    cells.remove(cell)
    # Append reach to the results
    reaches.append(r)
    # Create reaches for each upstream cell
    for c in in_cells:
        __create_reach_for_cell(reaches, outlets, r, c, cells, reach_geometry, subsurface_connection_type, width, depth,
                                diffusive)


def create_reaches_for_cells(cells_with_reach, reach_geometry='T', width=1.0, depth=0.25,
                             subsurface_connection_type=None, diffusive=None):
    """Creates reaches for a sequence of cells.
    """
    assert (subsurface_connection_type is None or issubclass(subsurface_connection_type, cmf.lateral_sub_surface_flux))
    cells = set(cells_with_reach)
    reaches = []
    outlets = []
    while cells:
        cur = min(cells, key=lambda c: c.z)
        __create_reach_for_cell(reaches, outlets, None, cur, cells, reach_geometry, subsurface_connection_type, width,
                                depth, diffusive)
    return cmf.node_list(reaches), cmf.node_list(outlets)
