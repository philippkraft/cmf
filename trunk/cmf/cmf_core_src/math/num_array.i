

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
#include "math/num_array.h"
%}
%rename (__set_data_from_adress) cmf::math::num_array::set_data_from_adress;
%include "math/num_array.h"

%extend cmf::math::num_array {
    double __getitem__(int index) 
    {
        int ndx = index < 0 ? $self->size() + index : index;
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
    cmf::math::num_array __radd__(real other)	{  return other+(*$self);	}
    cmf::math::num_array __rsub__(real other)	{  return other-(*$self);	}
    cmf::math::num_array __rmul__(real other)	{  return other*(*$self);	}
    cmf::math::num_array __rdiv__(real other)	{  return other/(*$self);	}
    %pythoncode {
    def __iter__(self):
        for i in xrange(len(self)):
            yield self[i]
    @property
    def __array_interface__(self):
        return dict(shape=(len(self),),typestr='|f8',data=(self.adress(),0),version=3)
    def ravel(self,order='C'):
        return self
    def __repr__(self):
        return 'cmf.num_array(%g, ..., %g, size=%i)' % (self[0],self[-1],self.size())
    }
}
%pythoncode {
    def to_num_array(arraylike):
        if hasattr(arraylike,'__array_interface__'):
            interface=arraylike.__array_interface__
            if len(interface['shape'])!=1:
                raise RuntimeError('Only 1d arrays are convertible to cmf.num_arrays')
            res=num_array(interface['shape'][0])
            res.__set_data_from_adress(interface['data'][0],interface['shape'][0])    
        else:
            res=num_array(len(arraylike))
            for i,v in enumerate(arraylike):
                res[i]=v
        return res
            
}

