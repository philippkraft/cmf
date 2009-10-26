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
	using namespace cmf::river;
	Reach_ptr R (new Reach(*this,shape,diffusive));
	R->weak_this = R;
	reaches.push_back(R);
	return reaches.back();
}

