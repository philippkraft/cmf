#include "Topology.h"

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

double cmf::upslope::Topology::ContributingArea( bool forceRecalc/*=false*/ )
{
	if (m_CatchmentSize<cell->Area() || forceRecalc)
	{
		m_CatchmentSize=cell->Area();
		for (NeighborIterator it=*this;it.valid();++it)
			if (*this==it->MainOutlet())
				m_CatchmentSize+=it->ContributingArea();
	}
	return m_CatchmentSize;
}