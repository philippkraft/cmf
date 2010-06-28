

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
%{
	// Include Upslope
	#include "upslope/Soil/RetentionCurve.h"
	#include "upslope/SoilLayer.h"
%}
// Get Upslope Classes

%include "upslope/Soil/RetentionCurve.h"
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
%attribute(cmf::upslope::SoilLayer,real,theta,get_theta,set_theta);
%attribute(cmf::upslope::SoilLayer,real,K,get_K);
%attribute(cmf::upslope::SoilLayer,real,Ksat,get_Ksat);
%attribute(cmf::upslope::SoilLayer,real,thickness,get_thickness);
%attribute(cmf::upslope::SoilLayer,real,lower_boundary,get_lower_boundary);
%attribute(cmf::upslope::SoilLayer,real,upper_boundary,get_upper_boundary);
%attribute(cmf::upslope::SoilLayer,real,porosity,get_porosity);
%attribute(cmf::upslope::SoilLayer,real,ice_fraction,get_ice_fraction,set_ice_fraction);

%attribute(cmf::upslope::SoilLayer,cmf::upslope::SoilLayer, upper, get_upper);
%attribute(cmf::upslope::SoilLayer,cmf::upslope::SoilLayer, lower, get_lower);
%include "upslope/SoilLayer.h"


%extend cmf::upslope::SoilLayer {
  %pythoncode {
    boundary=property(lambda self:(self.upper_boundary,self.lower_boundary),None,"Returns the upper and lower boundary of the layer")
    pF=property(lambda self : waterhead_to_pF(self.matrix_potential),None,"The actual pF value")
    soil=property(get_soil,set_soil,"The retention curve of the layer")
  }
}

EXTENT__REPR__(cmf::upslope::SoilLayer)


%attribute(cmf::upslope::layer_list,cmf::math::num_array,gravitational_potential,get_gravitational_potential);
%attribute(cmf::upslope::layer_list,cmf::math::num_array,matrix_potential,get_matrix_potential);
%attribute(cmf::upslope::layer_list,cmf::math::num_array,wetness,get_wetness);
%attribute(cmf::upslope::layer_list,cmf::math::num_array,volume,get_volume);
%attribute(cmf::upslope::layer_list,cmf::math::num_array,potential,get_potential);
%attribute(cmf::upslope::layer_list,cmf::math::num_array,K,get_K);
%attribute(cmf::upslope::layer_list,cmf::math::num_array,Ksat,get_Ksat);
%attribute(cmf::upslope::layer_list,cmf::math::num_array,thickness,get_thickness);
%attribute(cmf::upslope::layer_list,cmf::math::num_array,lower_boundary,get_lower_boundary);
%attribute(cmf::upslope::layer_list,cmf::math::num_array,upper_boundary,get_upper_boundary);
%attribute(cmf::upslope::layer_list,cmf::math::num_array,porosity,get_porosity);
%attribute(cmf::upslope::layer_list,cmf::math::num_array,ice_fraction,get_ice_fraction);

%include "upslope/layer_list.h"

%extend cmf::upslope::layer_list {
    cmf::upslope::SoilLayer::ptr __get(int index) { return (*$self)[index];}
%pythoncode {
    __repr__=lambda self: repr(list(self))
    __str__ =lambda self: str(list(self))
    __len__=lambda self: self.size()
    def __iadd__(self,other):
        self.append(other)
        return self
    def __add__(self,other):
        res = layer_list(self)
        res.append(other)
        return res
    def __iter__(self):
        for i in range(len(self)):
            yield self[i]
    def __getitem__(self,index):
        if (type(index)==slice):
            return self.get_slice(*index.indices(len(self)))
        try:
            gen=iter(index)
            return [self.__get(it) for it in gen]
        except TypeError:
             return self.__get(index)      
}}