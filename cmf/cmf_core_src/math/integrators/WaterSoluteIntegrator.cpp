// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
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

#include "WaterSoluteIntegrator.h"
#include "../../water/WaterStorage.h"
#include "../../water/SoluteStorage.h"

using namespace cmf::water;
using namespace cmf::math;


void SoluteWaterIntegrator::erase_integrators() {
	delete m_waterintegrator;
	for(Integrators::iterator it=m_soluteintegrators.begin();it!=m_soluteintegrators.end();++it){
		delete *it;
	}
	m_soluteintegrators.erase(m_soluteintegrators.begin(),m_soluteintegrators.end());
}


void SoluteWaterIntegrator::distribute_states()
{
	state_list water_states;
	std::vector<state_list> solute_states(m_soluteintegrators.size());

    for(auto& state: get_system())
	{
		WaterStorage::ptr ws = std::dynamic_pointer_cast<WaterStorage>(state);
		SoluteStorage::ptr ss = std::dynamic_pointer_cast<SoluteStorage>(state);
		if(ws)
		    water_states.append(state);
		else if (ss)
		    solute_states[ss->Solute.Id].append(state);
		else
		    throw std::runtime_error("SoluteWaterIntegrator: Got unknown state" + state->to_string());
	}
    m_waterintegrator->set_system(water_states);
    for (size_t i=0; i < m_soluteintegrators.size(); ++i) {
        m_soluteintegrators[i]->set_system(solute_states[i]);
    }
}

int SoluteWaterIntegrator::integrate( Time t_max,Time dt )
{
	m_waterintegrator->integrate(t_max,dt);
	Time t = m_waterintegrator->get_t();
	for(Integrator* integ: m_soluteintegrators) {
		integ->reset();
		integ->integrate_until(t,dt);
	}
	m_t=t;
	m_dt=m_waterintegrator->get_dt();
	return 1;
}

inline void createsoluteintegrators(		
	std::vector<Integrator*> & soluteintegrators,
	const cmf::water::solute_vector& solutes, 
	const cmf::math::Integrator& solute_integrator_templ ) 
{
	for(auto& solute: solutes){
		Integrator* new_solute_integ=solute_integrator_templ.copy();
		soluteintegrators.push_back(new_solute_integ);
	}

}

cmf::math::SoluteWaterIntegrator::SoluteWaterIntegrator( 
		const cmf::water::solute_vector& _solutes, 
		const cmf::math::Integrator& water_integrator_templ, 
		const cmf::math::Integrator& solute_integrator_templ ) 
	: Integrator(), solutes(_solutes), m_waterintegrator(0)
{
	m_waterintegrator =water_integrator_templ.copy();
	createsoluteintegrators(m_soluteintegrators,_solutes,solute_integrator_templ);
}

cmf::math::SoluteWaterIntegrator::SoluteWaterIntegrator( 
		cmf::water::solute_vector _solutes, 
		const cmf::math::Integrator& water_integrator_templ, 
		const cmf::math::Integrator& solute_integrator_templ, 
		const cmf::math::state_list& states )
		: Integrator(states), solutes(_solutes), m_waterintegrator(0)
{
	m_waterintegrator =water_integrator_templ.copy();
	createsoluteintegrators(m_soluteintegrators,_solutes,solute_integrator_templ);
	distribute_states();
}

void cmf::math::SoluteWaterIntegrator::reset()
{
	m_waterintegrator->reset();
    m_waterintegrator->set_t(m_t);
	for(auto& solute_integ : m_soluteintegrators){
	    solute_integ->reset();
		solute_integ->set_t(m_t);
	}

}

std::string cmf::math::SoluteWaterIntegrator::to_string() const
{
	std::stringstream str;
	str << "Solute water integrator with " << this->size() << " states total." << std::endl;
	str << "    " << this->m_waterintegrator->size() << " states for water" << std::endl;
	for(auto& solute: solutes) {
		str << "    " << m_soluteintegrators[solute.Id]->size() << " states for [" << solute.Name << "]" << std::endl;
	}
	str << "dt=" << m_waterintegrator->get_dt() << std::endl;
	return str.str();

}
SoluteWaterIntegrator* SoluteWaterIntegrator::copy() const
{
	return new SoluteWaterIntegrator(solutes,*m_waterintegrator,**(m_soluteintegrators.begin()));
}

