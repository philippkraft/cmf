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

