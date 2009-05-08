# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.38
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.
# This file is compatible with both classic and new-style classes.

from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        try:
            fp, pathname, description = imp.find_module('_cmf_math', [dirname(__file__)])
            _mod = imp.load_module('_cmf_math', fp, pathname, description)
        finally:
            if fp is not None: fp.close()
        return _mod
    _cmf_math = swig_import_helper()
    del swig_import_helper
else:
    import _cmf_math
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class SwigPyIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SwigPyIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SwigPyIterator, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    __swig_destroy__ = _cmf_math.delete_SwigPyIterator
    __del__ = lambda self : None;
    def value(self): return _cmf_math.SwigPyIterator_value(self)
    def incr(self, n = 1): return _cmf_math.SwigPyIterator_incr(self, n)
    def decr(self, n = 1): return _cmf_math.SwigPyIterator_decr(self, n)
    def distance(self, *args): return _cmf_math.SwigPyIterator_distance(self, *args)
    def equal(self, *args): return _cmf_math.SwigPyIterator_equal(self, *args)
    def copy(self): return _cmf_math.SwigPyIterator_copy(self)
    def next(self): return _cmf_math.SwigPyIterator_next(self)
    def __next__(self): return _cmf_math.SwigPyIterator___next__(self)
    def previous(self): return _cmf_math.SwigPyIterator_previous(self)
    def advance(self, *args): return _cmf_math.SwigPyIterator_advance(self, *args)
    def __eq__(self, *args): return _cmf_math.SwigPyIterator___eq__(self, *args)
    def __ne__(self, *args): return _cmf_math.SwigPyIterator___ne__(self, *args)
    def __iadd__(self, *args): return _cmf_math.SwigPyIterator___iadd__(self, *args)
    def __isub__(self, *args): return _cmf_math.SwigPyIterator___isub__(self, *args)
    def __add__(self, *args): return _cmf_math.SwigPyIterator___add__(self, *args)
    def __sub__(self, *args): return _cmf_math.SwigPyIterator___sub__(self, *args)
    def __iter__(self): return self
SwigPyIterator_swigregister = _cmf_math.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

import datetime

