
from __future__ import unicode_literals, print_function, absolute_import, division

from shapely.geometry import shape
from shapely.wkb import loads as __load_wkb
from shapely.geos import WKBReadingError as WKBReadingError
from time import time

import logging


from ..cmf_core import Cell
from .qtree import Quadtree


def add_geometry_property():
    """
    Extends the Cell class with a geometry attribute
    :return:
    """

    def get_geometry(c):
        try:
            return __load_wkb(c.get_WKB())
        except (TypeError, WKBReadingError):
            return None

    def set_geometry(c, geom):
        c.set_WKB(geom.wkb)

    def del_geometry(c):
        c.set_WKB(b'')

    prop = property(get_geometry, set_geometry, del_geometry, 'Geometry of the cell')
    setattr(Cell, 'geometry', prop)


def create_cell(project, polygon, height, id=None, with_surfacewater=True):
    """
    Creates a cell from a shapely polygon and stores the geometry in cell.geometry

    :param project: the cmf project of the cell
    :param polygon: the shapely Polygon
    :param height: the height of the cell
    :param id: the id of the cell, only set if not None
    :param with_surfacewater: True, if a surfacewater storage will be created
    :return: The new cell
    """

    # Get the center
    polygon = shape(polygon)
    center = polygon.centroid.x, polygon.centroid.y, height
    # Create the cell
    c = project.NewCell(*center, area=polygon.area, with_surfacewater=with_surfacewater)
    # Set the geometry
    c.geometry = polygon
    # Set the id, if given.
    if id is not None:
        c.Id = id
    return c


def mesh_project(project, min_intersection=0, verbose=False):
    """
    Get the topologcial information from the geometry
    This may take some time
    :param project: The cmf project. The cells of the project need to have geometry
    :param min_intersection: Minimum intersection length in m
    :param verbose: Set True for report of action and additional warnings
    :return:
    """
    # Create quad tree
    q_tree = Quadtree(sum(c.area for c in project))
    cell_count = 0
    for c in project:
        if c.geometry:
            q_tree.append(c, c.geometry.bounds)
            cell_count += 1
    
    if verbose and cell_count != len(project):
        logging.warning('Only {} of {} cells have a geometry and are used in the mesh'
                        .format(cell_count, len(project)))

    report_at = [10, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000]
    start = time()

    # Count connections and comparisons
    con_count = 0
    cmp_count = 0

    for i, cell in enumerate(c for c in project if c.geometry):

        # Report progress
        if verbose and i in report_at:
            print('>{:<7d} cells processed'.format(i), end='\r')

        cell_geom = cell.geometry

        # Get candidates for intersection from q_tree
        candidates = q_tree(cell.geometry.bounds)
        # We need as many comparisons as there are candidates
        cmp_count += len(candidates)
        # Find "real" intersections with the candidates
        for candidate in candidates:

            cand_geom = candidate.geometry

            does_intersect = (cell is not candidate) and (cell_geom.intersects(cand_geom))

            if does_intersect:
                # Get intersection lenght
                intersect = cell_geom.intersection(cand_geom).length
                if intersect > min_intersection:
                    cell.topology.AddNeighbor(candidate, intersect)
                    # Advance the connection count
                    con_count += 1
    if verbose:
        print('Processing {} cells took {:0.2f} sec. with {} comparisons and {} connections'
              .format(cell_count, time() - start, cmp_count, con_count))

