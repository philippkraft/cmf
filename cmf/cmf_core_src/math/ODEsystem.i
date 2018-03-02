

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
#include "math/integrators/cvode3.h"
#include "math/integrators/multiintegrator.h"
%}

%shared_ptr(cmf::math::StateVariable);
%shared_ptr(cmf::math::integratable);

%iterable_to_list(cmf::math::StateVariableList,cmf::math::StateVariable::ptr)
%iterable_to_list(cmf::math::integratable_list,cmf::math::integratable::ptr)
%attribute(cmf::math::StateVariable,real,state,get_state,set_state);

%rename(__getitem) cmf::math::integratable_list::operator[];

%include "math/statevariable.h"
%extend__repr__(cmf::math::StateVariable);
%extend cmf::math::StateVariableList {
    size_t __len__() const {
        return $self->size();
    }
}
%extend cmf::math::integratable_list {
     size_t __len__() const {
         return $self->size();
     }
    %pythoncode {
    def __iter__(self):
        for i in xrange(len(self)):
            yield self[i]
    def __getitem__(self,index):
        if isinstance(index,slice):
            return [self.__getitem(i) for i in range(*index.indices(len(self)))]
        else:
            try:
                it=iter(index)
                return [self.__getitem(i) for i in it]
            except:
                return self.__getitem(index)
    }
}    

%attributeval(cmf::math::CVode3, cmf::math::CVodeInfo, info, get_info);

%include "math/integrators/integrator.h"
%include "math/integrators/bdf2.h"
%include "math/integrators/explicit_euler.h"
%include "math/integrators/implicit_euler.h"
%include "math/integrators/RKFintegrator.h"
%include "math/integrators/cvode3.h"

%extend__repr__(cmf::math::CVodeInfo)

%extend cmf::math::Integrator { 
    StateVariable::ptr __getitem__(int index) {
        return (*$self)[index];
    }
%pythoncode {
    def __call__(self,t,reset=False):
        if t<self.t:
            self.integrate_until(self.t+t,reset=reset)
        else:
            self.integrate_until(t,reset=reset)
        return self.t
    t = property(get_t,set_t,doc="Sets the actual time of the solution")
    dt = property(get_dt,doc="Get the current time step of the solver")
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

%extend cmf::math::CVodeDense {
%pythoncode {
    def get_jacobian(self):
        return self._get_jacobian().reshape(self.size(), self.size())
}
}

%pythoncode {
class CVodeIntegrator:
    def __init__(self, *args, **kwargs):
        raise NotImplementedError('CVodeIntegrator has been removed in cmf 2, use CVodeDense, CVodeKrylov or others instead')
}