JULIANDAY_0_1_1900 = _cmf_math.JULIANDAY_0_1_1900
class Time(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Time, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Time, name)
    __repr__ = _swig_repr
    ms_per_day = _cmf_math.Time_ms_per_day
    def __init__(self, *args): 
        this = _cmf_math.new_Time(*args)
        try: self.this.append(this)
        except: self.this = this
    def AsDays(self): return _cmf_math.Time_AsDays(self)
    def AsHours(self): return _cmf_math.Time_AsHours(self)
    def AsMinutes(self): return _cmf_math.Time_AsMinutes(self)
    def AsSeconds(self): return _cmf_math.Time_AsSeconds(self)
    def AsMilliseconds(self): return _cmf_math.Time_AsMilliseconds(self)
    def AsDate(self): return _cmf_math.Time_AsDate(self)
    def ToString(self, seperator = ':'): return _cmf_math.Time_ToString(self, seperator)
    def __add__(self, *args): return _cmf_math.Time___add__(self, *args)
    def __sub__(self, *args): return _cmf_math.Time___sub__(self, *args)
    def __iadd__(self, *args): return _cmf_math.Time___iadd__(self, *args)
    def __isub__(self, *args): return _cmf_math.Time___isub__(self, *args)
    def __mul__(self, *args): return _cmf_math.Time___mul__(self, *args)
    def __imul__(self, *args): return _cmf_math.Time___imul__(self, *args)
    def __idiv__(self, *args): return _cmf_math.Time___idiv__(self, *args)
    def __div__(self, *args): return _cmf_math.Time___div__(self, *args)
    def __lt__(self, *args): return _cmf_math.Time___lt__(self, *args)
    def __gt__(self, *args): return _cmf_math.Time___gt__(self, *args)
    def __le__(self, *args): return _cmf_math.Time___le__(self, *args)
    def __ge__(self, *args): return _cmf_math.Time___ge__(self, *args)
    def __eq__(self, *args): return _cmf_math.Time___eq__(self, *args)
    def __ne__(self, *args): return _cmf_math.Time___ne__(self, *args)
    def NextDay(self): return _cmf_math.Time_NextDay(self)
    def NextFullHour(self): return _cmf_math.Time_NextFullHour(self)
    __swig_getmethods__["Years"] = lambda x: _cmf_math.Time_Years
    if _newclass:Years = staticmethod(_cmf_math.Time_Years)
    __swig_getmethods__["Days"] = lambda x: _cmf_math.Time_Days
    if _newclass:Days = staticmethod(_cmf_math.Time_Days)
    __swig_getmethods__["Hours"] = lambda x: _cmf_math.Time_Hours
    if _newclass:Hours = staticmethod(_cmf_math.Time_Hours)
    __swig_getmethods__["Minutes"] = lambda x: _cmf_math.Time_Minutes
    if _newclass:Minutes = staticmethod(_cmf_math.Time_Minutes)
    __swig_getmethods__["Seconds"] = lambda x: _cmf_math.Time_Seconds
    if _newclass:Seconds = staticmethod(_cmf_math.Time_Seconds)
    __swig_getmethods__["Milliseconds"] = lambda x: _cmf_math.Time_Milliseconds
    if _newclass:Milliseconds = staticmethod(_cmf_math.Time_Milliseconds)
    def __str__(self): return _cmf_math.Time___str__(self)
    def AsPython(self):
        d=self.AsDate()
        return datetime.datetime(d.year,d.month,d.day,d.hour,d.minute,d.second,d.ms*1000)

    __swig_destroy__ = _cmf_math.delete_Time
    __del__ = lambda self : None;
Time_swigregister = _cmf_math.Time_swigregister
Time_swigregister(Time)

def Time_Years(y = 1):
  return _cmf_math.Time_Years(y)
Time_Years = _cmf_math.Time_Years

def Time_Days(d = 1):
  return _cmf_math.Time_Days(d)
Time_Days = _cmf_math.Time_Days

def Time_Hours(h = 1):
  return _cmf_math.Time_Hours(h)
Time_Hours = _cmf_math.Time_Hours

def Time_Minutes(min = 1):
  return _cmf_math.Time_Minutes(min)
Time_Minutes = _cmf_math.Time_Minutes

def Time_Seconds(secs = 1):
  return _cmf_math.Time_Seconds(secs)
Time_Seconds = _cmf_math.Time_Seconds

def Time_Milliseconds(ms = 1):
  return _cmf_math.Time_Milliseconds(ms)
Time_Milliseconds = _cmf_math.Time_Milliseconds

