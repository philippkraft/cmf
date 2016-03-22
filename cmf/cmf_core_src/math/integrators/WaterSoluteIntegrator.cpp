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

void cmf::math::SoluteWaterIntegrator::add_states( cmf::math::StateVariableOwner& stateOwner )
{
	Integrator::add_states(stateOwner);
	distribute_states();
}

void SoluteWaterIntegrator::distribute_states()
{
	for(state_vector::iterator it = m_States.begin(); it != m_States.end(); ++it)
	{
		WaterStorage::ptr ws = std::dynamic_pointer_cast<WaterStorage>(*it);
		SoluteStorage::ptr ss = std::dynamic_pointer_cast<SoluteStorage>(*it);
		if(ws)	m_waterintegrator->add_single_state(ws);
		else if (ss) m_soluteintegrators[ss->Solute.Id]->add_single_state(ss);
		else throw std::runtime_error("SoluteWaterIntegrator: Got unknown state" + (**it).to_string());
	}
}

int SoluteWaterIntegrator::integrate( Time t_max,Time dt )
{
	m_waterintegrator->integrate(t_max,dt);
	Time t = m_waterintegrator->get_t();
	for(Integrators::iterator it=m_soluteintegrators.begin();it!=m_soluteintegrators.end();++it) {
		Integrator& integ= **it;
		integ.reset();
		integ.integrate_until(t,dt);
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
	for(solute_vector::const_iterator it=solutes.begin();it!=solutes.end();++it){
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
		cmf::math::StateVariableOwner& states ) 
		: Integrator(), solutes(_solutes), m_waterintegrator(0)
{
	m_waterintegrator =water_integrator_templ.copy();
	createsoluteintegrators(m_soluteintegrators,_solutes,solute_integrator_templ);
	add_states(states);
}

void cmf::math::SoluteWaterIntegrator::reset()
{
	m_waterintegrator->set_t(m_t);
	for(Integrators::iterator it=m_soluteintegrators.begin();it!=m_soluteintegrators.end();++it){
		(*it)->set_t(m_t);
	}

}

std::string cmf::math::SoluteWaterIntegrator::to_string() const
{
	std::stringstream str;
	str << "Solute water integrator with " << this->size() << " states total." << std::endl;
	str << "    " << this->m_waterintegrator->size() << " states for water" << std::endl;
	for(solute_vector::const_iterator it=solutes.begin();it!=solutes.end();++it) {
		str << "    " << m_soluteintegrators[it->Id]->size() << " states for [" << it->Name << "]" << std::endl;
	}
	str << "dt=" << m_waterintegrator->get_dt() << std::endl;
	return str.str();

}
SoluteWaterIntegrator* SoluteWaterIntegrator::copy() const
{
	return new SoluteWaterIntegrator(solutes,*m_waterintegrator,**(m_soluteintegrators.begin()));
}
