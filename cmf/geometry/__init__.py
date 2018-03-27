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

"""
cmf.geometry deals with the geometry of cmf objects. In many cases
users do not need to care about the geometry in cmf and therefore
can ignore this package. Even in a fully distributed model, it is
not the geometry that counts, but the topology of the objects.
However, topological information can be derived from geometric structure,
and this is what this module is for.

Depends on shapely
"""


try:

    from . import geocell as __geocell
    from .geocell import create_cell, mesh_project

except ImportError:

    raise ImportError('cmf.geometry is only available if shapely is installed')

else:

    __geocell.add_geometry_property()

