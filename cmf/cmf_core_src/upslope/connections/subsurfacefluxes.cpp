

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
#include "subsurfacefluxes.h"
#include "../../math/real.h"
#include "../../geometry/geometry.h"
using namespace cmf::water;
using namespace cmf::upslope;
using namespace cmf::geometry;
/************************************************************************/
/* Precolation and 3D fluxes                                            */
/************************************************************************/
// Richards eq.
const CellConnector connections::Richards_lateral::cell_connector=CellConnector(connections::Richards_lateral::connect_cells);
void connections::Richards_lateral::connect_cells( Cell & cell1,Cell & cell2,ptrdiff_t start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (size_t i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (size_t j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
				real ca=cell1.get_layer(i)->get_flow_crosssection(*cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i)->position.distanceTo(cell2.get_layer(j)->position);
					new Richards_lateral(cell1.get_layer(i),cell2.get_layer(j),w,d);
				}	}	}
	}
}
bool baseflow=false;
void connections::Richards_lateral::usebaseflow(bool use) {
	baseflow = use;
}
real connections::Richards_lateral::calc_q( cmf::math::Time t )
{
	SoilLayer::ptr 
		l1=sw1.lock(),
		l2=sw2.lock();
	conductable::ptr C2=c2.lock();
	// Richards flux
	point direction=left_node()->position - right_node()->position;
	real
		Psi_t1=l1->get_potential(),
		Psi_t2=right_node()->get_potential(),
		gradient=(Psi_t1-Psi_t2)/distance,
		K1=l1->get_K(direction),K2=0.0,
		K=0.0, Ksat =.0;
	if (l2) {
		K2=l2->get_K(direction);
		K= geo_mean(K1,K2);
		Ksat = geo_mean(l1->get_Ksat(),l2->get_Ksat());
	} else if (C2){
		K2=C2->get_K(direction);
		K= geo_mean(K1,K2);
		Ksat = geo_mean(l1->get_Ksat(),C2->get_K(direction));
	} else if (wet_right_node) {
		K2 = Psi_t1>Psi_t2 ? K1 : l1->get_Ksat();
		K = geo_mean(K1,K2);
		Ksat = l1->get_Ksat();
	} else {
		K=K1;
		Ksat = l1->get_Ksat();
	}
	real r_flow = K * gradient * flow_width * flow_thickness;
	if (baseflow) {
		double ft = flow_thickness + l1->get_matrix_potential();
		if (l2) ft = std::max(flow_thickness + l2->get_matrix_potential(),ft);
		ft = minmax(ft,0,flow_thickness);
		real d_flow = Ksat * gradient * flow_width * ft;
		r_flow *= (1-ft/flow_thickness);
		r_flow += d_flow;
	}

	// If flow from left to right
	return prevent_negative_volume(r_flow); 


}
/************************************************************************/
/* Lateral fluxes                                                       */
/************************************************************************/
// Darcy connection
real get_flow_thick(SoilLayer::ptr l)
{
	real 
		thick=l->get_thickness(),
		w_eff= l->get_soil().Wetness_eff(l->get_wetness(),2.5),
		mp = l->get_matrix_potential();
		
	return minmax(thick + mp /* w_eff*/ ,0.0,thick);
}
real connections::Darcy::calc_q( cmf::math::Time t )	
{
	SoilLayer::ptr 
		l1=sw1.lock(),
		l2=sw2.lock();
	// Calculate the gradient
	real
		Psi_t1 = left_node()->get_potential(),
		Psi_t2 = right_node()->get_potential(),
		gradient=(Psi_t1-Psi_t2)/distance;


	//SoilLayer::ptr source =  (gradient < 0) && l2 ? l2 : l1;
	real
		// Transmissivity
		flow_thick1 = get_flow_thick(l1), 
		flow_thick2 = l2 ? get_flow_thick(l2) : l1->get_thickness();
	if ((flow_thick1 + flow_thick2)<=0) return 0.0;
	real
		T1 = l1->get_Ksat() *  flow_thick1,
		T2 = l2 
			? l2->get_Ksat() * flow_thick2 
			: l1->get_Ksat() * flow_thick2,
		T = mean(T1,T2);

	
	return prevent_negative_volume(T*gradient*flow_width);

}

void connections::Darcy::connect_cells( Cell & cell1,Cell & cell2,ptrdiff_t start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (size_t i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (size_t j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
				real ca=cell1.get_layer(i)->get_flow_crosssection(*cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i)->position.distanceTo(cell2.get_layer(j)->position);
					new Darcy(cell1.get_layer(i),cell2.get_layer(j),w,d);
				}	}	}
	}
}
const CellConnector connections::Darcy::cell_connector=CellConnector(connections::Darcy::connect_cells);

