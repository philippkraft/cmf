#include "subsurfacefluxes.h"
#include "../../math/real.h"

/************************************************************************/
/* Precolation and 3D fluxes                                            */
/************************************************************************/
// Richards eq.
const cmf::upslope::CellConnector cmf::upslope::connections::Richards::cell_connector=CellConnector(cmf::upslope::connections::Richards::connect_cells);
void cmf::upslope::connections::Richards::connect_cells( cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer/*=0*/ )
{
	for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)
	{
		for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)
		{
			real ca=cell1.get_layer(i).get_flow_crosssection(cell2.get_layer(j));
			if (ca>0)
			{
				real d=cell1.get_layer(i).Location.distance3DTo(cell2.get_layer(j).Location);
				new Richards(cell1.get_layer(i),cell2.get_layer(j),ca,d);
			}
		}
	}
}

void cmf::upslope::connections::Richards::use_for_cell( cmf::upslope::Cell & cell,bool no_override/*=true*/ )
{
	for (int i = 0; i < cell.layer_count()-1 ; ++i)
	{
		cmf::upslope::SoilWaterStorage & l_upper=cell.get_layer(i), & l_lower=cell.get_layer(i+1);
		real distance=fabs((l_upper.get_upper_boundary()-l_lower.get_upper_boundary())*0.5+(l_upper.get_lower_boundary()-l_lower.get_lower_boundary())*0.5);
		if (!(no_override && l_upper.Connection(l_lower)))
			new Richards(l_upper,l_lower,cell.get_area(),distance);
	}
}

real cmf::upslope::connections::Richards::calc_q( cmf::math::Time t ) 
{
	// Richards flux
	real
		Psi_t1=sw1->get_potential(),
		Psi_t2=m_right->get_potential(),
		gradient=(Psi_t1-Psi_t2)/distance,
		//K=gradient<0 && sw2 ? sw2->K() : sw1->K();      
		K=sw2 ? geo_mean(sw1->get_K(),sw2->get_K()) : sw1->get_K();
 	if (fabs(K*gradient)>sw1->get_Ksat()) K=sw1->get_Ksat()/gradient;
 	if (sw2 && fabs(K*gradient)>sw2->get_Ksat()) K=sw2->get_Ksat()/gradient;
	real r_flow=K*gradient*flow_area;
	if (m_left->is_empty())
		r_flow=minimum(0,r_flow);
	if (m_right->is_empty())
		r_flow=maximum(0,r_flow);
	return r_flow; 
}


// SWAT Percolation
real cmf::upslope::connections::SWATPercolation::calc_q( cmf::math::Time t )
{
	real
		capacity=sw1->get_capacity(),
		fc_waterhead=pressure_to_waterhead(-33000), // Field capacity suction in m
		fc=sw1->get_soil().Wetness(fc_waterhead)*capacity, // Water storage at field capacity
		sw_excess=sw1->get_state()-fc; // drainable water volume
	if (sw_excess<=0) return 0.0;
	real
		Vol_d=sw1->get_capacity()-fc, // drainable pore volume in m3
		Ksat=sw1->get_Ksat(),
		// Percolation travel time in days = drainable volume[m3]/(get_area[m2] * Conductivity[m/day])
		tt_perc=maximum(sw1->get_capacity()-fc,0)/(sw1->cell.get_area() * Ksat),
		w_perc=sw_excess*(1-exp(-1/tt_perc));
	return w_perc*maximum(0,0.9-sw2->get_wetness());


}

