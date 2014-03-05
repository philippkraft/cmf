

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
#include "flux_node.h"
#include "flux_connection.h"
#include "collections.h"
#include "../project.h"

static int nextnodeid=0;
// Registers a connection at this node (only called by ctor of flux_connection)


cmf::geometry::point get_direction_to(const cmf::water::flux_node & node, const cmf::water::flux_node& cmp )
{
	cmf::geometry::point p1=node.position,p2=cmp.position;
	double d=p1.distance3DTo(p2);
	if (d<=0) 
		return cmf::geometry::point();
	else
		return (p2-p1)/d;
}


void cmf::water::flux_node::RegisterConnection(flux_connection* newConnection )
{
	// Get the other end of the connection
	int key=(newConnection->get_target(*this))->node_id;
	// Register the connection
	if (newConnection->weak_this.expired())
	{
		m_Connections[key] = flux_connection::ptr(newConnection);
		newConnection->weak_this = m_Connections[key];
	}
	else
		m_Connections[key] = newConnection->weak_this.lock();

}
// Deregisters  a connection at this node (only called by dtor of flux_connection)

void cmf::water::flux_node::DeregisterConnection(flux_connection* oldConnection)
{
	// throw std::runtime_error("Do we really need DeregisterConnection");
	// loops through the existing connections for deregister it. 
	// The fast access m_Connections.erase(oldconnection->get_target(this)->node_id) does not work, since
	// the target of old connection might be expired already
	for(ConnectionMap::iterator it = m_Connections.begin(); it != m_Connections.end(); ++it)
	{
		if (it->second.get() == oldConnection)
		{
			m_Connections.erase(it->first);
			return;
		}
	}
}


cmf::water::flux_connection* cmf::water::flux_node::connection_to( const cmf::water::flux_node& target )
{
	if (m_Connections.find(target.node_id)!=m_Connections.end())
		return m_Connections[target.node_id].get();
	else
		return 0;
}


real cmf::water::flux_node::flux_to( const flux_node& target,cmf::math::Time t )
{
	if (m_Connections.find(target.node_id)!=m_Connections.end())
		return -m_Connections[target.node_id]->q(*this,t);
	else
		return 0;
}



real cmf::water::flux_node::waterbalance( cmf::math::Time t,const flux_connection* Without/*=0*/ ) const
{
	real waterbalance=0;
	for(flux_node::ConnectionMap::const_iterator it = m_Connections.begin(); it != m_Connections.end(); ++it)
	{
		flux_connection* conn=it->second.get();
		if (conn!=Without)
			waterbalance+=conn->q(*this,t);
	}
	return waterbalance;
}


// dtor of flux_node, deletes all connections
cmf::water::flux_node::~flux_node()
{
	while (m_Connections.size())
	{
		flux_connection::ptr con = m_Connections.begin()->second;
		m_Connections.erase(m_Connections.begin());
		con->kill_me();
	}
}



bool cmf::water::flux_node::remove_connection( flux_node::ptr To )
{
	if (To)
	{
		flux_connection* con = connection_to(*To);
		if (con) return con->kill_me();
	}
	return false;
}

real cmf::water::flux_node::conc( cmf::math::Time t, const cmf::water::solute& _Solute ) const
{
	real influx_sum_water=0,influx_sum_solute=0;
	for(flux_node::ConnectionMap::const_iterator it = m_Connections.begin(); it != m_Connections.end(); ++it)
	{
		real q=it->second->q(*this,t);
		if (q>0)
		{
			influx_sum_water  += q;
			influx_sum_solute += q * it->second->conc(t,_Solute);
		}
	}
	return influx_sum_water > 0 ? influx_sum_solute/influx_sum_water : 0.0;
}


cmf::water::flux_node::flux_node(cmf::project& _project,cmf::geometry::point location ) 
: m_project(_project), node_id(nextnodeid++),position(location)
{
	//m_project.add_node(shared_from_this());
}


cmf::geometry::point cmf::water::flux_node::flux3d_to( const cmf::water::flux_node& target,cmf::math::Time t )
{
	if (m_Connections.find(target.node_id)!=m_Connections.end())
		return -m_Connections[target.node_id]->q(*this,t) * get_direction_to(*this,target);
	else
		return cmf::geometry::point();
}


cmf::geometry::point cmf::water::flux_node::get_3d_flux( cmf::math::Time t )
{
	cmf::geometry::point res;
	for(flux_node::ConnectionMap::iterator it = m_Connections.begin(); it != m_Connections.end(); ++it)
	{
		flux_node::ptr target=it->second->get_target(*this);
		real f=flux_to(*target,t);
		cmf::geometry::point dir=get_direction_to(*this,*target);
		res+= dir * f;
	}
	return res;


}

cmf::water::connection_list cmf::water::flux_node::get_connections() const
{
	connection_list res;
	for(ConnectionMap::const_iterator it = m_Connections.begin(); it != m_Connections.end(); ++it)
	{		
		res.append(it->second);
	}
	return res;
}

double cmf::water::flux_node::water_balance_without_refresh() const
{
	real waterbalance=0;
	for(flux_node::ConnectionMap::const_iterator it = m_Connections.begin(); it != m_Connections.end(); ++it)
	{
		flux_connection* conn=it->second.get();
		waterbalance+=this == conn->left_node().get() ? -conn->m_q : conn->m_q;
	}
	return waterbalance;

}
int cmf::water::count_node_references( flux_node::ptr node )
{
	return int(node.use_count());
}

cmf::water::flux_node::ptr cmf::water::get_higher_node( cmf::water::flux_node::ptr node1,cmf::water::flux_node::ptr node2 )
{
	return node1->position.z >= node2->position.z ? node1 : node2;
}

cmf::water::flux_node::ptr cmf::water::get_lower_node( cmf::water::flux_node::ptr node1,cmf::water::flux_node::ptr node2 )
{
	return node1->position.z >= node2->position.z ? node2 : node1;
}

void cmf::water::waterbalance_integrator::integrate( cmf::math::Time until )
{
	if (_node.expired()) {
		throw std::runtime_error("Connection for "+_name+" does not exist any more");
	}
	cmf::math::Time dt = until-_t;
	if (until<_t) {
		reset(until);
		return;
	}
	_t=until;
	flux_node::ptr node = _node.lock();
	_sum += node->water_balance_without_refresh() * dt.AsDays();

}

double cmf::water::waterbalance_integrator::avg() const
{
	if (_t>_start_time)
		return _sum/(_t-_start_time).AsDays();
	else
		return 0.0;
}
