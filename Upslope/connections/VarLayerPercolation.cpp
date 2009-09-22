#include "VarLayerPercolation.h"
#include "../../math/real.h"
#include "../Topology.h"
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


real cmf::upslope::connections::PIHMpercolation::calc_q( cmf::math::Time t )
{
	real Ks=m_sat->get_Ksat();
	// hg is the fill height of the saturated layer, zs the soil depth.
	// in the q0 equation only zs-hg is used, the saturated depth
	real zs_hg=m_sat->get_upper_boundary();
	// Fill height of the unsaturated layer
	real hu = m_unsat->get_thickness() * m_unsat->get_wetness();
	real hg = m_sat->get_thickness();
	//if (zs_hg<=1e-3 || hu<=1e-3) return 0.0;
	real q0=
		Ks * (1 - exp(-alpha*zs_hg) - alpha*hu)
		/(alpha*zs_hg-(1-exp(-alpha*zs_hg)));
	return -q0 * m_sat->cell.get_area();
}

void cmf::upslope::connections::PIHMlateral::connect_cells( cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer/*=0*/ )
{
	real fw=cell1.get_topology().flowwidth(cell2);
	if (fw>0)
	{
		cmf::upslope::VariableLayerSaturated * soil_left=0, * soil_right=0;

		for (int i = 0; i < cell1.layer_count() ; ++i)
		{
			cmf::upslope::VariableLayerSaturated* lyr=dynamic_cast<cmf::upslope::VariableLayerSaturated*>(&cell1.get_layer(i));
			if (lyr)
				soil_left=lyr;
		}
		for (int i = 0; i < cell2.layer_count() ; ++i)
		{
			cmf::upslope::VariableLayerSaturated* lyr=dynamic_cast<cmf::upslope::VariableLayerSaturated*>(&cell2.get_layer(i));
			if (lyr)
				soil_right=lyr;
		}

		if (soil_left && soil_right)
			new PIHMlateral(*soil_left,*soil_right,fw, cell1.get_distance_to(cell2));
	}
	
}
const cmf::upslope::CellConnector cmf::upslope::connections::PIHMlateral::cell_connector = cmf::upslope::CellConnector(cmf::upslope::connections::PIHMlateral::connect_cells);
real cmf::upslope::connections::PIHMlateral::calc_q( cmf::math::Time t )
{
	real
		Psi1= m_soil_left->cell.z-m_soil_left->get_upper_boundary(),
		Psi2= m_soil_right ? m_soil_right->cell.z-m_soil_right->get_upper_boundary() : m_right->get_potential(),
		zs1=m_soil_left->cell.get_layer(-1).get_lower_boundary(),
		zs2=m_soil_right->cell.get_layer(-1).get_lower_boundary(),
		hg1 = m_soil_left->get_thickness(),
		hg2 = m_soil_right ? m_soil_right->get_thickness() : hg1,
		Keff = harmonic_mean(m_soil_left->get_Ksat(), m_soil_right ? m_soil_right->get_Ksat() : m_soil_left->get_Ksat()),
		v = Keff * (Psi1-Psi2)/distance;
	// If the source is empty, no flux can be there
	if (Psi1>Psi2 && hg1<=0) return 0.0;
	if (Psi2>Psi1 && hg2<=0) return 0.0;
/*
	// If the target is saturated, no flux
	if (Psi2>Psi1 && hg1>=zs1) return 0.0;
	if (Psi1>Psi2 && hg2>=zs2) return 0.0;
*/

	return  v * flow_width * mean(hg1,hg2);

}