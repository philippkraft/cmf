

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

#include "cell.h"
#include "../project.h"
#include "SoilLayer.h"
#include "connections/AtmosphericFluxes.h"
#include "connections/surfacefluxes.h"
#include "connections/infiltration.h"
#include "../reach/ManningConnection.h"
#include "../reach/ReachType.h"
#include "../reach/Reach.h"
#include "Topology.h"
#include "../atmosphere/precipitation.h"
#include "../atmosphere/meteorology.h"

int cmf::upslope::Cell::cell_count=0;
cmf::upslope::Cell::~Cell()
{
	m_Layers.clear();
}


cmf::upslope::Cell::Cell( double _x,double _y,double _z,double area,cmf::project& _project/*=0*/ ) 
: x(_x),y(_y),z(_z),m_Area(area),m_project(_project),
m_SurfaceWater(new cmf::water::DricheletBoundary(_project,_z)),	Id(cell_count++),
m_meteo(new cmf::atmosphere::ConstantMeteorology)
{
	std::stringstream sstr;
	sstr << Id;
	std::string cell_id=sstr.str();
	m_SurfaceWater->Name="Surface water of cell #" + cell_id;
	m_SurfaceWater->Location=get_position();
	m_Evaporation.reset(new cmf::water::flux_node(_project,cmf::geometry::point(x,y,z+20)));
	m_Evaporation->Name="Evaporation of cell #" + cell_id;
	m_Transpiration.reset(new cmf::water::flux_node(_project,cmf::geometry::point(x,y,z+20)));
	m_Transpiration->Name="Transpiration of cell #" + cell_id;
	m_rainfall.reset(new cmf::atmosphere::RainCloud(*this));
	m_rainfall->Name="Rain of cell #" + cell_id;
	new connections::Rainfall(get_surfacewater(),*this);
}

cmf::upslope::Cell::Cell( const Cell& cpy ) 
: m_project(cpy.project()),
	x(cpy.x),y(cpy.y),z(cpy.z),m_Area(cpy.get_area())
{

}
real cmf::upslope::Cell::get_saturated_depth()	
{
	if (m_SatDepth<-10000)
	{
		// Find first saturated layer
		layer_list::const_iterator it;
		for (it = m_Layers.begin();it!=m_Layers.end();++it)
		{
			if ((**it).get_wetness() >= 1.0) {
				// Saturated depth is potential of deepest unsaturated layer
				m_SatDepth = this->z - (**it).get_potential();
				return m_SatDepth;
			}
		}
		m_SatDepth = this->z - m_Layers[-1]->get_potential();

	}
	return m_SatDepth;
}



void cmf::upslope::Cell::AddStateVariables( cmf::math::StateVariableVector& vector )
{
	for (int i = 0; i < storage_count() ; ++i)
		get_storage(i)->AddStateVariables(vector);
	for (int i = 0; i < layer_count() ; ++i)
		get_layer(i)->AddStateVariables(vector);
}


void cmf::upslope::Cell::add_layer(real lowerboundary,const cmf::upslope::RetentionCurve& r_curve,real saturateddepth/*=-10*/ )
{
	if (project().debug)
		std::cout << "Adds a layer to " << this->to_string() << " Name: ";
	SoilLayer::ptr layer(new SoilLayer(*this,lowerboundary,r_curve,saturateddepth));
	if (project().debug)
		std::cout << layer->Name << std::endl;
	if (m_Layers.size() == 0)
	{
		new connections::CompleteInfiltration(layer,m_SurfaceWater);
	}
	if (m_Layers.size()) {
		m_Layers[-1]->m_lower = SoilLayer::weak_ptr(layer);
		layer->m_upper = SoilLayer::weak_ptr(m_Layers[-1]);
	}
	m_Layers.append(layer);
}
cmf::upslope::SoilLayer::ptr cmf::upslope::Cell::get_layer(int index) const {
	return m_Layers[index];
}
cmf::upslope::vegetation::Vegetation cmf::upslope::Cell::get_vegetation() const
{
	vegetation::Vegetation res=m_vegetation;
	real snw_cvr=snow_coverage();
	res.albedo=snw_cvr * 0.9 + (1-snw_cvr)*res.albedo;
	return res;
}

cmf::atmosphere::Weather cmf::upslope::Cell::get_weather( cmf::math::Time t ) const
{
	return get_meteorology()(t);
}