class Date(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Date, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Date, name)
    __repr__ = _swig_repr
    __swig_setmethods__["year"] = _cmf_math.Date_year_set
    __swig_getmethods__["year"] = _cmf_math.Date_year_get
    if _newclass:year = _swig_property(_cmf_math.Date_year_get, _cmf_math.Date_year_set)
    __swig_setmethods__["month"] = _cmf_math.Date_month_set
    __swig_getmethods__["month"] = _cmf_math.Date_month_get
    if _newclass:month = _swig_property(_cmf_math.Date_month_get, _cmf_math.Date_month_set)
    __swig_setmethods__["day"] = _cmf_math.Date_day_set
    __swig_getmethods__["day"] = _cmf_math.Date_day_get
    if _newclass:day = _swig_property(_cmf_math.Date_day_get, _cmf_math.Date_day_set)
    __swig_setmethods__["hour"] = _cmf_math.Date_hour_set
    __swig_getmethods__["hour"] = _cmf_math.Date_hour_get
    if _newclass:hour = _swig_property(_cmf_math.Date_hour_get, _cmf_math.Date_hour_set)
    __swig_setmethods__["minute"] = _cmf_math.Date_minute_set
    __swig_getmethods__["minute"] = _cmf_math.Date_minute_get
    if _newclass:minute = _swig_property(_cmf_math.Date_minute_get, _cmf_math.Date_minute_set)
    __swig_setmethods__["second"] = _cmf_math.Date_second_set
    __swig_getmethods__["second"] = _cmf_math.Date_second_get
    if _newclass:second = _swig_property(_cmf_math.Date_second_get, _cmf_math.Date_second_set)
    __swig_setmethods__["ms"] = _cmf_math.Date_ms_set
    __swig_getmethods__["ms"] = _cmf_math.Date_ms_get
    if _newclass:ms = _swig_property(_cmf_math.Date_ms_get, _cmf_math.Date_ms_set)
    def __init__(self, *args): 
        this = _cmf_math.new_Date(*args)
        try: self.this.append(this)
        except: self.this = this
    def ToTime(self): return _cmf_math.Date_ToTime(self)
    def DOY(self): return _cmf_math.Date_DOY(self)
    def ToString(self): return _cmf_math.Date_ToString(self)
    def __str__(self): return _cmf_math.Date___str__(self)
    def AsPython(self):
        return datetime.datetime(self.year,self.month,self.day,self.hour,self.minute,self.second,self.ms*1000)

    __swig_destroy__ = _cmf_math.delete_Date
    __del__ = lambda self : None;
Date_swigregister = _cmf_math.Date_swigregister
Date_swigregister(Date)

class timeseries(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, timeseries, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, timeseries, name)
    __repr__ = _swig_repr
    __swig_setmethods__["begin"] = _cmf_math.timeseries_begin_set
    __swig_getmethods__["begin"] = _cmf_math.timeseries_begin_get
    if _newclass:begin = _swig_property(_cmf_math.timeseries_begin_get, _cmf_math.timeseries_begin_set)
    __swig_setmethods__["step"] = _cmf_math.timeseries_step_set
    __swig_getmethods__["step"] = _cmf_math.timeseries_step_get
    if _newclass:step = _swig_property(_cmf_math.timeseries_step_get, _cmf_math.timeseries_step_set)
    def end(self): return _cmf_math.timeseries_end(self)
    __swig_setmethods__["interpolationpower"] = _cmf_math.timeseries_interpolationpower_set
    __swig_getmethods__["interpolationpower"] = _cmf_math.timeseries_interpolationpower_get
    if _newclass:interpolationpower = _swig_property(_cmf_math.timeseries_interpolationpower_get, _cmf_math.timeseries_interpolationpower_set)
    def Add(self, *args): return _cmf_math.timeseries_Add(self, *args)
    def isempty(self): return _cmf_math.timeseries_isempty(self)
    def clear(self): return _cmf_math.timeseries_clear(self)
    def size(self): return _cmf_math.timeseries_size(self)
    def __iadd__(self, *args): return _cmf_math.timeseries___iadd__(self, *args)
    def __isub__(self, *args): return _cmf_math.timeseries___isub__(self, *args)
    def __imul__(self, *args): return _cmf_math.timeseries___imul__(self, *args)
    def __idiv__(self, *args): return _cmf_math.timeseries___idiv__(self, *args)
    def power(self, *args): return _cmf_math.timeseries_power(self, *args)
    def reduce_min(self, *args): return _cmf_math.timeseries_reduce_min(self, *args)
    def reduce_max(self, *args): return _cmf_math.timeseries_reduce_max(self, *args)
    def reduce_sum(self, *args): return _cmf_math.timeseries_reduce_sum(self, *args)
    def reduce_avg(self, *args): return _cmf_math.timeseries_reduce_avg(self, *args)
    def Save(self, *args): return _cmf_math.timeseries_Save(self, *args)
    def __init__(self, *args): 
        this = _cmf_math.new_timeseries(*args)
        try: self.this.append(this)
        except: self.this = this
    def __size__(self): return _cmf_math.timeseries___size__(self)
    def __getitem__(self, *args): return _cmf_math.timeseries___getitem__(self, *args)
    def __setitem__(self, *args): return _cmf_math.timeseries___setitem__(self, *args)
    def AddList(list) :
        """ Adds the values of a list to the timeseries"""
        for item in list :
            self.Add(float(item))

    __swig_destroy__ = _cmf_math.delete_timeseries
    __del__ = lambda self : None;
