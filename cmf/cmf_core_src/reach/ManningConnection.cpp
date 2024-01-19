

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
#include "../math/real.h"
#include "../project.h"
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
		sqrt_slope = get_sqrt_slope(t, lnode, rnode, d),
		abs_slope=square(sqrt_slope);
	// No slope, no flux
	if (abs_slope<=0) return 0.0;
	// Get the source of the flow
	OpenWaterStorage::ptr source=sqrt_slope > 0 ? ows1 : ows2;
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
	return prevent_negative_volume(qManning * sign(sqrt_slope) * (24*60*60));
}
void cmf::river::Manning::NewNodes()
{
	if (
		!is_diffusive_wave &&
		left_node()->position.z <= right_node()->position.z
		)
	{
		throw std::runtime_error(this->to_string() + ": Tried to create a kinematic wave connection that is not downslope");
	}
	w1 = cmf::river::OpenWaterStorage::cast(left_node());
	w2 = cmf::river::OpenWaterStorage::cast(right_node());
}
Manning* create_manning( cmf::river::OpenWaterStorage::ptr left,cmf::water::flux_node::ptr right, const cmf::river::IChannel& reachtype,bool diffusive_wave )
{
	if (diffusive_wave) {
		return new Manning_Diffusive(left,right,reachtype);
	} else {
		return new Manning_Kinematic(left,right,reachtype);
	}
}


real cmf::river::Manning_Kinematic::get_sqrt_slope(cmf::math::Time t, cmf::water::flux_node::ptr lnode,
                                                   cmf::water::flux_node::ptr rnode, real d)
{
	real grad = (lnode->position.z - rnode->position.z)/d;
	return sign(grad) * std::sqrt(grad);
}

real cmf::river::Manning_Diffusive::get_sqrt_slope(cmf::math::Time t, cmf::water::flux_node::ptr lnode,
                                                   cmf::water::flux_node::ptr rnode, real d)
{
/*
cd demo
from pylab import ion, plot
ion()
import numpy as np
import cmf
p=cmf.project()
r1 = p.NewReach(0,0,0, cmf.TriangularReach(100), True)
r2 = p.NewReach(100,0,0, cmf.TriangularReach(100), True)
r2.set_downstream(r1)
r1.depth = 1.0
def q(s):
    r2.depth = r1.depth + s
    return r2.flux_to(r1, cmf.Time())
qq = np.vectorize(q, 'f')
s = np.linspace(-0.1, 0.1, 1001)
plot(s, qq(s))
*/
    // Only a shortcut for faster writing
    real & dssp = cmf::options.diffusive_slope_singularity_protection;
    // get the current gradient
	real grad = (lnode->get_potential(t) - rnode->get_potential(t)) / d;
    real s_sqrt = sign(grad) * std::sqrt(std::abs(grad));
	if (dssp > 0.0) {
        // Weight of linear part
        real w_lin = exp(-square((grad / dssp)));
        // linear part using the slope at dssp/4
        real s_lin = grad / (2. * std::sqrt(dssp / 4));
        // Weighted sum of sqrt(grad) and a*grad
        real s_sqrt_w = w_lin * s_lin + (1-w_lin) * s_sqrt;
		// Remove sqrt
		return s_sqrt_w;
	} else {
		return s_sqrt;
	}
}
