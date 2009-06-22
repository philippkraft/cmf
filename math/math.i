%{
#include "datetime.h"
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
%init %{
PyDateTime_IMPORT;
%}

%{
static cmf::math::Time convert_datetime_to_cmftime(PyObject* dt)
{
    void * pt; 
    int res=SWIG_ConvertPtr(dt,&pt,SWIGTYPE_p_cmf__math__Time,0);
    if (SWIG_IsOK(res))
    {
        cmf::math::Time * temp = reinterpret_cast< cmf::math::Time * >(pt);
        return *temp;
    }
    else if (PyDateTime_Check(dt))
       return cmf::math::Time(PyDateTime_GET_DAY(dt),
                              PyDateTime_GET_MONTH(dt),
                              PyDateTime_GET_YEAR(dt),
                              PyDateTime_TIME_GET_HOUR(dt),
                              PyDateTime_TIME_GET_MINUTE(dt),
                              PyDateTime_TIME_GET_SECOND(dt),
                              PyDateTime_TIME_GET_MICROSECOND(dt)/1000);
   else if (PyDate_Check(dt))
       return cmf::math::Time(PyDateTime_GET_DAY(dt),
                              PyDateTime_GET_MONTH(dt),
                              PyDateTime_GET_YEAR(dt));
   else if (PyDelta_Check(dt))
   {
     PyDateTime_Delta* delta=(PyDateTime_Delta*)(dt);
     long long ms=24 * 3600;
     ms*=delta->days;
     ms+=delta->seconds;
     ms*=1000;
     ms+=delta->microseconds/1000;
     return cmf::math::Time::Milliseconds(ms);
   }
   else 
   {
     PyErr_SetString(PyExc_ValueError,"Type is neither a cmf.Time nor a Python datetime object");
     return cmf::math::Time();
   } 
}
%}                         

%typemap(in) cmf::math::Time {
    $1 = convert_datetime_to_cmftime($input);    
}
%typemap(typecheck,precedence=0) cmf::math::Time {
    void * pt;    
    int res=SWIG_ConvertPtr($input,&pt,SWIGTYPE_p_cmf__math__Time,0);
    $1=SWIG_IsOK(res) || PyDateTime_Check($input) || PyDelta_Check($input) || PyDate_Check($input);
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
%naturalvar cmf::math::Time;
%implicitconv cmf::math::Time;




%implicitconv cmf::math::Date;
%naturalvar cmf::math::timeseries;
%implicitconv cmf::math::timeseries;


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
    state=property(get_state,set_state,"Gets the currect state")
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

%extend cmf::math::Time {
    %pythoncode 
    {
    def __repr__(self):
        if self>year*40:
            return self.AsDate().ToString()
        else:
            return self.ToString()
    def __nonzero__(self):
        return self.is_not_0();
    def __rmul__(self,other):
        return self*other;
    def AsPython(self):
        d=self.AsDate()
        return datetime.datetime(d.year,d.month,d.day,d.hour,d.minute,d.second,d.ms*1000)
    }
}

%extend cmf::math::Date {
    %pythoncode 
    {
    def __repr__(self):
        return self.ToString()
    def AsPython(self):
        return datetime.datetime(self.year,self.month,self.day,self.hour,self.minute,self.second,self.ms*1000)
    }
}


%extend cmf::math::numVector {
	double __getitem__(int index) 
	{
		int ndx = index < 0 ? $self->size() - index : index;
		if (ndx < 0 || ndx>=$self->size())
		{
			throw std::out_of_range("Index out of range");
		}
		return (*$self)[ndx];
	}
	void __setitem__(int index,double value) 
	{
		int ndx = index < 0 ? $self->size() - index : index;
		if (ndx < 0 || ndx>=$self->size())
		{
			throw std::out_of_range("Index out of range");
		}
		(*$self)[ndx]=value;
	}
	int __len__() {return $self->size();}
	cmf::math::numVector __radd__(real other)	{  return other+(*$self);	}
	cmf::math::numVector __rsub__(real other)	{  return other-(*$self);	}
	cmf::math::numVector __rmul__(real other)	{  return other*(*$self);	}
	cmf::math::numVector __rdiv__(real other)	{  return other/(*$self);	}
	%pythoncode {
	def __iter__(self):
	    for i in xrange(len(self)):
	        yield self[i]
	@property
	def __array_interface__(self):
	    return dict(shape=(len(self),),typestr='|f8',data=(self.adress(),0),version=3)
	}
}


%extend cmf::math::Integrator { %pythoncode {
    def run(self,start=day*0,end=day*1,step=day*1):
        self.t=start
        while self.t<end:
            self(self.t+step)
            yield self.t
}}
%extend cmf::math::timeseries
{
	double __len__()
	{
		return $self->size();
	}
	%pythoncode
    {
    def extend(self,list) :
        """ Adds the values of a sequence to the timeseries"""
        for item in list :
            self.add(float(item))
    def __getitem__(self,index):
        if isinstance(index,int):
            return self.get_i(index)
        elif isinstance(index,slice):
            if index.step:
                return self.get_slice(index.start,index.stop,index.step)
            else:
                return self.get_slice(index.start,index.stop)
        else:
            return self.get_t(index)
    def __setitem__(self,index,value):
        if isinstance(index,int):
            self.set_i(index,value)
        if isinstance(index,slice):
            if index.step:
                raise ValueError("Slices must be continous, when used for setting")
            else:
                if not isinstance(value,timeseries):
                    value=timeseries(value)
                self.set_slice(index.start,index.stop,value)
        else:
            self.set_t(index,value)
    def __iter__(self):
        for i in xrange(self.size()):
            yield self.get_i(i)
    def interpolate(self,begin,end,step):
        """ Returns a generator returning the interpolated values at the timesteps """
        if step>self.step():
            ts=self.reduce_avg(begin,step)
        else:
            ts=self
        for t in timerange(step,end,step):
            yield ts[t]
    def __radd__(self,other):
        return self + other;
    def __rmul__(self,other):
        return self + other;
    def __rsub__(self,other):
        res=-self
        res+=other
        return res
    def __rdiv__(self,other):
        res=self.inv() 
        res*=other
        return res
    }
}
        

%pythoncode {
def AsCMFtime(date):
    """Converts a python datetime to cmf.Time"""
    return Time(date.day,date.month,date.year,date.hour,date.minute,date.second,date.microsecond/1000)
def timerange(start,end,step=day):
    """Creates a generator of cmf.Time, similar to the Python range function"""
    return [start+step*x for x in range(0,int((end-start)/step))]
def xtimerange(start,end,step=day):
    """Creates a generator of cmf.Time, similar to the Python range function"""
    return (start+step*x for x in range(0,int((end-start)/step)))
}
