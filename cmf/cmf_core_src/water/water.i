

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
	// Include Water
	#include "water/Solute.h"
	#include "water/SoluteStorage.h"
	#include "water/WaterStorage.h"
	#include "water/flux_connection.h"
    #include "water/boundary_condition.h"
%}
// Include Water
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
namespace cmf{namespace water {class flux_connection;}}
%template(connection_vector) std::vector<cmf::water::flux_connection* >;
%template(connection_set) std::set<cmf::water::flux_connection* >;
%node_downcast_all(cmf::water::flux_node::ptr cmf::water::flux_connection::get_target)
%node_downcast_all(cmf::water::flux_node::ptr cmf::water::node_list::get)


%attribute(cmf::water::flux_node,real,potential,get_potential,set_potential);
%attributeval(cmf::water::flux_node, cmf::water::connection_vector, connections, get_connections);
%attribute(cmf::water::flux_connection, real, tracer_filter, get_tracer_filter, set_tracer_filter);

%pythonappend cmf::water::flux_connection::flux_connection{
    self.thisown=0
}
%include "water/flux_node.h"
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
%extent cmf::water::DricheletBoundary {  %pythoncode {
    def __repr__(self): return self.to_string()
}}
%extent cmf::water::NeumannBoundary {   %pythoncode {
    def __repr__(self): return self.to_string()
}}


// WaterStorage
%attribute(cmf::water::WaterStorage,real,volume,get_volume,set_volume);

%include "water/WaterStorage.h"
%template(storage_vector) std::vector<cmf::water::WaterStorage::ptr>;
%include "water/collections.i"

// Add some function for down casting
// DOWNCASTdefinition(cmf::water::flux_node,cmf::water::WaterStorage,AsStorage)

