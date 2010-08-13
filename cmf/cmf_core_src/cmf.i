
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
// Include datetime
// %pythoncode
// {
// import datetime
// }
// Get Documentation

%feature("compactdefaultargs");
%include "docstrings.i"
%feature("autodoc","1");

// Usage for automated downcast
%include factory.i

// Include typemaps for STL
%include "std_string.i"

// enable exception support
%include "exception.i"
%exception {
    try {
        $action
    } catch (const std::out_of_range& e) {
        SWIG_exception(SWIG_IndexError, e.what());    
    } catch (const std::exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
    
}
%include "attribute.i"

#define SWIG_SHARED_PTR_SUBNAMESPACE tr1
%include <std_shared_ptr.i>

%shared_ptr(cmf::math::StateVariable);

%shared_ptr(cmf::water::SoluteStorage);

%shared_ptr(cmf::water::flux_node);
    %shared_ptr(cmf::water::DricheletBoundary);
    %shared_ptr(cmf::water::NeumannBoundary);
    %shared_ptr(cmf::atmosphere::RainSource);
        %shared_ptr(cmf::atmosphere::RainfallStationReference);
        %shared_ptr(cmf::atmosphere::IDWRainfall);
        %shared_ptr(cmf::atmosphere::ConstantRainSource);

    %shared_ptr(cmf::water::WaterStorage);
        %shared_ptr(cmf::river::OpenWaterStorage);
            %shared_ptr(cmf::river::Reach);
        %shared_ptr(cmf::upslope::SoilLayer);
        %shared_ptr(cmf::upslope::aquifer);

#define %ptr(Type) SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<Type>
#define %dynptrcast(Type,input) SWIG_SHARED_PTR_QNAMESPACE::dynamic_pointer_cast<Type>(input) 


%define %_node_down_cast(Type)
    if (dcast==0 && $1) /*check for Type*/  { 
        %ptr(Type) output = %dynptrcast(Type,$1);
        if (output) /*flux_node is Type */ {
            dcast=1; 
            %set_output(SWIG_NewPointerObj(new %ptr(Type)(output),$descriptor(%ptr(Type)*), SWIG_POINTER_OWN)); 
        }
    }
%enddef

%define %node_downcast(typemaptarget, Types...)
%typemap(out) typemaptarget {
   int dcast = 0;
   %formacro(%_node_down_cast, Types)
   if (!dcast) %set_output(SWIG_NewPointerObj($1 ? new %ptr(cmf::water::flux_node)($1) : 0,$descriptor(%ptr(cmf::water::flux_node)*), SWIG_POINTER_OWN));   
}
%enddef      

%define %node_downcast_all(Method)
//Downcast to all children of cmf::water::flux_node
%node_downcast(Method,
   cmf::atmosphere::RainSource,cmf::water::DricheletBoundary,cmf::water::NeumannBoundary,
   cmf::upslope::SoilLayer, cmf::upslope::aquifer,  cmf::river::Reach,cmf::river::OpenWaterStorage,cmf::water::WaterStorage
)
%enddef

// Returns the result of a to_string method of a type as __repr__
%define EXTENT__REPR__(TYPE) 
%extend TYPE { %pythoncode {
    def __repr__(self): 
        return self.to_string()
}}
%enddef

// Function to convert an iterable to a list type (class with append function). For use in typemaps
%{
    template<typename _item_type, typename _Ty> 
    int iterable_to_list(PyObject* iterable,swig_type_info* _item_descriptor, _Ty& temp_list) {
    	PyObject* iter = PyObject_GetIter(iterable);
        if (iter == 0) {
            // no iterator
	        return SWIG_ERROR;
	    }
	    while (PyObject* py_item = PyIter_Next(iter)) {
		    _item_type* item=0;
		    // if the item is a node, append to node_list
		    int is_ok = SWIG_ConvertPtr(py_item, (void**)&item, _item_descriptor, SWIG_POINTER_EXCEPTION);
		    if (is_ok == 0 && item != 0 ) { 
			    temp_list.append(*item);
		    }
		    Py_DECREF(py_item);
	    }
	    Py_DECREF(iter);
	    return SWIG_OK;
    }
%}

%define %iterable_to_list(LISTTYPE,ITEMTYPE)
%typemap(in) LISTTYPE& (LISTTYPE temp_list) {
    if (SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, SWIG_POINTER_EXCEPTION) == -1) {
        int res = iterable_to_list<ITEMTYPE, LISTTYPE>($input,$descriptor(ITEMTYPE*), temp_list);
        if (SWIG_IsOK(res)) {
    	    $1 = &temp_list;
    	} else {
    	    SWIG_exception_fail(SWIG_TypeError,"Only iterables can be converted to LISTTYPE");
    	}
    }
}
%typemap(typecheck,precedence=0) LISTTYPE& {
	void * pt;
	int res=SWIG_ConvertPtr($input,&pt,$1_descriptor,0);
	if (SWIG_IsOK(res)) {
		$1 = 1;
	} else  {
		PyObject* iter = PyObject_GetIter($input);
		if (iter != 0) {
			Py_DECREF(iter);
			$1 = 1;
		} 
		else 
		    $1 = 0;
	} 
}
%enddef

