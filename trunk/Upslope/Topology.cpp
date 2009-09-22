#include "Topology.h"
#include <algorithm>
cmf::upslope::Cell * cmf::upslope::Topology::MainOutlet( bool forceRecalc/*=false*/ )
{
	if (!m_MainOutlet || forceRecalc)
	{
		m_MainOutlet=0;
		double max_slope=0;
		for (NeighborIterator it=*this;it.valid();++it)
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
	return c1->z>c2->z;
}
void cmf::upslope::Topology::calculate_contributing_area( const cmf::upslope::cell_vector& cells)
{
	cmf::upslope::cell_vector sorted_cells(cells.begin(),cells.end());
	std::sort(sorted_cells.begin(),sorted_cells.end(),cell_is_higher);

	for(cmf::upslope::cell_vector::iterator it = sorted_cells.begin(); it != sorted_cells.end(); ++it)
		(**it).get_topology().m_CatchmentSize=0.0;

	for(cmf::upslope::cell_vector::iterator it = sorted_cells.begin(); it != sorted_cells.end(); ++it)
	{
		cmf::upslope::Topology& topo=(**it).get_topology();
		topo.m_CatchmentSize+=(**it).get_area();
		if (topo.MainOutlet())
			topo.MainOutlet()->get_topology().m_CatchmentSize+=topo.m_CatchmentSize;
	}
}