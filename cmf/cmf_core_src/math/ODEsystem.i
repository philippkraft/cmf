

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



%attribute(cmf::math::StateVariable,real,state,get_state,set_state);
%attributeval(cmf::math::Integrator,cmf::math::Time,t,get_t,set_t);
%attributeval(cmf::math::Integrator,cmf::math::Time,dt,get_dt);
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
    def __call__(self,t,dt=None):
        if dt is None:
            dt = Time()
        if t<self.t:
            self.integrate_until(self.t+t)
        else:
            self.integrate_until(t)
    def run(self,start=None,end=None,step=day*1):
        if not start is None:
            self.t=start
        if end is None:
            end = self.t + 100*step
        while self.t<end:
            self(self.t+step)
            yield self.t
}
}
