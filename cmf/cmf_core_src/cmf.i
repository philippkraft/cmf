
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

#define SWIG_SHARED_PTR_NAMESPACE std
#define SWIG_SHARED_PTR_SUBNAMESPACE tr1
%include <boost_shared_ptr.i>

// Makro to extend base classes with downcasts to child classes
%define DOWNCASTdefinition(BaseClass,ChildClass,funcname)
%extend BaseClass
{
	ChildClass* funcname()
	{
	  return dynamic_cast<ChildClass*>($self);
	}
}
%enddef

SWIG_SHARED_PTR(state_var,cmf::math::StateVariable);
   SWIG_SHARED_PTR_DERIVED(SoluteStorage,cmf::math::StateVariable,cmf::water::SoluteStorage);
SWIG_SHARED_PTR(flux_node,cmf::water::flux_node);
    SWIG_SHARED_PTR_DERIVED(DricheletBoundary,cmf::water::flux_node,cmf::water::DricheletBoundary);
    SWIG_SHARED_PTR_DERIVED(NeumannBoundary, cmf::water::flux_node, cmf::water::NeumannBoundary);
    SWIG_SHARED_PTR_DERIVED(RainSource, cmf::water::flux_node, cmf::atmosphere::RainSource);
        SWIG_SHARED_PTR_DERIVED(RainfallStationReference, cmf::atmosphere::RainSource, cmf::atmosphere::RainfallStationReference);
        SWIG_SHARED_PTR_DERIVED(IDWRainfall, cmf::atmosphere::RainSource, cmf::atmosphere::IDWRainfall);
        SWIG_SHARED_PTR_DERIVED(ConstantRainSource, cmf::atmosphere::RainSource, cmf::atmosphere::ConstantRainSource);
        
        
    SWIG_SHARED_PTR_DERIVED(WaterStorage,cmf::water::flux_node,cmf::water::WaterStorage);
        SWIG_SHARED_PTR_DERIVED(OpenWaterStorage,cmf::water::WaterStorage,cmf::river::OpenWaterStorage);
            SWIG_SHARED_PTR_DERIVED(Reach,cmf::river::OpenWaterStorage,cmf::river::Reach);
        SWIG_SHARED_PTR_DERIVED(SoilLayer,cmf::water::WaterStorage,cmf::upslope::SoilLayer);
        SWIG_SHARED_PTR_DERIVED(aquifer,cmf::water::WaterStorage,cmf::upslope::aquifer);

#define %ptr(Type) SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<Type>
#define %dynptrcast(Type,input) SWIG_SHARED_PTR_QNAMESPACE::dynamic_pointer_cast<Type>(input) 

// Workaround for SWIG bug 2408232: SWIG_SHARED_PTR doesn't work with deep/multiple inheritance
%define SWIG_SHARED_MULTICAST(CLASS,BASE)
%types(%ptr(CLASS) = 
        %ptr(BASE)) {
  *newmemory = SWIG_CAST_NEW_MEMORY;
  return (void *) new %ptr(BASE)(*(%ptr(CLASS) *)$from);
}
%enddef


%types (%ptr(cmf::water::flux_node) = cmf::geometry::Locatable)
{
  *newmemory = SWIG_POINTER_DISOWN;
  %ptr(cmf::water::flux_node)* from_with_type = (%ptr(cmf::water::flux_node)*)($from);
  return (from_with_type->get());  
}

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




SWIG_SHARED_MULTICAST(cmf::upslope::SoilLayer, cmf::water::flux_node)
SWIG_SHARED_MULTICAST(cmf::upslope::aquifer, cmf::water::flux_node)


SWIG_SHARED_MULTICAST(cmf::river::Reach, cmf::water::WaterStorage)
SWIG_SHARED_MULTICAST(cmf::river::Reach, cmf::water::flux_node)
SWIG_SHARED_MULTICAST(cmf::river::OpenWaterStorage, cmf::water::flux_node)

SWIG_SHARED_MULTICAST(cmf::upslope::SoilLayer, cmf::math::StateVariable)
SWIG_SHARED_MULTICAST(cmf::upslope::aquifer, cmf::math::StateVariable)
SWIG_SHARED_MULTICAST(cmf::river::OpenWaterStorage, cmf::math::StateVariable)
SWIG_SHARED_MULTICAST(cmf::river::Reach, cmf::math::StateVariable)
SWIG_SHARED_MULTICAST(cmf::water::WaterStorage, cmf::math::StateVariable)

SWIG_SHARED_MULTICAST(cmf::atmosphere::RainfallStationReference, cmf::water::flux_node);
SWIG_SHARED_MULTICAST(cmf::atmosphere::IDWRainfall, cmf::water::flux_node);
SWIG_SHARED_MULTICAST(cmf::atmosphere::ConstantRainSource, cmf::water::flux_node);



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

%include "upslope/connections/subsurfacefluxes.h"
%include "reach/ManningConnection.h"

%include "upslope/connections/surfacefluxes.h"
%include "upslope/connections/AtmosphericFluxes.h"
%include "upslope/connections/infiltration.h"
%include "upslope/connections/Percolation.h"
%include "upslope/connections/HBVflow.h"
%include "upslope/vegetation/ET.h"
%include "upslope/vegetation/ShuttleworthWallace.h"

%pythoncode
{
    def connect(type,left_node,right_node,*args):
        connection=type(left_node,right_node,*args)
        connection.thisown=0
}
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
    cell_vector.__repr__=lambda cv:"list of %i cells. first:%s, last: %s" % ((cv.size(),cv[0],cv[-1]) if len(cv) else (cv.size(),"None","None"))
}            

