%{
// Include geometry
#include "geometry/geometry.h"
#include "geometry/Raster.h"
%}

// Geometry.h
%include "geometry/geometry.h"
%extend cmf::geometry::point {
    inline int __len__() const { return 3; }

    %pythoncode 
    {
    def __getitem__(self,index) :
        if isinstance(index,slice):
            return [self[i] for i in range(*index.indices(len(p)))]
        if index==0 :
            return self.x
        elif index==1 :
            return self.y
        elif index==2 :
            return self.z
        else :
            raise IndexError("Only Index 0 .. 2 are allowed")
    def __setitem__(self,index,value) :
        if index==0 :
            self.x=value
        elif index==1 :
            self.y=value
        elif index==2 :
            self.z=value
        else :
            raise IndexError("Only Index 0 .. 2 are allowed")
    def __iter__(self):
        yield self.x
        yield self.y
        yield self.z
    def __repr__(self):
        return 'cmf.point(%g,%g,%g)' % (self.x,self.y,self.z)
	}
}

// Create helper function for mapping a tuple to a point
%{
static int convert_seq_to_point(PyObject* input,cmf::geometry::point& p)
{
	double temp[3];
	if (!PySequence_Check(input)) 
	{
		PyErr_SetString(PyExc_TypeError,"Expecting a sequence or point");
		return 0;
	}
	int len=PyObject_Length(input);
	if (len<2 || len>3)
	{
		PyErr_SetString(PyExc_ValueError,"sequence must have at 2 or 3 elements to be used as point");
		return 0;
	}
	for (int i=0;i<len;++i)
	{
		PyObject *o = PySequence_GetItem(input,i);
		if (!PyFloat_Check(o)) 
		{
			Py_XDECREF(o);
			PyErr_SetString(PyExc_ValueError,"Expecting a sequence of floats");
			return 0;
		}
		temp[i] = PyFloat_AsDouble(o);
		Py_DECREF(o);
	}
	p.x = temp[0];
	p.y = temp[1];
	p.z = len==3 ? temp[2] : 0.0;
	return 1;
}
%}
%typemap(in) cmf::geometry::point {
	cmf::geometry::point p;
	if (!convert_seq_to_point($input,p))
	{
		return 0;
	}
	else $1=p;
}
%typemap(typecheck) cmf::geometry::point {
	cmf::geometry::point p;
	$1=convert_seq_to_point($input,p);
}


%extend cmf::geometry::MultiPoint {
	inline size_t __len__() const { return $self->size();}
	inline cmf::geometry::point& __getitem__(int index)
	{
		return (*$self)[index];
	}
}
%extend cmf::geometry::Line {
	inline size_t __len__() const { return $self->size();}
	inline cmf::geometry::point& __getitem__(int index) 
	{
		return (*$self)[index];
	}
}
%extend cmf::geometry::Ring {
	inline size_t __len__() const { return $self->size();}
	inline cmf::geometry::point& __getitem__(int index)
	{
		return (*$self)[index];
	}
	%pythoncode
	{
    def __contains___(self,p):
        return self.Includes(p)
	}	
}
%extend cmf::geometry::PolyLine {
	inline size_t __len__() const { return $self->size();}
	inline cmf::geometry::Line& __getitem__(int index)
	{
		return (*$self)[index];
	}
}
%extend cmf::geometry::Polygon {
	inline size_t __len__() const { return $self->size();}
	inline cmf::geometry::Ring& __getitem__(int index)
	{
		return (*$self)[index];
	}
	%pythoncode
	{
    def __contains___(self,p):
        return self.Includes(p)
	}	
}
%template(Rings) std::vector<cmf::geometry::Ring>;
%template(Lines) std::vector<cmf::geometry::Line>;
%template(Points) std::vector<cmf::geometry::point>;
%template(MultiPoints) std::vector<cmf::geometry::MultiPoint>;
%template(PolyLines) std::vector<cmf::geometry::PolyLine>;
%template(Polygons) std::vector<cmf::geometry::Polygon>;


// Raster.h
 
%nodefaultctor cmf::geometry::Raster;
%newobject cmf::geometry::Raster::ToInt();
%newobject cmf::geometry::Raster::ToDouble();
%newobject cmf::geometry::Raster::ToFloat();

%include "geometry/Raster.h"


%template(DoubleRaster) cmf::geometry::Raster<double>;
%template(IntRaster) cmf::geometry::Raster<int>;
%template(SingleRaster) cmf::geometry::Raster<float>;
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