timeseries_swigregister = _cmf_math.timeseries_swigregister
timeseries_swigregister(timeseries)
cvar = _cmf_math.cvar
ms = cvar.ms
sec = cvar.sec
min = cvar.min
h = cvar.h
day = cvar.day
week = cvar.week
month = cvar.month
year = cvar.year

class numVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, numVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, numVector, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _cmf_math.new_numVector(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _cmf_math.delete_numVector
    __del__ = lambda self : None;
    def size(self): return _cmf_math.numVector_size(self)
    def resize(self, *args): return _cmf_math.numVector_resize(self, *args)
    def __neg__(self): return _cmf_math.numVector___neg__(self)
    def power(self, *args): return _cmf_math.numVector_power(self, *args)
    def __iadd__(self, *args): return _cmf_math.numVector___iadd__(self, *args)
    def __isub__(self, *args): return _cmf_math.numVector___isub__(self, *args)
    def __imul__(self, *args): return _cmf_math.numVector___imul__(self, *args)
    def __idiv__(self, *args): return _cmf_math.numVector___idiv__(self, *args)
    def apply(self, *args): return _cmf_math.numVector_apply(self, *args)
    def dot(self, *args): return _cmf_math.numVector_dot(self, *args)
    def sum(self): return _cmf_math.numVector_sum(self)
    def max(self): return _cmf_math.numVector_max(self)
    def min(self): return _cmf_math.numVector_min(self)
    def norm(self, normtype = 0): return _cmf_math.numVector_norm(self, normtype)
    def __getitem__(self, *args): return _cmf_math.numVector___getitem__(self, *args)
    def __setitem__(self, *args): return _cmf_math.numVector___setitem__(self, *args)
    def __len__(self): return _cmf_math.numVector___len__(self)
numVector_swigregister = _cmf_math.numVector_swigregister
numVector_swigregister(numVector)

class svVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, svVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, svVector, name)
    __repr__ = _swig_repr
    def iterator(self): return _cmf_math.svVector_iterator(self)
    def __iter__(self): return self.iterator()
    def __nonzero__(self): return _cmf_math.svVector___nonzero__(self)
    def __bool__(self): return _cmf_math.svVector___bool__(self)
    def __len__(self): return _cmf_math.svVector___len__(self)
    def pop(self): return _cmf_math.svVector_pop(self)
    def __getslice__(self, *args): return _cmf_math.svVector___getslice__(self, *args)
    def __setslice__(self, *args): return _cmf_math.svVector___setslice__(self, *args)
    def __delslice__(self, *args): return _cmf_math.svVector___delslice__(self, *args)
    def __delitem__(self, *args): return _cmf_math.svVector___delitem__(self, *args)
    def __getitem__(self, *args): return _cmf_math.svVector___getitem__(self, *args)
    def __setitem__(self, *args): return _cmf_math.svVector___setitem__(self, *args)
    def append(self, *args): return _cmf_math.svVector_append(self, *args)
    def empty(self): return _cmf_math.svVector_empty(self)
    def size(self): return _cmf_math.svVector_size(self)
    def clear(self): return _cmf_math.svVector_clear(self)
    def swap(self, *args): return _cmf_math.svVector_swap(self, *args)
    def get_allocator(self): return _cmf_math.svVector_get_allocator(self)
    def begin(self): return _cmf_math.svVector_begin(self)
    def end(self): return _cmf_math.svVector_end(self)
    def rbegin(self): return _cmf_math.svVector_rbegin(self)
    def rend(self): return _cmf_math.svVector_rend(self)
    def pop_back(self): return _cmf_math.svVector_pop_back(self)
    def erase(self, *args): return _cmf_math.svVector_erase(self, *args)
    def __init__(self, *args): 
        this = _cmf_math.new_svVector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(self, *args): return _cmf_math.svVector_push_back(self, *args)
    def front(self): return _cmf_math.svVector_front(self)
    def back(self): return _cmf_math.svVector_back(self)
    def assign(self, *args): return _cmf_math.svVector_assign(self, *args)
    def resize(self, *args): return _cmf_math.svVector_resize(self, *args)
    def insert(self, *args): return _cmf_math.svVector_insert(self, *args)
    def reserve(self, *args): return _cmf_math.svVector_reserve(self, *args)
    def capacity(self): return _cmf_math.svVector_capacity(self)
    __swig_destroy__ = _cmf_math.delete_svVector
    __del__ = lambda self : None;
