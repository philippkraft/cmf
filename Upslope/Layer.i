%{
	// Include Upslope
	#include "upslope/Soil/RetentionCurve.h"
	#include "upslope/SoilLayer.h"
	#include "upslope/VarLayerPair.h"
%}
// Get Upslope Classes

%include "upslope/soil/RetentionCurve.h"
%extend cmf::upslope::BrooksCoreyRetentionCurve
{
    %pythoncode {
    def __repr__(self):
        return "Brooks-Corey (Ksat=%g,porosity=%g,b=%g,wetness @ h=%g @ %g)" % (self.K(1,0),self.Porosity(0),self.b(),self.wetness_X,self.Psi_X)
    }
}
%extend cmf::upslope::VanGenuchtenMualem
{
    %pythoncode {
    def __repr__(self):
        return "VanGenuchten-Mualem (Ksat=%g,porosity=%g,alpha=%g, n=%g)" % (self.K(1,0),self.Porosity(0),self.alpha,self.n)
    }
}

%immutable cmf::upslope::SoilLayer::cell;

%factory(cmf::upslope::RetentionCurve& cmf::upslope::SoilLayer::get_soil,cmf::upslope::BrooksCoreyRetentionCurve, cmf::upslope::VanGenuchtenMualem);

%attribute(cmf::upslope::SoilLayer,real,gravitational_potential,get_gravitational_potential);
%attribute(cmf::upslope::SoilLayer,real,matrix_potential,get_matrix_potential);
%attribute(cmf::upslope::SoilLayer,real,wetness,get_wetness,set_wetness);
%attribute(cmf::upslope::SoilLayer,real,K,get_K);
%attribute(cmf::upslope::SoilLayer,real,Ksat,get_Ksat);
%attribute(cmf::upslope::SoilLayer,real,thickness,get_thickness);
%attribute(cmf::upslope::SoilLayer,real,lower_boundary,get_lower_boundary);
%attribute(cmf::upslope::SoilLayer,real,upper_boundary,get_upper_boundary);

%include "upslope/SoilLayer.h"


%extend cmf::upslope::SoilLayer {
  %pythoncode {
    boundary=property(lambda self:(self.upper_boundary,self.lower_boundary),None,"Returns the upper and lower boundary of the layer")
    pF=property(lambda self : waterhead_to_pF(self.matrix_potential),None,"The actual pF value")
    soil=property(get_soil,set_soil,"The retention curve of the layer")
  }
}

%include "upslope/VarLayerPair.h"

EXTENT__REPR__(cmf::upslope::VariableLayerSaturated)
EXTENT__REPR__(cmf::upslope::VariableLayerUnsaturated)
EXTENT__REPR__(cmf::upslope::SoilLayer)

