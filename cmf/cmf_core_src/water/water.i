

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


%shared_ptr(cmf::water::SoluteStorage);

%shared_ptr(cmf::water::flux_node);
%shared_ptr(cmf::water::WaterStorage);
%shared_ptr(cmf::water::DirichletBoundary);
%shared_ptr(cmf::water::NeumannBoundary);
%shared_ptr(cmf::water::SystemBridge);

%shared_ptr(cmf::water::waterbalance_integrator);
%shared_ptr(cmf::water::flux_integrator);

%{
	// Include Water
	#include "water/Solute.h"
	#include "water/SoluteStorage.h"
	#include "water/WaterStorage.h"
	#include "water/flux_connection.h"
    #include "water/boundary_condition.h"
    #include "water/simple_connections.h"
    #include "water/system_bridge.h"
%}
// Include Water
%include "water/adsorption.h"
%include "water/Solute.h"
%extend cmf::water::solute { 
    std::string __repr__() { return "[" + $self->Name + "]"; }
}
%extend cmf::water::solute_vector
{
	cmf::water::solute* __getitem__(int i)
	{
		return $self->get_solute(i);
	}
	size_t __len__() { return $self->size();}
	%pythoncode
	{
    def __iter__(self):
        for i in range(len(self)):
            yield self[i]
    def __repr__(self):
        return str([s.Name for s in self])
	}
}
%extend cmf::water::SoluteTimeseries
{
	cmf::math::timeseries& __getitem__(const cmf::water::solute& solute)
	{
		return (*$self)[solute];
	}
	void __setitem__(const cmf::water::solute& solute,cmf::math::timeseries concentration)
	{
		(*$self)[solute]=concentration;
	}
	size_t __len__() const
	{
		return $self->size();
	}
}


%include "water/SoluteStorage.h"

// flux_connection and Node
%feature("ref") cmf::water::flux_connection ""
%feature("unref") cmf::water::flux_connection ""


namespace cmf{namespace water {class flux_connection;}}

%node_downcast_all(cmf::water::flux_node::ptr cmf::water::flux_connection::get_target)

%attribute2(cmf::water::flux_node,cmf::project,project,get_project);

%attribute(cmf::water::flux_node,real,potential,get_potential,set_potential);
%attributeval(cmf::water::flux_node, cmf::water::connection_list, connections, get_connections);
%shared_attr(cmf::water::waterbalance_integrator, waterbalance_integrator, node, get_node, set_node);
// %attribute(cmf::water::flux_connection, real, tracer_filter, get_tracer_filter, set_tracer_filter);

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
%extend cmf::water::connection_list {
    size_t __len__() const { return $self->size();}
    bool __contains__(const cmf::water::flux_connection::ptr& con) const { return $self->contains(con);}
%pythoncode {
    def __repr__(self):
        return repr(list(self)) + "<cmf.connection_list>"
    def __getitem__(self,index):
        return self.at(index)
    def __iter__(self):
        for i in xrange(len(self)):
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
    flux = property(get_flux,set_flux,"The flux over the boundary condition")
}}
%extend__repr__(cmf::water::WaterStorage)

%include "water/simple_connections.h"

%include "water/collections.i"

// system bridge

%include "water/system_bridge.h"


%pythoncode {
    def integrate_over(item,solver=None):
        """Returns a suitable cmf.integratable implementation for item, if available.
        The created integratable is integrated by solver, if given"""
        try:
            it = iter(item)
        except:
            it=None
        if it:
            res = integratable_list()
            for i in it:
                integ = integrate_over(i,solver)
                res.append(integ)
            return res
        elif isinstance(item,flux_node):
            res = waterbalance_integrator(item)
        elif isinstance(item,flux_connection):
            res = flux_integrator(item)
        else:
            raise TypeError("""Only the waterbalance of flux_nodes and the flux of flux_connections
                are integratable. Received: """ + str(item))
        if isinstance(solver,Integrator):
            solver.integratables.append(res)
        return res
}

