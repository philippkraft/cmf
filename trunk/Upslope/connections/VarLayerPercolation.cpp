#include "VarLayerPercolation.h"
// Connection of a flexible size saturated zone
real cmf::upslope::connections::VarLayerPercolationRichards::calc_q( cmf::math::Time t ) 
{
	Cell & cell=m_sat->cell;
	real
		K_act=geo_mean(m_unsat->get_K(),m_sat->get_K()),
		sat_wb=m_sat->water_balance(t,this),
		max_sat_thick=m_sat->MaximumThickness(),
		sat_lb=m_sat->get_lower_boundary(),
		sat_ub_act=m_sat->get_upper_boundary(),
		sat_ub_min=m_sat->get_lower_boundary()-m_sat->MaximumThickness(),
		usat_ub=m_unsat->get_upper_boundary(),
		sat_vol_max=m_sat->get_soil().VoidVolume(sat_ub_min,sat_lb,cell.get_area()),
		gradient=(m_unsat->get_potential()-m_sat->get_potential())/((m_sat->get_lower_boundary() - m_unsat->get_upper_boundary() )*0.5),
		K=K_act * gradient * cell.get_area(),
		w_u=minimum(m_unsat->get_wetness(),.9),
		Exw=(1/(1-w_u)-1) * (sat_wb+K),
		sat_vol_act=m_sat->get_state(),
		sat_vol_left=sat_vol_max - sat_vol_act,
		next_hour_sat_inflow=(K + sat_wb + Exw)/24, 
		excess= 0 * maximum(next_hour_sat_inflow - sat_vol_left,0);
	return K + Exw - excess;

}

real cmf::upslope::connections::VarLayerPercolationSimple::calc_q( cmf::math::Time t )
{
	Cell & cell=m_sat->cell;
	real
		K_act=m_unsat->get_K(),
		sat_wb=m_sat->water_balance(t,this),
		max_sat_thick=m_sat->MaximumThickness(),
		sat_lb=m_sat->get_lower_boundary(),
		sat_ub_act=m_sat->get_upper_boundary(),
		sat_ub_min=m_sat->get_lower_boundary()-m_sat->MaximumThickness(),
		usat_ub=m_unsat->get_upper_boundary(),
		sat_vol_max=m_sat->get_soil().VoidVolume(sat_ub_min,sat_lb,cell.get_area()),
		w_cap=m_unsat->get_soil().Wetness(-sat_lb),
		K_cap=m_unsat->get_soil().K(w_cap,0.5*(usat_ub+sat_ub_act)),
		gradient=(m_unsat->get_potential()-m_sat->get_potential())/((m_sat->get_lower_boundary() - m_unsat->get_upper_boundary() )*0.5),
		K= (K_act-K_cap) * cell.get_area(),
		w_u=minimum(m_unsat->get_wetness(),.9),
		Exw=(1/(1-w_u)-1) * (sat_wb+K),
		sat_vol_act=m_sat->get_state(),
		sat_vol_left=sat_vol_max - sat_vol_act,
		next_hour_sat_inflow=(K + sat_wb + Exw)/24, 
		excess= 24 * maximum(next_hour_sat_inflow - sat_vol_left,0);
	return K + Exw - excess;

	
}

