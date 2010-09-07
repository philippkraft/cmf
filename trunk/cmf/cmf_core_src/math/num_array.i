

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


// import_array() has to be called once to avoid nasty seg faults
%init %{ import_array(); %}

// Some useful tools to get some information
%header %{

// numpy/arrayobject.h requires this Macro-constant. Some Compilers (e.g. gcc) defines it internally, others (e.g. VS2008) not
#ifndef SIZEOF_LONG_DOUBLE
#define SIZEOF_LONG_DOUBLE sizeof(long double) 
#endif

// including numpy array definition
#include <numpy/arrayobject.h>
// including my array header
#include "math/num_array.h"

bool test_npy_array(PyObject* op) {
	PyObject* ao = PyArray_ContiguousFromAny(op,NPY_DOUBLE,1,1);
	if (!ao) // If given object was not convertable into an array
		return false;
	else {
		Py_DECREF(ao);
		return true;
	}
}
// This function creates a numpy array from an num_array.
// The data of the num_array is afterwards owned by the numpy array.
PyObject* as_npy_array(cmf::math::num_array& a)
{
	// Size of the 1d array
	npy_intp dims[1] = {a.size()};
	// pointer to the data of the 1d array
	cmf::math::num_array::iterator start = a.release();
	// Creates the numpy array using size, place and type information
	PyArrayObject* pyA =(PyArrayObject*)PyArray_SimpleNewFromData(1,dims,NPY_DOUBLE,(void *)start);
	return (PyObject*)(pyA);
}
// This function creates a new array<T> from any object providing the array interface or from a sequence.
// First a numpy array is created to ensure right ordering, secondly an array<T> is created by copying the given data.
size_t from_npy_array(PyObject* op,double ** data) {
	// Create a contiguous and behaved numpy array from the given object
	// May include a copy of the data
	// Returns NULL if:
	// - given object was not a sequence of numbers or an object exposing the array interface or
	// - array had more dimensions
	PyObject* ao = PyArray_ContiguousFromAny(op,NPY_DOUBLE,1,1);
	if (!ao) {// If given object was not convertable into an array (no sequence of numbers or implementation of the array interface)	
		return 0;
	} else	{
		// Get pointer to data
		*data = (double*)PyArray_DATA(ao);
		// Get size
		size_t size = (size_t)PyArray_DIM(ao,0);
		if (ao->ob_refcnt == 1) {
		    // if array is newly constructed in this function, data needs to be copied prior
			double * begin = (double*)PyArray_DATA(ao);
			*data = new double[size];
			std::copy(begin,begin+size,*data);
		}
		Py_DECREF(ao);
		// Return the size
		return size;
	}
}
%}
// The actual typemaps have to be defined for each specialization
%typemap(in) cmf::math::num_array {
    // Convert a array_wrapper from numpy array
    double * data=0;
    size_t size = from_npy_array($input,&data); 
    if (!data) {
        SWIG_exception_fail(SWIG_TypeError,"Input data is not 'array-like' (in the sense of numpy arrays)");
        return NULL;
    }
    $1 = cmf::math::num_array(data,size);
}
// The actual typemaps have to be defined for each specialization
%typemap(in) const cmf::math::num_array& (cmf::math::num_array temp_array) {
    // Convert a array_wrapper from numpy array
    double * data=0;
    size_t size = from_npy_array($input,&data); 
    if (!data) {
        SWIG_exception_fail(SWIG_TypeError,"Input data is not 'array-like' (in the sense of numpy arrays)");
        return NULL;
    }
    temp_array = cmf::math::num_array(data,size);
    $1 = &temp_array;
}

// Typecheck typemap for dispatching of overloaded functions
%typemap(typecheck,precedence=0) cmf::math::num_array {
    $1 = test_npy_array<Class>($input);
}

// Out typemap, returns the data of array<T> wrapped as a numpy array, 
// thus converting the reference counting mechanism to the Python reference counting
%typemap(out) cmf::math::num_array {
    $result = as_npy_array($1);
}



