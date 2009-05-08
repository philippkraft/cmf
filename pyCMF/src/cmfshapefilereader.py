# -*- coding=utf-8 -*-
""" Loads a shapefile from disk into memory, the features are very simple """
import dbf
import struct
from math import *
import cmf
    
class shapefile:
    """ The shapefile class, create it from a .shp file"""
    def __readfmt(self,fmt,f):
        return struct.unpack(fmt,f.read(struct.calcsize(fmt)))        
    def readHeader(self,f):
        fmt_begin='>i5ii'
        fmt_Version_Type_MBR_zrange_mrange='<ii4d2d2d'
        begin=self.__readfmt(fmt_begin, f)
        #vtmzm=self.__readfmt(fmt_Version_Type_MBR_zrange_mrange, f)
        self.version=self.__readfmt('<i', f)
        self.type=self.__readfmt('<i',f)
        bbox=self.__readfmt('<4d',f)
        self.boundingbox=cmf.BoundingBox(bbox[0],bbox[1],bbox[2],bbox[3]) 
        self.zRange=self.__readfmt('<2d',f)
        self.mRange=self.__readfmt('<2d',f)
    def readRecord(self,f):
        number,length=self.__readfmt('>ii', f)
        type=self.__readfmt('<i', f)[0]
        obj=None
        if type==0: # Null shape
            pass
        elif type==1: # Point 
            x,y=self.__readfmt('<dd', f)
            obj=cmf.point(x,y)
        elif type==8: # Multipoint
            bbox=self.__readfmt('<4d', f)
            count=self.__readfmt('<i', f)
            obj=cmf.MultiPoint()
            for i in range(count):
                x,y=self.__readfmt('<dd', f)
                obj.AddPoint(x,y)
        elif type==3: # Polyline
            bbox=self.__readfmt('<4d', f)
            numparts,numpoints=self.__readfmt('<ii', f)
            partpositions=self.__readfmt('<%ii' % numparts, f)
            obj=cmf.PolyLine()
            for i in range(numpoints):
                if i in partpositions:
                    part=obj.AddPart()
                x,y=self.__readfmt('<dd', f)
                part.AddPoint(x,y)
        elif type==5: # Polygon
            bbox=self.__readfmt('<4d', f)
            numparts,numpoints=self.__readfmt('<ii', f)
            partpositions=self.__readfmt('<%ii' % numparts, f)
            obj=cmf.Polygon()
            for i in range(numpoints):
                if i in partpositions:
                    part=obj.AddPart()
                x,y=self.__readfmt('<dd', f)
                part.AddPoint(x, y)
        elif type==11: # PointZ
            x,y,z,m=self.__readfmt('<dddd',f)
            obj=cmf.point(x,y,z)
        elif type==18 : #MultiPointZ
            bbox=self.__readfmt('<4d', f)
            count=self.__readfmt('<i', f)
            obj=cmf.MultiPoint()
            for i in range(count):
                x,y,z,m=self.__readfmt('<dddd', f)
                obj.AddPoint(x,y,z)
        elif type==13: # PolylineZ
            bbox=self.__readfmt('<4d', f)
            numparts,numpoints=self.__readfmt('<ii', f)
            partpositions=self.__readfmt('<%ii' % numparts, f)
            obj=cmf.PolyLine()
            for i in range(numpoints):
                if i in partpositions:
                    part=obj.AddPart()
                x,y,z,m=self.__readfmt('<dddd', f)
                part.AddPoint(x,y,z)
        elif type==15: # PolygonZ
            bbox=self.__readfmt('<4d', f)
            numparts,numpoints=self.__readfmt('<ii', f)
            partpositions=self.__readfmt('<%ii' % numparts, f)
            obj=cmf.Polygon()
            for i in range(numpoints):
                if i in partpositions:
                    part=obj.AddPart()
                x,y,z,m=self.__readfmt('<dddd', f)
                part.AddPoint(x, y,z)  
        else :
            print " Shape type %i not implemented!" % type
                
        return number,obj 
    def __init__(self,filename):
        f=file(filename,'rb')
        self.readHeader(f)
        
        shapes={}
        number,obj=self.readRecord(f)
        shapes[number]=obj
        while 1:
            try:
                number,obj=self.readRecord(f)
                shapes[number]=obj
            except :
                break
        f.close()
        self.__data=dbf.dbfclass('attributes', filename.lower().replace('.shp','.dbf'))
        for i,rec in enumerate(self.__data):
            rec.shape=shapes[i+1]
    def shapes(self):
        return [rec.shape for rec in self]
    def __getitem__(self,index):
        return self.__data[index]
    def __iter__(self):
        return iter(self.__data)
    def __len__(self):
        return len(self.__data)
        
        

