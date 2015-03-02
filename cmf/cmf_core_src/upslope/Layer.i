

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

%shared_ptr(cmf::upslope::SoilLayer);

%{
	// Include Upslope
	#include "upslope/Soil/RetentionCurve.h"
	#include "upslope/SoilLayer.h"
%}
// Get Upslope Classes
%attribute(cmf::upslope::BrooksCoreyRetentionCurve,real, b,get_b,set_b);
%include "upslope/Soil/RetentionCurve.h"
%extend cmf::upslope::BrooksCoreyRetentionCurve
{
    %pythoncode {
    def __repr__(self):
        return "cmf.BrooksCoreyRetentionCurve(Ksat=%g,phi=%g,b=%g,wfp0=%0.1f,pF0=%0.2f)" % (self.Ksat,self.Porosity(0.0),self.b,self.wetness_X,waterhead_to_pF(self.Psi_X))
    def __str__(self):
        return "Brooks-Corey retention curve: Ksat=%0.3g m/day,por.=%0.0f%%,b=%g,%0.1f%% water content at pF%0.2f)" % (self.Ksat,self.Porosity(0.0)*100,self.b,self.wetness_X * self.Porosity(0.0) * 100,waterhead_to_pF(self.Psi_X))
    }
}
%extend cmf::upslope::VanGenuchtenMualem
{
    %pythoncode {
    def __repr__(self):
        return "cmf.VanGenuchtenMualem(Ksat=%0.3g,Phi=%0.3g,alpha=%0.3g,n=%0.3g,m=%0.3g)" % (self.Ksat,self.Phi,self.alpha,self.n,self.m)
    def __str__(self):
    	return "VanGenuchten / Mualem retention curve: Ksat=%0.3g m/day, %0.3g%% Pores, alpha=%0.3g 1/cm, n=%0.3g" % (self.Ksat,self.Phi*100,self.alpha,self.n) 
    }
}

%immutable cmf::upslope::SoilLayer::cell;

%factory(cmf::upslope::RetentionCurve& cmf::upslope::SoilLayer::get_soil,cmf::upslope::BrooksCoreyRetentionCurve, cmf::upslope::VanGenuchtenMualem, cmf::upslope::LinearRetention);

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
%attribute(cmf::upslope::SoilLayer,real,rootfraction,get_rootfraction,set_rootfraction);

%shared_attr(cmf::upslope::SoilLayer,SoilLayer, upper, get_upper);
%shared_attr(cmf::upslope::SoilLayer,SoilLayer, lower, get_lower);






%include "upslope/SoilLayer.h"


%extend cmf::upslope::SoilLayer {
  %pythoncode {
    boundary=property(lambda self:(self.upper_boundary,self.lower_boundary),None,"Returns the upper and lower boundary of the layer")
    pF=property(lambda self : waterhead_to_pF(self.matrix_potential),None,"The actual pF value")
    soil=property(get_soil,set_soil,"The retention curve of the layer")
  }
}

%extend__repr__(cmf::upslope::SoilLayer)
%extend__repr__(cmf::upslope::MacroPore)


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
%attribute(cmf::upslope::layer_list,cmf::math::num_array,theta,get_theta);
%attribute(cmf::upslope::layer_list,cmf::math::num_array,root,get_rootfraction);

%iterable_to_list(cmf::upslope::layer_list,cmf::upslope::SoilLayer::ptr)


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

%{
#include "upslope/macropore.h"
%}

%shared_ptr(cmf::upslope::MacroPore);
%shared_attr(cmf::upslope::MacroPore, MacroPore, layer,get_layer);

%attribute(cmf::upslope::MacroPore,real,filled_fraction,get_filled_fraction);
%attribute(cmf::upslope::MacroPore,real,K,get_K);
%attribute(cmf::upslope::MacroPore,real,capacity,get_capacity);
%attribute(cmf::upslope::MacroPore,cmf::upslope::Cell&,cell,get_cell);


%include "upslope/macropore.h"

