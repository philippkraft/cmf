

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
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
#include "upslope/Topology.h"
#include "upslope/algorithm.h"
#include <set>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <algorithm>

cmf::_Options cmf::options;

cmf::project::project(std::string solutenames)
:	solutes(solutenames)
{
}

cmf::project::~project()
{
	for(auto& cell: m_cells)
		delete &cell;
}


cmf::river::Reach::ptr cmf::project::get_reach( ptrdiff_t index )
{
	return m_reaches.at(index<0 ? index + m_reaches.size() : index);
}

cmf::water::node_list cmf::project::get_storages()
{
	using namespace cmf::upslope;
	using namespace cmf::river;
	using namespace cmf::water;
	using namespace std;

	node_list res;
	for(const auto& reach: m_reaches)
	{
		res.append(reach);
	}

	for(const auto& cell: m_cells)
	{
		for (size_t i = 0; i < cell.storage_count() ; ++i)
			res.append(cell.get_storage(i));
		for (size_t i = 0; i < cell.layer_count() ; ++i)
			res.append(cell.get_layer(i));
	}
	for(const auto& node: m_nodes)
	{
		WaterStorage::ptr ws = WaterStorage::cast(node);
		if (ws) res.append(ws);
	}
	return res;
}


void cmf::project::use_IDW_meteo( double z_weight/*=0*/,double power/*=2*/ )
{
	for(auto& cell: m_cells)
	{
		cmf::atmosphere::IDW_Meteorology meteo(cell.get_position(),this->meteo_stations,z_weight,power);
		cell.set_meteorology(meteo);
	}
}

void cmf::project::use_nearest_meteo( double z_weight/*=0*/ )
{
	for(auto& cell: m_cells)
	{	
		cell.set_meteorology(this->meteo_stations.reference_to_nearest(cell.get_position(),z_weight));
	}

}

void cmf::project::use_IDW_rainfall( double z_weight/*=0*/,double power/*=2*/ )
{
	for(auto& cell: m_cells)
	{
		cmf::atmosphere::IDWRainfall::ptr rs = cmf::atmosphere::IDWRainfall::create(*this,cell.get_position(),z_weight,power);
		cell.set_rain_source(rs);
	}
}

void cmf::project::use_nearest_rainfall( double z_weight/*=0*/ )
{
	for(auto& cell: m_cells)
		cell.set_rain_source(cmf::atmosphere::RainfallStationReference::from_nearest_station(*this,cell.get_position(),z_weight));
}

cmf::project::operator cmf::math::state_list()
{
	using namespace cmf::math;
	using namespace cmf::water;
	return cmf::math::state_list(this->get_storages());

}

size_t cmf::project::add_node( cmf::water::flux_node::ptr node )
{
	m_nodes.append(node);
	return m_nodes.size();
}

size_t cmf::project::remove_node( cmf::water::flux_node::ptr node )
{
	m_nodes.remove(node);
	return m_nodes.size();
}

cmf::water::WaterStorage::ptr cmf::project::NewStorage( std::string Name, double x, double y, double z )
{
	cmf::water::WaterStorage::ptr s = cmf::water::WaterStorage::create(*this);
	s->position = cmf::geometry::point(x,y,z);
	s->Name = Name;
	add_node(s);
	return s;
}

cmf::river::OpenWaterStorage::ptr cmf::project::NewOpenStorage( std::string Name, double x, double y, double z,double area )
{
	cmf::river::OpenWaterStorage::ptr os = cmf::river::OpenWaterStorage::create(*this,area);
	os->position = cmf::geometry::point(x,y,z);
	os->Name = Name;
	add_node(os);
	return os;
}


cmf::river::Reach::ptr cmf::project::NewReach( double x,double y, double z, cmf::river::IChannel& shape, bool diffusive/*=false*/ )
{
	cmf::river::Reach::ptr R = cmf::river::Reach::create(*this,shape,diffusive);
	R->position=cmf::geometry::point(x,y,z);
	m_reaches.push_back(R);
	return m_reaches.back();

}

cmf::water::DirichletBoundary::ptr cmf::project::NewOutlet( std::string name,double x, double y, double z)
{
	cmf::water::DirichletBoundary::ptr res(new cmf::water::DirichletBoundary(*this,z,cmf::geometry::point(x,y,z) ));
	res->Name = name;
	this->add_node(res);
	return res;
}


cmf::upslope::Cell* cmf::project::NewCell( double x,double y,double z, double area,bool with_surfacewater/*=false*/ )
{
	cmf::upslope::Cell* new_cell=new cmf::upslope::Cell(x,y,z,area,*this);
	if (with_surfacewater) new_cell->surfacewater_as_storage();
	m_cells.append(*new_cell);
	return new_cell;
}

cmf::water::NeumannBoundary::ptr cmf::project::NewNeumannBoundary( std::string name, cmf::water::WaterStorage::ptr target )
{
	cmf::water::NeumannBoundary::ptr new_nb= cmf::water::NeumannBoundary::create(target);
	new_nb->Name = name;
	this->add_node(new_nb);
	return new_nb;
}

