

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
%shared_ptr(cmf::math::integratable);

// %iterable_to_list(cmf::math::StateVariableList,cmf::math::StateVariable::ptr)
%implicitconv cmf::math::StateVariableList;
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
        for i in range(len(self)):
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

