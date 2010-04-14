

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
#include "math/statevariable.h"
#include "math/integrators/integrator.h"
#include "math/integrators/bdf2.h"
#include "math/integrators/explicit_euler.h"
#include "math/integrators/implicit_euler.h"
#include "math/integrators/RKFintegrator.h"
#include "math/integrators/cvodeintegrator.h"
#include "math/integrators/multiintegrator.h"
%}

%ignore svVector;

class cmf::math::StateVariable;
namespace std {
	%template(svVector) vector<cmf::math::StateVariable*>; 
}


%attribute(cmf::math::StateVariable,real,state,get_state,set_state);
%include "math/statevariable.h"

%include "math/integrators/integrator.h"
%include "math/integrators/bdf2.h"
%include "math/integrators/explicit_euler.h"
%include "math/integrators/implicit_euler.h"
%include "math/integrators/RKFintegrator.h"
%include "math/integrators/cvodeintegrator.h"
%include "math/integrators/multiintegrator.h"


%extend cmf::math::Integrator { 
%pythoncode {
    t=property(ModelTime,ModelTime,"Gets or sets the model time of the integrator")
    dt=property(TimeStep,None,"Gets the length of the last internal time step of the integrator")
    def __call__(self,t):
        if t<self.t:
            self.IntegrateUntil(self.t+t)
        else:
            self.IntegrateUntil(t)
    def run(self,start=day*0,end=day*1,step=day*1):
        self.t=start
        while self.t<end:
            self(self.t+step)
            yield self.t
}
}
