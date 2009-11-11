#include "subsurfacefluxes.h"
#include "../../math/real.h"

/************************************************************************/
/* Precolation and 3D fluxes                                            */
/************************************************************************/
// Richards eq.
const cmf::upslope::CellConnector cmf::upslope::connections::Richards_lateral::cell_connector=CellConnector(cmf::upslope::connections::Richards_lateral::connect_cells);
void cmf::upslope::connections::Richards_lateral::connect_cells( cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
				real ca=cell1.get_layer(i)->get_flow_crosssection(*cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i)->get_distance_to(*cell2.get_layer(j));
					new Richards_lateral(cell1.get_layer(i),cell2.get_layer(j),w,d);
				}	}	}
	}
}
real cmf::upslope::connections::Richards_lateral::calc_q( cmf::math::Time t )
{
	cmf::upslope::SoilLayer::ptr 
		l1=sw1.lock(),
		l2=sw2.lock();
	// Richards flux
	real
		Psi_t1=l1->get_potential(),
		Psi_t2=right_node()->get_potential(),
		gradient=(Psi_t1-Psi_t2)/distance,
		//K=gradient<0 && l2 ? l2->K() : l1->K();      
		K=l2 ? geo_mean(l1->get_K(),l2->get_K()) : l1->get_K();
	if (fabs(K*gradient)>l1->get_Ksat()) K=l1->get_Ksat()/gradient;
	if (l2 && fabs(K*gradient)>l2->get_Ksat()) K=l2->get_Ksat()/gradient;
	real r_flow=K*gradient*flow_width * flow_thickness;
	if (left_node()->is_empty())
		r_flow=minimum(0,r_flow);
	if (right_node()->is_empty())
		r_flow=maximum(0,r_flow);
	return r_flow; 


}
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
		distance = l2 ? 
										fabs((l1->get_upper_boundary()-l2->get_upper_boundary())*0.5+(l1->get_lower_boundary()-l2->get_lower_boundary())*0.5)
									: l1->get_distance_to(*right_node()),
		Psi_t1=l1->get_potential(),
		Psi_t2=right_node()->get_potential(),
		gradient=(Psi_t1-Psi_t2)/distance,
		K=0.0;
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
	if (left_node()->is_empty())
		r_flow=minimum(0,r_flow);
	if (right_node()->is_empty())
		r_flow=maximum(0,r_flow);
	return r_flow; 
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
		sw_excess=l1->get_state()-fc; // drainable water volume
	if (sw_excess<=0) return 0.0;
	real
		Vol_d=l1->get_capacity()-fc, // drainable pore volume in m3
		Ksat=l1->get_Ksat(),
		// Percolation travel time in days = drainable volume[m3]/(get_area[m2] * Conductivity[m/day])
		tt_perc=maximum(l1->get_capacity()-fc,0)/(l1->cell.get_area() * Ksat),
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
// unsaturated Darcy
const cmf::upslope::CellConnector cmf::upslope::connections::UnsaturatedDarcy::cell_connector=CellConnector(cmf::upslope::connections::UnsaturatedDarcy::connect_cells);
real cmf::upslope::connections::UnsaturatedDarcy::calc_q( cmf::math::Time t )
{
	cmf::upslope::SoilLayer::ptr 
		l1=sw1.lock(),
		l2=sw2.lock();

	real
		Psi1=l1->cell.z-l1->get_upper_boundary(),
		Psi2=l2->cell.z-l2->get_upper_boundary(),
		gradient=(Psi1-Psi2)/distance,
		K=gradient>0 ? l1->get_K() : l2->get_K();
	SoilLayer::ptr target=gradient>0 ? l2 : l1;
	real f_upwelling=1-2*boltzmann(target->get_wetness(),1,0.01);
	if (f_upwelling<0)
	{
		real sat_wb=target->water_balance(t,this);
		return f_upwelling*sat_wb;
	}
	else
	{
		return K*gradient * flow_width * l1->get_thickness() * f_upwelling;
	}
	
}

void cmf::upslope::connections::UnsaturatedDarcy::connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer/*=0*/ )
{
	for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)
	{
		for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)
		{
			real ca=cell1.get_layer(i)->get_flow_crosssection(*cell2.get_layer(j));
			if (ca>0)
			{
				real d=cell1.get_layer(i)->get_distance_to(*cell2.get_layer(j));
				new UnsaturatedDarcy(cell1.get_layer(i),cell2.get_layer(j),ca,d);
			}
		}
	}
}

void cmf::upslope::connections::UnsaturatedDarcy::use_for_cell( cmf::upslope::Cell & cell,bool no_override/*=true*/ )
{
	for (int i = 0; i < cell.layer_count()-1 ; ++i)
	{
		cmf::upslope::SoilLayer::ptr 
			l_upper=cell.get_layer(i), 
			l_lower=cell.get_layer(i+1);
		real distance=fabs((l_upper->get_upper_boundary()-l_lower->get_upper_boundary())*0.5+(l_upper->get_lower_boundary()-l_lower->get_lower_boundary())*0.5);
		if (!(no_override && l_upper->get_connection(*l_lower)))
			new UnsaturatedDarcy(l_upper,l_lower,cell.get_area(),distance);
	}

}