svVector_swigregister = _cmf_math.svVector_swigregister
svVector_swigregister(svVector)

def __add__(*args):
  return _cmf_math.__add__(*args)
__add__ = _cmf_math.__add__

def __sub__(*args):
  return _cmf_math.__sub__(*args)
__sub__ = _cmf_math.__sub__

def __mul__(*args):
  return _cmf_math.__mul__(*args)
__mul__ = _cmf_math.__mul__

def __div__(*args):
  return _cmf_math.__div__(*args)
__div__ = _cmf_math.__div__

class StateVariable(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, StateVariable, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, StateVariable, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    def Derivate(self, *args): return _cmf_math.StateVariable_Derivate(self, *args)
    def State(self, *args): return _cmf_math.StateVariable_State(self, *args)
    def StateIsChanged(self): return _cmf_math.StateVariable_StateIsChanged(self)
    __swig_destroy__ = _cmf_math.delete_StateVariable
    __del__ = lambda self : None;
StateVariable_swigregister = _cmf_math.StateVariable_swigregister
StateVariable_swigregister(StateVariable)

class StateVariableVector(svVector):
    __swig_setmethods__ = {}
    for _s in [svVector]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, StateVariableVector, name, value)
    __swig_getmethods__ = {}
    for _s in [svVector]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, StateVariableVector, name)
    __repr__ = _swig_repr
    def CopyStates(self, *args): return _cmf_math.StateVariableVector_CopyStates(self, *args)
    def SetStates(self, *args): return _cmf_math.StateVariableVector_SetStates(self, *args)
    def __iadd__(self, *args): return _cmf_math.StateVariableVector___iadd__(self, *args)
    def CopyDerivs(self, *args): return _cmf_math.StateVariableVector_CopyDerivs(self, *args)
    def GetStates(self): return _cmf_math.StateVariableVector_GetStates(self)
    def GetDerivs(self, *args): return _cmf_math.StateVariableVector_GetDerivs(self, *args)
    def __init__(self): 
        this = _cmf_math.new_StateVariableVector()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _cmf_math.delete_StateVariableVector
    __del__ = lambda self : None;
StateVariableVector_swigregister = _cmf_math.StateVariableVector_swigregister
StateVariableVector_swigregister(StateVariableVector)

