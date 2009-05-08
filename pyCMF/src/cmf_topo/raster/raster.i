%include "stl.i"
%module raster
%{
// Include geometry
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
 
%nodefaultctor cmf::geometry::Raster;
%newobject cmf::geometry::Raster::ToInt();
%newobject cmf::geometry::Raster::ToDouble();
%newobject cmf::geometry::Raster::ToFloat();

%include "raster.h"


%template(double_raster) cmf::geometry::Raster<double>;
%template(int_raster) cmf::geometry::Raster<int>;
%template(single_raster) cmf::geometry::Raster<float>;
%extend cmf::geometry::Histogram
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
    class Raster:
        def __init__(self,filename=None,dtype="f",shape=None,corner=(0,0),cellsize=(1,1),NoData=-9999,raster=None):
            if (raster):
                self.raster=raster
                return
            if (dtype=="f"):
                rtype=double_raster
            elif (dtype=="s"):
                rtype=single_raster
            elif (dtype=="i"):
                rtype=int_raster
            if (filename):
                self.raster=rtype(filename)
            else:
                try:
                    col,row=shape
                except:
                    raise ValueError("If no filename is given the dimensions of the raster has to be specified as a tuple, e.g. (10,10)")
                try:
                    x_size,y_size=cellsize
                except:
                    x_size=y_size=cellsize
                self.raster=rtype(col,row,corner[0],corner[1],x_size,y_size,NoData,NoData)
        @property
        def llcorner(self):
            return (self.raster.Xllcorner(),self.raster.Yllcorner())
        @property
        def cellsize(self):
            return (self.raster.XCellsize(),self.raster.YCellsize())
        @property
        def shape(self):
            return (self.raster.ColumnCount(),self.raster.RowCount())
        @property
        def extent(self):
            return (self.raster.cellsize[0]*self.raster.shape[0],
                    self.raster.cellsize[1]*self.raster.shape[1])
        def neighbors(self,x,y):
            """ Returns a list of the neighbors to the given position
            x,y are intepreted as real coordinates, if they are floating point numbers,
                otherwise they are interpreted as column and row of the raster
            Result: A tuple for each adjacent neighbor of type (column,row,value,distance
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
                    if (x or y) and v!=self.nodata:
                        res.append((col+x,row+y,v,sqrt((x*self.cellsize[0])**2+(y*self.cellsize[1]**2)))
            return res
        def xy(self,col,row):
            return (col*self.cellsize[0]+self.corner[0],(self.shape[1]-row)*self.cellsize[1]+self.corner[1])
        def colrow(self,x,y):
            return (int((x-self.corner[0])/self.cellsize[0]),int(self.shape[1]-(y-self.corner[1])/self.cellsize[1]))
        @property
        def nodata(self):
            return self.raster.NoData()
        @property
        def rows(self):
            for r in range(self.shape[1]):
                yield [self.raster.GetData(r,c) for c in range(self.shape[0])]
        @property
        def cells(self):
            """ Returns an iterator over all cells of the raster, that returns the tuple (x,y,value,area) """
            for r in range(self.shape[1]):
                for c in range(self.shape[0]):
                    if self[c,r]!=self.nodata:
                        yield((self.llcorner[0]+self.cellsize[0]*(c+0.5),self.llcorner[1]+self.cellsize[1]*(self.shape[1]-(r+0.5)),self[c,r],self.cellsize[0]*self.cellsize[1]))
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
        def __call__(self,x,y):
            x=float(x) if type(x) is int else x
            y=float(y) if type(y) is int else y
            return self.raster.GetData(x,y)
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
            return Raster(raster=res)
        def focal_max(self,n=3):
            """ Focal maximum function: each cell of the new raster gets the maximum value of the surrounding n x n matrix"""
            res=self.raster.focal_max(n)
            return Raster(self,raster=res)
        def focal_mean(self,n=3):
            """ Focal mean function: each cell of the new raster gets the mean value of the surrounding n x n matrix"""
            res=self.raster.focal_mean(n)
            return Raster(raster=res)
        def focal_stdev(self,n=3):
            """ Focal standard deviation function: each cell of the new raster gets the standard deviation value of the surrounding n x n matrix"""
            res=self.raster.focal_stdev(n)
            return Raster(raster=res)
        def focal_majority(self,n=3):
            """ Focal majority function: each cell of the new raster gets the most frequent value of the surrounding n x n matrix. 
                If each value is unique in the matrix, the result is the old value"""
            res=self.raster.focal_majority(n)
            return Raster(raster=res)
        def focal_mean_difference(self,n=3):
            """ Optimised shortcut for =r-r.focal_mean(n) """
            res=self.raster.focal_mean_difference(n)
            return Raster(raster=res)
        def to_int(self):
            """ Creates a new raster of type integer from self (int 32)"""
            res=self.raster.ToInt()
            return Raster(raster=res)
        def to_double(self):
            """ Creates a new raster of type double (floating point 64) from self """
            res=self.raster.ToDouble()
            return Raster(raster=res)
        def to_single(self):
            """ Creates a new raster of type single (floating point 32) from self """
            res=self.raster.ToSingle()
            return Raster(raster=res)
        def clone(self):
            """ Creates a copy of this raster """
            res=self.raster.clone()
            return Raster(raster=res)
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
                res=self.raster+other.raster
                return Raster(raster=res)
            else:
                try:
                   res=self.raster+other
                   return Raster(raster=res)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __mul__(self,other):
            if type(other) is type(self):
                return Raster(raster=self.raster*other.raster)
            else:
                try:
                   return Raster(raster=self.raster*other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __sub__(self,other):
            if type(other) is type(self):
                return Raster(raster=self.raster-other.raster)
            else:
                try:
                   return Raster(raster=self.raster-other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __div__(self,other):
            if type(other) is type(self):
                return Raster(raster=self.raster/other.raster)
            else:
                try:
                   return Raster(raster=self.raster/other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __radd__(self,other):
            if type(other) is type(self):
                return Raster(raster=self.raster+other.raster)
            else:
                try:
                   return Raster(raster=self.raster+other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __rmul__(self,other):
            if type(other) is type(self):
                return Raster(raster=self.raster*other.raster)
            else:
                try:
                   return Raster(raster=self.raster*other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __rsub__(self,other):
            if type(other) is type(self):
                return Raster(raster=self.raster-other.raster)
            else:
                try:
                   return Raster(raster=self.raster-other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
        def __rdiv__(self,other):
            if type(other) is type(self):
                return Raster(raster=self.raster/other.raster)
            else:
                try:
                   return Raster(raster=self.raster/other)
                except TypeError:
                   raise TypeError("Can't add a %s to this raster %s" % (typename(other),typename(self.raster)))
}


