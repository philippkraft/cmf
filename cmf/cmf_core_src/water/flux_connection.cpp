

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

int cmf::water::flux_connection::nextconnectionid=0;
cmf::water::flux_connection::~flux_connection()
{
}

cmf::water::flux_connection::flux_connection( cmf::water::flux_node::ptr left,cmf::water::flux_node::ptr right,std::string _type ) 
: m_left(left),m_right(right),RecalcAlways(false),type(_type),connection_id(nextconnectionid++),m_tracer_filter(1.0)
{
	if ((!left) || (!right)) 
		throw std::runtime_error("Can't create " + this->type + " if a node is null");
	const project& p=left->project();
	if (p.debug)
		std::cout << "Create " << to_string() << std::endl;
	left->RegisterConnection(this);
	right->RegisterConnection(this);
}

void cmf::water::flux_connection::exchange_target( cmf::water::flux_node::ptr oldtarget,cmf::water::flux_node::ptr newTarget )
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
	if (newTarget->project().debug)
	{
		std::cout << "Replaced " << oldtarget->Name << " with " << newTarget->Name << " in " << this->to_string() << std::endl;
	}

}

std::string cmf::water::flux_connection::to_string() const
{
	return type + "(" + left_node()->to_string() + "<->" + right_node()->to_string() + ")";
}


cmf::water::flux_node::ptr cmf::water::flux_connection::get_target( int index ) const
{
	if (index==0 || index==-2)
		return left_node();
	else if (abs(index)==1)
		return right_node();
	else
		throw std::out_of_range("Only indices 0 and 1 are valid for querying a connection target");
}

cmf::water::flux_node::ptr cmf::water::flux_connection::get_target(const flux_node& inquirer )
{
	if 		   (left_node() && inquirer == *left_node()) 
			return right_node();
	else if (right_node() && inquirer == *right_node()) 
			return left_node();
	else
			return flux_node::ptr();
}

real cmf::water::flux_connection::conc( cmf::math::Time t, const cmf::water::solute& _Solute )
{
	real _q=q(t);
	if (_q>0) return left_node()->conc(t,_Solute);
	else if (_q<0) return right_node()->conc(t,_Solute);
	else return 0.0;
}

std::string cmf::water::flux_connection::short_string() const
{
	std::stringstream str;
	str << type << "#" << connection_id;
	return str.str();
}

bool cmf::water::flux_connection::kill_me()
{
	flux_node::ptr 
		left=left_node(),
		right=right_node();
	if (left) 
		left->DeregisterConnection(this);
	if (right)
		right->DeregisterConnection(this);
	return weak_this.use_count() <= 1;
}
void cmf::water::replace_node(cmf::water::flux_node::ptr oldnode,cmf::water::flux_node::ptr newnode)
{
	newnode->Location=oldnode->Location;
	newnode->Name=oldnode->Name;
	connection_vector cons=oldnode->get_connections();
	for(connection_vector::iterator it = cons.begin(); it != cons.end(); ++it)
	{
		(**it).exchange_target(oldnode,newnode);
	}
}
