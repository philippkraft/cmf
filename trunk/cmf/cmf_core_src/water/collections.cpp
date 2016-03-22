

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
#include <algorithm>
#include "collections.h"
#include "WaterStorage.h"
real cmf::water::node_list::global_water_balance( cmf::math::Time t )	const
{
	real sum=0;
#pragma omp parallel for reduction(+ : sum)
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
	{
		sum+=m_nodes[i]->waterbalance(t);					
	}
	return sum;
}

cmf::math::num_array cmf::water::node_list::water_balance( cmf::math::Time t ) const
{
	cmf::math::num_array res(size());
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
	{
		res[i]=m_nodes[i]->waterbalance(t);
	}
	return res;
}

cmf::math::num_array cmf::water::node_list::get_fluxes_to( const cmf::water::node_list& targets ,cmf::math::Time t ) const 
{
	if (size() != targets.size())
		throw std::invalid_argument("The list for the target nodes need to have the same length as this node_list");
	cmf::math::num_array res(size());
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < (ptrdiff_t)res.size() ; ++i)
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
	for (ptrdiff_t i = 0; i < (ptrdiff_t)res.size() ; ++i)
	{
		res.set(i,m_nodes[i]->flux3d_to(*targets.m_nodes[i],t));
	}
	return res;


}

cmf::geometry::point_vector cmf::water::node_list::get_fluxes3d( cmf::math::Time t ) const
{
	cmf::geometry::point_vector res(size());
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < (ptrdiff_t)res.size() ; ++i)
	{
		res.set(i,m_nodes[i]->get_3d_flux(t));
	}
	return res;


}

cmf::geometry::point_vector cmf::water::node_list::get_positions() const
{
	cmf::geometry::point_vector res(size());
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < (ptrdiff_t)res.size() ; ++i)
	{
		res.set(i,m_nodes[i]->position);
	}
	return res;

}

ptrdiff_t cmf::water::node_list::set_potentials( const cmf::math::num_array& potentials )
{
	if (size()!=potentials.size())
		throw std::out_of_range("Size of potential array does not fit the size of the node_list");
	size_t ok_count=size();
	for (size_t i = 0; i < size() ; ++i)
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
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
	{
		res[i]=m_nodes[i]->get_potential();		
	}
	return res;
}

cmf::math::num_array cmf::water::node_list::conc( cmf::math::Time t, const cmf::water::solute& _Solute ) const
{
	cmf::math::num_array res(size());
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
	{
		res[i]=m_nodes[i]->conc(t,_Solute);
	}
	return res;
}

ptrdiff_t cmf::water::node_list::set_solute_source( const cmf::water::solute& _Solute, cmf::math::num_array source_fluxes)
{
	if (size()!=source_fluxes.size())
		throw std::out_of_range("Size of solute source array does not fit the size of the node_list");
	ptrdiff_t ok_count=size();
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
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


void cmf::water::node_list::append( flux_node::ptr node )
{
	if (node==0) throw std::invalid_argument("node_list may only contain valid objects");
	m_nodes.push_back(node);
}

cmf::math::StateVariableList cmf::water::node_list::get_states()
{
	cmf::math::StateVariableList q;
	for(node_vector::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
	{
		cmf::math::StateVariableOwner *state_owner = dynamic_cast<cmf::math::StateVariableOwner *>(it->get());
		cmf::math::StateVariable::ptr state = std::dynamic_pointer_cast<cmf::math::StateVariable>(*it);
		if(state_owner) 
			q.extend(*state_owner);
		else if (state) 
			q.append(state);
	}
	return q;
}

cmf::water::node_list& cmf::water::node_list::operator+=( const cmf::water::node_list& right )
{
	for (size_t i = 0; i < right.size() ; ++i)
	{
		append(right[i]);
	}
	return *this;
}

cmf::water::node_list cmf::water::node_list::operator+( const cmf::water::node_list & right ) const
{
	cmf::water::node_list res(*this);
	res+=right;
	return res;
}

cmf::water::flux_node::ptr cmf::water::node_list::get( ptrdiff_t index ) const
{
	return m_nodes.at(index<0 ? size()+index : index);
}

cmf::water::node_list cmf::water::node_list::getslice( ptrdiff_t begin,ptrdiff_t end,ptrdiff_t step/*=1*/ ) const
{
	node_list res;
	for (ptrdiff_t i = begin; i <end  ; i+=step)
	{
		res.append(get(i));
	}
	return res;
}

bool cmf::water::node_list::remove( flux_node::ptr node )
{
	using namespace cmf::water;
	node_list::iterator it = 
		std::find(begin(),end(),node);
	if (it!=end()) {
		m_nodes.erase(it);
		return true;
	} else {
		return false;
	}

}
/************************************************************************/
/* Neumann-Boundary list                                                */
/************************************************************************/

cmf::math::num_array cmf::water::NeumannBoundary_list::get_fluxes( cmf::math::Time t ) const
{
	cmf::math::num_array res(m_boundaries.size());
	for (ptrdiff_t i = 0; i < ptrdiff_t(m_boundaries.size()) ; ++i)
	{
		res[i]=m_boundaries[i]->get_flux()[t];
	}
	return res;
}

void cmf::water::NeumannBoundary_list::set_fluxes( cmf::math::num_array values )
{
	if (values.size()!=m_boundaries.size())
		throw std::runtime_error("The input array with fluxes need to have the same size as this list");
	else
	{
		for (ptrdiff_t i = 0; i < values.size() ; ++i)
		{
			m_boundaries[i]->set_flux(values[i]);
		}
	}
}

real cmf::water::NeumannBoundary_list::global_water_balance( cmf::math::Time t )	const
{
	real sum=0;
#pragma omp parallel for reduction(+ : sum)
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
	{
		sum+=m_boundaries[i]->waterbalance(t);					
	}
	return sum;
}

cmf::math::num_array cmf::water::NeumannBoundary_list::water_balance( cmf::math::Time t ) const
{
	cmf::math::num_array res(size());
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
	{
		res[i]=m_boundaries[i]->waterbalance(t);
	}
	return res;
}

cmf::water::NeumannBoundary_list::NeumannBoundary_list( const cmf::water::node_list& copy )
{
	for (size_t i = 0; i < copy.size() ; ++i)
	{
		NeumannBoundary::ptr nbc=std::dynamic_pointer_cast<cmf::water::NeumannBoundary>(copy[i]);
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
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()) ; ++i)
	{
		res.append(get(i));
	}
	return res;
}