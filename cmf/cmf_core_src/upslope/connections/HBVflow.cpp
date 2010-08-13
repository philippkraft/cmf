

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
#include "HBVflow.h"

real cmf::upslope::connections::HBVpercolation::calc_q(cmf::math::Time t)
{
	cmf::upslope::SoilLayer::ptr 
		layer = sw1.lock(),
		lowerlayer = sw2.lock();
	HBVparameters* params = HBVparameters::cast(layer->get_soil());
	if (!params) throw std::runtime_error("Can't create a lateral HBV flow if left node has not HBVparameter as retention curve");
	real
		Vsource=layer->get_thickness() * layer->cell.get_area();
	if (layer->Position)	// Percolation from upper to lower box
	{
		// Percolation is the constant percolation if enough water is present
		real perc = layer->get_state() > 0 ? 
			minimum(params->perc * 0.001 * layer->cell.get_area(),layer->get_state()) 
			: 0.0;
		real max_perc = perc;
		if (lowerlayer)
		{
			max_perc = params->lowlim * Vsource  - lowerlayer->get_state();
		}
		return minimum(perc,max_perc);
	}
	else // percolation from unsat to upper box
	{
		real inflow = layer->cell.get_surfacewater()->flux_to(*layer,t);
		real FC = params->fc * Vsource;
		real fPerc = pow(layer->get_state()/FC,params->beta);
	  return inflow * fPerc;
	}

}

void cmf::upslope::connections::HBVpercolation::NewNodes()
{
	sw1=cmf::upslope::SoilLayer::cast(left_node());
// 	HBVparameters* Params = HBVparameters::cast(sw1.lock()->get_soil());
// 	if (!Params) throw std::runtime_error("Can't create a lateral HBV flow if left node has not HBVparameter as retention curve");
// 	params.reset(Params->copy());
// 
	sw2=cmf::upslope::SoilLayer::cast(right_node());
}

real cmf::upslope::connections::HBVlateral::calc_q(cmf::math::Time t)
{
	cmf::upslope::SoilLayer::ptr 
		layer = sw1.lock();
	if (!layer) throw std::runtime_error("Layer is not existent!");
	HBVparameters* params = HBVparameters::cast(layer->get_soil());
	if (!params) throw std::runtime_error("Can't create a lateral HBV flow if left node has not HBVparameter as retention curve");
	real
		Vsource=layer->get_thickness() * layer->cell.get_area();
	if (layer->Position==0)
		return 0.0;
	else if (layer->Position == 1)
	{
		real Q0 = params->k0 * maximum(layer->get_state() - Vsource * params->uplim , 0) * flow_width / distance;
		real Q1 = params->k1 * layer->get_state() * flow_width / distance;
		return Q0 + Q1;
	}
	else
		return params->k2 * layer->get_state() * flow_width / distance;
	
}

void cmf::upslope::connections::HBVlateral::connect_cells( cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
				real ca=cell1.get_layer(i)->get_flow_crosssection(*cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i)->position.distanceTo(cell2.get_layer(j)->position);
					cmf::upslope::SoilLayer::ptr l_up = cmf::upslope::SoilLayer::cast(cmf::water::get_higher_node(cell1.get_layer(i),cell2.get_layer(j)));
					cmf::upslope::SoilLayer::ptr l_low = cmf::upslope::SoilLayer::cast(cmf::water::get_lower_node(cell1.get_layer(i),cell2.get_layer(j)));
					new HBVlateral(l_up,l_low,w,d);
				}	}	}
	}
}

void cmf::upslope::connections::HBVlateral::NewNodes()
{
	sw1=cmf::upslope::SoilLayer::cast(left_node());
// 	HBVparameters* Params = HBVparameters::cast(sw1.lock()->get_soil());
// 	if (!Params) 
// 		throw std::runtime_error("Can't create a lateral HBV flow if left node has not HBVparameter as retention curve");
// 	else 
// 		params.reset(Params->copy());
}
const cmf::upslope::CellConnector cmf::upslope::connections::HBVlateral::cell_connector=CellConnector(cmf::upslope::connections::HBVlateral::connect_cells);

void cmf::upslope::connections::HBVinstall( HBVparameters parameters,cmf::upslope::Cell& cell )
{
	cell.remove_layers();
	cell.add_layer(1.0,parameters);
	cell.add_layer(2.0,parameters);
	cell.add_layer(3.0,parameters);
	cmf::upslope::SoilLayer::ptr 
		l_unsat=cell.get_layer(0),
		l_up=cell.get_layer(1),
		l_low=cell.get_layer(2);
	new HBVpercolation(l_unsat,l_up);
	new HBVpercolation(l_up,l_low);
}