void cmf::upslope::connections::SWATPercolation::use_for_cell( cmf::upslope::Cell & cell,bool no_override )
{
	for (int i = 0; i < cell.layer_count()-1 ; ++i)
	{
		cmf::upslope::SoilWaterStorage & l_upper=cell.get_layer(i), & l_lower=cell.get_layer(i+1);
		real distance=fabs((l_upper.get_upper_boundary()-l_lower.get_upper_boundary())*0.5+(l_upper.get_lower_boundary()-l_lower.get_lower_boundary())*0.5);
		if (!(no_override && l_upper.Connection(l_lower)))
			new SWATPercolation(l_upper,l_lower);
	}

}
// unsaturated Darcy
const cmf::upslope::CellConnector cmf::upslope::connections::UnsaturatedDarcy::cell_connector=CellConnector(cmf::upslope::connections::UnsaturatedDarcy::connect_cells);
real cmf::upslope::connections::UnsaturatedDarcy::calc_q( cmf::math::Time t )
{
	real
		Psi1=sw1->cell.z-sw1->get_upper_boundary(),
		Psi2=sw2->cell.z-sw2->get_upper_boundary(),
		gradient=(Psi1-Psi2)/distance,
		K=gradient>0 ? sw1->get_K() : sw2->get_K();
	SoilWaterStorage& target=gradient>0 ? *sw2 : *sw1;
	real f_upwelling=1-2*boltzmann(target.get_wetness(),1,0.01);
	if (f_upwelling<0)
	{
		real sat_wb=target.water_balance(t,this);
		return f_upwelling*sat_wb;
	}
	else
	{
		return K*gradient*flow_area*f_upwelling;
	}
	
}

void cmf::upslope::connections::UnsaturatedDarcy::connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer/*=0*/ )
{
	for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)
	{
		for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)
		{
			real ca=cell1.get_layer(i).get_flow_crosssection(cell2.get_layer(j));
			if (ca>0)
			{
				real d=cell1.get_layer(i).Location.distance3DTo(cell2.get_layer(i).Location);
				new UnsaturatedDarcy(cell1.get_layer(i),cell2.get_layer(j),ca,d);
			}
		}
	}
}

void cmf::upslope::connections::UnsaturatedDarcy::use_for_cell( cmf::upslope::Cell & cell,bool no_override/*=true*/ )
{
	for (int i = 0; i < cell.layer_count()-1 ; ++i)
	{
		cmf::upslope::SoilWaterStorage & l_upper=cell.get_layer(i), & l_lower=cell.get_layer(i+1);
		real distance=fabs((l_upper.get_upper_boundary()-l_lower.get_upper_boundary())*0.5+(l_upper.get_lower_boundary()-l_lower.get_lower_boundary())*0.5);
		if (!(no_override && l_upper.Connection(l_lower)))
			new UnsaturatedDarcy(l_upper,l_lower,cell.get_area(),distance);
	}

}

/************************************************************************/
/* Lateral fluxes                                                       */
/************************************************************************/
// Darcy connection
real cmf::upslope::connections::Darcy::calc_q( cmf::math::Time t )	
{
	// Darcy flux
	real
		flow_thick1=minimum(sw1->get_thickness(),sw1->get_lower_boundary()-sw1->get_saturated_depth()),
		flow_thick2=sw2 ? minimum(sw2->get_thickness(),sw2->get_lower_boundary()-sw2->get_saturated_depth()) : flow_thick1;
	if (flow_thick1<0 || flow_thick2<0)
		return 0.0;
	real
		// Gravitational gradient
		Psi_t1=sw1->cell.z - sw1->get_saturated_depth(),
		Psi_t2=sw2 ? sw2->cell.z - sw2->get_saturated_depth() : m_right->get_potential(),
		gradient=(Psi_t1-Psi_t2)/distance,
		// Transmissivity
		T1=sw1->get_soil().Transmissivity(sw1->get_lower_boundary()-flow_thick1,sw1->get_lower_boundary(),sw1->get_wetness()),
		T2=sw2 ? sw2->get_soil().Transmissivity(sw2->get_lower_boundary()-flow_thick2,sw2->get_lower_boundary(),sw2->get_wetness()) : T1,
		T = mean(T1,T2);
	if (m_right->is_empty() && gradient<=0)
		return 0.0;
	return T*gradient*flow_width;
}

