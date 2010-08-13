

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
#include "upslope/Topology.h"
#include "upslope/algorithm.h"
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
		delete it.ptr();
}



cmf::river::Reach::ptr cmf::project::NewReach( double x,double y, double z, double length, char Type/*='T'*/,double width/*=0.5*/,double depth/*=0.1*/, bool diffusive/*=false*/ )
{
	cmf::river::Channel ch(Type,length,width,depth);
	cmf::river::Reach::ptr R = cmf::river::Reach::create(*this,ch,diffusive);
	R->position=cmf::geometry::point(x,y,z);
	m_reaches.push_back(R);
	return m_reaches.back();
}

cmf::river::Reach::ptr cmf::project::get_reach( int index )
{
	return m_reaches.at(index<0 ? index + m_reaches.size() : index);
}

cmf::water::node_list cmf::project::get_storages()
{
	using namespace cmf::upslope;
	using namespace cmf::river;

	cmf::water::node_list res;
	std::map<Cell*, Reach::ptr> reaches;
	for(std::vector<cmf::river::Reach::ptr>::iterator r_it = m_reaches.begin(); r_it != m_reaches.end(); ++r_it)
	{
		cmf::river::Reach::ptr reach=*r_it;
		res.append(reach);
	}

	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
	{
		for (int i = 0; i < it->layer_count() ; ++i)
			res.append(it->get_layer(i));
		for (int i = 0; i < it->storage_count() ; ++i)
			res.append(it->get_storage(i));
		if (reaches.find(it)!=reaches.end())
			res.append(reaches[it]);
	}
	return res;


}

cmf::water::DricheletBoundary::ptr cmf::project::NewOutlet( std::string name,double x, double y, double z)
{
	cmf::water::DricheletBoundary::ptr res(new cmf::water::DricheletBoundary(*this,z,cmf::geometry::point(x,y,z) ));
	res->Name = name;
	outlets.append(res);
	return res;
}

void cmf::project::use_IDW_meteo( double z_weight/*=0*/,double power/*=2*/ )
{
	for(upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
	{
		cmf::atmosphere::IDW_Meteorology meteo(it->get_position(),this->meteo_stations,z_weight,power);
		it->set_meteorology(meteo);
	}
}

void cmf::project::use_nearest_meteo( double z_weight/*=0*/ )
{
	for(upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
	{	
		it->set_meteorology(this->meteo_stations.reference_to_nearest(it->get_position(),z_weight));
	}

}

void cmf::project::use_IDW_rainfall( double z_weight/*=0*/,double power/*=2*/ )
{
	for(upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
	{
		cmf::atmosphere::IDWRainfall::ptr rs = cmf::atmosphere::IDWRainfall::create(*this,it->get_position(),z_weight,power);
		it->set_rain_source(rs);
	}
}

void cmf::project::use_nearest_rainfall( double z_weight/*=0*/ )
{
	for(upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		it->set_rain_source(cmf::atmosphere::RainfallStationReference::from_nearest_station(*this,it->get_position(),z_weight));
}

cmf::math::state_queue cmf::project::get_states()
{
	cmf::math::state_queue q;
	using namespace cmf::upslope;
	using namespace cmf::river;

	for(std::vector<cmf::river::Reach::ptr>::iterator r_it = m_reaches.begin(); r_it != m_reaches.end(); ++r_it)
		q.push(**r_it);

	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		q.push(*it);
	return q;
}