class StateVariableOwner(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, StateVariableOwner, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, StateVariableOwner, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    def AddStateVariables(self, *args): return _cmf_math.StateVariableOwner_AddStateVariables(self, *args)
    __swig_destroy__ = _cmf_math.delete_StateVariableOwner
    __del__ = lambda self : None;
StateVariableOwner_swigregister = _cmf_math.StateVariableOwner_swigregister
StateVariableOwner_swigregister(StateVariableOwner)

class Integrator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Integrator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Integrator, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    __swig_setmethods__["Tag"] = _cmf_math.Integrator_Tag_set
    __swig_getmethods__["Tag"] = _cmf_math.Integrator_Tag_get
    if _newclass:Tag = _swig_property(_cmf_math.Integrator_Tag_get, _cmf_math.Integrator_Tag_set)
    def count(self): return _cmf_math.Integrator_count(self)
    def state(self, *args): return _cmf_math.Integrator_state(self, *args)
    def AddStatesFromOwner(self, *args): return _cmf_math.Integrator_AddStatesFromOwner(self, *args)
    def AddState(self, *args): return _cmf_math.Integrator_AddState(self, *args)
    def ModelTime(self, *args): return _cmf_math.Integrator_ModelTime(self, *args)
    def TimeStep(self): return _cmf_math.Integrator_TimeStep(self)
    def NextTimeStep(self, *args): return _cmf_math.Integrator_NextTimeStep(self, *args)
    def MinTimestep(self): return _cmf_math.Integrator_MinTimestep(self)
    def Iterations(self): return _cmf_math.Integrator_Iterations(self)
    def ResetIterations(self): return _cmf_math.Integrator_ResetIterations(self)
    def Reset(self): return _cmf_math.Integrator_Reset(self)
    def Copy(self): return _cmf_math.Integrator_Copy(self)
    def Integrate(self, *args): return _cmf_math.Integrator_Integrate(self, *args)
    def IntegrateUntil(self, *args): return _cmf_math.Integrator_IntegrateUntil(self, *args)
    def __call__(self, *args): return _cmf_math.Integrator___call__(self, *args)
    __swig_destroy__ = _cmf_math.delete_Integrator
    __del__ = lambda self : None;
Integrator_swigregister = _cmf_math.Integrator_swigregister
Integrator_swigregister(Integrator)

class BDF2(Integrator):
    __swig_setmethods__ = {}
    for _s in [Integrator]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, BDF2, name, value)
    __swig_getmethods__ = {}
    for _s in [Integrator]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, BDF2, name)
    __repr__ = _swig_repr
    def AddStatesFromOwner(self, *args): return _cmf_math.BDF2_AddStatesFromOwner(self, *args)
    def __init__(self, *args): 
        this = _cmf_math.new_BDF2(*args)
        try: self.this.append(this)
        except: self.this = this
    def Copy(self): return _cmf_math.BDF2_Copy(self)
    def Integrate(self, *args): return _cmf_math.BDF2_Integrate(self, *args)
    __swig_destroy__ = _cmf_math.delete_BDF2
    __del__ = lambda self : None;
BDF2_swigregister = _cmf_math.BDF2_swigregister
BDF2_swigregister(BDF2)

