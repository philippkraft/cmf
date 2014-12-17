

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
import os
from distutils.core import setup,Extension

files=['raster_src/raster.i']

raster = Extension('_raster',
                        sources=files,
                        extra_compile_args=["/openmp","/EHsc"],
                        swig_opts=['-c++','-Wextra','-w512','-w511','-keyword','-castmode','-O'],
                    )



setup(name='raster',version='1.0',ext_modules=[raster],py_modules=['raster','__init__'])

