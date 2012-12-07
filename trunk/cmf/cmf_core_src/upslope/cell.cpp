

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
	  m_SurfaceWater(new cmf::water::DirichletBoundary(_project,_z)),	Id(cell_count++),
	  m_meteo(new cmf::atmosphere::ConstantMeteorology), Tground(-300)
{
	std::stringstream sstr;
	sstr << Id;
	std::string cell_id=sstr.str();
	if (area<=0) {
		throw std::runtime_error("Can't create cell #"+cell_id+". Area<=0.0");
	}
	m_SurfaceWater->Name="Surface water of cell #" + cell_id;
	m_SurfaceWater->position=get_position();
	m_Evaporation.reset(new cmf::water::flux_node(_project,cmf::geometry::point(x,y,z+20)));
	m_Evaporation->Name="Evaporation of cell #" + cell_id;
	m_Transpiration.reset(new cmf::water::flux_node(_project,cmf::geometry::point(x,y,z+20)));
	m_Transpiration->Name="Transpiration of cell #" + cell_id;
	m_rainfall.reset(new cmf::atmosphere::ConstantRainSource(_project,cmf::geometry::point(x,y,z+20),0.0));
	new connections::Rainfall(get_surfacewater(),*this);
	m_aerodynamic_resistance = cmf::atmosphere::aerodynamic_resistance::ptr(new cmf::atmosphere::log_wind_profile(*this));
}

cmf::upslope::Cell::Cell( const Cell& cpy ) 
: m_project(cpy.get_project()),
	x(cpy.x),y(cpy.y),z(cpy.z),m_Area(cpy.get_area())
{

}
real cmf::upslope::Cell::get_saturated_depth() const
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





void cmf::upslope::Cell::add_layer(real lowerboundary,const cmf::upslope::RetentionCurve& r_curve,real saturateddepth/*=-10*/ )
{
	if (get_project().debug)
		std::cout << "Adds a layer to " << this->to_string() << " Name: ";
	SoilLayer::ptr layer(new SoilLayer(*this,lowerboundary,r_curve,saturateddepth));
	if (get_project().debug)
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

cmf::atmosphere::Weather cmf::upslope::Cell::get_weather( cmf::math::Time t ) const
{
	cmf::atmosphere::Weather w = get_meteorology()(t);
	if (Tground>-273) 
		w.Tground = Tground;
	else if (snow_coverage()>0.5) 
		w.Tground = std::min(w.Tground, 0.0);
	return w;
}

void cmf::upslope::Cell::remove_layers()
{
	m_Layers.clear();
}

void cmf::upslope::Cell::remove_last_layer()
{
	m_Layers.pop();
}

const cmf::project& cmf::upslope::Cell::get_project() const
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
		ws = cmf::river::OpenWaterStorage::create(get_project(),get_area());
	else
		ws = WaterStorage::create(get_project());
	ws->position=get_position();
	ws->Name=Name;
	if (storage_role=='C')
	{
		m_Canopy = ws;
		ws->position.z+=vegetation.Height;
	}
	else if (storage_role=='S')
	{
		m_Snow = ws;
	}
	m_storages.push_back(ws);
	if (get_project().debug) 
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
		m_SurfaceWater.reset(new cmf::water::flux_node(get_project()));
		m_SurfaceWater->Name=storage.Name;
		m_SurfaceWater->position=storage.position;
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

std::string cmf::upslope::Cell::to_string() const
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
	cmf::atmosphere::ConstantRainSource* new_source = new cmf::atmosphere::ConstantRainSource(this->get_project(),this->get_position(),rainfall);
	cmf::atmosphere::ConstantRainSource::ptr new_source_ptr(new_source);
	cmf::water::replace_node(m_rainfall,new_source_ptr);
	m_rainfall = new_source_ptr;

}

void cmf::upslope::Cell::set_rain_source( cmf::atmosphere::RainSource::ptr new_source )
{
	std::string new_name = new_source->Name;
	if (m_rainfall) {
		cmf::water::replace_node(m_rainfall,new_source);
	}
	new_source->Name = new_name;
	m_rainfall = new_source;
}

cmf::math::StateVariableList cmf::upslope::Cell::get_states()
{
	cmf::math::StateVariableList q;
	for (int i = 0; i < storage_count() ; ++i)
		q.extend(*get_storage(i));
	for (int i = 0; i < layer_count() ; ++i)
		q.extend(*get_layer(i));
	return q;
}

real cmf::upslope::Cell::heat_flux( cmf::math::Time t) const
{
	cmf::atmosphere::Weather w = get_weather(t);
	real Rn = w.Rn(albedo()); // surface heat flux in MJ/(m2 day)
	
	// Calculate latent heat flux (Ql) MJ/(m2 day)
	double 
		Lv = 2448, // MJ/Mg * 1Mg/m3 = MJ/m3
		lat_T = (*m_Transpiration)(t) / m_Area * Lv, // latent heat flux by transpiration m3  * day-1 * m-2 * MJ/m3 = MJ/(m2 day)
		lat_E = (*m_Evaporation)(t) / m_Area * Lv,   // latent heat flux by evaporation
		Ql = -lat_T - lat_E;
	
	// Calculate sensible heat flux
	double
		c_w = 4.180, // J/(K g) -> MJ/(K * m3)
		c_aV = 1.009 * 1.2 * 1e-3, // Volumetric specific heat of air kJ/(K kg) * kg/m3 * MJ/kJ = MJ/(K m3)
		r_ag=100, r_ac=50;		         // mean air density at constant pressure
	// get resistances
	m_aerodynamic_resistance->get_aerodynamic_resistance(r_ag,r_ac,t);
	//MJ/(m2 * day) = MJ/(K m3) *        K         * m/s * 24*60*60 s/day
	double Qs       =   c_aV    * (w.T - w.Tground)/r_ag * 24*60*60; 
	return Rn + Qs + Ql;
}

real cmf::upslope::Cell::albedo() const
{
	double 
		snow_cov = snow_coverage(),
		surf_cov = surface_water_coverage();

	return vegetation.albedo * (1-snow_cov-surf_cov) 
		+ vegetation.snow_albedo * snow_cov
		+ 0.5 * surf_cov;
}

real cmf::upslope::Cell::snow_coverage() const
{
	if (m_Snow)
		return piecewise_linear(m_Snow->get_volume()/get_area()*1e3,0,4., 0 , 1-surface_water_coverage());
	else
		return 0.0;
}
real cmf::upslope::Cell::surface_amplitude = 0.01;
real cmf::upslope::Cell::surface_water_coverage() const
{
	if (m_SurfaceWaterStorage)
		return piecewise_linear(m_SurfaceWaterStorage->get_depth(),0,surface_amplitude);
	else
		return 0.0;
}


double cmf::upslope::Cell::get_soildepth() const
{
	int lc=layer_count();
	if (lc) 
		return get_layer(lc-1)->get_lower_boundary();
	else
		return 0.0;

}

cmf::math::num_array cmf::upslope::Cell::get_percolation( cmf::math::Time t ) const
{
	cmf::math::num_array res(layer_count());
	if (layer_count()) {
		res[0] = m_Layers[0]->cell.get_surfacewater()->flux_to(*get_layer(0),t);
		for (int i = 1; i < layer_count() ; ++i)
			res[i] = get_layer(i-1)->flux_to(*get_layer(i),t);
	} 
	return res;


}
