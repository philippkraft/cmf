

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
#include "water/collections.h"
%}
%attribute(cmf::water::node_list,cmf::math::num_array,potentials,get_potentials,set_potentials);
%attribute(cmf::water::NeumannBoundary_list,cmf::math::num_array,fluxes,get_fluxes,set_fluxes);
%node_downcast_all(cmf::water::flux_node::ptr cmf::water::node_list::get)
%rename(__get) cmf::water::node_list::get;
%rename(__getslice) cmf::water::node_list::getslice;

%iterable_to_list(cmf::water::node_list,cmf::water::flux_node::ptr)
%iterable_to_list(cmf::water::NeumannBoundary_list,cmf::water::NeumannBoundary::ptr)

%include "water/collections.h"

%extend cmf::water::node_list {
%pythoncode {
    def __getitem__(self,index):
        if isinstance(index,slice):
            return self.__getslice(*index.indices(self.size())) 
        else:
            try:
                it = iter(index)
                return node_list(self.__get(i) for i in it)
            except:
                return self.__get(index)
                
    def __len__(self):
        return self.size()       
    def __iter__(self):
        for i in xrange(self.size()):
            yield self[i]
    def __repr__(self):
        if len(self): return "[%i nodes: %s ... %s]" % (len(self),self[0], self[-1])
        else: return "[empty node list]"
    def extend(self,sequence):
        """Extends the node list with the sequence (any iterable will do) """
        for o in sequence:
            self.append(o)
    @staticmethod
    def from_sequence(sequence):
        """Returns a new node list populated from the sequence (any iterable will do) """
        nl=node_list()
        nl.extend(sequence)
        return nl
    }
}
%extend cmf::water::NeumannBoundary_list {
    %pythoncode {
    def __getitem__(self,index):
        return self.get(index)
    def __len__(self):
        return self.size()       
    def __iter__(self):
        for i in xrange(self.size()):
            yield self[i]
    def extend(self,sequence):
        """Extends the list of Neumann boundaries with the sequence (any iterable will do) """
        for o in sequence:
            self.append(o)
    @staticmethod
    def from_sequence(sequence):
        """Returns a new list of Neumann boundaries populated from the sequence (any iterable will do) """
        nl=NeumannBoundary_list()
        nl.extend(sequence)
        return nl
    }
}
