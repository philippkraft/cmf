# -*- coding=utf-8 -*-
""" Loads a shapefile from disk into memory, the features are very simple 
Dependencies: 
 - shapely http://pypi.python.org/pypi/Shapely
 - dbf (Dbfreader with dynamic class creation, should be distributed with geos_shpreader)
"""
import dbf
import struct
from math import *
import shapely.geometry
    
class shapefile:
    """ The shapefile class, create it from a .shp file
    
    The shapefile class implements most of the sequence protocol
    Usage:
    shp=shapefile('theshapes.shp') # Load the shape file
    print shp[0].shape.area        # Print area of shape
    print shp[0].Name              # Prints the name of the shape, assuming theshapes.dbf has a field 'Name'             
    """
    
    def __readfmt(self,fmt,f):
        """ internal function to read a format fmt from file f """
        return struct.unpack(fmt,f.read(struct.calcsize(fmt)))        
    def readHeader(self,f):
        """ reads the header of a shape file (see ESRI Shapefile Whitepaper (http://www.esri.com/library/whitepapers/pdfs/shapefile.pdf)"""
        fmt_begin='>i5i'
        begin=self.__readfmt(fmt_begin, f)
        self.file_size=self.__readfmt('>i', f)[0]*2
        #vtmzm=self.__readfmt(fmt_Version_Type_MBR_zrange_mrange, f)
        self.version=self.__readfmt('<i', f)
        self.type=self.__readfmt('<i',f)
        bbox=self.__readfmt('<4d',f)
        self.boundingbox=bbox # cmf.BoundingBox(bbox[0],bbox[1],bbox[2],bbox[3]) 
        self.zRange=self.__readfmt('<2d',f)
        self.mRange=self.__readfmt('<2d',f)
    def readRecord(self,f):
        """ Reads a record from the shapefile see: http://www.esri.com/library/whitepapers/pdfs/shapefile.pdf """
        # get Id and Length
        number,length=self.__readfmt('>ii', f)
        # get type of shape
        type=self.__readfmt('<i', f)[0]
        # Initialize output object
        obj=None
        if type==0: # Null shape
            pass
        elif type==1: # Point 
            x,y=self.__readfmt('<dd', f)
            obj=shapely.geometry.Point(x,y)
        elif type==8: # Multipoint
            bbox=self.__readfmt('<4d', f)
            count=self.__readfmt('<i', f)
            coords=[]
            for i in range(count):
                x,y=self.__readfmt('<dd', f)
                coords.append((x,y))
            obj=shapely.geometry.MultiPoint(coords)
            
            obj=shapely.geometry.MultiPoint()
        elif type==3: # Polyline
            bbox=self.__readfmt('<4d', f)
            numparts,numpoints=self.__readfmt('<ii', f)
            partpositions=self.__readfmt('<%ii' % numparts, f)
            coords=[]
            for i in range(numpoints):
                if i in partpositions:
                    coords.append([])
                x,y=self.__readfmt('<dd', f)
                coords[-1].append((x,y))
            obj=shapely.geometry.MultiLineString(coords)
        elif type==5: # Polygon
            bbox=self.__readfmt('<4d', f)
            numparts,numpoints=self.__readfmt('<ii', f)
            partpositions=self.__readfmt('<%ii' % numparts, f)
            coords=[]
            for i in range(numpoints):
                if i in partpositions:
                    coords.append([])
                x,y=self.__readfmt('<dd', f)
                coords[-1].append((x,y))
            if len(coords)>1 :
                obj=shapely.geometry.Polygon(coords[0],coords[1:])
            else:
                obj=shapely.geometry.Polygon(coords[0])
        elif type==11: # PointZ
            x,y,z,m=self.__readfmt('<dddd',f)
            obj=shapely.geometry.Point(x,y,z)
        elif type==18 : #MultiPointZ
            bbox=self.__readfmt('<4d', f)
            count=self.__readfmt('<i', f)
            coords=[]
            for i in range(count):
                x,y,z,m=self.__readfmt('<dddd', f)
                coords.append((x,y,z))
            obj=shapely.geometry.MultiPoint(coords)
        elif type==13: # PolylineZ
            bbox=self.__readfmt('<4d', f)
            numparts,numpoints=self.__readfmt('<ii', f)
            partpositions=self.__readfmt('<%ii' % numparts, f)
            for i in range(numpoints):
                x,y=self.__readfmt('<dd', f)
                points.append((x,y,0.0))
            zrange=self.__readfmt('<dd', f)
            z_s=self.__readfmt('<%id' % numpoints, f)
            m_range=self.__readfmt('<dd', f)
            m_s=self.__readfmt('<%id' % numpoints, f)
            coords=[]
            for i,p in enumerate(points):
                if i in partpositions:
                    coords.append([])
                coords[-1].append((p[0],p[1],z_s[i]))
            obj=shapely.geometry.MultiLineString(coords)
        elif type==15: # PolygonZ
            bbox=self.__readfmt('<4d', f)
            numparts,numpoints=self.__readfmt('<ii', f)
            partpositions=self.__readfmt('<%ii' % numparts, f)
            points=[]
            for i in range(numpoints):
                x,y=self.__readfmt('<dd', f)
                points.append((x,y,0.0))
            zrange=self.__readfmt('<dd', f)
            z_s=self.__readfmt('<%id' % numpoints, f)
            m_range=self.__readfmt('<dd', f)
            m_s=self.__readfmt('<%id' % numpoints, f)
            coords=[]
            for i,p in enumerate(points):
                if i in partpositions:
                    coords.append([])
                coords[-1].append((p[0],p[1],z_s[i]))
            if len(coords)==1 :
                obj=shapely.geometry.Polygon(coords[0])
            else:
                obj=shapely.geometry.Polygon(coords[0],coords[1:])
        else :
            raise ValueError( " Shape type %i not implemented!" % type)
               
        return number,obj 
    def __init__(self,filename):
        """ Loads a shapefile from a filename"""
        f=file(filename,'rb')
        self.readHeader(f)
        
        shapes={}
        number,obj=self.readRecord(f)
        shapes[number]=obj
        while f.tell()<self.file_size :
            try:
                number,obj=self.readRecord(f)
                shapes[number]=obj
            except ValueError,e:
                print e
                break
            except Exception,e:
                print "file pos=%i, file size=%i" % (f.tell(),self.file_size) 
                print e
                break
        f.close()
        self.__data=dbf.dbfclass('attributes', filename.lower().replace('.shp','.dbf'))
        for i,rec in enumerate(self.__data):
            rec.shape=shapes[i+1]
    def shapes(self):
        """ Returns a list of shapes """
        return [rec.shape for rec in self]
    
    # Sequence protocol
    def __getitem__(self,index):
        return self.__data[index]
    def __iter__(self):
        return iter(self.__data)
    def __len__(self):
        return len(self.__data)
        
        

