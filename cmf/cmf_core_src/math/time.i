

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
#include "datetime.h"
#include "math/real.h"
#include "math/time.h"
#include "math/timeseries.h"
%}
// Get Math
%pythoncode
{
import datetime
import struct
}
%init %{
PyDateTime_IMPORT;
%}

%{
static cmf::math::Time convert_datetime_to_cmftime(PyObject* dt) {
    cmf::math::Time* swigtime=0;
    int res=SWIG_ConvertPtr(dt,(void**)(&swigtime),SWIGTYPE_p_cmf__math__Time,0);
    if (SWIG_IsOK(res) && swigtime!=0) {
        return *swigtime;
    } else if (PyDateTime_Check(dt)) {
        return cmf::math::Time(PyDateTime_GET_DAY(dt),
                              PyDateTime_GET_MONTH(dt),
                              PyDateTime_GET_YEAR(dt),
                              PyDateTime_DATE_GET_HOUR(dt),
                              PyDateTime_DATE_GET_MINUTE(dt),
                              PyDateTime_DATE_GET_SECOND(dt),
                              PyDateTime_DATE_GET_MICROSECOND(dt)/1000);
    } else if (PyDate_Check(dt)) {
       return cmf::math::Time(PyDateTime_GET_DAY(dt),
                              PyDateTime_GET_MONTH(dt),
                              PyDateTime_GET_YEAR(dt));
    } else if (PyDelta_Check(dt)) {
        PyDateTime_Delta* delta=(PyDateTime_Delta*)(dt);
        long long ms=24 * 3600;
        ms*=delta->days;
        ms+=delta->seconds;
        ms*=1000;
        ms+=delta->microseconds/1000;
        return cmf::math::timespan(ms);
    } else {
       return cmf::math::never;
    } 
 
}
static bool check_time(PyObject* dt) {
    void * pT = 0;
    int res=SWIG_ConvertPtr(dt,&pT,SWIGTYPE_p_cmf__math__Time,0);
    if (SWIG_IsOK(res) && pT!=0) return true;
    else if (PyDateTime_Check(dt)) return true;
    else if (PyDelta_Check(dt)) return true;
    else if (PyDate_Check(dt)) return true;
    else return false;
}


%}                         

%typemap(in) cmf::math::Time {
    $1 = convert_datetime_to_cmftime($input);
    if ($1 == cmf::math::never)  {
        SWIG_exception_fail(SWIG_TypeError,"Can't convert input value to cmf.Time object");
    }
}
%typemap(typecheck,precedence=0) cmf::math::Time {
    $1 = check_time($input);
}

/**%typemap(in) cmf::math::Time * (cmf::math::Time T) {
    // typemap Time*
    T = convert_datetime_to_cmftime($input);
    if (T == cmf::math::never)  {
        SWIG_exception_fail(SWIG_TypeError,"Can't convert input value to cmf.Time object");
    } else {
        $1 = &T;
    }
}
%typemap(typecheck,precedence=100) cmf::math::Time *{
    $1 = check_time($input); // typecheck cmf::math::Time *
}**/
%typemap(in) cmf::math::Time & (cmf::math::Time T){
    // typemape Time&
    T = convert_datetime_to_cmftime($input);
    if (T == cmf::math::never) {
        SWIG_exception_fail(SWIG_TypeError,"Can't convert input value to cmf.Time object");
    } else {
        $1 = &T;
    }
}
%typemap(typecheck,precedence=100) cmf::math::Time &{
    $1 = check_time($input); // typecheck cmf::math::Time &
}
%typemap(in) const cmf::math::Time & (cmf::math::Time T){
    // typemape const Time&
    T = convert_datetime_to_cmftime($input);
    if (T == cmf::math::never) {
        SWIG_exception_fail(SWIG_TypeError,"Can't convert input value to cmf.Time object");
    } else {
        $1 = &T;
    }
}
%typemap(typecheck,precedence=100) const cmf::math::Time &{
    $1 = check_time($input); // typecheck const cmf::math::Time &
}

%implicitconv cmf::math::Time;
%implicitconv cmf::math::Date;

%include "math/real.h"
%include "math/time.h"


%typemap(in) cmf::math::timeseries {
    cmf::math::timeseries *p_ts = &$1;
    int res=SWIG_ConvertPtr($input,(void**)(&p_ts),$descriptor(cmf::math::timeseries*),0);
    if (!(SWIG_IsOK(res)) || p_ts==0) {
        if (PyNumber_Check($input)) {
            double scalar = PyFloat_AsDouble($input);
            $1 = cmf::math::timeseries::from_scalar(scalar);
        } else {
            SWIG_exception_fail(SWIG_TypeError,"Can't convert input value to cmf.timeseries object");
        }   
    } else {
        $1 = *p_ts;
    }      
}
%typemap(typecheck,precedence=200) cmf::math::timeseries {
    void * pt;    
    int res=SWIG_ConvertPtr($input,&pt,$descriptor(cmf::math::timeseries*),0);
    $1=SWIG_IsOK(res) || PyNumber_Check($input);
}

