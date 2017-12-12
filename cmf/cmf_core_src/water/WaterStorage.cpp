

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
#include "WaterStorage.h"
#include "../project.h"

using namespace cmf::water;


void WaterStorage::initializeSoluteStorages(const solute_vector& solutes) {
	for (solute_vector::const_iterator it=solutes.begin();it!=solutes.end();++it)
	{
		std::shared_ptr<SoluteStorage> s(new SoluteStorage(this,*it));
		m_Concentrations.push_back(s);
	}
}


WaterStorage::WaterStorage(cmf::project& _project, const std::string& _Name,
	double InitialState/*=0*/, double scale/*=1*/ ) 
: cmf::math::StateVariable(InitialState, scale),flux_node(_project) ,m_Concentrations(), m_state_variable_content('V')
{
	initializeSoluteStorages(_project.solutes);
	this->Name = _Name;
}


SoluteStorage& WaterStorage::Solute( const solute& _Solute )
{
	return *m_Concentrations[_Solute.Id];
}


real WaterStorage::conc(const solute& _Solute) const
{
	return Solute(_Solute).conc();
}

std::shared_ptr<WaterStorage> WaterStorage::from_node( flux_node::ptr node )
{
	WaterStorage* ws=new WaterStorage(node->get_project(), node->Name);
	WaterStorage::ptr result(ws);

	replace_node(node,result);
	return result;
}

void WaterStorage::set_state_variable_content(char content) {
	if (content == 'V' || content == 'h') {
		if (m_state_variable_content != content) {
			real oldstate=get_state();
			real newstate=0.0;
			try
			{
				if (content == 'h') {
					newstate = volume_to_head(oldstate);
					real V = head_to_volume(newstate);
				} else {
					newstate =  head_to_volume(oldstate);
				}
				set_state(newstate);
				m_state_variable_content=content;
			}
			catch (std::runtime_error) {
				throw std::runtime_error(this->Name + std::string(" can only integrated over ") + m_state_variable_content + std::string(". Please do not change the variable."));
			}
		}
	}
	else {
		throw std::runtime_error("Only 'V' (volume) and 'h' (head) are allowed values to describe the integrated variable in " + this->Name);
	}
}

real cmf::water::WaterStorage::head_to_volume( real head ) const
{
	throw std::runtime_error("Head to volume function not implemented in " + this->Name);
	return 0.0;
}

real cmf::water::WaterStorage::volume_to_head( real volume ) const
{
	return position.z;
}

real cmf::water::WaterStorage::dxdt( const cmf::math::Time& time )
{
	// Gets the net fluxes of this water storage in m3/day
	real dVdt = waterbalance(time);
	// If head is the integrated variable
	if (get_state_variable_content()=='h')
	{
		// dh/dt = (h(V) - h(V+dV/dt * dt)/dt
		// dh/dV(V) is calculated numerically using a discrete timestep of one minute
		real dt = 1./(24.*60.);  // 1 min in days
		real V0 = get_volume();
		real h0 = get_state(); // current head
		real V1 = V0 + dVdt * dt; // Projected volume at t+dt
		real h1 = volume_to_head(V1); // Projected head at t+dt
		real dhdt = (h1-h0)/dt;
		return dhdt; // dh/dt in m/day
	}
	else // Integrate over volume
		// The net flux of this water storage is the derivate of the Volume
		return dVdt;
}

cmf::math::StateVariableList cmf::water::WaterStorage::get_states() 
{
	cmf::math::StateVariableList q;
	q.append(cmf::water::WaterStorage::ptr(*this));
	for(SoluteStorageMap::const_iterator it = m_Concentrations.begin(); it != m_Concentrations.end(); ++it)
	{
		q.append(*it);
	}
	return q;
}

