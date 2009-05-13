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
	m_meteo =meteo_pointer(new cmf::atmosphere::ConstantMeteorology);
	m_precipitation=prec_pointer(new cmf::atmosphere::RainfallNode(*this));
	m_precipitation->Name="Precipitation";
	add_boundary_condition("Evaporation");
	add_boundary_condition("Transpiration");
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

void cmf::project::ClearLayers()
{
	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
	{
		cmf::upslope::Cell & cell=**it;
		cell.RemoveLayers();
	}
}

void cmf::project::add_boundary_condition( std::string name,cmf::geometry::point Location/*=cmf::geometry::point()*/ )
{
	cmf::water::FluxNode* node=new cmf::water::FluxNode(*this,Location);
	node_pointer pNode=node_pointer(node);
	m_boundaries[name]=pNode;
}

void cmf::project::add_boundary_condition(cmf::water::FluxNode* new_boundary_condition )
{
	node_pointer bc=node_pointer(new_boundary_condition);
	m_boundaries[bc->Name]=bc;
}
cmf::water::FluxNode& cmf::project::get_boundary_condition( std::string name ) const
{
	boundary_map::const_iterator it=m_boundaries.find(name);
	if (it==m_boundaries.end())
		throw std::out_of_range(std::string("Boundary condition "+name+std::string(" not found in project!")));
	else
		return *it->second;


}

void cmf::project::LoadMeteorology( std::string filename )
{
	cmf::atmosphere::SingleMeteorology* newmeteo=new cmf::atmosphere::SingleMeteorology(filename);
	m_meteo.reset(newmeteo);
}
double cmf::project::Area() const
{
	return cmf::upslope::area(*this);
}

