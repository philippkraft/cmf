

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
#include <algorithm>
#include "flux_connection.h"
#include "../project.h"
  /*************************************************************************/
 /* flux_connection                                                       */
/*************************************************************************/

using namespace cmf::water;
using namespace cmf::math;

int flux_connection::nextconnectionid=0;

real flux_connection::q(cmf::math::Time t) {
#ifndef NOQCACHE
	if (RecalcAlways || m_left.lock()->RecalcFluxes(t) || m_right.lock()->RecalcFluxes(t))
#endif
	{
		m_q = calc_q(t);
		if (!std::isfinite(m_q)) {
			throw std::runtime_error("Flux of " + this->to_string() + " at " + t.AsDate().to_string() + " is not finite");
		}
	}
	return m_q;
}



flux_connection::~flux_connection()
{
	
}

flux_connection::flux_connection( flux_node::ptr left,flux_node::ptr right,std::string _type ) 
: m_left(left),m_right(right),RecalcAlways(false),type(_type),connection_id(nextconnectionid++),m_tracer_filter(1.0)
{
	if ((!left) || (!right)) 
		throw std::runtime_error("Can't create " + this->type + " if a node is null");
	const project& p=left->get_project();
	left->RegisterConnection(this);
	right->RegisterConnection(this);
}

void flux_connection::exchange_target( flux_node::ptr oldtarget,flux_node::ptr newTarget )
{
	flux_connection::ptr This(weak_this);
	left_node()->DeregisterConnection(this);
	right_node()->DeregisterConnection(this);
	if (oldtarget==left_node())
	{
		m_left = std::weak_ptr<flux_node>(newTarget);
	}
	else if (oldtarget==right_node())
	{
		m_right = std::weak_ptr<flux_node>(newTarget);
	}
	left_node()->RegisterConnection(this);
	right_node()->RegisterConnection(this);
	NewNodes();

}

std::string flux_connection::to_string() const
{
	return type + "(" + left_node()->to_string() + "<->" + right_node()->to_string() + ")";
}


flux_node::ptr flux_connection::get_target( int index ) const
{
	if (index==0 || index==-2)
		return left_node();
	else if (abs(index)==1)
		return right_node();
	else
		throw std::out_of_range("Only indices 0 and 1 are valid for querying a connection target");
}

flux_node::ptr flux_connection::get_target(const flux_node& inquirer )
{
	if 		   (left_node() && inquirer == *left_node()) 
			return right_node();
	else if (right_node() && inquirer == *right_node()) 
			return left_node();
	else
			return flux_node::ptr();
}

real flux_connection::conc( cmf::math::Time t, const solute& _Solute )
{
	real _q=q(t);
	if (_q>0) return left_node()->conc(t, _Solute) * this->get_tracer_filter(_Solute);
	else if (_q<0) return right_node()->conc(t, _Solute) * this->get_tracer_filter(_Solute);
	else return 0.0;
}

std::string flux_connection::short_string() const
{
	std::stringstream str;
	str << type << "#" << connection_id;
	return str.str();
}

// delete the connection
bool flux_connection::kill_me()
{
	flux_connection::ptr shared_this(weak_this);
	flux_node::ptr 
		left=left_node(),
		right=right_node();
	if (left) 
		left->DeregisterConnection(this);
	if (right)
		right->DeregisterConnection(this);
	return weak_this.use_count() <= 1;
}

real flux_connection::get_tracer_filter(solute S) {
	// find entry in the specific tracer filter map
	std::map< solute, real>::const_iterator  it = m_tracer_filter_map.find(S);
	// Use that entry for the specific filter, else 1.0 as a default
	real specific_filter = (it == m_tracer_filter_map.end()) ? 1.0 : it->second;
	return specific_filter * m_tracer_filter;
}


// returns the average flux over the timestep
double cmf::water::flux_integrator::avg() const
{
	// if variable is already integrated
	if (_t>_start_time) 
		return _sum/(_t-_start_time).AsDays();
	else // else return zero...
		return 0.0;
}

