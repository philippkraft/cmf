# -*- coding=utf-8 -*-


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
""" Loads a shapefile from disk into memory, the features are very simple 
Dependencies: 
 - shapely http://pypi.python.org/pypi/Shapely
 - dbf (Dbfreader with dynamic class creation, should be distributed with geos_shpreader)
"""
from __future__ import print_function, division
import struct
import os
from collections import namedtuple

try:
    from shapely import geometry
except ImportError:
    geometry = None

try:
    from dbfread import DBF
except ImportError:
    DBF = None


class Shapefile:
    """ The shapefile class, create it from a .shp file

    The shapefile class implements most of the sequence protocol
    Usage:
    shp=Shapefile('theshapes.shp') # Load the shape file
    print(shp[0].shape.area)        # Print area of shape
    print(shp[0].Name)              # Prints the name of the shape, assuming theshapes.dbf has a field 'Name'
    """
    @staticmethod
    def __readfmt(fmt, f):
        """ internal function to read a format fmt from file f """
        return struct.unpack(fmt, f.read(struct.calcsize(fmt)))

    def readheader(self, f):
        """ reads the header of a shape file (see ESRI Shapefile Whitepaper
        http://www.esri.com/library/whitepapers/pdfs/shapefile.pdf)"""
        fmt_begin = '>i5i'
        _ = self.__readfmt(fmt_begin, f)
        self.file_size = self.__readfmt('>i', f)[0] * 2
        self.version = self.__readfmt('<i', f)
        self.type = self.__readfmt('<i', f)
        bbox = self.__readfmt('<4d', f)
        self.boundingbox = bbox  # cmf.BoundingBox(bbox[0],bbox[1],bbox[2],bbox[3])
        self.zRange = self.__readfmt('<2d', f)
        self.mRange = self.__readfmt('<2d', f)

    def readrecord(self, f, coordinatedigits):
        """ Reads a record from the shapefile see: http://www.esri.com/library/whitepapers/pdfs/shapefile.pdf """
        # get Id and Length
        number, length = self.__readfmt('>ii', f)
        # get type of shape
        stype = self.__readfmt('<i', f)[0]
        # Initialize output object
        obj = None

        def roundxy(x, y):
            return round(x, coordinatedigits), round(y, coordinatedigits)

        def roundxyz(x, y, z, _=None):
            return round(x, coordinatedigits), round(y, coordinatedigits), round(z, coordinatedigits)

        if stype == 0:  # Null shape
            pass

        elif stype == 1:  # Point
            c = self.__readfmt('<dd', f)
            obj = geometry.Point(*roundxy(*c))

        elif stype == 8:  # Multipoint
            _ = self.__readfmt('<4d', f)
            count, = self.__readfmt('<i', f)
            coords = []
            for i in range(count):
                c = self.__readfmt('<dd', f)
                coords.append(roundxy(*c))

            obj = geometry.MultiPoint(coords)

        elif stype == 3:  # Polyline
            _ = self.__readfmt('<4d', f)
            numparts, numpoints = self.__readfmt('<ii', f)
            partpositions = self.__readfmt('<%ii' % numparts, f)
            coords = []
            for i in range(numpoints):
                if i in partpositions:
                    coords.append([])
                c = self.__readfmt('<dd', f)
                coords[-1].append(roundxy(*c))
            obj = geometry.MultiLineString(coords)

        elif stype == 5:  # Polygon
            _ = self.__readfmt('<4d', f)
            numparts, numpoints = self.__readfmt('<ii', f)
            partpositions = self.__readfmt('<%ii' % numparts, f)
            coords = []
            for i in range(numpoints):
                if i in partpositions:
                    coords.append([])
                c = self.__readfmt('<dd', f)
                coords[-1].append(roundxy(*c))
            if len(coords) > 1:
                obj = geometry.Polygon(coords[0], coords[1:])
            else:
                obj = geometry.Polygon(coords[0])

        elif stype == 11:  # PointZ
            c = self.__readfmt('<dddd', f)
            obj = geometry.Point(*roundxyz(*c))

        elif stype == 18:  # MultiPointZ
            _ = self.__readfmt('<4d', f)
            count, = self.__readfmt('<i', f)
            coords = []
            for i in range(count):
                c = self.__readfmt('<dddd', f)
                coords.append(roundxyz(*c))
            obj = geometry.MultiPoint(coords)

        elif stype == 13:  # PolylineZ
            _ = self.__readfmt('<4d', f)
            numparts, numpoints = self.__readfmt('<ii', f)
            partpositions = self.__readfmt('<%ii' % numparts, f)
            points = []
            for i in range(numpoints):
                c = self.__readfmt('<dd', f)
                points.append(roundxy(*c) + (0.0,))
            _ = self.__readfmt('<dd', f)
            z_s = self.__readfmt('<%id' % numpoints, f)
            _ = self.__readfmt('<dd', f)
            _ = self.__readfmt('<%id' % numpoints, f)
            coords = []
            for i, p in enumerate(points):
                if i in partpositions:
                    coords.append([])
                coords[-1].append((p[0], p[1], z_s[i]))
            obj = geometry.MultiLineString(coords)

        elif stype == 15:  # PolygonZ
            _ = self.__readfmt('<4d', f)
            numparts, numpoints = self.__readfmt('<ii', f)
            partpositions = self.__readfmt('<%ii' % numparts, f)
            points = []
            for i in range(numpoints):
                c = self.__readfmt('<dd', f)
                points.append(roundxy(*c) + (0.0,))
            _ = self.__readfmt('<dd', f)
            z_s = self.__readfmt('<%id' % numpoints, f)
            _ = self.__readfmt('<dd', f)
            _ = self.__readfmt('<%id' % numpoints, f)
            coords = []
            for i, p in enumerate(points):
                if i in partpositions:
                    coords.append([])
                coords[-1].append((p[0], p[1], z_s[i]))
            if len(coords) == 1:
                obj = geometry.Polygon(coords[0])
            else:
                obj = geometry.Polygon(coords[0], coords[1:])
        else:
            raise ValueError("Shape file object #%i: shape type %i not implemented!" % (number, stype))

        return number, obj

    def __init__(self, filename, coordinatedigits=12):
        """ Loads a shapefile from a filename"""

        missing_packages = []
        if not DBF:
            missing_packages.append('dbfreader')
        if not geometry:
            missing_packages.append('shapely')
        if missing_packages:
            raise ImportError('You need to install the {} package(s) shapefiles'.format(', '.join(missing_packages)))

        if os.path.exists(filename + '.shp'):
            filename += '.shp'

        dbffile = filename.replace('.shp', '.dbf')
        # read encoding from .cpg file
        cpgfile = filename.replace('.shp', '.cpg')
        encoding = open(cpgfile).read().strip() if os.path.exists(cpgfile) else 'latin-1'

        self.file_size = 0
        self.version = 0
        self.type = 0
        self.boundingbox = (0, 0, 0, 0)
        self.zRange = (0, 0)
        self.mRange = (0, 0)

        with open(filename, 'rb') as f:
            self.readheader(f)
            self.__data = []
            shpname = os.path.basename(filename).replace('.', '_')
            dbf = DBF(dbffile, encoding=encoding)
            self.fields = [field.name for field in dbf.fields]
            TYPE = namedtuple(shpname, ['shape', 'OID'] + self.fields)
            for dbfrecord in dbf:
                assert f.tell() < self.file_size, 'Reached end of .shp file before all .dbf records are read'
                number = 0
                try:

                    number, obj = self.readrecord(f, coordinatedigits)
                    self.__data.append(TYPE(obj, number, *dbfrecord.values()))

                except ValueError:
                    print("Last imported object number:", number)
                    raise

                except Exception:
                    print("file pos=%i, file size=%i" % (f.tell(), self.file_size))
                    raise

    def shapes(self):
        """ Returns a list of shapes """
        return [rec.shape for rec in self]

    # Sequence protocol
    def __getitem__(self, index):
        return self.__data[index]

    def __iter__(self):
        return iter(self.__data)

    def __len__(self):
        return len(self.__data)