class ExplicitEuler_fixed(Integrator):
    __swig_setmethods__ = {}
    for _s in [Integrator]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, ExplicitEuler_fixed, name, value)
    __swig_getmethods__ = {}
    for _s in [Integrator]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, ExplicitEuler_fixed, name)
    __repr__ = _swig_repr
    def AddStatesFromOwner(self, *args): return _cmf_math.ExplicitEuler_fixed_AddStatesFromOwner(self, *args)
    def __init__(self, *args): 
        this = _cmf_math.new_ExplicitEuler_fixed(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _cmf_math.delete_ExplicitEuler_fixed
    __del__ = lambda self : None;
    def Copy(self): return _cmf_math.ExplicitEuler_fixed_Copy(self)
    def Integrate(self, *args): return _cmf_math.ExplicitEuler_fixed_Integrate(self, *args)
ExplicitEuler_fixed_swigregister = _cmf_math.ExplicitEuler_fixed_swigregister
ExplicitEuler_fixed_swigregister(ExplicitEuler_fixed)

class ExplicitEuler_variable(Integrator):
    __swig_setmethods__ = {}
    for _s in [Integrator]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, ExplicitEuler_variable, name, value)
    __swig_getmethods__ = {}
    for _s in [Integrator]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, ExplicitEuler_variable, name)
    __repr__ = _swig_repr
    __swig_setmethods__["WithStepsizeControl"] = _cmf_math.ExplicitEuler_variable_WithStepsizeControl_set
    __swig_getmethods__["WithStepsizeControl"] = _cmf_math.ExplicitEuler_variable_WithStepsizeControl_get
    if _newclass:WithStepsizeControl = _swig_property(_cmf_math.ExplicitEuler_variable_WithStepsizeControl_get, _cmf_math.ExplicitEuler_variable_WithStepsizeControl_set)
    def AddStatesFromOwner(self, *args): return _cmf_math.ExplicitEuler_variable_AddStatesFromOwner(self, *args)
    def __init__(self, *args): 
        this = _cmf_math.new_ExplicitEuler_variable(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _cmf_math.delete_ExplicitEuler_variable
    __del__ = lambda self : None;
    def Copy(self): return _cmf_math.ExplicitEuler_variable_Copy(self)
    def Integrate(self, *args): return _cmf_math.ExplicitEuler_variable_Integrate(self, *args)
ExplicitEuler_variable_swigregister = _cmf_math.ExplicitEuler_variable_swigregister
ExplicitEuler_variable_swigregister(ExplicitEuler_variable)

class ImplicitEuler(Integrator):
    __swig_setmethods__ = {}
    for _s in [Integrator]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, ImplicitEuler, name, value)
    __swig_getmethods__ = {}
    for _s in [Integrator]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, ImplicitEuler, name)
    __repr__ = _swig_repr
    def AddStatesFromOwner(self, *args): return _cmf_math.ImplicitEuler_AddStatesFromOwner(self, *args)
    def __init__(self, *args): 
        this = _cmf_math.new_ImplicitEuler(*args)
        try: self.this.append(this)
        except: self.this = this
    def Copy(self): return _cmf_math.ImplicitEuler_Copy(self)
    def Integrate(self, *args): return _cmf_math.ImplicitEuler_Integrate(self, *args)
    __swig_destroy__ = _cmf_math.delete_ImplicitEuler
    __del__ = lambda self : None;
ImplicitEuler_swigregister = _cmf_math.ImplicitEuler_swigregister
ImplicitEuler_swigregister(ImplicitEuler)

class RKFIntegrator(Integrator):
    __swig_setmethods__ = {}
    for _s in [Integrator]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, RKFIntegrator, name, value)
    __swig_getmethods__ = {}
    for _s in [Integrator]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, RKFIntegrator, name)
    __repr__ = _swig_repr
    def AddStatesFromOwner(self, *args): return _cmf_math.RKFIntegrator_AddStatesFromOwner(self, *args)
    def __init__(self, *args): 
        this = _cmf_math.new_RKFIntegrator(*args)
        try: self.this.append(this)
        except: self.this = this
    def Copy(self): return _cmf_math.RKFIntegrator_Copy(self)
    def Integrate(self, *args): return _cmf_math.RKFIntegrator_Integrate(self, *args)
    __swig_destroy__ = _cmf_math.delete_RKFIntegrator
    __del__ = lambda self : None;
RKFIntegrator_swigregister = _cmf_math.RKFIntegrator_swigregister
RKFIntegrator_swigregister(RKFIntegrator)

GEAR_MAX_ITER = _cmf_math.GEAR_MAX_ITER
GEAR_RECOVER_AFTER_STEPS = _cmf_math.GEAR_RECOVER_AFTER_STEPS
class Gears_Fixpoint(Integrator):
    __swig_setmethods__ = {}
    for _s in [Integrator]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, Gears_Fixpoint, name, value)
    __swig_getmethods__ = {}
    for _s in [Integrator]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, Gears_Fixpoint, name)
    __repr__ = _swig_repr
    def Time_reduce_stage(self): return _cmf_math.Gears_Fixpoint_Time_reduce_stage(self)
    def AddStatesFromOwner(self, *args): return _cmf_math.Gears_Fixpoint_AddStatesFromOwner(self, *args)
    def __init__(self, *args): 
        this = _cmf_math.new_Gears_Fixpoint(*args)
        try: self.this.append(this)
        except: self.this = this
    def Copy(self): return _cmf_math.Gears_Fixpoint_Copy(self)
    __swig_destroy__ = _cmf_math.delete_Gears_Fixpoint
    __del__ = lambda self : None;
    def Integrate(self, *args): return _cmf_math.Gears_Fixpoint_Integrate(self, *args)
