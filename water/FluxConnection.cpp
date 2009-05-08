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

real cmf::water::FluxNode::FluxTo( const FluxNode& target,cmf::math::Time t )
{
	if (m_Connections.find(target.node_id)!=m_Connections.end())
		return -m_Connections[target.node_id]->q(*this,t);
	else
		return 0;
}


real cmf::water::FluxNode::Waterbalance( cmf::math::Time t,const FluxConnection* except/*=0*/ )
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


void cmf::water::FluxNode::RemoveConnection( const FluxNode & To )
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
/************************************************************************/
/* FluxConnection                                                       */
/************************************************************************/
int cmf::water::FluxConnection::nextconnectionid=0;
cmf::water::FluxConnection::~FluxConnection()
{
	if (m_left->project().debug)
		std::cout << "Delete " << ToString() << std::endl;
	m_left->DeregisterConnection(this);
	m_right->DeregisterConnection(this);
}

cmf::water::FluxConnection::FluxConnection( FluxNode& left,FluxNode& right,std::string _type ) : m_left(&left),m_right(&right),RecalcAlways(false),type(_type),connection_id(nextconnectionid++)
{
	if (m_left->project().debug)
		std::cout << "Create " << ToString() << std::endl;
	m_left->RegisterConnection(this);
	m_right->RegisterConnection(this);
}

void cmf::water::FluxConnection::ExchangeTarget( FluxNode& oldtarget,FluxNode& newTarget )
{
	m_left->DeregisterConnection(this);
	m_right->DeregisterConnection(this);
	if (&oldtarget==m_left)
	{
		m_left=&newTarget;
	}
	else if (&oldtarget==m_right)
	{
		m_right=&newTarget;
	}
	m_left->RegisterConnection(this);
	m_right->RegisterConnection(this);
	NewNodes();
	if (newTarget.project().debug)
	{
		std::cout << "Replaced " << oldtarget.Name << " with " << newTarget.Name << " in " << this->ToString() << std::endl;
	}

}
void cmf::water::replace_node(cmf::water::FluxNode& oldnode,cmf::water::FluxNode& newnode)
{
	newnode.Location=oldnode.Location;
	newnode.Name=oldnode.Name;
	std::vector<FluxConnection*> cons=oldnode.Connections();
	for(std::vector<FluxConnection*>::iterator it = cons.begin(); it != cons.end(); ++it)
	{
		(**it).ExchangeTarget(oldnode,newnode);
	}
}
