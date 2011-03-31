

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
void connections::Richards_lateral::connect_cells( Cell & cell1,Cell & cell2,int start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
				real ca=cell1.get_layer(i)->get_flow_crosssection(*cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i)->position.distanceTo(cell2.get_layer(j)->position);
					new Richards_lateral(cell1.get_layer(i),cell2.get_layer(j),w,d);
				}	}	}
	}
}
bool connections::Richards_lateral::baseflow=false;
real connections::Richards_lateral::calc_q( cmf::math::Time t )
{
	SoilLayer::ptr 
		l1=sw1.lock(),
		l2=sw2.lock();
	conductable::ptr C2=c2.lock();
	// Richards flux
	real
		Psi_t1=l1->get_potential(),
		Psi_t2=right_node()->get_potential(),
		gradient=(Psi_t1-Psi_t2)/distance,
		//K=gradient<0 && l2 ? l2->K() : l1->K();      
		K=0.0, Ksat =.0;
	point direction=left_node()->position - right_node()->position;
	if (l2) {
		K= geo_mean(l1->get_K(direction),l2->get_K(direction));
		Ksat = geo_mean(l1->get_Ksat(),l2->get_Ksat())
	}
	if (C2){
		K= geo_mean(l1->get_K(direction),C2->get_K(direction));
		Ksat = geo_mean(l1->get_Ksat(direction),C2->get_K(direction))
	}
	else {
		K=l1->get_K(direction);
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

	
	return T*gradient*flow_width;

}

void connections::Darcy::connect_cells( Cell & cell1,Cell & cell2,int start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
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

void connections::TopographicGradientDarcy::connect_cells( Cell & cell1,Cell & cell2,int start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
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

void connections::DarcyKinematic::connect_cells( Cell & cell1,Cell & cell2,int start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
				real ca=cell1.get_layer(i)->get_flow_crosssection(*cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i)->position.distanceTo(cell2.get_layer(j)->position);
				new DarcyKinematic(cell1.get_layer(i),cell2.get_layer(j),w,d);
			}	}	}
	}

}
const CellConnector connections::DarcyKinematic::cell_connector(connections::DarcyKinematic::connect_cells);




real connections::OHDISflow::calc_q( cmf::math::Time t )
{
	SoilLayer::ptr 
		l1=sw1.lock(),
		l2=sw2.lock();
	// The resulting flux
	real q=0;

	// The equivalent of capillary pores as length
	real dm1=l1->get_thickness()*l1->get_soil().Wetness_pF(1)*l1->get_soil().Porosity(l1->get_upper_boundary());
	real dm2=l2 ? l2->get_thickness()*l2->get_soil().Wetness_pF(1)*l2->get_soil().Porosity(l2->get_upper_boundary()) : dm1;
	// the equivalent filling depth
	real h1=l1->get_thickness()*l1->get_theta();
	real h2=l2 ? l2->get_thickness()*l2->get_theta() : h1;

	// the unsaturated conductivity
	real k1=l1->get_K();
	real k2=l2 ? l2->get_K() : k1;

	// topographic slope
	real z1=l1->cell.z;
	real z2=l2 ? l2->cell.z : right_node()->get_potential();
	real slope=(z1-z2)/distance;

	// Values a cell boundary
	real h_avg=mean(h1,h2);
	real dm_avg=mean(dm1,dm2);
	// soil depth
	real t1=l1->get_thickness();
	real t2=l2 ? l2->get_thickness() : t1;
	real da_avg=mean(t1,t2);
	// capillary lateral flux
	q+=dm_avg*geo_mean(k1,k2)*slope;

	// macropore lateral flux
	if (h_avg>dm_avg)
		q+=(h_avg-dm_avg)*l1->get_Ksat()*slope;
	// overbound (Manning) flux
	if (h_avg>da_avg)
		q+=sign(slope) * sqrt(fabs(slope))/(0.035)*pow(h_avg-da_avg,2./3.);
	return q*flow_width;


}
const CellConnector connections::OHDISflow::cell_connector(connections::OHDISflow::connect_cells);
void connections::OHDISflow::connect_cells( Cell & cell1,Cell & cell2,int start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
				real ca=cell1.get_layer(i)->get_flow_crosssection(*cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i)->position.distanceTo(cell2.get_layer(j)->position);
					new OHDISflow(cell1.get_layer(i),cell2.get_layer(j),w,d);
				}	}	}
	}

}


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