

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
from .cmf_core import *
from .describe import describe
from .timetools import StopWatch, datetime_to_cmf, timerange


__version__ = '2.0.0b6'

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


