

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
#include "ManningConnection.h"
#include "../upslope/cell.h"
#include "../upslope/Topology.h"
real cmf::river::Manning::calc_q( cmf::math::Time t )
{
	OpenWaterStorage::ptr ows1=w1.lock(),ows2=w2.lock();
	real 
		// Distance between source and target
		d=flux_geometry.get_length(),
		// Gradient of the reach
		slope = is_diffusive_wave ?  
		/*diffusive slope*/	        (left_node()->get_potential()-right_node()->get_potential())/d
		/*kinematic slope*/	      : (left_node()->Location.z - right_node()->Location.z)/d,
		
		abs_slope=fabs(slope);
	// No slope, no flux
	if (abs_slope<=0) return 0.0;
	// Get the source of the flow
	OpenWaterStorage::ptr source=slope > 0 ? ows1 : ows2;
	if (source==0) return 0; // Never generate flow from a flux node
	// Wetted crossectional area, use mean volume of the water storages if both sides are water storages
	real
		// Flow height between elements is the mean of the flow height, but exceeds never the flow height of the source
		h=minimum(source->get_depth(),ows2 ? mean(ows1->get_depth(),ows2->get_depth()) : ows1->get_depth());
	if (h<=1e-6) return 0;
	real
		// Depth of the reach
		A=flux_geometry.get_flux_crossection(h),
		// Wetted perimeter of the reach
		P=flux_geometry.get_wetted_perimeter(h);
	if (A<=0 || P<=0) return 0.0;
	// Absolute flux in m3/s
	real qManning = A * pow((A/P),0.6666667)*sqrt(abs_slope)/flux_geometry.get_nManning();
	// Return flux with correct sign in m3/day
	return qManning * sign(slope) * (24*60*60);
}

void cmf::river::Manning::connect_cells( cmf::upslope::Cell& c1,cmf::upslope::Cell& c2,bool diffusive)
{
	real w=c1.get_topology().flowwidth(c2);
	real d=c1.get_distance_to(c2);
	if (w<=0) return;
	RectangularReach r_type(d,w);
	
	cmf::river::OpenWaterStorage::ptr sows1= OpenWaterStorage::cast(c1.get_surfacewater());
	cmf::river::OpenWaterStorage::ptr sows2= OpenWaterStorage::cast(c2.get_surfacewater());
	if (sows1)
		new Manning(sows1,c2.get_surfacewater(),r_type,diffusive);
	else if (sows2)
		new Manning(sows2,c1.get_surfacewater(),r_type,diffusive);
	else
		throw std::runtime_error("Surface water of " + c1.to_string() + " and " + c2.to_string() + " were not connected with Manning's equation. Missing storages.");
}

const cmf::upslope::CellConnector
	cmf::river::Manning_Kinematic::cell_connector
				=cmf::upslope::CellConnector(&connect_cells),
	cmf::river::Manning_Diffusive::cell_connector
				=cmf::upslope::CellConnector(&connect_cells);
