#include "FluxConnection.h"
#include "../project.h"
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
