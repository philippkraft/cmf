%{
#include "water/collections.h"
%}
%attributeval(cmf::water::node_list,cmf::math::num_array,potentials,get_potentials,set_potentials);
%attributeval(cmf::water::NeumannBoundary_list,cmf::math::num_array,fluxes,get_fluxes,set_fluxes);

//%factory(cmf::water::flux_node& cmf::water::node_list::get,cmf::water::NeumannBoundary, cmf::water::DricheletBoundary,cmf::river::OpenWaterStorage,cmf::upslope::SoilLayer,cmf::water::WaterStorage,cmf::water::flux_node);


%include "water/collections.h"

%extend cmf::water::node_list {
%pythoncode {
    def __getitem__(self,index):
        return self.get(index)
    def __getslice__(self,slice):
        indices=slice.indices(self.size())
        return self.get(indices[0],indices[1],indices[2])
    def __len__(self):
        return self.size()       
    def __iter__(self):
        for i in xrange(self.size()):
            yield self[i]
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
