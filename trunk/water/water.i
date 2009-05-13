%{
	// Include Water
	#include "water/Solute.h"
	#include "water/WaterFlux.h"
	//#include "water/Reaction.h"
	#include "water/SoluteStorage.h"
	#include "water/WaterStorage.h"
	#include "water/FluxConnection.h"
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
%factory(cmf::water::FluxNode& cmf::water::FluxConnection::Target,cmf::atmosphere::RainfallNode,cmf::river::OpenWaterStorage,cmf::upslope::SoilWaterStorage,cmf::water::WaterStorage,cmf::water::FluxNode);
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
%extend cmf::water::FluxNode { %pythoncode {
    def __repr__(self):
        return self.Name
    def fluxes(self,t):
        for con in self.Connections():
            yield (con.q(self,t),con.Target(self))
    def connections(self):
        for con in self.Connections():
            yield con.Target(self)
}}


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
// Add some function for down casting
DOWNCASTdefinition(cmf::water::FluxNode,cmf::water::WaterStorage,AsStorage)

