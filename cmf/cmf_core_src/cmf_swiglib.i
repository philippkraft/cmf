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

// This file contains some useful swig macro definitions
// They are used throughout the SWIG interface files

/*
Included macros:
%ptr(TYPE) 
    Shortcut for a shared_ptr of TYPE

%dynptrcast(TYPE,INPUT) 
    Shortcut for dynamic casts to %ptr(TYPE) from INPUT

%shared_attr(owner, name, getter, setter)
    %attribute surrogate for shared_ptr. Uses Python to create the property and hides the
    getter / setter function by adding underscores and NOT with %ignore.

%node_downcast(typemaptarget, Types...)

%define %node_downcast_all(Method)

%iterable_to_list(LISTTYPE,ITEMTYPE) 
    Creates a typemap from an iterable PyObject to LISTTYPE storing ITEMTYPE. 
    LISTTYPE needs to have a member function append(ITEMTYPE&) and a default c'tor.
    ITEMTYPE needs to be assignable and copy-constructable (e.g. a shared_ptr)

%extend__repr__(TYPE) Returns the result of a to_string method of a type as __repr__

*/



// **********************************************************************************************
// %ptr(TYPE) -> std::shared_ptr<TYPE> (or boost::shared_ptr<TYPE> or std::shared_ptr<TYPE>)
// %dynptrcast(TYPE,INPUT) -> std::dynamic_pointer_cast<TYPE>(INPUT)

#define %ptr(Type) std::shared_ptr<Type>
#define %dynptrcast(Type,input) std::dynamic_pointer_cast<Type>(input) 

