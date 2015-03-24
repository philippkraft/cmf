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
#include "infiltration.h"
/************************************************************************/
/* Infiltration                                                         */
/************************************************************************/

real cmf::upslope::connections::MatrixInfiltration::calc_q( cmf::math::Time t )
{
	cmf::upslope::SoilLayer::ptr soilwater=m_soilwater.lock();
	cmf::river::OpenWaterStorage::ptr surfacewaterstorage=m_surfacewaterstorage.lock();
	cmf::water::flux_node::ptr surfacewater=left_node();
	cmf::upslope::Cell& cell=soilwater->cell;
	real
		// get_potential of the surface water				
		Pot_surf = surfacewater->get_potential(),
		// get_potential of the soil water
		Pot_soil = soilwater->get_potential(),
		// Gradient surface->soil
		gradient = (Pot_surf-Pot_soil)/(0.5*soilwater->get_thickness()),								
		// Conductivity in m/day
		K = geo_mean(soilwater->get_K(),soilwater->get_Ksat()),										        
		maxInfiltration = gradient * K * cell.get_area();

	real inflow=0;
	if (surfacewaterstorage) // If the surface water is a storage
	{
		// get the state dependend outflow
		inflow = maxInfiltration * cell.surface_water_coverage();		
	}
	else // inflow is the sum of the inflows to surface water
		inflow = surfacewater->waterbalance(t,this);							

	return minimum(maxInfiltration,inflow);
}

real cmf::upslope::connections::GreenAmptInfiltration::calc_q( cmf::math::Time t )
{
	cmf::upslope::SoilLayer::ptr soilwater=m_soilwater.lock();
	cmf::upslope::SurfaceWater::ptr surfacewaterstorage=m_surfacewaterstorage.lock();
	cmf::water::flux_node::ptr surfacewater=left_node();

	cmf::upslope::Cell& cell=soilwater->cell;
	real
		Vsurface = surfacewaterstorage ? surfacewaterstorage->get_volume() : 0.0,
		Vsoil = soilwater->get_volume(),
		F = (Vsoil + Vsurface) / cell.get_area(),								// Water content in m 
		MP = fabs(soilwater->get_matrix_potential()),							// Matrix potential in m
		sat_deficit = soilwater->get_porosity() - soilwater->get_theta(),       // Saturation deficit of soil layer [-]
		K = soilwater->get_Ksat(),										        // Conductivity in m/day
		maxInfiltration = K * (MP * sat_deficit)/F ;       // Green-Ampt formula
	real inflow=0;
	if (sat_deficit<0) { // Ponding water
		return -K * cell.get_area() * MP/F;
	}
	else if (surfacewaterstorage) // infiltration from a surface water storage
	{
		// get the state dependend outflow
		return maxInfiltration * surfacewaterstorage->wet_area();		
	}
	else // inflow is the sum of the inflows to surface water
		return minimum(maxInfiltration * cell.get_area(), surfacewater->waterbalance(t,this));
	return 0.0;

}


real cmf::upslope::connections::SimpleInfiltration::calc_q( cmf::math::Time t )
{
	using namespace cmf::water;
	using namespace cmf::upslope;
	SoilLayer::ptr soilwater=m_soilwater.lock();
	SurfaceWater::ptr surfacewaterstorage=m_surfacewaterstorage.lock();
	flux_node::ptr surfacewater=left_node();
	
	// Sum the incoming fluxes to the surfacewater
	real influxes = 0.0;
	connection_list swcons = surfacewater->get_connections();
	// Loop through each connection of surface water
	for(connection_list::const_iterator it = swcons.begin();it!=swcons.end();++it) {
		flux_connection::ptr con = *it;
		// Include only non-waterbalance connections and not this
		if (con->type!="waterbalance connection" && con.get()!=this) {
			// Sum only incoming fluxes
			influxes += std::max(0.0, con->q(*surfacewater,t));
		}
	}
	// Potential infiltration (if soil layer is not full)
	real potinf = std::min(soilwater->get_Ksat() * soilwater->cell.get_area(), influxes);
	// Get wetness of target
	real W = soilwater->get_wetness();
	// Do piecewise linear function
	real f_full = 1-boltzmann(W,W0,(1-W0)*0.2);
	return potinf * f_full;
}

cmf::upslope::connections::SimpleInfiltration::SimpleInfiltration( cmf::upslope::SoilLayer::ptr soilwater,cmf::water::flux_node::ptr surfacewater,real Wfull/*=0.9*/ ) : flux_connection(surfacewater,soilwater,"simple infiltration"),W0(Wfull)
{
	NewNodes();
}
