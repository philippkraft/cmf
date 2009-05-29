#include "project.h"
#include "upslope/SoilWaterStorage.h"
#include "Upslope/Topology.h"
#include "Upslope/algorithm.h"
#include <set>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <algorithm>
cmf::project::project() :	debug(false)
{
}

cmf::project::~project()
{
	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		delete *it;
	m_cells.clear();
}

cmf::upslope::Cell* cmf::project::Cell( cmf::geometry::point p,double max_dist/*=1e20*/ )
{
	return cmf::upslope::find_cell(m_cells,p,max_dist);
}

void cmf::project::clear_layers()
{
	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
	{
		cmf::upslope::Cell & cell=**it;
		cell.remove_layers();
	}
}


double cmf::project::Area() const
{
	return cmf::upslope::area(*this);
}

