

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
#include "../math/real.h"
#include "ManningConnection.h"
#include "../upslope/cell.h"
#include "../upslope/Topology.h"
using namespace cmf::river;
using namespace cmf::water;

real cmf::river::Manning::calc_q( cmf::math::Time t )
{ 
	flux_node::ptr lnode = left_node(), rnode=right_node();
	OpenWaterStorage::ptr ows1=w1.lock(),ows2=w2.lock();
	real d=lnode->position.distanceTo(rnode->position);
	const IChannel *cast = dynamic_cast<const IChannel *>(&flux_geometry);
	if (cast) 
		d=cast->get_length();
	real 
		// Gradient of the reach
		slope = get_slope(lnode,rnode,d),
		abs_slope=fabs(slope);
	// No slope, no flux
	if (abs_slope<=0) return 0.0;
	// Get the source of the flow
	OpenWaterStorage::ptr source=slope > 0 ? ows1 : ows2;
	if (source==0) return 0; // Never generate flow from a flux node
	// Wetted crossectional area, use mean volume of the water storages if both sides are water storages
	real h;
	if (is_diffusive_wave)
		// Flow height between elements is the mean of the flow height, but exceeds never the flow height of the source
		h=minimum(source->get_depth(),ows2 ? mean(ows1->get_depth(),ows2->get_depth()) : ows1->get_depth());
	else 
		h = source->get_depth();
	if (h<=0) return 0.0;
	// Absolute flux in m3/s
	real qManning = flux_geometry.q(h,abs_slope);
	// Return flux with correct sign in m3/day
	return prevent_negative_volume(qManning * sign(slope) * (24*60*60));
}
Manning* create_manning( cmf::river::OpenWaterStorage::ptr left,cmf::water::flux_node::ptr right, const cmf::river::IChannel& reachtype,bool diffusive_wave )
{
	if (diffusive_wave) {
		return new Manning_Diffusive(left,right,reachtype);
	} else {
		return new Manning_Kinematic(left,right,reachtype);
	}
}

void cmf::river::Manning::connect_cells( cmf::upslope::Cell& c1,cmf::upslope::Cell& c2,bool diffusive)
{
	static bool DeprecatedWarning = true;
	if (DeprecatedWarning) {
		std::cerr << "Depreciation Warning: connect_cells_with_flux(cmf.Manning_Kinematic)" << std::endl << std::endl
				  << "Manning_Kinematic and Manning_Diffusive are designed for channel runoff. " << std::endl
				  << "Automatic connection of surface water storages using 'connect_cells_with_flux' " << std::endl
				  << "is depreciated and will be removed in the future. Use KinematicSurfaceRunoff or" << std::endl 
				  << "DiffusiveSurfaceRunoff instead." << std::endl;
		DeprecatedWarning = false;
	}
	real w=c1.get_topology().flowwidth(c2);
	real d=c1.get_position().distanceTo(c2.get_position());
	if (w<=0) return;
	RectangularReach r_type(d,w);
	OpenWaterStorage::ptr sows1= OpenWaterStorage::cast(c1.get_surfacewater());
	OpenWaterStorage::ptr sows2= OpenWaterStorage::cast(c2.get_surfacewater());
	if (sows1)
		create_manning(sows1,c2.get_surfacewater(),r_type,diffusive);
	else if (sows2)
		create_manning(sows2,c1.get_surfacewater(),r_type,diffusive);
	else
		throw std::runtime_error("Surface water of " + c1.to_string() + " and " + c2.to_string() + " were not connected with Manning's equation. Missing storages.");
}


const cmf::upslope::CellConnector
	cmf::river::Manning_Kinematic::cell_connector
				= cmf::upslope::CellConnector(&connect_cells),
	cmf::river::Manning_Diffusive::cell_connector
				= cmf::upslope::CellConnector(&connect_cells);


real cmf::river::Manning_Kinematic::get_slope(cmf::water::flux_node::ptr lnode, cmf::water::flux_node::ptr rnode, real d)
{
	return  (lnode->position.z - rnode->position.z)/d;	
}

real cmf::river::Manning_Diffusive::get_slope(cmf::water::flux_node::ptr lnode, cmf::water::flux_node::ptr rnode, real d)
{
	real s = (lnode->get_potential()-rnode->get_potential())/d;
	if (this->linear_slope_width) {
		real
			// Only a shortcut for faster writing
			s0 = this->linear_slope_width,
			// Weight of linear part
			w_lin = exp(-square((s/s0))),
			// linear part using the slope at s0/4 
			s_lin = s/(2.*sqrt(s0/4)),
			// Weighted sum of sqrt(s) and a*s
			s_sqrt_w = w_lin * s_lin + (1-w_lin) * sqrt(abs(s));
		// Remove sqrt
		return sign(s) * square(s_sqrt_w);
	} else {
		return s;
	}
}
