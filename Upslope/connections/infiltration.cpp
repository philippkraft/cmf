#include "infiltration.h"
#include "../../math/Time.h"
/************************************************************************/
/* Infiltration                                                         */
/************************************************************************/
real cmf::upslope::connections::MatrixInfiltration::calc_q( cmf::math::Time t )
{
	cmf::upslope::Cell& cell=m_soilwater->cell;
	real
		Pot_surf = m_surfacewater->get_potential(),																				// get_potential of the surface water				
		Pot_soil = m_soilwater->get_potential(),																					// get_potential of the soil water
		gradient = (Pot_surf-Pot_soil)/(0.5*m_soilwater->get_thickness()),								// Gradient surface->soil
		K = geo_mean(m_soilwater->get_K(),m_soilwater->get_Ksat()),										        // Conductivity in m/day
		maxInfiltration = gradient * K * cell.get_area();

	real
		inflow=0,
		f_wb=1;
	if (m_surfacewaterstorage) // If the surface water is a storage
	{
		f_wb=1-sqrt(piecewise_linear(m_surfacewaterstorage->h(),0,0.01)); // get a factor how dominant the inflow to the surface water, versus the state dependent outflow is
		inflow=(1-f_wb) * m_soilwater->get_Ksat()*m_soilwater->cell.get_area();		// get the state dependend outflow
	}
	//else // inflow is the sum of the inflows to surface water
	inflow += f_wb * m_surfacewater->water_balance(t,this);							// get the inflow dependent outflow

	return minimum(maxInfiltration,inflow);
}

real cmf::upslope::connections::CompleteInfiltration::calc_q( cmf::math::Time t )
{
	cmf::upslope::Cell& cell=m_soilwater->cell;
	real
		Pot_surf=m_surfacewater->get_potential(),																				// get_potential of the surface water				
		Pot_soil=m_soilwater->get_potential(),																					// get_potential of the soil water
		gradient=(Pot_surf-Pot_soil)/(0.5*m_soilwater->get_thickness()),								// Gradient surface->soil
		K=m_soilwater->get_Ksat(),										        // Conductivity in m/day
		maxInfiltration = gradient * K * cell.get_area();

	real
		inflow=0,
		f_wb=1;
	if (m_surfacewaterstorage) // If the surface water is a storage
	{
		f_wb=1-sqrt(piecewise_linear(m_surfacewaterstorage->h(),0,0.01)); // get a factor how dominant the inflow to the surface water, versus the state dependent outflow is
		inflow=(1-f_wb) * m_soilwater->get_Ksat()*m_soilwater->cell.get_area();		// get the state dependend outflow
	}
	//else // inflow is the sum of the inflows to surface water
	inflow += f_wb * m_surfacewater->water_balance(t,this);							// get the inflow dependent outflow

	return minimum(maxInfiltration,inflow);

//	real
// 		K=m_soilwater->get_Ksat(),																							// Conductivity in m/day
// 		max_inflow=K*m_soilwater->cell.get_area(),
// 		space_left=m_soilwater->get_state()-m_soilwater->get_capacity(),
// 		inflow=maximum(m_surfacewater->water_balance(t,this),0);
// 	if (!m_surfacewater->is_empty())
// 		inflow += m_surfacewaterstorage->h() * max_inflow;
// 
// 	real excess=24 * maximum((inflow + m_soilwater->water_balance(t,this))/24 - space_left,0);
// 	return inflow-excess;
// 
}

