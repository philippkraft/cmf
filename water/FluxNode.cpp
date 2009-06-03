#include "FluxNode.h"
#include "FluxConnection.h"

#include "../project.h"
int cmf::water::FluxNode::nextnodeid=0;
// Registers a connection at this node (only called by ctor of FluxConnection)
void cmf::water::FluxNode::RegisterConnection( FluxConnection* newConnection )
{
	// Get the other end of the connection
	int key=(newConnection->Target(*this)).node_id;
	// Find a connection that already exist to that other end
	ConnectionMap::iterator OldConnection=m_Connections.find(key);
	// If such a connection exists, delete it (dtor of FluxConnection does the Deregistering)
	if (OldConnection!=m_Connections.end())
	{
		delete OldConnection->second;
	}
	// Register the connection
	m_Connections[key]=newConnection;
}
// Deregisters  a connection at this node (only called by dtor of FluxConnection)
void cmf::water::FluxNode::DeregisterConnection( FluxConnection* oldConnection )
{
	// Get the other end of the connection	
	int key=oldConnection->Target(*this).node_id;
	// Find the connection in the registration map
	ConnectionMap::iterator OldConnection=m_Connections.find(key);
	// If it exists, throw it out of the connection
	if (OldConnection!=m_Connections.end())
		m_Connections.erase(OldConnection);
	// No deleting of the connection needed, since the function is only called by the dtor
}

cmf::water::FluxConnection* cmf::water::FluxNode::Connection( const cmf::water::FluxNode& target )
{
	if (m_Connections.find(target.node_id)!=m_Connections.end())
		return m_Connections[target.node_id];
	else
		return 0;
}

real cmf::water::FluxNode::flux_to( const FluxNode& target,cmf::math::Time t )
{
	if (m_Connections.find(target.node_id)!=m_Connections.end())
		return -m_Connections[target.node_id]->q(*this,t);
	else
		return 0;
}


real cmf::water::FluxNode::water_balance( cmf::math::Time t,const FluxConnection* except/*=0*/ )
{
	real waterbalance=0;
	for(FluxNode::ConnectionMap::iterator it = m_Connections.begin(); it != m_Connections.end(); ++it)
	{
		FluxConnection* conn=it->second;
		if (conn!=except)
			waterbalance+=conn->q(*this,t);
	}
	return waterbalance;
}

// dtor of FluxNode, deletes all connections
cmf::water::FluxNode::~FluxNode()
{
	while (m_Connections.size()>0)
		delete m_Connections.begin()->second;
	if (project().debug)
		std::cout << "Deleted " << Name << std::endl;
}


void cmf::water::FluxNode::remove_connection( const FluxNode & To )
{
	delete Connection(To);
}

cmf::water::WaterQuality cmf::water::FluxNode::conc( cmf::math::Time t ) const
{
	WaterFlux influx_sum;
	for(FluxNode::ConnectionMap::const_iterator it = m_Connections.begin(); it != m_Connections.end(); ++it)
	{
		real q=it->second->q(*this,t);
		if (q>0)
		{
			influx_sum.Add(q,it->second->Target(*this).conc(t));						
		}
	}
	return influx_sum.c;
}

cmf::water::FluxNode::FluxNode( const cmf::project& _project,cmf::geometry::point location ) : m_project(_project), node_id(nextnodeid++),Location(location)
{
}
