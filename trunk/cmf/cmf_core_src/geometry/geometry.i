

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

%echo "Entering geometry.i"
%{
// Include geometry
#include "geometry/geometry.h"
%}
//%attribute(cmf::geometry::point_vector,cmf::math::num_array,x,get_x);
//%attribute(cmf::geometry::point_vector,cmf::math::num_array,y,get_y);
//%attribute(cmf::geometry::point_vector,cmf::math::num_array,z,get_z);
// Geometry.h
%include "geometry/geometry.h"

%extend cmf::geometry::point {
    inline int __len__() const { return 3; }
    cmf::geometry::point __rmul__(double val)    {
        return val*(*$self);
    }
    cmf::geometry::point __rdiv__(double val)    {
        return val/(*$self);
    }


    %pythoncode 
    {
    def __getitem__(self,index) :
        if isinstance(index,slice):
            return [self[i] for i in range(*index.indices(len(self)))]
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
static bool check_seq_point(PyObject* input) {
  if (PySequence_Check(input)) {
    Py_ssize_t len = PySequence_Length(input);
    if (len==2 || len==3) 
      return true;
  }
  return false;
}
static std::string convert_seq_to_point(PyObject* input,cmf::geometry::point& p)
{
  double temp[3];
  int len=PyObject_Length(input);
  if (len<2) {
    std::string res = "Sequence <";
    res+=PyObject_REPR(input);
    res+="> has to few items to convert to cmf.point. At least to items are needed";
    return res;
  } else if (len>3) {
    std::string res = "Sequence <";
    res+=PyObject_REPR(input);
    res+="> has to many items (more than 3) to convert to cmf.point.";
    return res;
  }

  for (int i=0;i<len;++i) {
    PyObject *o = PySequence_GetItem(input,i);
    if (!PyNumber_Check(o)) {
      std::string res="Can't convert <";
      res+=PyObject_REPR(o);
      res+="> to point coordinate";
      Py_XDECREF(o);
      return res;
    }
    temp[i] = PyFloat_AsDouble(o);
    Py_DECREF(o);
  }
  p.x = temp[0];
  p.y = temp[1];
  p.z = len==3 ? temp[2] : 0.0;
  return "";
}

static bool check_xy(PyObject* input) {
  return PyObject_HasAttrString(input,"x")
    &&	 PyObject_HasAttrString(input,"y");
}
// Helper function to convert any class with an x,y,z (z optional) attribute to a point (with check)
static std::string convert_xyz_to_point(PyObject* input,cmf::geometry::point& p)
{
  PyObject
    *x= PyObject_GetAttrString(input, "x"),
    *y= PyObject_GetAttrString(input, "y"),
    *z= PyObject_HasAttrString(input, "z") ? PyObject_GetAttrString(input, "z") : 0;
  if (PyNumber_Check(x) && PyNumber_Check(y))  {
    p.x=PyFloat_AsDouble(x);
    p.y=PyFloat_AsDouble(y);
    p.z=0.0;
  } else {
    std::string res="Can't convert <";
    res += PyObject_REPR(x);
    res += "> to x coordinate or <";
    res += PyObject_REPR(y);
    res += "> to y coordinate";
    Py_DECREF(x);Py_DECREF(y);Py_XDECREF(z);
    return res;
  }
  if (z && PyNumber_Check(z)) {
    p.z=PyFloat_AsDouble(z);
  } 
  Py_DECREF(x);Py_DECREF(z);Py_XDECREF(z);
  return "";
}

%}

// typemap to convert python object to point
%typemap(in) cmf::geometry::point {
    if (check_xy($input)) {
      std::string res = convert_xyz_to_point($input,$1);
      if (res.size()) {
        SWIG_exception_fail(SWIG_ValueError,res.c_str());
        return NULL;
      }
    } 
    else if (PySequence_Check($input)) {
      std::string res = convert_seq_to_point($input,$1);
      if (res.size()) {
        SWIG_exception_fail(SWIG_ValueError, res.c_str());
      }
    } else {
      std::string res="<";
      res+=PyObject_REPR($input);
      res+="> has to be a cmf.point, a sequence, or any object with x and y (optional z) attributes";
      SWIG_exception_fail(SWIG_ValueError,res.c_str());
    }
}
%typemap(typecheck,precedence=150) cmf::geometry::point {
	cmf::geometry::point p;
	$1=check_xy($input) || check_seq_point($input);
}

// typemap to convert python object to point
%typemap(in) cmf::geometry::point* (cmf::geometry::point p) {
  if (SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, SWIG_POINTER_EXCEPTION) == -1) {
    if (check_xy($input)) {
      std::string res = convert_xyz_to_point($input,p);
      if (res.size()) {
        SWIG_exception_fail(SWIG_ValueError,res.c_str());
        return NULL;
      }
    } 
    else if (PySequence_Check($input)) {
      std::string res = convert_seq_to_point($input,p);
      if (res.size()) {
        SWIG_exception_fail(SWIG_ValueError, res.c_str());
      }
    } else {
      std::string res="<";
      res+=PyObject_REPR($input);
      res+="> has to be a cmf.point, a sequence, or any object with x and y (optional z) attributes";
      SWIG_exception_fail(SWIG_ValueError,res.c_str());
    }
    $1 = &p;
  }
}
%typemap(typecheck,precedence=150) cmf::geometry::point* {
	cmf::geometry::point p;
	$1=check_xy($input) || check_seq_point($input);
}