/************************************************************************/
/* Lateral fluxes                                                       */
/************************************************************************/
// Darcy connection
real get_flow_thick(cmf::upslope::SoilLayer::ptr l)
{
	real 
		thick=l->get_thickness(),
		w_eff= l->get_soil().Wetness_eff(l->get_wetness(),2.5),
		mp = l->get_matrix_potential();
		
	return minmax(thick + mp /* w_eff*/ ,0.0,thick);
}
real cmf::upslope::connections::Darcy::calc_q( cmf::math::Time t )	
{
	cmf::upslope::SoilLayer::ptr 
		l1=sw1.lock(),
		l2=sw2.lock();
	// Calculate the gradient
	real
		Psi_t1 = l1->cell.z - l1->cell.get_saturated_depth(),
		Psi_t2 = l2->cell.z - l2->cell.get_saturated_depth(),
		gradient=(Psi_t1-Psi_t2)/distance;

	if (right_node()->is_empty() && gradient<=0)
		return 0.0;

	//cmf::upslope::SoilLayer::ptr source =  (gradient < 0) && l2 ? l2 : l1;
	real
		// Transmissivity
		flow_thick1 = get_flow_thick(l1), 
		flow_thick2 = l2 ? get_flow_thick(l2) : flow_thick1;
	if ((flow_thick1 + flow_thick2)<=0) return 0.0;
	real
		T1 = l1->get_soil().Transmissivity(l1->get_lower_boundary()-flow_thick1,l1->get_lower_boundary(),1),
		T2 = l2 ? l2->get_soil().Transmissivity(l2->get_lower_boundary()-flow_thick2, l2->get_lower_boundary(),1) : T1,
		T = mean(T1,T2);

	
	return T*gradient*flow_width;

}

void cmf::upslope::connections::Darcy::connect_cells( cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
				real ca=cell1.get_layer(i)->get_flow_crosssection(*cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i)->get_distance_to(*cell2.get_layer(j));
					new Darcy(cell1.get_layer(i),cell2.get_layer(j),w,d);
				}	}	}
	}
}
const cmf::upslope::CellConnector cmf::upslope::connections::Darcy::cell_connector=CellConnector(cmf::upslope::connections::Darcy::connect_cells);

// Topographic gradient connection
const cmf::upslope::CellConnector cmf::upslope::connections::TopographicGradientDarcy::cell_connector=CellConnector(cmf::upslope::connections::TopographicGradientDarcy::connect_cells);
real cmf::upslope::connections::TopographicGradientDarcy::calc_q( cmf::math::Time t )
{
	// Darcy flux
	cmf::upslope::SoilLayer::ptr 
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
		T1=l1->get_soil().Transmissivity(l1->get_lower_boundary()-flow_thick1,l1->get_lower_boundary(),l1->get_wetness()),
		T2=l2 ? l2->get_soil().Transmissivity(l2->get_lower_boundary()-flow_thick2,l2->get_lower_boundary(),l2->get_wetness()) : T1,
		T = gradient>0 ? T1 : T2;
	if (right_node()->is_empty() && gradient<0)
		return 0.0;
	return T*gradient*flow_width;

}

void cmf::upslope::connections::TopographicGradientDarcy::connect_cells( cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
				real ca=cell1.get_layer(i)->get_flow_crosssection(*cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i)->get_distance_to(*cell2.get_layer(j));
				new TopographicGradientDarcy(cell1.get_layer(i),cell2.get_layer(j),w,d);
			}	}	}
	}

}





real cmf::upslope::connections::OHDISflow::calc_q( cmf::math::Time t )
{
	cmf::upslope::SoilLayer::ptr 
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
const cmf::upslope::CellConnector cmf::upslope::connections::OHDISflow::cell_connector(cmf::upslope::connections::OHDISflow::connect_cells);
void cmf::upslope::connections::OHDISflow::connect_cells( cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
				real ca=cell1.get_layer(i)->get_flow_crosssection(*cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i)->get_distance_to(*cell2.get_layer(j));
					new OHDISflow(cell1.get_layer(i),cell2.get_layer(j),w,d);
				}	}	}
	}

}


void cmf::upslope::connections::SubSurfaceFlux::NewNodes()
{
	cmf::upslope::SoilLayer::ptr l1,l2;
	
	l1=cmf::upslope::SoilLayer::cast(left_node());
	l2=cmf::upslope::SoilLayer::cast(right_node());

	if (!l1) 
		throw std::runtime_error("Can't create " + this->type + ", left node {" + left_node()->Name + "} is not a soil layer!" );
	sw1 = l1;
	sw2 = l2;

}