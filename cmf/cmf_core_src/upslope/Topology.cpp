

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
#include "Topology.h"
#include <algorithm>
cmf::upslope::Cell * cmf::upslope::Topology::MainOutlet( bool forceRecalc/*=false*/ )
{
	if (!m_MainOutlet || forceRecalc)
	{
		m_MainOutlet=0;
		double max_slope=0;
		for (neighbor_iterator it=*this;it.valid();++it)
		{
			double slope=slope_to(*it);
			if (slope<max_slope)
			{
				m_MainOutlet=&it.cell();
				max_slope=slope;
			}					
		}
	}
	return m_MainOutlet;
}

double cmf::upslope::Topology::ContributingArea()	const
{
	if (m_CatchmentSize	< cell->get_area())
		throw std::runtime_error("Contributing area is not calculated! Please calculate the contributing area for a list of cells prior to querying");
	return m_CatchmentSize;
}

bool cell_is_higher(cmf::upslope::Cell* c1,cmf::upslope::Cell* c2)
{
	return c1->z > c2->z;
}

void cmf::upslope::Topology::calculate_contributing_area( const cmf::upslope::cell_vector& cells)
{
	cmf::upslope::cell_vector sorted_cells(cells);
	std::sort(sorted_cells.ptr_begin(),sorted_cells.ptr_end(),cell_is_higher);

	for(cmf::upslope::cell_vector::iterator it = sorted_cells.begin(); it != sorted_cells.end(); ++it)
	{
		it->get_topology().m_CatchmentSize=0.0;
		it->get_topology().MainOutlet(true);
	}

	for(cmf::upslope::cell_vector::iterator it = sorted_cells.begin(); it != sorted_cells.end(); ++it)
	{
		cmf::upslope::Topology& topo=it->get_topology();
		topo.m_CatchmentSize+=it->get_area();
		if (topo.MainOutlet())
			topo.MainOutlet()->get_topology().m_CatchmentSize+=topo.m_CatchmentSize;
	}
}

void cmf::upslope::Topology::AddNeighbor( Topology & target,double flowwidth )
{
	if (&target != this)
	{
		m_Neighbors[&target]=flowwidth;
		target.m_Neighbors[this]=flowwidth;
		m_MainOutlet = 0;
	}

}

void cmf::upslope::Topology::RemoveNeighbor( Topology & target )
{
	if (m_Neighbors.find(&target)!=m_Neighbors.end())
	{
		m_Neighbors.erase(&target);
		target.RemoveNeighbor(*this);
		m_MainOutlet = 0;
	}
}