%attributeval(cmf::math::timeseries,cmf::math::Time,begin,begin);
%attributeval(cmf::math::timeseries,cmf::math::Time,step,step);
%attributeval(cmf::math::timeseries,cmf::math::Time,end,end);
//%naturalvar cmf::math::timeseries;

%implicitconv cmf::math::timeseries;

%include "math/timeseries.h"






%extend cmf::math::Time {
    %pythoncode 
    {
    def __repr__(self):
        if self>year*40:
            return self.AsDate().to_string()
        else:
            return self.to_string()
    def __nonzero__(self):
        return self.is_not_0()
    def __rmul__(self,other):
        return self*other
    def __radd__(self,other):
        return self + other
    def AsPython(self):
        d=self.AsDate()
        return datetime.datetime(d.year,d.month,d.day,d.hour,d.minute,d.second,d.ms*1000)
    year   = property(lambda self: self.AsDate().year)
    month  = property(lambda self: self.AsDate().month)
    day    = property(lambda self: self.AsDate().day)
    hour   = property(lambda self: self.AsDate().hour)
    minute = property(lambda self: self.AsDate().minute)
    second = property(lambda self: self.AsDate().second)
    ms     = property(lambda self: self.AsDate().ms)
    }
}

%extend cmf::math::Date {
    %pythoncode 
    {
    def __repr__(self):
        return self.to_string()
    def AsPython(self):
        return datetime.datetime(self.year,self.month,self.day,self.hour,self.minute,self.second,self.ms*1000)
    }
}



%extend cmf::math::timeseries
{
    ptrdiff_t __len__()
    {
        return $self->size();
    }
    %pythoncode
    {
    def __repr__(self):
       return "cmf.timeseries(%s:%s:%s,count=%i)" % (self.begin,self.end,self.step,self.size())
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
        elif isinstance(index,slice):
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
    def iter_time(self, as_float=0):
        """Returns an iterator to iterate over each timestep
        as_float if True, the timesteps will returned as floating point numbers representing the days after 1.1.0001 00:00
        """
        for i in xrange(len(self)):
            if as_float:
                yield ((self.begin + self.step * i) - cmf.Time(1,1,1)).AsDays()
            else:
                yield self.begin + self.step * i
    def to_buffer(self):
        """Returns a binary buffer filled with the data of self"""
        return struct.pack('qqqq%id' % self.size(),self.begin.AsMilliseconds(),self.step.AsMilliseconds(),self.interpolationpower(), *self)
    def to_file(self,f):
        """ Saves a timeseries in a special binary format.
        The format consists of 4 integers with 64 bit, indicating the milliseconds after the 31.12.1899 00:00 of the beginning of the timeseries, the milliseconds of the time step,
        the interpolation power and the number of values. The following 64 bit floats, are the values of the timeseries
        """
        if isinstance(f,str):
            f=open(f,'wb')
        elif not hasattr(f,'write'):
            raise TypeError("The file f must be either an object providing a write method, like a file, or a valid file name")
        f.write(struct.pack('qqqq%id' % self.size(),  self.size(), self.begin.AsMilliseconds(),self.step.AsMilliseconds(),self.interpolationpower(), *self))
    def to_pandas(self):
        """
        Returns the timeseries as a pandas Series object
        :return: A pandas.Series object with the timesteps as index
        """
        import pandas as pd
        import numpy as np

        return pd.Series(data=np.array(self),index=(t.AsPython() for t in self.iter_time()))
        
    @classmethod
    def from_sequence(cls,begin,step,sequence=[],interpolation_mode=1):
        res=cls(begin,step,interpolation_mode)
        res.extend(sequence)
        return res
        
    @classmethod
    def from_buffer(cls,buf):
        header_length=struct.calcsize('qqqq') 
        header=struct.unpack('qqqq',buffer[:header_length])
        res=cls(header[1]*ms,header[2]*ms,header[3])
        res.extend(struct.unpack('%id' % header[0],*buffer(buf,header_length,header[0]*8)))
        return res
    @classmethod
    def from_file(cls,f):
        """ Loads a timeseries saved with to_file from a file 
        Description of the file layout:
        byte: 
        0   Number of (int64)
        8   Begin of timeseries (in ms since 31.12.1899 00:00) (int64)
        16  Step size of timeseries (in ms) (int64)
        24  Interpolation power (int64)
        32  First value of timeseries (float64)
        """
        if isinstance(f,str):
            f=open(f,'rb')
        elif not hasattr(f,'read'):
            raise TypeError("The file f must either implement a 'read' method, like a file, or must be a vild file name")
        header_length=struct.calcsize('qqqq') 
        header=struct.unpack('qqqq',f.read(header_length))
        res=cls(header[1]*ms,header[2]*ms,header[3])
        res.extend(struct.unpack('%id' % header[0],f.read(-1)))
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
