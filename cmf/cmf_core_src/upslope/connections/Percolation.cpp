

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
#include "Percolation.h"

using namespace cmf::water;
void cmf::upslope::connections::Richards::use_for_cell( cmf::upslope::Cell & cell,bool no_override/*=true*/ )
{
	for (int i = 0; i < cell.layer_count()-1 ; ++i)
	{
		cmf::upslope::SoilLayer::ptr l_upper=cell.get_layer(i), l_lower=cell.get_layer(i+1);
		if (!(no_override && l_upper->get_connection(*l_lower)))
			new Richards(l_upper,l_lower);
	}
}

real cmf::upslope::connections::Richards::calc_q( cmf::math::Time t ) 
{
	// Richards flux
	cmf::upslope::SoilLayer::ptr 
		l1=sw1.lock(),
		l2=sw2.lock();

	real
		distance = l1->position.z - right_node()->position.z,		
		Psi_t1=l1->get_potential(),
		Psi_t2=right_node()->get_potential(),
		gradient=(Psi_t1-Psi_t2)/distance,
		K=0.0;
	if (distance == 0.0) distance = l1->get_thickness();
	//K=gradient<0 && l2 ? l2->K() : l1->K();      
	if (l2)
		K = geo_mean(l1->get_K(),l2->get_K());
	else if (right_node()->is_empty() || right_node()->get_potential() < l1->get_gravitational_potential())
		K = l1->get_K();
	else
		K = geo_mean(l1->get_K(),l1->get_Ksat());
	//  	if (fabs(K*gradient)>l1->get_Ksat()) K=l1->get_Ksat()/fabs(gradient);
	//  	if (l2 && fabs(K*gradient)>l2->get_Ksat()) K=l2->get_Ksat()/fabs(gradient);
	real r_flow=K*gradient*l1->cell.get_area();
	return prevent_negative_volume(r_flow);
}
void cmf::upslope::connections::SimplRichards::use_for_cell( cmf::upslope::Cell & cell,bool no_override/*=true*/ )
{
	for (int i = 0; i < cell.layer_count()-1 ; ++i)
	{
		cmf::upslope::SoilLayer::ptr l_upper=cell.get_layer(i), l_lower=cell.get_layer(i+1);
		if (!(no_override && l_upper->get_connection(*l_lower)))
			new SimplRichards(l_upper,l_lower);
	}
}

real cmf::upslope::connections::SimplRichards::calc_q( cmf::math::Time t )
{
	// Richards flux
	cmf::upslope::SoilLayer::ptr
		l1=sw1.lock(),
		l2=sw2.lock();

	real
		K = l1->get_K();
	if (l2)
		K *= 1 - l2->get_wetness();
	real r_flow=K * l1->cell.get_area();
	return prevent_negative_volume(r_flow);
}


// SWAT Percolation
real cmf::upslope::connections::SWATPercolation::calc_q( cmf::math::Time t )
{
	cmf::upslope::SoilLayer::ptr 
		l1=sw1.lock(),
		l2=sw2.lock();

	real
		capacity=l1->get_capacity(),
		fc_waterhead=pressure_to_waterhead(-33000), // Field capacity suction in m
		fc=l1->get_soil().Wetness(fc_waterhead)*capacity, // Water storage at field capacity
		sw_excess=l1->get_volume()-fc; // drainable water volume
	if (sw_excess<=0) return 0.0;
	real
		Ksat=l1->get_Ksat(),
		// Percolation travel time in days = drainable volume[m3]/(get_area[m2] * Conductivity[m/day])
		tt_perc=sw_excess/(l1->cell.get_area() * Ksat),
		w_perc=sw_excess*(1-exp(-1/tt_perc));
	return w_perc*maximum(0,0.9-l2->get_wetness());


}

void cmf::upslope::connections::SWATPercolation::use_for_cell( cmf::upslope::Cell & cell,bool no_override )
{
	for (int i = 0; i < cell.layer_count()-1 ; ++i)
	{
		cmf::upslope::SoilLayer::ptr l_upper=cell.get_layer(i),  l_lower=cell.get_layer(i+1);
		if (!(no_override && l_upper->get_connection(*l_lower)))
			new SWATPercolation(l_upper,l_lower);
	}

}

