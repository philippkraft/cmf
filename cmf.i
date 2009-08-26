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
    } 
    SWIG_CATCH_STDEXCEPT
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


// Start my Module
%module cmf_core

%include "geometry/geometry.i"

%echo "geometry.i OK";

%include "math/math.i"

%echo "math.i OK";

%include "water/water.i"
%echo "water.i OK";

%include "Atmosphere/Meteorology.i"
%echo "Atmosphere OK!";

%{
	// Include Upslope
	#include "upslope/Soil/RetentionCurve.h"
	#include "upslope/SoilWaterStorage.h"
%}
// Get Upslope Classes

%include "upslope/soil/RetentionCurve.h"
%extent cmf::upslope::BrooksCoreyRetentionCurve
{
    %pythoncode {
    def __repr__(self):
        return "Brooks-Corey (Ksat=%g,porosity=%g,b=%g,wetness @ h=%g @ %g)" % (self.K(1,0),self.Porosity(0),self.b(),self.wetness_X,self.Psi_X)
    }
}
%extent cmf::upslope::VanGenuchtenMualem
{
    %pythoncode {
    def __repr__(self):
        return "VanGenuchten-Mualem (Ksat=%g,porosity=%g,alpha=%g, n=%g)" % (self.K(1,0),self.Porosity(0),self.alpha,self.n)
    }
}

%include "Upslope/cell.i"
%echo "Cell OK";

%{
	#include "upslope/VarLayerPair.h"
	#include "upslope/connections/subsurfacefluxes.h"
	#include "upslope/connections/VarLayerPercolation.h"
	#include "upslope/connections/surfacefluxes.h"
	#include "upslope/connections/atmosphericfluxes.h"
	#include "upslope/connections/infiltration.h"
	#include "upslope/vegetation/ET.h"
	// Include river model
	#include "Reach/ReachType.h"
	#include "Reach/VolumeHeightRelation.h"
	#include "Reach/ManningConnection.h"
	#include "Reach/reach.h"
	#include "project.h"
	// Include the combined solver
	#include "math/Integrators/WaterSoluteIntegrator.h"
	// Include Factories and helper IO
%}




%immutable cmf::upslope::SoilWaterStorage::cell;

%factory(cmf::upslope::RetentionCurve& cmf::upslope::SoilWaterStorage::get_soil,cmf::upslope::BrooksCoreyRetentionCurve, cmf::upslope::VanGenuchtenMualem);

%attribute(cmf::upslope::SoilWaterStorage,real,gravitational_potential,get_gravitational_potential);
%attribute(cmf::upslope::SoilWaterStorage,real,matrix_potential,get_matrix_potential);
%attribute(cmf::upslope::SoilWaterStorage,real,wetness,get_wetness,set_wetness);
%attribute(cmf::upslope::SoilWaterStorage,real,K,get_K);
%attribute(cmf::upslope::SoilWaterStorage,real,Ksat,get_Ksat);
%attribute(cmf::upslope::SoilWaterStorage,real,thickness,get_thickness);
%attribute(cmf::upslope::SoilWaterStorage,real,lower_boundary,get_lower_boundary);
%attribute(cmf::upslope::SoilWaterStorage,real,upper_boundary,get_upper_boundary);
%include "upslope/SoilWaterStorage.h"


%extend cmf::upslope::SoilWaterStorage {
  %pythoncode {
    boundary=property(lambda self:(self.upper_boundary,self.lower_boundary),None,"Returns the upper and lower boundary of the layer")
    def __repr__(self):
        return self.Name
    pF=property(lambda self : waterhead_to_pF(self.matrix_potential),None,"The actual pF value")
    soil=property(get_soil,set_soil,"The retention curve of the layer")
  }
}


%include "upslope/connections/subsurfacefluxes.h"

%include "upslope/VarLayerPair.h"
%include "upslope/connections/VarLayerPercolation.h"
%include "upslope/connections/surfacefluxes.h"
%include "upslope/connections/atmosphericfluxes.h"
%include "upslope/connections/infiltration.h"
%include "upslope/vegetation/ET.h"

%pythoncode
{
    def connect(type,left_node,right_node,*args):
        connection=type(left_node,right_node,*args)
        connection.thisown=0
}
%echo "upslope::connections OK!";

// Get river model classes
%include "Reach/ReachType.h"
%include "Reach/VolumeHeightRelation.h"
%newobject cmf::water::OpenWaterStorage::FromNode;
%include "Reach/ManningConnection.h"

%attribute2(cmf::river::Reach,cmf::upslope::Cell,cell,get_cell);
%attribute(cmf::river::Reach,cmf::river::Reach*,downstream,get_downstream);
%attribute(cmf::river::Reach,cmf::river::Reach*,root,get_root);
%attribute2(cmf::river::Reach,cmf::river::OpenWaterStorage,water,get_water);
%attribute(cmf::river::Reach,cmf::water::FluxNode*,outlet,get_outlet);
%attribute(cmf::river::Reach,int,upstream_count,upstream_count);
%attribute(cmf::river::Reach,real,depth,get_depth,set_depth);

%attribute(cmf::river::ReachIterator,cmf::river::Reach*,reach,reach);
%attribute(cmf::river::ReachIterator,double,position,position);

%include "Reach/reach.h"
%extend cmf::river::Reach {
%pythoncode {
    @property
    def upstream(self):
        """Returns a list containing all reaches flowing into self"""
        return [self.get_upstream(i) for i in range(self.upstream_count)]
    def __hash__(self):
        return hash(self.water.node_id)
    def __repr__(self):
        return "A reach (node #%i) at %s" % (self.water.node_id,self.cell)
       
}}
%extend cmf::river::ReachIterator {
%pythoncode {
    def __iter__(self):
        while self.valid():
            self.next()
            yield (self.reach,self.position)
}}
%echo "cmf::river OK!";
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

    cell_vector.__repr__=lambda cv:"list of %i cells. first:%s, last: %s" % ((cv.size(),cv[0],cv[-1]) if len(cv) else (cv.size(),"None","None"))
}            





