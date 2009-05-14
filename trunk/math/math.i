%{
#include "math/real.h"
#include "math/Time.h"
#include "math/timeseries.h"
#include "math/numVector.h"
#include "math/StateVariable.h"
#include "math/Jacobian.h"
#include "math/Integrators/Integrator.h"
#include "math/Integrators/BDF2.h"
#include "math/Integrators/ExplicitEuler_fixed.h"
#include "math/Integrators/ExplicitEuler_variable.h"
#include "math/Integrators/FixpointImplicitEuler.h"
#include "math/Integrators/RKFIntegrator.h"
#include "math/Integrators/Gears_Fixpoint.h"
#include "math/Integrators/cvodeIntegrator.h"
%}
// Get Math
%pythoncode
{
import datetime
}
%include "math/real.h"
%include "math/Time.h"
%include "math/timeseries.h"
%include "math/numVector.h"

%ignore svVector;

// %attribute(cmf::math::Integrator,cmf::math::Time,t,ModelTime)
// %attribute(cmf::math::Integrator,cmf::math::Time,dt,TimeStep)

class cmf::math::StateVariable;
namespace std {
	%template(svVector) vector<cmf::math::StateVariable*>; 
}
//%attribute(cmf::math:StateVariable,double,s,State);
%include "math/StateVariable.h"
%include "math/Integrators/Integrator.h"
%include "math/Integrators/BDF2.h"
%include "math/Integrators/ExplicitEuler_fixed.h"
%include "math/Integrators/ExplicitEuler_variable.h"
%include "math/Integrators/FixpointImplicitEuler.h"
%include "math/Integrators/RKFIntegrator.h"
%include "math/Integrators/Gears_Fixpoint.h"
%include "math/Integrators/cvodeIntegrator.h"
%include "math/Jacobian.h"
%extend cmf::math::StateVariable
{
  %pythoncode {
    state=property(State,None,"Gets the currect state")
  }
}
%extend cmf::math::Integrator {
   void __call__(cmf::math::Time t)
   {
     $self->IntegrateUntil(t);
   }
   %pythoncode {
       t=property(ModelTime,ModelTime,"Gets or sets the model time of the integrator")
       dt=property(TimeStep,None,"Gets the length of the last internal time step of the integrator")
   }
}
%implicitconv cmf::math::Time;
%implicitconv cmf::math::Date;

%pythoncode {
def AsCMFtime(date):
    """Converts a python datetime to cmf.Time"""
    return Time(date.day,date.month,date.year,date.hour,date.minute,date.second,date.microsecond/1000)
def timerange(start,end,step=day):
    """Creates a list of cmf.Time, similar to the Python range function"""
    return [start+step*x for x in range(0,int((end-start)/step+0.999))]
}