%define %shared_attr(owner, owner_short, name, getter, setter...)
    %rename(__ ## getter) owner::getter;
    //%ignore owner::getter ;
    #if #setter==""
        %extend owner { %pythoncode {
        name = property(fget=_cmf_core. ## owner_short ##___## getter)
        }}
    #else
        %rename(__ ## setter) owner::setter;
        //%ignore owner::setter;
        %extend owner { %pythoncode {
        name = property(fget=_cmf_core. ## owner_short ##___## getter,
                        fset=_cmf_core. ## owner_short ##___## setter)
        }}
    #endif
%enddef


// **********************************************************************************************
// %node_downcast and node_downcast_all definitions + helper macros
%define %_node_down_cast(Type)
    if (dcast==0 && $1) /*check for Type*/  { 
        %ptr(Type) output = %dynptrcast(Type,$1);
        if (output) /*flux_node is Type */ {
            dcast=1; 
            %set_output(SWIG_NewPointerObj(new %ptr(Type)(output),$descriptor(%ptr(Type)*), SWIG_POINTER_OWN)); 
        }
    }
%enddef

%define %node_downcast(typemaptarget, Types...)
%typemap(out) typemaptarget {
   if (!($1)) %set_output(SWIG_NewPointerObj(0,$descriptor(%ptr(cmf::water::flux_node)*), SWIG_POINTER_OWN));
   int dcast = 0;
   %formacro(%_node_down_cast, Types)
   if (!dcast) %set_output(SWIG_NewPointerObj(new %ptr(cmf::water::flux_node)($1),$descriptor(%ptr(cmf::water::flux_node)*), SWIG_POINTER_OWN));
}
%enddef      

%define %node_downcast_all(Method)
//Downcast to all children of cmf::water::flux_node
%node_downcast(Method,
   cmf::atmosphere::RainSource,cmf::water::DirichletBoundary,cmf::water::NeumannBoundary,
   cmf::upslope::SoilLayer, cmf::upslope::aquifer,  cmf::river::Reach,cmf::river::OpenWaterStorage,
   cmf::water::WaterStorage, cmf::upslope::MacroPore, cmf::upslope::SurfaceWater
)
%enddef

%define %_state_downcast(typemaptarget, Types...)
%typemap(out) typemaptarget {
   if (!($1)) %set_output(SWIG_NewPointerObj(0,$descriptor(%ptr(cmf::math::StateVariable)*), SWIG_POINTER_OWN));
   int dcast = 0;
   %formacro(%_node_down_cast, Types)
   if (!dcast) %set_output(SWIG_NewPointerObj(new %ptr(cmf::math::StateVariable)($1),
						   $descriptor(%ptr(cmf::math::StateVariable)*), SWIG_POINTER_OWN));
}
%enddef      

%define %state_downcast(Method)
//Downcast to all children of cmf::math::StateVariable
%_state_downcast(Method,
   cmf::water::WaterStorage, cmf::water::SoluteStorage
)
%enddef


// **********************************************************************************************
// %extend__repr__(TYPE) Returns the result of a to_string method of a type as __repr__

%define %extend__repr__(TYPE) 
%extend TYPE { %pythoncode {
    def __repr__(self):
        return self.to_string()
}}
%enddef
%{
// **********************************************************************************************
// %iterable_to_list(LISTTYPE,ITEMTYPE) typemap system. Puts a function template into the header for general usage
// Function to convert an iterable to a list type (class with append function). For use in typemaps
    template<typename _item_type, typename _Ty> 
    int iterable_to_list(PyObject* iterable,swig_type_info* _item_descriptor, _Ty& temp_list, int * cannot_convert=0) {
    	PyObject* iter = PyObject_GetIter(iterable);
        if (iter == 0) {
            // no iterator
	        return SWIG_ERROR;
	    }
	    while (PyObject* py_item = PyIter_Next(iter)) {
		    _item_type* item=0;
		    // if the item is a node, append to node_list
		    int is_ok = SWIG_ConvertPtr(py_item, (void**)&item, _item_descriptor, SWIG_POINTER_EXCEPTION);
		    if (is_ok == 0 && item != 0 ) { 
			    temp_list.append(*item);
		    } else if (cannot_convert) {
             ++(*cannot_convert);
          }
		    Py_DECREF(py_item);
	    }
	    Py_DECREF(iter);
	    return SWIG_OK;
    }
    
    int is_listtype_or_iterable(PyObject* input, swig_type_info* listtype_descriptor) {
        // Checks if a type has the given descriptor or is iterable
	    void * pt;
	    int res=SWIG_ConvertPtr(input,&pt,listtype_descriptor,0);
	    if (SWIG_IsOK(res)) {
		    return 1;
	    } else  {
		    PyObject* iter = PyObject_GetIter(input);
		    if (iter != 0) {
			    Py_DECREF(iter);
			    return 2;
		    } 
		    else return 0;
	    }         
    }
%}

%define %iterable_to_list(LISTTYPE,ITEMTYPE)
%typemap(in) LISTTYPE& (LISTTYPE temp_list) {
    if (SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, SWIG_POINTER_EXCEPTION) == -1) {
        int conversion_errors = 0;
        int res = iterable_to_list<ITEMTYPE, LISTTYPE>($input,$descriptor(ITEMTYPE*), temp_list, &conversion_errors);
        if (SWIG_IsOK(res)) {
    	    $1 = &temp_list;
    	} else {
    	    SWIG_exception_fail(SWIG_TypeError,"Only iterables can be converted to LISTTYPE");
    	}
    }
}
%typemap(in) const LISTTYPE& (LISTTYPE temp_list) {
    if (SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, SWIG_POINTER_EXCEPTION) == -1) {
        int conversion_errors = 0;
        int res = iterable_to_list<ITEMTYPE, LISTTYPE>($input,$descriptor(ITEMTYPE*), temp_list, &conversion_errors);
        if (SWIG_IsOK(res)) {
    	    $1 = &temp_list;
    	} else {
    	    SWIG_exception_fail(SWIG_TypeError,"Only iterables can be converted to LISTTYPE");
    	}
    }
}
%typemap(in) LISTTYPE* (LISTTYPE temp_list) {
    if (SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, SWIG_POINTER_EXCEPTION) == -1) {
        int conversion_errors = 0;
        int res = iterable_to_list<ITEMTYPE, LISTTYPE>($input,$descriptor(ITEMTYPE*), temp_list, &conversion_errors);
        if (SWIG_IsOK(res)) {
            $1 = &temp_list;
        } else {
            SWIG_exception_fail(SWIG_TypeError,"Only iterables can be converted to LISTTYPE");
        }
    }
}

%typemap(typecheck,precedence=0) LISTTYPE& {
	$1 = is_listtype_or_iterable($input,$1_descriptor);
}
%enddef


%{
    template<typename _listtype>
    int list_getitem_from_index(const _listtype& source, PyObject* item, _listtype& target) {
        PyObject* iter = PyObject_GetIter(item);
        if (iter) {
            while (PyObject* py_item = PyIter_Next(iter)) {
                Py_ssize_t index = PyLong_AsSsize_t(py_item);
                Py_DECREF(py_item);
                if (PyErr_Occurred()) return 0;
                target.append(source[index]);
            }
            Py_DECREF(iter);
            return 1;
        } else {
            PyErr_Clear();
        }
        if (PySlice_Check(item)) {
            Py_ssize_t start, stop, step;
            PySlice_GetIndices(item, source.size(), &start, &stop, &step);
            for (Py_ssize_t i=start; i<stop; i+=step) {
                target.append(source[i]);
            }
            return 1;
        }
        else if (PyLong_Check(item)) {
            Py_ssize_t i = PyLong_AsSsize_t(item);
            target.append(source[i]);
            return 1;
        }
        else {
            SWIG_Error(SWIG_ValueError, "Index for item assessment must be either int, sequence or slice");
            return 0;
        }
        
        
    }
%}

%define %extend_pysequence(LISTTYPE)
%extend LISTTYPE {
    size_t __len__() const {
        return $self->size();
    }
    %pythoncode {
    def __iter__(self):
        for i in range(len(self)):
            yield self[i]

    def __getitem__(self, index):

        if isinstance(index,slice):
            res = type(self)()
            for i in range(*index.indices(len(self))):
                res.append(self.__getitem(i))
            return res
        else:
            try:
                it=iter(index)
                res = type(self)()
                for o in it:
                    res.append(self.__getitem(o))
                return res
            except:
                return self.__getitem(index)

}}
%enddef

%define %extend_getitem(LISTTYPE,ITEMTYPE)
%extend LISTTYPE {
PyObject* __getitem__(PyObject* item){
    auto* result = new LISTTYPE<ITEMTYPE>();
    int res=list_getitem_from_index(*$self, item, *result);
    if (res == 0 || result->size() == 0) {
        delete result;
        Py_RETURN_NONE;
    }
    if (result->size() == 1) {
        ITEMTYPE obj = (*result)[0];
        delete result;
        return SWIG_NewPointerObj(
            new ITEMTYPE(obj),
            $descriptor(ITEMTYPE*),
            SWIG_POINTER_OWN);
    }
    else  {
        return SWIG_NewPointerObj(
            result, 
            $descriptor(LISTTYPE<ITEMTYPE>*),
            SWIG_POINTER_OWN);
    }
}
void __setitem__(long long index, T& item) {
    (*$self)[index] = item;
}
void __delitem__(long long index) {
    $self->remove_at(index);
}
long long __len__(){
    return $self->size();
}

bool __contains__(const T& what) {
    try
    {
        size_t index = $self->index(what);
        return true;
    }
    catch (const std::out_of_range&)
    {
        return false;
    }
}
%pythoncode {
    def __repr__(self):
        return type(self).__name__ + '([' + ', '.join(repr(obj) for obj in self) + '])'
}
}
%enddef
