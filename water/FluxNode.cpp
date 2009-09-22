#include "FluxNode.h"
#include "FluxConnection.h"
#ifdef _OPENMP
#include <omp.h>
#endif

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

cmf::geometry::point cmf::water::FluxNode::flux3d_to( const cmf::water::FluxNode& target,cmf::math::Time t )
{
	if (m_Connections.find(target.node_id)!=m_Connections.end())
		return -m_Connections[target.node_id]->q(*this,t) * get_direction_to(target);
	else
		return cmf::geometry::point();
}

cmf::geometry::point cmf::water::FluxNode::get_3d_flux( cmf::math::Time t )
{
	cmf::geometry::point res;
	for(FluxNode::ConnectionMap::iterator it = m_Connections.begin(); it != m_Connections.end(); ++it)
	{
		FluxNode& target=it->second->Target(*this);
		real f=flux_to(target,t);
		cmf::geometry::point dir=get_direction_to(target);
		res+= dir * f;
	}
	return res;


}
real cmf::water::node_list::global_water_balance( cmf::math::Time t )	const
{
	real sum=0;
#pragma omp parallel for reduction(+ : sum)
	for (int i = 0; i < (int)size() ; ++i)
	{
		sum+=m_nodes[i]->water_balance(t);					
	}
	return sum;
}

cmf::math::numVector cmf::water::node_list::water_balance( cmf::math::Time t ) const
{
	cmf::math::numVector res(size());
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; ++i)
	{
		res[i]=m_nodes[i]->water_balance(t);
	}
	return res;
}

cmf::math::numVector cmf::water::node_list::get_fluxes_to( const cmf::water::node_list& targets ,cmf::math::Time t ) const 
{
	if (size() != targets.size())
		throw std::invalid_argument("The list for the target nodes need to have the same length as this node_list");
		cmf::math::numVector res(size());
#pragma omp parallel for
	for (int i = 0; i < (int)res.size() ; ++i)
	{
		res[i]=m_nodes[i]->flux_to(*targets.m_nodes[i],t);
	}
	return res;

}

cmf::geometry::point_vector cmf::water::node_list::get_fluxes3d_to( const cmf::water::node_list& targets ,cmf::math::Time t ) const
{
	if (size() != targets.size())
		throw std::invalid_argument("The list for the target nodes need to have the same length as this node_list");
	cmf::geometry::point_vector res(size());
#pragma omp parallel for
	for (int i = 0; i < (int)res.size() ; ++i)
	{
		res.set(i,m_nodes[i]->flux3d_to(*targets.m_nodes[i],t));
	}
	return res;


}

cmf::geometry::point_vector cmf::water::node_list::get_fluxes3d( cmf::math::Time t ) const
{
	cmf::geometry::point_vector res(size());
#pragma omp parallel for
	for (int i = 0; i < (int)res.size() ; ++i)
	{
		res.set(i,m_nodes[i]->get_3d_flux(t));
	}
	return res;


}

cmf::geometry::point_vector cmf::water::node_list::get_positions() const
{
	cmf::geometry::point_vector res(size());
#pragma omp parallel for
	for (int i = 0; i < (int)res.size() ; ++i)
	{
		res.set(i,m_nodes[i]->get_position());
	}
	return res;

}

int cmf::water::node_list::set_potentials( const cmf::math::numVector& potentials )
{
	if (size()!=potentials.size())
		throw std::out_of_range("Size of potential array does not fit the size of the node_list");
	int ok_count=size();
	for (int i = 0; i < size() ; ++i)
	{
		try
		{
			this->get(i)->set_potential(potentials[i]);
		}
		catch (std::runtime_error)
		{
			--ok_count;
		}
	}
	return ok_count;
}