// Start my Module
%module cmf_core
%echo "Start include.."
%include "math/num_array.i"
%include "geometry/geometry.i"

%echo "geometry.i OK";

%include "math/time.i"
%echo "time.i OK";


%include "math/ODEsystem.i"
%echo "ODEsystem.i OK";


%include "water/water.i"

EXTENT__REPR__(cmf::water::DricheletBoundary)
EXTENT__REPR__(cmf::water::NeumannBoundary)
EXTENT__REPR__(cmf::water::WaterStorage)
%echo "water.i OK";


%include "atmosphere/meteorology.i"
EXTENT__REPR__(cmf::atmosphere::RainSource)
EXTENT__REPR__(cmf::atmosphere::RainfallStationReference)
EXTENT__REPR__(cmf::atmosphere::ConstantRainSource)
EXTENT__REPR__(cmf::atmosphere::IDWRainfall)

%echo "Atmosphere OK!";


%include "upslope/cell.i"
%echo "Cell OK";

%include "upslope/Layer.i"
%echo "Layer ok"

%include "reach/Reach.i"
%echo "Reach ok"

%{
	#include "upslope/groundwater.h"
	#include "upslope/connections/subsurfacefluxes.h"
	#include "upslope/connections/surfacefluxes.h"
	#include "upslope/connections/AtmosphericFluxes.h"
	#include "upslope/connections/infiltration.h"
	#include "upslope/connections/Percolation.h"
	#include "upslope/vegetation/ET.h"
	#include "upslope/vegetation/ShuttleworthWallace.h"
    #include "upslope/connections/HBVflow.h"
	// Include river model
	#include "reach/ManningConnection.h"
	// Include the combined solver
	#include "math/integrators/WaterSoluteIntegrator.h"
%}


%attribute(cmf::upslope::aquifer,double,base_height,get_base_height);
%attribute(cmf::upslope::aquifer,double,top_height,get_top_height);
%include "upslope/groundwater.h"

EXTENT__REPR__(cmf::upslope::aquifer)

%include "upslope/connections/subsurfacefluxes.h"
%include "reach/ManningConnection.h"

%include "upslope/connections/surfacefluxes.h"
%include "upslope/connections/AtmosphericFluxes.h"
%include "upslope/connections/infiltration.h"
%include "upslope/connections/Percolation.h"
%include "upslope/connections/HBVflow.h"
%include "upslope/vegetation/ET.h"
%include "upslope/vegetation/ShuttleworthWallace.h"

%echo "upslope::connections OK!";

%include "project.i"
%attribute(cmf::math::SoluteWaterIntegrator,cmf::math::Integrator*,solute_integrator,get_solute_integrator,set_solute_integrator);
%attribute(cmf::math::SoluteWaterIntegrator,cmf::math::Integrator*,water_integrator,get_water_integrator,set_water_integrator);
%include "math/integrators/WaterSoluteIntegrator.h"

%echo "cmf::project OK!";

%pythoncode {
    def get_layers(cells):
        for c in cells:
            for l in c.layers:
               yield l
    def count_layers(cells):
        res=0
        for c in cells:
            res+=c.layer_count()
        return res
    def __doc__(self):
        return "cmf -> Catchment Model Framework, extending Python with hydrological elements " + VERSION
}            

