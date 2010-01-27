

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   cmf is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
//   
%include "stl.i"
%module raster
%pythoncode {
from math import *
}
%{
#include "raster.h"
%}

%exception {
    try {
        $action
    } 
    catch (std::runtime_error& e) {
			SWIG_exception(SWIG_RuntimeError, e.what() );
		}
		catch (...) {
			SWIG_exception(SWIG_UnknownError, "unknown exception");
		}
}
%template(double_vector) std::vector<double>;
%template(float_vector) std::vector<float>;
%template(int_vector) std::vector<int>;

// Raster.h
 
%nodefaultctor raster::Raster;
%newobject raster::Raster::ToInt();
%newobject raster::Raster::ToDouble();
%newobject raster::Raster::ToFloat();

%include "raster.h"

%template(double_raster) raster::Raster<double>;
%template(int_raster) raster::Raster<int>;
%template(single_raster) raster::Raster<float>;
%extend raster::Histogram
{
	%pythoncode
	{
    def __getitem__(self,index):
        if index<0 : index=self.size()+index
        if index<0 or index>=self.size() : raise IndexError("Bar of histogram not available")
        return (self.barcenter(index),self.frequency(index))
    def __len__(self):
        return self.size()
	}
}

%pythoncode {
    try:
        import numpy
    except ImportError:
        numpy=None
    try:
        import matplotlib.pyplot as pyplot
    except ImportError:
        pyplot=None
    class Raster:
        def __init__(self,filename=None,dtype="f",shape=None,corner=(0,0),cellsize=(1,1),NoData=-9999,raster=None):
            """ Creates a raster
               filename = The filename of an raster file in the ESRI ASCII format
               dtype = shortcut for the data type, either 'f' for float, 's' for float32 or 'i' for integer (32 bit)
            The next parameters have no meaning, if a filename was given
               shape = the dimensins of the raster, a tuple of integers (rows,columns)
               corner = (x,y) the position of the lower left corner in world coordinates
               cellsize = the cellsize in map units, can be a tuple with one vlaue for x direction and another for the y direction
               NoData = Value to indicate no data
            The raster keyword is only for internal usage
            """
            self.dtype=dtype
            if (raster):
                self.raster=raster
                return
            if (dtype in ["f","f8"]):
                rtype=double_raster
            elif (dtype in ["s","f4"]):
                rtype=single_raster
            elif (dtype in ["i","i4"]):
                rtype=int_raster
            else:
                raise ValueError("Data type most be f,s or i")
            if (filename):
                self.raster=rtype(filename)
            else:
                try:
                    row,col=shape
                except:
                    raise ValueError("If no filename is given the dimensions of the raster has to be specified as a tuple, e.g. (10,10)")
                try:
                    x_size,y_size=cellsize
                except:
                    x_size=y_size=cellsize
                self.raster=rtype(col,row,corner[0],corner[1],x_size,y_size,NoData,NoData)
        @property
        def llcorner(self):
            "The lower left corner (x,y)"
            return (self.raster.Xllcorner(),self.raster.Yllcorner())
        @property
        def cellsize(self):
            "The cell size (x,y) in map coordinates"
            return (self.raster.XCellsize(),self.raster.YCellsize())
        @property
        def shape(self):
            "The tuple (rows,cols)"
            return (self.raster.RowCount(),self.raster.ColumnCount())
        @property
        def extent(self):
            "The extent of the raster (width,height) in map coordinates"
            return (self.cellsize[0]*self.shape[1],
                    self.cellsize[1]*self.shape[0])
        def neighbors(self,x,y):
            """ Returns a list of the neighbors to the given position
            x,y are intepreted as real coordinates, if they are floating point numbers,
                otherwise they are interpreted as column and row of the raster
            Result: A tuple for each adjacent neighbor of type (column,row,value,distance,x_direction,y_direction)
            """
            if type(x) is float or type(y) is float:
                col,row=colrow(x,y)
            else:
                col,row=int(x),int(y)
            x_pos=y_pos=[-1,0,1]
            res=[]
            for y in y_pos:
                for x in x_pos:
                    v=self[col+x,row+y]
                    if (x or y) and v!=self.nodata:   # Exclude the cell itself and nodata cells
                        distance=sqrt((x*self.cellsize[0])**2+(y*self.cellsize[1])**2)
                        res.append((col+x,row+y,v,distance,x,y))
            return res
        def xy(self,col,row):
            "Gets a position in world coordinates from a position in the dataset (col,row)->(x,y)"
            return (col*self.cellsize[0]+self.llcorner[0],(self.shape[0]-row)*self.cellsize[1]+self.llcorner[1])
        def colrow(self,x,y):
            "Gets a position in the dataset from world coordinates (x,y)->(colr,row)"
            return (int((x-self.corner[0])/self.cellsize[0]),int(self.shape[0]-(y-self.corner[1])/self.cellsize[1]))
        @property
        def nodata(self):
            "Gets the no data value of the raster"
            return self.raster.NoData()
        @property
        def rows(self):
            "Gets the number of rows in the data set"
            for r in range(self.shape[0]):
                yield [self.raster.GetData(r,c) for c in range(self.shape[1])]
        @property
        def cells(self):
            """ Returns an iterator over all cells of the raster, that returns the tuple (x,y,value,area,column,row) """
            for r in range(self.shape[0]):
                for c in range(self.shape[1]):
                    if self[c,r]!=self.nodata:
                        x=self.llcorner[0]+self.cellsize[0]*(c+0.5)
                        y=self.llcorner[1]+self.cellsize[1]*(self.shape[0]-(r+0.5))
                        v=self[c,r]
                        area=self.cellsize[0]*self.cellsize[1]
                        yield((x,y,v,area,c,r))
        def __iter__(self):
            for r in range(self.shape[0]):
                for c in range(self.shape[1]):
                    yield c,r
        @property 
        def values(self):
            """Returns a dictionary containing all distinct values and the number of their appearances """
            res={}
            for c,r in self:
                v=self[c,r]
                if v==self.nodata: continue
                if v in res:
                    res[v]+=1
                else:
                    res[v]=1
            return res
        def __getitem__(self,pos):
            if hasattr(pos,"__len__"):
                if (len(pos)==2):
                    return self.raster.GetData(*pos)
            raise ValueError("Only two coordinates, x and y can be used for __getitem__")
        def __setitem__(self,pos,value):
            if hasattr(pos,"__len__"):
                if (len(pos)==2):
                    return self.raster.SetData(pos[0],pos[1],value)
            raise ValueError("Only two coordinates, x and y can be used for __getitem__")
        def __call__(self,x,y,z=0):
            x=float(x) if type(x) is int else x
            y=float(y) if type(y) is int else y
            v=self.raster.GetData(x,y)
            return v if not v==self.nodata else None
        def histogram(self,bins=100):
            return self.raster.histogram(bins)
        @property
        def statistics(self):
            return self.raster.statistics()
        def save(self,filename,binary=False):
            if binary:
                self.raster.WriteToBinary(filename)
            else:
                self.raster.WriteToASCFile(filename)
        def focal_min(self,n=3):
            """ Focal minimum function: each cell of the new raster gets the minimum value of the surrounding n x n matrix"""
            res=self.raster.focal_min(n)
            return Raster(dtype=self.dtype,raster=res)
        def focal_max(self,n=3):
            """ Focal maximum function: each cell of the new raster gets the maximum value of the surrounding n x n matrix"""
            res=self.raster.focal_max(n)
            return Raster(self,raster=res)
        def focal_mean(self,n=3):
            """ Focal mean function: each cell of the new raster gets the mean value of the surrounding n x n matrix"""
            res=self.raster.focal_mean(n)
            return Raster(dtype=self.dtype,raster=res)
        def focal_stdev(self,n=3):
            """ Focal standard deviation function: each cell of the new raster gets the standard deviation value of the surrounding n x n matrix"""
            res=self.raster.focal_stdev(n)
            return Raster(dtype=self.dtype,raster=res)
        def focal_majority(self,n=3):
            """ Focal majority function: each cell of the new raster gets the most frequent value of the surrounding n x n matrix. 
                If each value is unique in the matrix, the result is the old value"""
            res=self.raster.focal_majority(n)
            return Raster(dtype=self.dtype,raster=res)
        def focal_mean_difference(self,n=3):
            """ Optimised shortcut for =r-r.focal_mean(n) """
            res=self.raster.focal_mean_difference(n)
            return Raster(dtype=self.dtype,raster=res)
        def to_int(self):
            """ Creates a new raster of type integer from self (int 32)"""
            res=self.raster.ToInt()
            return Raster(dtype=self.dtype,raster=res)
        def to_double(self):
            """ Creates a new raster of type double (floating point 64) from self """
            res=self.raster.ToDouble()
            return Raster(dtype=self.dtype,raster=res)
        def to_single(self):
            """ Creates a new raster of type single (floating point 32) from self """
            res=self.raster.ToSingle()
            return Raster(dtype=self.dtype,raster=res)
        def clone(self):
            """ Creates a copy of this raster """
            res=self.raster.clone()
            return Raster(dtype=self.dtype,raster=res)
        def __iadd__(self,other):
            if type(other) is type(self):
                self.raster+=other.raster
            else:
                try:
                   self.raster+=other
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __imul__(self,other):
            if type(other) is type(self):
                self.raster*=other.raster
            else:
                try:
                   self.raster*=other
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __isub__(self,other):
            if type(other) is type(self):
                self.raster-=other.raster
            else:
                try:
                   self.raster-=other
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __idiv__(self,other):
            if type(other) is type(self):
                self.raster/=other.raster
            else:
                try:
                   self.raster/=other
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __add__(self,other):
            if type(other) is type(self):
                return Raster(dtype=self.dtype,raster=self.raster+other.raster)
            else:
                try:
                   return Raster(dtype=self.dtype,raster=self.raster+other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __mul__(self,other):
            if type(other) is type(self):
                return Raster(dtype=self.dtype,raster=self.raster*other.raster)
            else:
                try:
                   return Raster(dtype=self.dtype,raster=self.raster*other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __sub__(self,other):
            if type(other) is type(self):
                return Raster(dtype=self.dtype,raster=self.raster-other.raster)
            else:
                try:
                   return Raster(dtype=self.dtype,raster=self.raster-other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __div__(self,other):
            if type(other) is type(self):
                return Raster(dtype=self.dtype,raster=self.raster/other.raster)
            else:
                try:
                   return Raster(dtype=self.dtype,raster=self.raster/other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __radd__(self,other):
            if type(other) is type(self):
                return Raster(dtype=self.dtype,raster=self.raster+other.raster)
            else:
                try:
                   return Raster(dtype=self.dtype,raster=self.raster+other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __rmul__(self,other):
            if type(other) is type(self):
                return Raster(dtype=self.dtype,raster=self.raster*other.raster)
            else:
                try:
                   return Raster(dtype=self.dtype,raster=self.raster*other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __rsub__(self,other):
            if type(other) is type(self):
                return Raster(dtype=self.dtype,raster=self.raster-other.raster)
            else:
                try:
                   return Raster(dtype=self.dtype,raster=self.raster-other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __rdiv__(self,other):
            if type(other) is type(self):
                return Raster(dtype=self.dtype,raster=self.raster/other.raster)
            else:
                try:
                   return Raster(dtype=self.dtype,raster=self.raster/other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        
        @property
        def __array_interface__(self):
            "Returns the array interface for the raster."
            types={'i':'|i4','f':'|f8','s':'|f4'}
            return dict(shape=self.shape,data=(self.raster.adress(),0),typestr=types[self.dtype])
        def as_array(self):
            if numpy:                
                res=numpy.asarray(self)
                if self.dtype=='i':
                    mres=numpy.ma.masked_equal(res,self.nodata)
                else:
                    mres=numpy.ma.masked_values(res,self.nodata)
                return mres
            else:
                raise NotImplementedError("as_array needs an installation of numpy to work!")
        def draw(self,cmap=None,vmin=None,vmax=None,hold=1,interpolation='nearest',**kwargs):
            if pyplot:
                if cmap is None:
                    cmap=pyplot.cm.jet
                if vmin is None:
                    vmin=self.statistics.min
                if vmax is None:
                    vmax=self.statistics.max
                pyplot.imshow(self.as_array(),cmap,interpolation=interpolation,
                              aspect='equal',vmin=vmin,vmax=vmax,hold=hold,
                              extent=(self.llcorner[0],self.llcorner[0]+self.extent[0],self.llcorner[1],self.llcorner[1]+self.extent[1]),
                              **kwargs)
            else:
                raise NotImplementedError("draw needs an installation of matplotlib to work")
        def __repr__(self):
            stat=self.statistics
            fmt="Raster<%s>: n=%i,min=%g,mean=%g,max=%g,stdev=%g,row,col=%s,cellsize=(%g,%g)"
            return fmt % (self.dtype,stat.count,stat.min,stat.mean,stat.max,stat.stdev,
                          self.shape,self.cellsize[0],self.cellsize[1])

}


