

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


%shared_ptr(cmf::water::flux_node);
%shared_ptr(cmf::water::WaterStorage);
%shared_ptr(cmf::water::DirichletBoundary);
%shared_ptr(cmf::water::NeumannBoundary);
%shared_ptr(cmf::water::waterbalance_integrator);
%shared_ptr(cmf::water::flux_integrator);

%{
	#include "water/WaterStorage.h"
	#include "water/flux_connection.h"
    #include "water/boundary_condition.h"
    #include "water/simple_connections.h"
%}

// flux_connection and Node
%feature("ref") cmf::water::flux_connection ""
%feature("unref") cmf::water::flux_connection ""


namespace cmf{namespace water {class flux_connection;}}

%node_downcast_all(cmf::water::flux_node::ptr cmf::water::flux_connection::get_target)

%attribute2(cmf::water::flux_node,cmf::project,project,get_project);

%attribute(cmf::water::flux_node,real,potential,get_potential,set_potential);
%attributeval(cmf::water::flux_node, cmf::water::connection_list, connections, get_connections);

%pythonappend cmf::water::flux_connection::flux_connection{
    self.thisown=0
}

%typemap(out) cmf::water::connection_list* {
  // %typemap(out) cmf::water::connection_vector {
  $result = PyList_New($1->size());
  for (Py_ssize_t i = 0; i <Py_ssize_t($1->size()); ++i) {
    cmf::water::flux_connection* item = (*$1)[i].get();
    PyObject* py_item = SWIG_NewPointerObj(item,$descriptor(cmf::water::flux_connection*),0);
    PyList_SetItem($result,i,py_item);
  }
  delete $1;
}



%include "water/flux_node.h"
%iterable_to_list(cmf::water::connection_list,cmf::water::flux_connection::ptr);
%include "water/flux_connection.h"

%extend cmf::water::flux_connection { %pythoncode {

    def __repr__(self):
        return self.to_string()

    def __getitem__(self,index):
        return self.get_target(index)

    def __iter__(self):
        yield self.get_target(0)
        yield self.get_target(1)

    def __contains__(self,cmp):
        return cmp==self[0] or cmp==self[1]
}}
%{
    namespace cmf { namespace water {
        class BaseConnection: public flux_connection{
            public:
            BaseConnection(cmf::water::WaterStorage::ptr left, cmf::water::flux_node::ptr right, std::string type)
            : flux_connection(left, right, type) {}
            real calc_q(cmf::math::Time t) override    {
                throw std::runtime_error("BaseConnection.calc_q needs to be overriden by child class");
            }
    };

    }}
%}

%feature("director") cmf::water::BaseConnection;

namespace cmf { namespace water {
    class BaseConnection: public flux_connection{
    public:
        BaseConnection(cmf::water::WaterStorage::ptr left, cmf::water::flux_node::ptr right, std::string type);
        virtual real calc_q(cmf::math::Time t);
    };
}}

%extend cmf::water::connection_list {
    size_t __len__() const { return $self->size();}
    bool __contains__(const cmf::water::flux_connection::ptr& con) const { return $self->contains(con);}
%pythoncode {

    def __repr__(self):
        return repr(list(self)) + "<cmf.connection_list>"

    def __getitem__(self,index):
        return self.at(index)

    def __iter__(self):
        for i in range(len(self)):
            yield self.at(i)
    }
}


%extend cmf::water::flux_node { 
%pythoncode {
    def __repr__(self): return self.to_string()
    def fluxes(self,t):
        return [(con.q(self,t),con[self]) for con in self.connections]
    @property
    def connected_nodes(self):
        return [con[self] for con in self.connections]
  }
}


%include "water/boundary_condition.h"


// WaterStorage
// ignore the constructor
%ignore cmf::water::WaterStorage(cmf::project&,const std::string&,double);
%attribute(cmf::water::WaterStorage,real,volume,get_volume,set_volume);
%attribute(cmf::water::WaterStorage,char,statevariable,get_state_variable_content,set_state_variable_content);
%include "water/WaterStorage.h"

%extend__repr__(cmf::water::DirichletBoundary)
%extend__repr__(cmf::water::NeumannBoundary)
%extend cmf::water::NeumannBoundary {
%pythoncode {
    flux = property(get_flux,set_flux, doc="The flux over the boundary condition")
}}
%extend__repr__(cmf::water::WaterStorage)

%extend cmf::water::WaterStorage {
	cmf::water::SoluteStorage& __getitem__(cmf::water::solute X) {
		return (*$self)[X];
	}
}

%include "water/simple_connections.h"

%include "water/collections.i"

