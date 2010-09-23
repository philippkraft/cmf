#include "system_bridge.h"
using namespace cmf::water;
SystemBridge::SystemBridge( cmf::project& p , flux_connection& con) 
: flux_node(p), m_ci(new flux_integrator(con))
{
}


SystemBridge::ptr cmf::water::system_bridge( cmf::project& p, flux_node::ptr upper,flux_node::ptr lower )
{
	flux_connection* con = upper->connection_to(*lower);
	if (con == 0)
		throw std::runtime_error("Cannot create SystemBridge if no connection exists between " + upper->to_string() + " and " + lower->to_string());
	SystemBridge::ptr self(new SystemBridge(p,*con));
	self->position = lower->position;
	self->Name = "System bridge between " + upper->Name + " and " +lower->Name;
	con->exchange_target(lower,self);
	new SystemBridgeConnection(self,lower);
	return self;
}