cmf::water::flux_integrator::flux_integrator( cmf::water::flux_connection& connection ) 
	:	_connection(connection.weak_this), 
		_sum(0.0), _t(cmf::math::year*5000), 
		_name(connection.to_string()+ " (Integrator)"), invert(false)
{}

flux_integrator::flux_integrator( flux_node::ptr left, flux_node::ptr right )
: 	_sum(0.0), _t(cmf::math::year*5000)
{
	flux_connection* con = left->connection_to(*right);
	if (con) {
		_connection = con->weak_this;
		_name = con->to_string() + " (Integrator)";
		invert = con->left_node() == right;
	} else 
		throw std::runtime_error("Can't create flux_integrator between " + left->Name + " and " + right->Name + ". No connection.");
}

void cmf::water::flux_integrator::reset( cmf::math::Time t )
{
	_start_time = t;
	_t=_start_time;
	_sum=0.0;
}

flux_connection::ptr cmf::water::flux_integrator::connection() const
{
	return _connection.lock();
}



int cmf::water::replace_node(flux_node::ptr oldnode,flux_node::ptr newnode)
{
	int changes=0;
	if (oldnode && newnode) {
		newnode->position=oldnode->position;
		newnode->Name=oldnode->Name;
		connection_list cons=oldnode->get_connections();
		for(connection_list::iterator it = cons.begin(); it != cons.end(); ++it) {
			++changes;
			(**it).exchange_target(oldnode,newnode);
		}
	}
	return changes;

}


#ifdef _OPENMP
#include <omp.h>
#endif


inline bool connection_less(const flux_connection::ptr& con1,const flux_connection::ptr& con2) {
	const int 
		con1_left  = con1->left_node()->node_id, 
		con2_left  = con2->left_node()->node_id, 
		con1_right = con1->right_node()->node_id, 
		con2_right = con1->right_node()->node_id;
	return con1_left < con2_left  || ((con1_left == con2_left) && (con1_right<con2_right));
}
void cmf::water::connection_list::do_action( Time t , bool use_OpenMP)
{
	if (use_OpenMP) 
	{
		#pragma omp parallel for
		for (int i = 0; i < (int)m_con.size() ; ++i)
		{
			m_con[i]->refresh(t);			
		}
	} /* if OpenMP */
	else 
	{
		for(_list::const_iterator it = m_con.begin(); it != m_con.end(); ++it)
		{
		    (**it).refresh(t);
		}

	} /* if no OpenMP */

}
bool cmf::water::connection_list::contains(flux_connection::ptr connection ) const
{
	return std::binary_search(begin(),end(), connection, connection_less);
}
bool cmf::water::connection_list::append( flux_connection::ptr connection )
{
	_list::iterator it = std::lower_bound(begin(),end(),connection, connection_less);
	if ((it == end()) || (*it != connection)) {
		m_con.insert(it,connection);
		return true;
	} else {
		return false;
	}
}
void cmf::water::connection_list::extend(const connection_list& connections) {
	for(connection_list::const_iterator it = connections.begin(); it != connections.end(); ++it)
	    append(*it);
}
bool cmf::water::connection_list::remove(flux_connection::ptr connection )
{
	iterator it = std::lower_bound(begin(),end(),connection, connection_less);
	if (it!=end()) {
		m_con.erase(it);
		return true;
	} else return false;
}


void cmf::water::flux_integrator::integrate( cmf::math::Time until )
{
	/* If connection is expired, throw error */
	if (_connection.expired()) {
		throw std::runtime_error("Connection for "+_name+" does not exist any more");
	} else if /* end time is before current time, reset the integration*/ (until<_t) {	
		reset(until);
	} else /* integrate */ {
		// get timestep length
		cmf::math::Time dt = until-_t;
		// get the connection
		flux_connection::ptr con = _connection.lock();
		// add flux in timestep to current sum
		if (invert)
			_sum -= con->m_q * dt.AsDays();
		else
			_sum += con->m_q * dt.AsDays();
		// set new current time
		_t=until;
	}
}

