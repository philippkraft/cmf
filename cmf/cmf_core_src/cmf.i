// Include datetime
// %pythoncode
// {
// import datetime
// }
// Get Documentation

%feature("compactdefaultargs");
%include "docstrings.i"
%feature("autodoc","1") ;

// Usage for automated downcast
%include factory.i

// Include typemaps for STL
%include "stl.i"
%include "std_set.i"

// enable exception support
%include "exception.i"
%exception {
    try {
        $action
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
//SWIG_SHARED_PTR(Locatable,cmf::geometry::Locatable)   
    SWIG_SHARED_PTR_DERIVED(flux_node,cmf::geometry::Locatable,cmf::water::flux_node);
SWIG_SHARED_PTR(flux_node,cmf::water::flux_node);
    SWIG_SHARED_PTR_DERIVED(DricheletBoundary,cmf::water::flux_node,cmf::water::DricheletBoundary);
    SWIG_SHARED_PTR_DERIVED(NeumannBoundary, cmf::water::flux_node, cmf::water::NeumannBoundary);
        SWIG_SHARED_PTR_DERIVED(RainCloud, cmf::water::NeumannBoundary, cmf::atmosphere::RainCloud);
    SWIG_SHARED_PTR_DERIVED(WaterStorage,cmf::water::flux_node,cmf::water::WaterStorage);
        SWIG_SHARED_PTR_DERIVED(OpenWaterStorage,cmf::water::WaterStorage,cmf::river::OpenWaterStorage);
            SWIG_SHARED_PTR_DERIVED(Reach,cmf::river::OpenWaterStorage,cmf::river::Reach);
        SWIG_SHARED_PTR_DERIVED(SoilLayer,cmf::water::WaterStorage,cmf::upslope::SoilLayer);

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
  %ptr(cmf::water::flux_node)* from_with_type = (%ptr(cmf::water::flux_node)*)($from);
  return (from_with_type->get());  
}

// Start my Module
%module cmf_core
%echo "Start include.."
%include "geometry/geometry.i"

%echo "geometry.i OK";

%include "math/time.i"
%echo "time.i OK";

%include "math/num_array.i"
%echo "num_array.i OK";

%include "math/ODEsystem.i"
%echo "ODEsystem.i OK";




SWIG_SHARED_MULTICAST(cmf::upslope::SoilLayer, cmf::water::flux_node)


SWIG_SHARED_MULTICAST(cmf::river::Reach, cmf::water::WaterStorage)
SWIG_SHARED_MULTICAST(cmf::river::Reach, cmf::water::flux_node)
SWIG_SHARED_MULTICAST(cmf::river::OpenWaterStorage, cmf::water::flux_node)

SWIG_SHARED_MULTICAST(cmf::upslope::SoilLayer, cmf::math::StateVariable)
SWIG_SHARED_MULTICAST(cmf::river::OpenWaterStorage, cmf::math::StateVariable)
SWIG_SHARED_MULTICAST(cmf::river::Reach, cmf::math::StateVariable)
SWIG_SHARED_MULTICAST(cmf::water::WaterStorage, cmf::math::StateVariable)

SWIG_SHARED_MULTICAST(cmf::atmosphere::RainCloud, cmf::water::flux_node)



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
   cmf::atmosphere::RainCloud,cmf::water::DricheletBoundary,cmf::water::NeumannBoundary,
   cmf::upslope::SoilLayer,  cmf::river::Reach,cmf::river::OpenWaterStorage,cmf::water::WaterStorage
)
%enddef


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


%include "Atmosphere/Meteorology.i"
EXTENT__REPR__(cmf::atmosphere::RainCloud)

%echo "Atmosphere OK!";


%include "Upslope/cell.i"
%echo "Cell OK";

%include "Upslope/Layer.i"
%echo "Layer ok"

%include "Reach/Reach.i"
%echo "Reach ok"

%{
	#include "upslope/connections/subsurfacefluxes.h"
	#include "upslope/connections/surfacefluxes.h"
	#include "upslope/connections/atmosphericfluxes.h"
	#include "upslope/connections/infiltration.h"
	#include "upslope/connections/Percolation.h"
	#include "upslope/vegetation/ET.h"
    #include "upslope/connections/HBVflow.h"
	// Include river model
	#include "Reach/ManningConnection.h"
	// Include the combined solver
	#include "math/Integrators/WaterSoluteIntegrator.h"
%}



%include "upslope/connections/subsurfacefluxes.h"
%include "Reach/ManningConnection.h"

%include "upslope/connections/surfacefluxes.h"
%include "upslope/connections/atmosphericfluxes.h"
%include "upslope/connections/infiltration.h"
%include "upslope/connections/Percolation.h"
%include "upslope/connections/HBVflow.h"
%include "upslope/vegetation/ET.h"

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
%include "math/Integrators/WaterSoluteIntegrator.h"

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

