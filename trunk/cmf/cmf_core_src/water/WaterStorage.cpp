

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

struct null_deleter {	void operator()(void const *) const {}};

void WaterStorage::initializeSoluteStorages(const solute_vector& solutes)
{
	for (solute_vector::const_iterator it=solutes.begin();it!=solutes.end();++it)
	{
		std::tr1::shared_ptr<SoluteStorage> s(new SoluteStorage(this,*it));
		m_Concentrations.push_back(s);
	}
}

void WaterStorage::AddStateVariables( cmf::math::StateVariableVector& vector )
{
	vector.push_back(this);
	for(SoluteStorageMap::const_iterator it = m_Concentrations.begin(); it != m_Concentrations.end(); ++it)
	{
	    vector.push_back(it->get());
	}
}

WaterStorage::WaterStorage(const cmf::project& _project,double InitialState/*=0*/ ) 
: cmf::math::StateVariable(InitialState),flux_node(_project) ,m_Concentrations(), m_state_variable_content('V')
{
	initializeSoluteStorages(_project.solutes);
}


SoluteStorage& WaterStorage::Solute( const solute& _Solute )
{
	return *m_Concentrations[_Solute.Id];
}


real WaterStorage::conc(const solute& _Solute) const
{
	return Solute(_Solute).conc();
}

std::tr1::shared_ptr<WaterStorage> WaterStorage::from_node( flux_node::ptr node )
{
	WaterStorage* ws=new WaterStorage(node->project());
	WaterStorage::ptr result(ws);

	replace_node(node,result);
	return result;
}

void WaterStorage::set_state_variable_content(char content) {
	if (content == 'V' || content == 'h')
		m_state_variable_content=content;
	else
		throw std::runtime_error("Only 'V' (volume) and 'h' (head) are allowed values to describe the integrated variable in " + this->Name);
}
