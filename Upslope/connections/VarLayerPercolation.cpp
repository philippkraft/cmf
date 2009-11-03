#include "VarLayerPercolation.h"
#include "../../math/real.h"
#include "../Topology.h"
// Connection of a flexible size saturated zone
real cmf::upslope::connections::VarLayerPercolationRichards::calc_q( cmf::math::Time t ) 
{
	std::tr1::shared_ptr<cmf::upslope::VariableLayerUnsaturated> unsat=m_unsat.lock();
	std::tr1::shared_ptr<cmf::upslope::VariableLayerSaturated> sat = m_sat.lock();
	Cell & cell=sat->cell;
	real
		K_act=geo_mean(unsat->get_K(),sat->get_K()),
		sat_wb=sat->water_balance(t,this),
		max_sat_thick=sat->MaximumThickness(),
		sat_lb=sat->get_lower_boundary(),
		sat_ub_act=sat->get_upper_boundary(),
		sat_ub_min=sat->get_lower_boundary()-sat->MaximumThickness(),
		usat_ub=unsat->get_upper_boundary(),
		sat_vol_max=sat->get_soil().VoidVolume(sat_ub_min,sat_lb,cell.get_area()),
		gradient=(unsat->get_potential()-sat->get_potential())/((sat->get_lower_boundary() - unsat->get_upper_boundary() )*0.5),
		K=K_act * gradient * cell.get_area(),
		w_u=minimum(unsat->get_wetness(),.9),
		Exw=(1/(1-w_u)-1) * (sat_wb+K),
		sat_vol_act=sat->get_state(),
		sat_vol_left=sat_vol_max - sat_vol_act,
		next_hour_sat_inflow=(K + sat_wb + Exw)/24, 
		excess= 0 * maximum(next_hour_sat_inflow - sat_vol_left,0);
	return K + Exw - excess;

}

real cmf::upslope::connections::VarLayerPercolationSimple::calc_q( cmf::math::Time t )
{
	std::tr1::shared_ptr<cmf::upslope::VariableLayerUnsaturated> unsat=m_unsat.lock();
	std::tr1::shared_ptr<cmf::upslope::VariableLayerSaturated> sat = m_sat.lock();
	Cell & cell=sat->cell;
	real
		K_act=unsat->get_K(),
		sat_wb=sat->water_balance(t,this),
		max_sat_thick=sat->MaximumThickness(),
		sat_lb=sat->get_lower_boundary(),
		sat_ub_act=sat->get_upper_boundary(),
		sat_ub_min=sat->get_lower_boundary()-sat->MaximumThickness(),
		usat_ub=unsat->get_upper_boundary(),
		sat_vol_max=sat->get_soil().VoidVolume(sat_ub_min,sat_lb,cell.get_area()),
		w_cap=unsat->get_soil().Wetness(-sat_lb),
		K_cap=unsat->get_soil().K(w_cap,0.5*(usat_ub+sat_ub_act)),
		gradient=(unsat->get_potential()-sat->get_potential())/((sat->get_lower_boundary() - unsat->get_upper_boundary() )*0.5),
		K= (K_act-K_cap) * cell.get_area(),
		w_u=minimum(unsat->get_wetness(),.9),
		Exw=(1/(1-w_u)-1) * (sat_wb+K),
		sat_vol_act=sat->get_state(),
		sat_vol_left=sat_vol_max - sat_vol_act,
		next_hour_sat_inflow=(K + sat_wb + Exw)/24, 
		excess= 24 * maximum(next_hour_sat_inflow - sat_vol_left,0);
	return K + Exw - excess;

	
}


real cmf::upslope::connections::PIHMpercolation::calc_q( cmf::math::Time t )
{
	std::tr1::shared_ptr<cmf::upslope::VariableLayerUnsaturated> unsat=m_unsat.lock();
	std::tr1::shared_ptr<cmf::upslope::VariableLayerSaturated> sat = m_sat.lock();

	real Ks=sat->get_Ksat();
	// hg is the fill height of the saturated layer, zs the soil depth.
	// in the q0 equation only zs-hg is used, the saturated depth
	real zs_hg=sat->get_upper_boundary();
	// Fill height of the unsaturated layer
	real hu = unsat->get_thickness() * unsat->get_wetness();
	real hg = sat->get_thickness();
	//if (zs_hg<=1e-3 || hu<=1e-3) return 0.0;
	real q0=
		Ks * (1 - exp(-alpha*zs_hg) - alpha*hu)
		/(alpha*zs_hg-(1-exp(-alpha*zs_hg)));
	return -q0 * sat->cell.get_area();
}

void cmf::upslope::connections::PIHMlateral::connect_cells( cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer/*=0*/ )
{
	
	real fw=cell1.get_topology().flowwidth(cell2);
	if (fw>0)
	{
		std::tr1::shared_ptr<cmf::upslope::VariableLayerSaturated> soil_left, soil_right;

		for (int i = 0; i < cell1.layer_count() ; ++i)
		{
			std::tr1::shared_ptr<cmf::upslope::VariableLayerSaturated> lyr=std::tr1::dynamic_pointer_cast<cmf::upslope::VariableLayerSaturated>(cell1.get_layer(i));
			if (lyr)
				soil_left=lyr;
		}
		for (int i = 0; i < cell2.layer_count() ; ++i)
		{
			std::tr1::shared_ptr<cmf::upslope::VariableLayerSaturated> lyr=std::tr1::dynamic_pointer_cast<cmf::upslope::VariableLayerSaturated>(cell2.get_layer(i));
			if (lyr)
				soil_right=lyr;
		}

		if (soil_left && soil_right)
			new PIHMlateral(soil_left,soil_right,fw, cell1.get_distance_to(cell2));
	}
	
}
const cmf::upslope::CellConnector cmf::upslope::connections::PIHMlateral::cell_connector = cmf::upslope::CellConnector(cmf::upslope::connections::PIHMlateral::connect_cells);
real cmf::upslope::connections::PIHMlateral::calc_q( cmf::math::Time t )
{
	cmf::upslope::SoilLayer::ptr left=m_soil_left.lock();
	cmf::upslope::SoilLayer::ptr right= m_soil_right.lock();

	real
		Psi1= left->cell.z-left->get_upper_boundary(),
		Psi2= right ? right->cell.z-right->get_upper_boundary() : right_node()->get_potential(),
		zs1=left->cell.get_layer(-1)->get_lower_boundary(),
		zs2=right->cell.get_layer(-1)->get_lower_boundary(),
		hg1 = left->get_thickness(),
		hg2 = right ? right->get_thickness() : hg1,
		Keff = harmonic_mean(left->get_Ksat(), right ? right->get_Ksat() : left->get_Ksat()),
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