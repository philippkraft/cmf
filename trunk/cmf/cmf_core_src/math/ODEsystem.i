%{
#include "math/StateVariable.h"
#include "math/Integrators/Integrator.h"
#include "math/Integrators/BDF2.h"
#include "math/Integrators/ExplicitEuler_fixed.h"
#include "math/Integrators/FixpointImplicitEuler.h"
#include "math/Integrators/RKFIntegrator.h"
#include "math/Integrators/cvodeIntegrator.h"
#include "math/Integrators/MultiIntegrator.h"
%}

%ignore svVector;

class cmf::math::StateVariable;
namespace std {
	%template(svVector) vector<cmf::math::StateVariable*>; 
}


%attribute(cmf::math::StateVariable,real,state,get_state,set_state);
%include "math/StateVariable.h"

%include "math/Integrators/Integrator.h"
%include "math/Integrators/BDF2.h"
%include "math/Integrators/ExplicitEuler_fixed.h"
%include "math/Integrators/FixpointImplicitEuler.h"
%include "math/Integrators/RKFIntegrator.h"
%include "math/Integrators/cvodeIntegrator.h"
%include "math/Integrators/MultiIntegrator.h"


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