Gears_Fixpoint_swigregister = _cmf_math.Gears_Fixpoint_swigregister
Gears_Fixpoint_swigregister(Gears_Fixpoint)

class CVodeIntegrator(Integrator):
    __swig_setmethods__ = {}
    for _s in [Integrator]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, CVodeIntegrator, name, value)
    __swig_getmethods__ = {}
    for _s in [Integrator]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, CVodeIntegrator, name)
    __repr__ = _swig_repr
    __swig_setmethods__["preconditioner"] = _cmf_math.CVodeIntegrator_preconditioner_set
    __swig_getmethods__["preconditioner"] = _cmf_math.CVodeIntegrator_preconditioner_get
    if _newclass:preconditioner = _swig_property(_cmf_math.CVodeIntegrator_preconditioner_get, _cmf_math.CVodeIntegrator_preconditioner_set)
    __swig_setmethods__["maxl"] = _cmf_math.CVodeIntegrator_maxl_set
    __swig_getmethods__["maxl"] = _cmf_math.CVodeIntegrator_maxl_get
    if _newclass:maxl = _swig_property(_cmf_math.CVodeIntegrator_maxl_get, _cmf_math.CVodeIntegrator_maxl_set)
    __swig_setmethods__["LinearSolver"] = _cmf_math.CVodeIntegrator_LinearSolver_set
    __swig_getmethods__["LinearSolver"] = _cmf_math.CVodeIntegrator_LinearSolver_get
    if _newclass:LinearSolver = _swig_property(_cmf_math.CVodeIntegrator_LinearSolver_get, _cmf_math.CVodeIntegrator_LinearSolver_set)
    __swig_setmethods__["MaxOrder"] = _cmf_math.CVodeIntegrator_MaxOrder_set
    __swig_getmethods__["MaxOrder"] = _cmf_math.CVodeIntegrator_MaxOrder_get
    if _newclass:MaxOrder = _swig_property(_cmf_math.CVodeIntegrator_MaxOrder_get, _cmf_math.CVodeIntegrator_MaxOrder_set)
    def GetOrder(self): return _cmf_math.CVodeIntegrator_GetOrder(self)
    def Initialize(self): return _cmf_math.CVodeIntegrator_Initialize(self)
    def Integrate(self, *args): return _cmf_math.CVodeIntegrator_Integrate(self, *args)
    def Reset(self): return _cmf_math.CVodeIntegrator_Reset(self)
    def __init__(self, *args): 
        this = _cmf_math.new_CVodeIntegrator(*args)
        try: self.this.append(this)
        except: self.this = this
    def Copy(self): return _cmf_math.CVodeIntegrator_Copy(self)
    __swig_destroy__ = _cmf_math.delete_CVodeIntegrator
    __del__ = lambda self : None;
CVodeIntegrator_swigregister = _cmf_math.CVodeIntegrator_swigregister
CVodeIntegrator_swigregister(CVodeIntegrator)

def AsCMFtime(date):
    """Converts a python datetime to cmf.Time"""
    return Time(date.day,date.month,date.year,date.hour,date.minute,date.second,date.microsecond/1000)
def timerange(start,end,step=day):
    """Creates a list of cmf.Time, similar to the Python range function"""
    return [start+step*x for x in range(0,int((end-start)/step+0.999))]



