// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
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
// Get Documentation

%feature("compactdefaultargs");
%include "docstrings.i"
%feature("autodoc","1");

// Usage for automated downcast
%include "factory.i"

// Include typemaps for STL
%include "std_string.i"
%include "std_vector.i"
// enable exception support
%include "exception.i"
%exception {
    try {
        $action
    } catch (const std::out_of_range& e) {
        SWIG_exception(SWIG_IndexError, e.what());    
    } catch (const std::exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    } catch (...) {
        SWIG_exception(SWIG_RuntimeError, "unknown error");
    }
}

%include "attribute.i"

/* #define SWIG_SHARED_PTR_SUBNAMESPACE */
%include <std_shared_ptr.i>
%{
	#include <memory>
%}
%include "cmf_swiglib.i"

%{
std::string pyrepr(PyObject* o) {
	PyObject* s = PyObject_Repr(o);
	#if PY_MAJOR_VERSION < 3
		std::string res = PyString_AsString(s);
	#else
		std::string res = PyUnicode_AsUTF8(s);
	#endif
	Py_XDECREF(s);
	return res;
}
%}

// Start my Module
%module(directors="1") cmf_core
%include "math/num_array.i"
%include "math/geometry.i"
%include "math/time.i"

%include "math/ODEsystem.i"

%ignore *::operator[];
%{
    #include "list.h"
%}
%include "list.h"
%extend_getitem(cmf::List, T);

%include "water/solute.i"
%include "water/water.i"
%include "atmosphere/meteorology.i"
%include "upslope/cell.i"
%include "upslope/Layer.i"
%include "reach/Reach.i"

///////////////////////////
// Surfacewater
//////////////////////////

%shared_ptr(cmf::upslope::SurfaceWater);
%attribute(cmf::upslope::SurfaceWater,real,puddledepth,get_puddledepth,set_puddledepth);
%attribute(cmf::upslope::SurfaceWater,real,nManning,get_nManning,set_nManning);

%include "upslope/surfacewater.h"
%extend__repr__(cmf::upslope::SurfaceWater)

/////////////////////////////
%{
	#include "upslope/groundwater.h"
	#include "upslope/connections/subsurfacefluxes.h"
	#include "upslope/connections/surfacefluxes.h"
	#include "upslope/connections/AtmosphericFluxes.h"
	#include "upslope/connections/infiltration.h"
	#include "upslope/connections/Percolation.h"
	#include "upslope/vegetation/ET.h"
	#include "upslope/vegetation/ShuttleworthWallace.h"
	// Include river model
	#include "reach/ManningConnection.h"
%}

%shared_ptr(cmf::upslope::aquifer);
%attribute(cmf::upslope::aquifer,double,base_height,get_base_height);
%attribute(cmf::upslope::aquifer,double,top_height,get_top_height);
%include "upslope/groundwater.h"
%extend__repr__(cmf::upslope::aquifer)

%include "upslope/connections/subsurfacefluxes.h"
%include "reach/ManningConnection.h"

%include "upslope/connections/surfacefluxes.h"
%include "upslope/connections/AtmosphericFluxes.h"
%include "upslope/connections/infiltration.h"
%include "upslope/connections/Percolation.h"
%include "upslope/vegetation/waterstress.h"

%shared_ptr(cmf::atmosphere::log_wind_profile);
%include "upslope/vegetation/ET.h"

%shared_ptr(cmf::upslope::ET::ShuttleworthWallace);
%include "upslope/vegetation/ShuttleworthWallace.h"

%include "project.i"

%include "math/integrator.i"



