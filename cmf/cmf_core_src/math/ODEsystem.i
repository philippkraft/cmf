

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
#include "math/statevariable.h"
%}

%shared_ptr(cmf::math::StateVariable);


%attribute(cmf::math::StateVariable,real,state,get_state,set_state);

%typemap(in) const cmf::math::state_list& (cmf::math::state_list temp_list) {
   if (SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, SWIG_POINTER_EXCEPTION) == -1) {
      int res = PyObject_HasAttrString($input, "__cmf_state_list_interface__");
      if (res) { // Found __cmf_state_list_interface__
         PyObject* list = PyObject_CallMethod($input, "__cmf_state_list_interface__", "");
         if (list==NULL) { SWIG_exception_fail(SWIG_TypeError,"state list interface fails");}
         res = SWIG_ConvertPtr(list, (void **) &$1, $1_descriptor, SWIG_POINTER_EXCEPTION);
         if (!SWIG_IsOK(res)) {SWIG_exception_fail(SWIG_TypeError,"state list interface fails to convert to state_list");}
      } 
      else {// Test iterable, no __cmf_state_list_interface__
         int conversion_errors = 0;
         int res = iterable_to_list<cmf::math::StateVariable::ptr, cmf::math::state_list>
                  ($input,$descriptor(cmf::math::StateVariable::ptr*), temp_list, &conversion_errors);
         if (SWIG_IsOK(res)) {
             if (temp_list.size() > 0 || conversion_errors == 0)
                  $1 = &temp_list;
             else
                  SWIG_exception_fail(SWIG_TypeError,"Input iterable contains no states");
         } else {
             SWIG_exception_fail(SWIG_TypeError,"Input not state list like");
         }
      }
   }
}

%typemap(typecheck,precedence=0) const cmf::math::state_list& {
   // 1. Check if object is wrapped state_list
   if (SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0))) {
      $1 = 1;
   } // 2. Check if object has __cmf_state_list_interface__
   else if (SWIG_IsOK(PyObject_HasAttrString($input, "__cmf_state_list_interface__"))) {
      $1 = 2;
   }
   else // 3. Check if object is iterable
   {
      PyObject* iter = PyObject_GetIter($input);
      if (iter != 0) {
          Py_DECREF(iter);
          $1 = 3;
      } 
      else {
         $1 = 0;
      }
   }
}


%rename(__getitem) cmf::math::state_list::operator[];

%iterable_to_list(cmf::math::integratable_list,cmf::math::integratable::ptr)
%rename(__getitem) cmf::math::integratable_list::operator[];

%state_downcast(cmf::math::StateVariable::ptr cmf::math::state_list::operator[]);

%include "math/statevariable.h"

%extend__repr__(cmf::math::StateVariable);

%extend_pysequence(cmf::math::state_list);

%rename(__cmf_state_list_interface__) *::operator cmf::math::state_list;