void cmf::upslope::Cell::remove_layers()
{
	m_Layers.clear();
}

void cmf::upslope::Cell::remove_last_layer()
{
	m_Layers.pop();
}

const cmf::project& cmf::upslope::Cell::project() const
{
return m_project;
}


cmf::water::WaterStorage::ptr cmf::upslope::Cell::add_storage( std::string Name,char storage_role/*='N'*/, bool isopenwater/*=false*/ )
{
	using namespace cmf::water;
	if (storage_role=='C' && m_Canopy) return m_Canopy;
	if (storage_role=='S' && m_Snow)   return m_Snow;
	WaterStorage::ptr ws;
	if (isopenwater)
		ws = cmf::river::OpenWaterStorage::create(project(),get_area());
	else
		ws = WaterStorage::create(project());
	ws->Location=get_position();
	ws->Name=Name;
	if (storage_role=='C')
	{
		m_Canopy = ws;
		ws->Location.z+=get_vegetation().Height;
	}
	else if (storage_role=='S')
	{
		m_Snow = ws;
	}
	m_storages.push_back(ws);
	if (project().debug) 
		std::cout << " Add storage " << ws->Name << " to " << this->to_string() 
		<< " as " << (storage_role=='W' ? "surface water" : storage_role=='C' ? "canopy" : storage_role=='S' ? "snow" : "anything")
		<< std::endl;
	return m_storages.back();
	
}

cmf::water::WaterStorage::ptr cmf::upslope::Cell::get_storage( int index ) const
{
	return m_storages.at(index<0 ? m_storages.size()+index : index);
}


void cmf::upslope::Cell::set_saturated_depth( real depth )
{
	for (int i = 0; i < layer_count() ; ++i)
	{
		get_layer(i)->set_potential(z-depth);
	}
}


cmf::water::WaterStorage::ptr cmf::upslope::Cell::get_snow() const
{
	return m_Snow;
}

cmf::water::WaterStorage::ptr cmf::upslope::Cell::get_canopy() const
{
	return m_Canopy;
}


void cmf::upslope::Cell::remove_storage( cmf::water::WaterStorage& storage )
{
	if (&storage==m_Canopy.get()) m_Canopy.reset();
	if (&storage==m_Snow.get()) m_Snow.reset();
	if (&storage==m_SurfaceWaterStorage.get()) {
		m_SurfaceWater.reset(new cmf::water::flux_node(project()));
		m_SurfaceWater->Name=storage.Name;
		m_SurfaceWater->Location=storage.Location;
		cmf::water::replace_node(m_SurfaceWaterStorage,m_SurfaceWater);
		m_SurfaceWaterStorage.reset();
	}
	for (int i = 0; i < this->storage_count(); ++i)
	{
		if (&storage==this->get_storage(i).get())
		{
			m_storages.erase(m_storages.begin()+i);
			return;
		}
	}
}

cmf::upslope::Topology& cmf::upslope::Cell::get_topology()
{
	if (!m_topo.get()) 
	{
		Topology* new_topo=new Topology(this);
		m_topo.reset(new_topo);
	}
	return *m_topo;
}

cmf::water::flux_node::ptr cmf::upslope::Cell::get_evaporation()
{
	return m_Evaporation;
}

cmf::water::flux_node::ptr cmf::upslope::Cell::get_transpiration()
{
	return m_Transpiration;
}

void cmf::upslope::Cell::surfacewater_as_storage()
{
	if (m_SurfaceWaterStorage==0) 
	{
		m_SurfaceWaterStorage =cmf::river::OpenWaterStorage::from_node(m_SurfaceWater,get_area());
		m_storages.push_back(m_SurfaceWaterStorage);
		m_SurfaceWater.reset();
	}
}

std::string cmf::upslope::Cell::to_string()
{
	std::stringstream sstr;
	sstr << "cell #" << Id << "(" << x << "," << y << "," << z << ")";
	return sstr.str();
}


cmf::water::flux_node::ptr cmf::upslope::Cell::get_surfacewater()
{
	if (m_SurfaceWaterStorage)
		return m_SurfaceWaterStorage;
	else if (m_SurfaceWater)
		return m_SurfaceWater;
	else
		throw std::runtime_error("Neither the surface water flux node, nor the storage exists. Please inform author");
}

void cmf::upslope::Cell::set_rainfall( double rainfall )
{
	get_rainfall()->flux = rainfall;
}
