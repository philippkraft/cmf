

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
from .cmf_core import *
from .describe import describe
from .stopwatch import StopWatch

__version__ = '1.1.1'

from .cmf_core import connect_cells_with_flux as __ccwf


def connect_cells_with_flux(cells, connection, start_at_layer=0):
    """Connects all cells in cells (sequence or generator) with a flux connection
    connection is an subclass of cmf.FluxConnection which exposes the cell_connector callable
                    (e.g. lateral subsurface fluxes and surface manning flux)
    start_at_layer : if the flux connection should only be used for deeper layers
    """
    if (hasattr(connection, 'cell_connector') and
        isinstance(connection.cell_connector, CellConnector)):
        __ccwf(list(cells), connection.cell_connector, start_at_layer)
    else:
        raise TypeError("flux_connection does not implement the cell_connector protocol")


def __add_geometry_property():

    # If shapely is available, add a geometry property to Cell, that consists of a shapely geometry
    try:
        from shapely.wkb import loads as __load_wkb
        from shapely.geos import WKBReadingError as WKBReadingError
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

    except ImportError:
        __load_wkb = None


__add_geometry_property()
