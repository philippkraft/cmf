

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
#include "project.h"
#include "upslope/SoilLayer.h"
#include "Upslope/Topology.h"
#include "Upslope/algorithm.h"
#include <set>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <algorithm>
cmf::project::project(std::string solutenames) :	debug(false), solutes(solutenames)
{
}

cmf::project::~project()
{
	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		delete *it;
	m_cells.clear();
}

cmf::river::Reach_ptr cmf::project::NewReach( cmf::river::Channel shape, bool diffusive/*=false*/ )
{
	cmf::river::Reach::ptr R = cmf::river::Reach::create(*this,shape,diffusive);
	m_reaches.push_back(R);
	return m_reaches.back();
}

void cmf::project::AddStateVariables( cmf::math::StateVariableVector& vector )
{
	using namespace cmf::upslope;
	using namespace cmf::river;

	std::map<Cell*, Reach_ptr> reaches;
	for(std::vector<cmf::river::Reach_ptr>::iterator r_it = m_reaches.begin(); r_it != m_reaches.end(); ++r_it)
	{
		cmf::river::Reach_ptr reach=*r_it;
		cell_vector cells=reach->get_cells();
		for(cell_vector::iterator it = cells.begin(); it != cells.end(); ++it)
		{
			reaches[*it] = reach;
		}
	}

	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
	{
		for (int i = 0; i < (**it).layer_count() ; ++i)
			(**it).get_layer(i)->AddStateVariables(vector);
		for (int i = 0; i < (**it).storage_count() ; ++i)
			(**it).get_storage(i)->AddStateVariables(vector);
		if (reaches.find(*it)!=reaches.end())
			reaches[*it]->AddStateVariables(vector);
	}

}
cmf::river::Reach_ptr cmf::project::get_reach( int index )
{
	return m_reaches.at(index<0 ? index + m_reaches.size() : index);
}

cmf::water::node_list cmf::project::get_storages()
{
	using namespace cmf::upslope;
	using namespace cmf::river;

	cmf::water::node_list res;
	std::map<Cell*, Reach_ptr> reaches;
	for(std::vector<cmf::river::Reach_ptr>::iterator r_it = m_reaches.begin(); r_it != m_reaches.end(); ++r_it)
	{
		cmf::river::Reach_ptr reach=*r_it;
		cell_vector cells=reach->get_cells();
		for(cell_vector::iterator it = cells.begin(); it != cells.end(); ++it)
		{
			reaches[*it] = reach;
		}
	}

	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
	{
		for (int i = 0; i < (**it).layer_count() ; ++i)
			res.append((**it).get_layer(i));
		for (int i = 0; i < (**it).storage_count() ; ++i)
			res.append((**it).get_storage(i));
		if (reaches.find(*it)!=reaches.end())
			res.append(reaches[*it]);
	}
	return res;


}

cmf::water::DricheletBoundary::ptr cmf::project::NewOutlet( std::string name,cmf::geometry::point p )
{
	cmf::water::DricheletBoundary::ptr res(new cmf::water::DricheletBoundary(*this,p.z,p));
	res->Name = name;
	outlets.append(res);
	return res;
}