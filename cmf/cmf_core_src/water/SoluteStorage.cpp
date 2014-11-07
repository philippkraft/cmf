

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
#include <limits>
#include "SoluteStorage.h"
#include "WaterStorage.h"
#include "flux_connection.h"
using namespace cmf::water;
real SoluteStorage::dxdt( const cmf::math::Time& time )
{
 	// Sums up the fluxes as water fluxes (mol/day)
	
	connection_list connections=m_water->get_connections();
	real inflow=0, outflow=0;
	for (connection_list::iterator it = connections.begin();it!=connections.end();++it)
	{
		flux_connection& con=**it;
		real q=con.q(*m_water,time);
		if (q>0)
		{
			inflow += q * con.conc(time,Solute);
		}
		else if (q<0)
		{
			outflow += q * con.conc(time,Solute);
		}
	}
	real source_term = source;
	real decay_term = this->get_state() * decay;
	return inflow + outflow + source_term - decay_term;
}

real SoluteStorage::conc() const
{
	real V = m_water->get_volume();
	real c = this->adsorption->freesolute(get_state(),V) /V;
	if (c > std::numeric_limits<real>::min() && c < std::numeric_limits<real>::max())
		return c;
	else
		return 0.0;
}

std::string SoluteStorage::to_string() const
{
	return m_water->to_string() + "[" + Solute.Name + "]";
}





void cmf::water::SoluteStorage::set_conc( real NewConcentration )
{
	real
		V = m_water->get_volume(),
		xf = NewConcentration * V,
		xt = this->adsorption->totalsolute(xf,V);
	this->set_state(xt);
	

}
