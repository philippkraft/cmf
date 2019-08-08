

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
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
%typemap(out) cmf::bytestring {
        // Converting cmf::bytestring to PyBytes
    $result = PyBytes_FromStringAndSize($1.c_str(), $1.size());
}
%typemap(in) cmf::bytestring {
        // Convert PyBytes to cmf::bytestring
        if (PyBytes_Check($input)) {
                Py_ssize_t size = PyBytes_Size($input);
                $1 = cmf::bytestring(PyBytes_AsString($input), size);
        } else {
                SWIG_exception_fail(SWIG_TypeError,"WKB expects byte string");
        }
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

    def __getstate__(self):
        return self.AsMilliseconds()

    def __setstate__(self, data):
        self.__init__(data)

    def as_datetime(self):
        d=self.AsDate()
        return datetime.datetime(d.year,d.month,d.day,d.hour,d.minute,d.second,d.ms*1000)

    def as_timedelta(self):
        return datetime.timedelta(milliseconds=self.AsMilliseconds())

    year   = property(lambda self: self.AsDate().year)
    month  = property(lambda self: self.AsDate().month)
    day    = property(lambda self: self.AsDate().day)
    hour   = property(lambda self: self.AsDate().hour)
    minute = property(lambda self: self.AsDate().minute)
    second = property(lambda self: self.AsDate().second)
    ms     = property(lambda self: self.AsDate().ms)

    def __format__(self, fmt):
        return self.as_datetime().__format__(fmt)
    }
}

%extend cmf::math::Date {
    %pythoncode 
    {
    def __repr__(self):
        return self.to_string()

    def __getstate__(self):
        return Date.ToTime().__getstate__()

    def __setstate__(self, data):
        t = Time(data)
        self.__init__(t)

    def as_datetime(self):
        return datetime.datetime(self.year,self.month,self.day,self.hour,self.minute,self.second,self.ms*1000)

    def __format__(self, fmt):
        return self.as_datetime().__format__(fmt)
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
        for i in range(self.size()):
            yield self.get_i(i)

    def __radd__(self,other):
        return self + other

    def __rmul__(self,other):
        return self + other

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
