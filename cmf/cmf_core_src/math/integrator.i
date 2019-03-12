%{
#include "math/statevariable.h"
#include "math/integrators/integrator.h"
#include "math/integrators/bdf2.h"
#include "math/integrators/explicit_euler.h"
#include "math/integrators/implicit_euler.h"
#include "math/integrators/RKFintegrator.h"
#include "math/integrators/cvodeintegrator.h"
#include "math/integrators/multiintegrator.h"
#include "math/integrators/WaterSoluteIntegrator.h"
%}

%attribute(cmf::math::CVodeIntegrator,int,order,get_order);
%rename(__getitem__) cmf::math::Integrator::operator[];
%state_downcast(cmf::math::StateVariable::ptr cmf::math::Integrator::operator[])

%include "math/integrators/integrator.h"
%include "math/integrators/bdf2.h"
%include "math/integrators/explicit_euler.h"
%include "math/integrators/implicit_euler.h"
%include "math/integrators/RKFintegrator.h"
%include "math/integrators/cvodeintegrator.h"
%include "math/integrators/multiintegrator.h"
%include "math/integrators/WaterSoluteIntegrator.h"


%extend__repr__(cmf::math::Integrator);

%extend cmf::math::Integrator { 
    size_t __len__() {
        return $self->size();
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
