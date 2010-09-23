

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
#include "flux_connection.h"
#include "../project.h"
  /*************************************************************************/
 /* flux_connection                                                       */
/*************************************************************************/

using namespace cmf::water;
using namespace cmf::math;

int flux_connection::nextconnectionid=0;
flux_connection::~flux_connection()
{
	
}

flux_connection::flux_connection( flux_node::ptr left,flux_node::ptr right,std::string _type ) 
: m_left(left),m_right(right),RecalcAlways(false),type(_type),connection_id(nextconnectionid++),m_tracer_filter(1.0)
{
	if ((!left) || (!right)) 
		throw std::runtime_error("Can't create " + this->type + " if a node is null");
	const project& p=left->get_project();
	if (p.debug)
		std::cout << "Create " << to_string() << std::endl;
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
		m_left = std::tr1::weak_ptr<flux_node>(newTarget);
	}
	else if (oldtarget==right_node())
	{
		m_right = std::tr1::weak_ptr<flux_node>(newTarget);
	}
	left_node()->RegisterConnection(this);
	right_node()->RegisterConnection(this);
	NewNodes();
	if (newTarget->get_project().debug)
	{
		std::cout << "Replaced " << oldtarget->Name << " with " << newTarget->Name << " in " << this->to_string() << std::endl;
	}

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
	if (_q>0) return left_node()->conc(t,_Solute);
	else if (_q<0) return right_node()->conc(t,_Solute);
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
int cmf::water::replace_node(flux_node::ptr oldnode,flux_node::ptr newnode)
{
	int changes=0;
	if (oldnode && newnode) {
		newnode->position=oldnode->position;
		newnode->Name=oldnode->Name;
		connection_vector cons=oldnode->get_connections();
		for(connection_vector::iterator it = cons.begin(); it != cons.end(); ++it) {
			++changes;
			(**it).exchange_target(oldnode,newnode);
		}
	}
	return changes;

}

void cmf::water::set_flux( flux_node::ptr source,flux_node::ptr target,real flux_value)
{
	external_control_connection* con = dynamic_cast<external_control_connection*>(source->connection_to(*target));
	if (con) {
		con->flux = (con->left_node() == source ? flux_value  : -flux_value);
	} else {
		throw std::runtime_error("No external controlled connection between " 
			+ source->to_string() + " and " 
			+ target->to_string()+". \n You can create one overriding any existing connection (Python):\n>>> cmf.connect(" + source->Name + "," + target->Name + ",flux)");
	}

}

bool cmf::water::can_set_flux( flux_node::ptr source,flux_node::ptr target )
{
	external_control_connection* con = dynamic_cast<external_control_connection*>(source->connection_to(*target));
	return con != 0;
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
		_sum += con->m_q * dt.AsDays();
		// set new current time
		_t=until;
	}
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
		_name(connection.to_string()+ " (Integrator)")
{}


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