%{
	// Include Water
	#include "water/Solute.h"
	#include "water/WaterFlux.h"
	//#include "water/Reaction.h"
	#include "water/SoluteStorage.h"
	#include "water/WaterStorage.h"
	#include "water/FluxConnection.h"
    #include "water/boundary_condition.h"
%}
// Include Water
%include "water/Solute.h"
%include "water/WaterFlux.h"
//%include "water/Reaction.h"
%include "water/SoluteStorage.h"

// FluxConnection and Node
namespace cmf{namespace water {class FluxConnection;}}
%template(connection_vector) std::vector<cmf::water::FluxConnection*>;
%template(connection_set) std::set<cmf::water::FluxConnection*>;
%template(node_vector) std::vector<cmf::water::FluxNode*>;
%factory(cmf::water::FluxNode& cmf::water::FluxConnection::Target,cmf::atmosphere::RainCloud,cmf::river::OpenWaterStorage,cmf::upslope::SoilWaterStorage,cmf::water::WaterStorage,cmf::water::FluxNode);
%attribute(cmf::water::FluxNode,real,potential,get_potential,set_potential);

%pythonappend cmf::water::FluxConnection::FluxConnection{
    self.thisown=0
}
%include "water/FluxNode.h"
%include "water/FluxConnection.h"
%extend cmf::water::FluxConnection { %pythoncode {
    def __repr__(self):
        return self.ToString()
    def __getitem__(self,index):
        return self.Target(index)
    def __iter__(self):
        yield self.Target(0)
        yield self.Target(1)
    def __contains__(self,cmp):
        return cmp==self[0] or cmp==self[1]
}}
%extend cmf::water::FluxNode { 
%pythoncode {
    def __repr__(self):
        return self.Name
    def fluxes(self,t):
        return [(con.q(self,t),con.Target(self)) for con in self.Connections()]
  }
}

%include "water/boundary_condition.h"
// WaterStorage
%newobject cmf::water::WaterStorage::FromNode;
%include "water/WaterStorage.h"

%extend cmf::water::WaterStorage
{
	std::string __repr__()
	{
		return $self->ToString();
	}
}
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
// Add some function for down casting
DOWNCASTdefinition(cmf::water::FluxNode,cmf::water::WaterStorage,AsStorage)

