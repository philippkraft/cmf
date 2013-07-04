

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
#include "boundary_condition.h"
#include "../math/real.h"
void cmf::water::NeumannBoundary::connect_to( cmf::water::flux_node::ptr target )
{
	new NeumannFlux(*this, target);
}

real cmf::water::NeumannBoundary::operator()( cmf::math::Time t ) const
{
	if (_flux.is_empty())
		return 0.0;
	else
		return scale_function(_flux[t]);
}

cmf::water::NeumannBoundary::NeumannBoundary(cmf::project& _project, 
											 cmf::math::timeseries flux,
											 cmf::water::SoluteTimeseries _concentration/*=cmf::water::SoluteTimeseries()*/,
											 cmf::geometry::point loc/*=cmf::geometry::point()*/
																						) 
: cmf::water::flux_node(_project,loc), _flux(flux),concentration(_concentration)
{

}

cmf::water::NeumannBoundary::NeumannBoundary(cmf::project& _project,cmf::geometry::point loc) 
: cmf::water::flux_node(_project,loc)
{

}

cmf::water::NeumannBoundary::ptr cmf::water::NeumannBoundary::create( cmf::water::flux_node::ptr target )
{
	using cmf::water::NeumannBoundary;
	NeumannBoundary* new_bound = new NeumannBoundary(target->get_project(),target->position);
	NeumannBoundary::ptr res(new_bound);
	res->Name = "Boundary at " + target->Name;
	res->connect_to(target);
	return res;
}

real cmf::water::NeumannFlux::calc_q( cmf::math::Time t )
{
	real f=(*m_bc.lock())(t);
	if (f<0)
		return f * (1 - right_node()->is_empty());
	else
		return f;
}


cmf::water::DirichletBoundary::DirichletBoundary(cmf::project& _p,real potential,cmf::geometry::point position/*=cmf::geometry::point()*/ ) 
: flux_node(_p,position),m_Potential(potential), is_source(0)
{	
}

real cmf::water::DirichletBoundary::conc( cmf::math::Time t, const cmf::water::solute& _Solute ) const
{
	real node_conc=	cmf::water::flux_node::conc(t,_Solute);
	if (node_conc<=0.0 && (is_empty()<1.0))
	{
		conc_map::const_iterator it=m_concentration.find(_Solute);
		if (it!=m_concentration.end())
			return it->second;
		else
			return 0.0;
	}
	else
		return node_conc;
}

void cmf::water::DirichletBoundary::set_conc( const cmf::water::solute& _Solute, double value )
{
	m_concentration[_Solute]=value;
}