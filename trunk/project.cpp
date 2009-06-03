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


