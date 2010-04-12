

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
#ifdef _OPENMP
#include <omp.h>
#endif
#include "collections.h"
#include "WaterStorage.h"
real cmf::water::node_list::global_water_balance( cmf::math::Time t )	const
{
	real sum=0;
#pragma omp parallel for reduction(+ : sum)
	for (int i = 0; i < (int)size() ; ++i)
	{
		sum+=m_nodes[i]->water_balance(t);					
	}
	return sum;
}

cmf::math::num_array cmf::water::node_list::water_balance( cmf::math::Time t ) const
{
	cmf::math::num_array res(size());
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; ++i)
	{
		res[i]=m_nodes[i]->water_balance(t);
	}
	return res;
}

cmf::math::num_array cmf::water::node_list::get_fluxes_to( const cmf::water::node_list& targets ,cmf::math::Time t ) const 
{
	if (size() != targets.size())
		throw std::invalid_argument("The list for the target nodes need to have the same length as this node_list");
	cmf::math::num_array res(size());
#pragma omp parallel for
	for (int i = 0; i < (int)res.size() ; ++i)
	{
		res[i]=m_nodes[i]->flux_to(*targets.m_nodes[i],t);
	}
	return res;

}

cmf::geometry::point_vector cmf::water::node_list::get_fluxes3d_to( const cmf::water::node_list& targets ,cmf::math::Time t ) const
{
	if (size() != targets.size())
		throw std::invalid_argument("The list for the target nodes need to have the same length as this node_list");
	cmf::geometry::point_vector res(size());
#pragma omp parallel for
	for (int i = 0; i < (int)res.size() ; ++i)
	{
		res.set(i,m_nodes[i]->flux3d_to(*targets.m_nodes[i],t));
	}
	return res;


}

cmf::geometry::point_vector cmf::water::node_list::get_fluxes3d( cmf::math::Time t ) const
{
	cmf::geometry::point_vector res(size());
#pragma omp parallel for
	for (int i = 0; i < (int)res.size() ; ++i)
	{
		res.set(i,m_nodes[i]->get_3d_flux(t));
	}
	return res;


}

cmf::geometry::point_vector cmf::water::node_list::get_positions() const
{
	cmf::geometry::point_vector res(size());
#pragma omp parallel for
	for (int i = 0; i < (int)res.size() ; ++i)
	{
		res.set(i,m_nodes[i]->Location);
	}
	return res;

}

int cmf::water::node_list::set_potentials( const cmf::math::num_array& potentials )
{
	if (size()!=potentials.size())
		throw std::out_of_range("Size of potential array does not fit the size of the node_list");
	int ok_count=size();
	for (int i = 0; i < size() ; ++i)
	{
		try
		{
			this->get(i)->set_potential(potentials[i]);
		}
		catch (std::runtime_error)
		{
			--ok_count;
		}
	}
	return ok_count;
}

cmf::math::num_array cmf::water::node_list::get_potentials()
{
	cmf::math::num_array res(size());
	#pragma omp parallel for
	for (int i = 0; i < (int)size() ; ++i)
	{
		res[i]=m_nodes[i]->get_potential();		
	}
	return res;
}

cmf::math::num_array cmf::water::node_list::conc( cmf::math::Time t, const cmf::water::solute& _Solute ) const
{
	cmf::math::num_array res(size());
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; ++i)
	{
		res[i]=m_nodes[i]->conc(t,_Solute);
	}
	return res;
}

int cmf::water::node_list::set_solute_source( const cmf::water::solute& _Solute, cmf::math::num_array source_fluxes)
{
	if (size()!=source_fluxes.size())
		throw std::out_of_range("Size of solute source array does not fit the size of the node_list");
	int ok_count=size();
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; ++i)
	{
		cmf::water::WaterStorage* storage=dynamic_cast<cmf::water::WaterStorage*>(m_nodes[i].get());
		if (storage)
		{
			storage->Solute(_Solute).source = source_fluxes[i];
		}
		else
			--ok_count;
	}
	return ok_count;

}

void cmf::water::node_list::AddStateVariables( cmf::math::StateVariableVector& vector )
{
	for(node_vector::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
	{
		cmf::math::StateVariableOwner *state_owner = dynamic_cast<cmf::math::StateVariableOwner *>(it->get());
		cmf::math::StateVariable* state=dynamic_cast<cmf::math::StateVariable*>(it->get());
		if(state_owner) 
			state_owner->AddStateVariables(vector);
		else if (state) 
			vector.push_back(state);
	}
}

void cmf::water::node_list::append( flux_node::ptr node )
{
	if (node==0) throw std::invalid_argument("node_list may only contain valid objects");
	m_nodes.push_back(node);
}
/************************************************************************/
/* Neumann-Boundary list                                                */
/************************************************************************/

cmf::math::num_array cmf::water::NeumannBoundary_list::get_fluxes( cmf::math::Time t ) const
{
	cmf::math::num_array res(m_boundaries.size());
	for (int i = 0; i < int(m_boundaries.size()) ; ++i)
	{
		res[i]=m_boundaries[i]->flux[t];
	}
	return res;
}

void cmf::water::NeumannBoundary_list::set_fluxes( cmf::math::num_array values )
{
	if (values.size()!=m_boundaries.size())
		throw std::runtime_error("The input array with fluxes need to have the same size as this list");
	else
	{
		for (int i = 0; i < values.size() ; ++i)
		{
			m_boundaries[i]->flux = values[i];
		}
	}
}

real cmf::water::NeumannBoundary_list::global_water_balance( cmf::math::Time t )	const
{
	real sum=0;
#pragma omp parallel for reduction(+ : sum)
	for (int i = 0; i < (int)size() ; ++i)
	{
		sum+=m_boundaries[i]->water_balance(t);					
	}
	return sum;
}

cmf::math::num_array cmf::water::NeumannBoundary_list::water_balance( cmf::math::Time t ) const
{
	cmf::math::num_array res(size());
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; ++i)
	{
		res[i]=m_boundaries[i]->water_balance(t);
	}
	return res;
}

cmf::water::NeumannBoundary_list::NeumannBoundary_list( const cmf::water::node_list& copy )
{
	for (int i = 0; i < copy.size() ; ++i)
	{
		NeumannBoundary_ptr nbc=std::tr1::dynamic_pointer_cast<cmf::water::NeumannBoundary>(copy[i]);
		if (nbc)
			m_boundaries.push_back(nbc);
	}
}

cmf::water::NeumannBoundary_list::NeumannBoundary_list( const NeumannBoundary_list& copy ) 
: m_boundaries(copy.m_boundaries.begin(),copy.m_boundaries.end())
{

}

cmf::water::node_list cmf::water::NeumannBoundary_list::to_node_list() const
{
	cmf::water::node_list res;
	for (int i = 0; i < int(size()) ; ++i)
	{
		res.append(get(i));
	}
	return res;
}