void cmf::upslope::connections::Darcy::connect_cells( cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer/*=0*/ )
{
	real w=cell1.get_topology().flowwidth(cell2);
	if (w>0)
	{
		for (int i = start_at_layer; i < (start_at_layer>=0 ? cell1.layer_count() : 0) ; ++i)	{
			for (int j = start_at_layer; j < (start_at_layer>=0 ? cell2.layer_count() : 0) ; ++j)	{
				real ca=cell1.get_layer(i).get_flow_crosssection(cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i).Location.distance3DTo(cell2.get_layer(i).Location);
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
	real
		flow_thick1=minimum(sw1->get_thickness(),sw1->get_lower_boundary()-sw1->cell.get_saturated_depth()),
		flow_thick2=sw2 ? minimum(sw2->get_thickness(),sw2->get_lower_boundary()-sw2->cell.get_saturated_depth()) : flow_thick1;
	if (flow_thick1<=0 || flow_thick2<=0) return 0;
	real
		flow_thick=mean(flow_thick1,flow_thick2),
		// Topographic gradient
		Psi_t1=sw1->cell.z,
		Psi_t2=sw2 ? sw2->cell.z : m_right->get_potential(),
		gradient=(Psi_t1-Psi_t2)/distance,
		// Transmissivity
		T1=sw1->get_soil().Transmissivity(sw1->get_lower_boundary()-flow_thick1,sw1->get_lower_boundary(),sw1->get_wetness()),
		T2=sw2 ? sw2->get_soil().Transmissivity(sw2->get_lower_boundary()-flow_thick2,sw2->get_lower_boundary(),sw2->get_wetness()) : T1,
		T = gradient>0 ? T1 : T2;
	if (m_right->is_empty() && gradient<0)
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
				real ca=cell1.get_layer(i).get_flow_crosssection(cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i).Location.distance3DTo(cell2.get_layer(i).Location);
				new TopographicGradientDarcy(cell1.get_layer(i),cell2.get_layer(j),w,d);
			}	}	}
	}

}





real cmf::upslope::connections::OHDISflow::calc_q( cmf::math::Time t )
{
	// The resulting flux
	real q=0;

	// The equivalent of capillary pores as length
	real dm1=sw1->get_thickness()*sw1->get_soil().Wetness_pF(1)*sw1->get_soil().Porosity(sw1->get_upper_boundary());
	real dm2=sw2 ? sw2->get_thickness()*sw2->get_soil().Wetness_pF(1)*sw2->get_soil().Porosity(sw2->get_upper_boundary()) : dm1;
	// the equivalent filling depth
	real h1=sw1->get_thickness()*sw1->get_theta();
	real h2=sw2 ? sw2->get_thickness()*sw2->get_theta() : h1;

	// the unsaturated conductivity
	real k1=sw1->get_K();
	real k2=sw2 ? sw2->get_K() : k1;

	// topographic slope
	real z1=sw1->cell.z;
	real z2=sw2 ? sw2->cell.z : m_right->get_potential();
	real slope=(z1-z2)/distance;

	// Values a cell boundary
	real h_avg=mean(h1,h2);
	real dm_avg=mean(dm1,dm2);
	// soil depth
	real t1=sw1->get_thickness();
	real t2=sw2 ? sw2->get_thickness() : t1;
	real da_avg=mean(t1,t2);
	// capillary lateral flux
	q+=dm_avg*geo_mean(k1,k2)*slope;

	// macropore lateral flux
	if (h_avg>dm_avg)
		q+=(h_avg-dm_avg)*sw1->get_Ksat()*slope;
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
				real ca=cell1.get_layer(i).get_flow_crosssection(cell2.get_layer(j));
				if (ca>0)	{
					real d=cell1.get_layer(i).Location.distance3DTo(cell2.get_layer(i).Location);
					new OHDISflow(cell1.get_layer(i),cell2.get_layer(j),w,d);
				}	}	}
	}

}