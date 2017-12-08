

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

from .maps import Map, nearest_neighbor_map, raster_map


from .cell_factory import project_from_dem, create_reaches_for_cells


from .extend_project import add_layers_to_cells
from .extend_project import change_vegetation, connect_cells_with_flux
from .extend_project import profile
from .stopwatch import StopWatch

__version__ = '1.0.4a'