// Topographic gradient connection
const CellConnector connections::TopographicGradientDarcy::cell_connector=CellConnector(connections::TopographicGradientDarcy::connect_cells);
real connections::TopographicGradientDarcy::calc_q( cmf::math::Time t )
{
	// Darcy flux
	SoilLayer::ptr 
		l1=sw1.lock(),
		l2=sw2.lock();

	real
		flow_thick1=get_flow_thick(l1), // minimum(l1->get_thickness(),l1->get_lower_boundary()-l1->cell.get_saturated_depth()),
		flow_thick2=l2 ? get_flow_thick(l2) : flow_thick1; // minimum(l2->get_thickness(),l2->get_lower_boundary()-l2->cell.get_saturated_depth()) : flow_thick1;
	if (flow_thick1+flow_thick2<=0) return 0;
	real
		flow_thick=mean(flow_thick1,flow_thick2),
		// Topographic gradient
		Psi_t1=l1->cell.z,
		Psi_t2=l2 ? l2->cell.z : right_node()->get_potential(),
		gradient=(Psi_t1-Psi_t2)/distance,
		// Transmissivity
		T1 = l1->get_Ksat() *  (l1->get_lower_boundary() - (l1->get_lower_boundary()-flow_thick1)),
		T2 = l2 ? l2->get_Ksat() * (l2->get_lower_boundary() - (l2->get_lower_boundary()-flow_thick2)) : T1,
		T = gradient>0 ? T1 : T2;
	return prevent_negative_volume(T*gradient*flow_width);

}

void connections::TopographicGradientDarcy::connect_cells( Cell & cell1,Cell & cell2,ptrdiff_t start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (size_t i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (size_t j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
				real ca=cell1.get_layer(i)->get_flow_crosssection(*cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i)->position.distanceTo(cell2.get_layer(j)->position);
				new TopographicGradientDarcy(cell1.get_layer(i),cell2.get_layer(j),w,d);
			}	}	}
	}

}

real connections::DarcyKinematic::calc_q( cmf::math::Time t )
{
	// Darcy flux
	SoilLayer::ptr
		l1=sw1.lock(),
		l2=sw2.lock(),
		source = (l2 && l1->cell.z < l2->cell.z) ? l2 : l1;
	real
		K = source->get_K(),
		// Topographic gradient
		Psi_t1=l1->cell.z,
		Psi_t2=l2 ? l2->cell.z : right_node()->get_potential(),
		gradient=(Psi_t1-Psi_t2)/distance,
		flow=K*source->get_thickness()*flow_width*gradient;
	return prevent_negative_volume(flow);

}

void connections::DarcyKinematic::connect_cells( Cell & cell1,Cell & cell2,ptrdiff_t start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (size_t i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (size_t j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
				real ca=cell1.get_layer(i)->get_flow_crosssection(*cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i)->position.distanceTo(cell2.get_layer(j)->position);
				new DarcyKinematic(cell1.get_layer(i),cell2.get_layer(j),w,d);
			}	}	}
	}

}
const CellConnector connections::DarcyKinematic::cell_connector(connections::DarcyKinematic::connect_cells);



void connections::lateral_sub_surface_flux::NewNodes()
{
	SoilLayer::ptr l1,l2;
	
	l1=SoilLayer::cast(left_node());
	l2=SoilLayer::cast(right_node());
	if (!l1) 
		throw std::runtime_error("Can't create " + this->type + ", left node {" + left_node()->Name + "} is not a soil layer!" );
	sw1 = l1;
	sw2 = l2;
	c2=conductable::cast(right_node());

}

connections::lateral_sub_surface_flux::lateral_sub_surface_flux( SoilLayer::ptr left,cmf::water::flux_node::ptr right, std::string Name,real FlowWidth,real Distance/*=0*/ ) : cmf::water::flux_connection(left,right,Name), flow_width(FlowWidth), distance(Distance)
{
	NewNodes();
	if (Distance<=0)
		distance=left->position.distanceTo(right->position);
}

cmf::upslope::connections::TOPModelFlow::TOPModelFlow( cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right,
			real _T0,real _m,real _flowwidth, real _distance ) 
	: cmf::water::flux_connection(left,right,"TOPModelFlow"),flow_width(_flowwidth),distance(_distance),m(_m),T0(_T0)
{
	NewNodes();
}

real cmf::upslope::connections::TOPModelFlow::calc_q( cmf::math::Time t )
{
	cmf::upslope::SoilLayer::ptr layer=this->sw1.lock();
	cmf::water::flux_node::ptr out=this->right_node();
	real d = this->distance<=0.0 ? layer->position.distanceTo(out->position) : this->distance;
	real slope = (layer->position.z - out->position.z) / d;
	real Di = (layer->get_capacity() - layer->get_volume())/layer->cell.get_area();
	real T = T0 * exp(-Di/m);
	return flow_width * T * slope;
}
