

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
try:
    import shapely
    from shapely.geometry import Polygon,MultiPolygon,Point,LineString,MultiLineString,MultiPoint,GeometryCollection
    has_shapely = True
except ImportError:
    print "Could not found shapely. Creating cells from polygons is disabled"
    has_shapely= False
from cmf_core import *
from cell_factory import create_reaches,cells_from_dem
from maps import Map,nearest_neighbor_map,raster_map

if has_shapely:
    from cell_factory import cells_from_polygons
    from geos_shapereader import shapefile
    from maps import polygon_map

from cell_factory import project_from_dem,create_reaches_for_cells


#from raster import Raster
from extend_project import add_layers_to_cells
from extend_project import change_vegetation,connect_cells_with_flux
from extend_project import profile
from stopwatch import StopWatch
import draw