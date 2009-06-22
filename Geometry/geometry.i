%{
// Include geometry
#include "geometry/geometry.h"
%}

// Geometry.h
%include "geometry/geometry.h"
%extend cmf::geometry::Locatable{
    %pythoncode{
    position=property(get_position,set_position,"The position ofthe locatabe object")
    }
}
%extend cmf::geometry::point {
    inline int __len__() const { return 3; }
    cmf::geometry::point __rmul__(double val)
    {
        return val*(*$self);
    }
    cmf::geometry::point __rdiv__(double val)
    {
        return val/(*$self);
    }

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

//Create helper function for mapping a tuple to a point
%{
// Helper function to convert a sequence to a point (with check)
static int convert_seq_to_point(PyObject* input,cmf::geometry::point& p)
{
	double temp[3];
	if (!PySequence_Check(input)) 
	{
		return 0;
	}
	int len=PyObject_Length(input);
	if (len<2 || len>3)
	{
		return 0;
	}
	for (int i=0;i<len;++i)
	{
		PyObject *o = PySequence_GetItem(input,i);
		if (!PyNumber_Check(o)) 
		{
			Py_XDECREF(o);
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
static int convert_xyz_to_point(PyObject* input,cmf::geometry::point& p)
// Helper function to convert any class with an x,y,z (z optional) attribute to a point (with check)
{
    PyObject
        *x= PyObject_HasAttrString(input, "x") ? PyObject_GetAttrString(input, "x") : 0,
        *y= PyObject_HasAttrString(input, "y") ? PyObject_GetAttrString(input, "y") : 0,
        *z= PyObject_HasAttrString(input, "z") ? PyObject_GetAttrString(input, "z") : 0;
    if (x && y)
    {
        if (PyNumber_Check(x) && PyNumber_Check(y))
        {
            p.x=PyFloat_AsDouble(x);
            p.y=PyFloat_AsDouble(y);
            p.z=0.0;
        }
        else
        {
            Py_XDECREF(x);Py_XDECREF(y);Py_XDECREF(z);
            return 0;
        }
        if (z && PyNumber_Check(z)) p.z=PyFloat_AsDouble(z);
        Py_DECREF(x);Py_DECREF(z);Py_XDECREF(z);
        return 1;
    }
    else
    {
        Py_XDECREF(x);Py_XDECREF(y);Py_XDECREF(z);
        PyErr_SetString(PyExc_ValueError,"x or y attribute are missing, can't convert to point");
        return 0;
    }
    
}
%}
// typemap to convert python object to point
%typemap(in) cmf::geometry::point {
	cmf::geometry::point p;
	if (((!convert_xyz_to_point($input,p)) || !convert_seq_to_point($input,p)))
	{
		PyErr_SetString(PyExc_ValueError,"The object to convert needs to be either a sequence of length 2 or 3 or an object exposing an x and y attribute of type float(z is used if present)");
	}
	$1=p;
}
%typemap(typecheck,precedence=150) cmf::geometry::point {
	cmf::geometry::point p;
	$1=convert_seq_to_point($input,p) || convert_xyz_to_point($input,p);
}


