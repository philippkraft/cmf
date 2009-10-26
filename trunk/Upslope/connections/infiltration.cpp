#include "infiltration.h"
#include "../../math/Time.h"
/************************************************************************/
/* Infiltration                                                         */
/************************************************************************/
real cmf::upslope::connections::MatrixInfiltration::calc_q( cmf::math::Time t )
{
	cmf::upslope::SoilLayer_ptr soilwater=m_soilwater.lock();
	cmf::river::open_water_storage_ptr surfacewaterstorage=m_surfacewaterstorage.lock();
	cmf::water::flux_node::ptr surfacewater=left_node();
	cmf::upslope::Cell& cell=soilwater->cell;
	real
		Pot_surf = surfacewater->get_potential(),																				// get_potential of the surface water				
		Pot_soil = soilwater->get_potential(),																					// get_potential of the soil water
		gradient = (Pot_surf-Pot_soil)/(0.5*soilwater->get_thickness()),								// Gradient surface->soil
		K = geo_mean(soilwater->get_K(),soilwater->get_Ksat()),										        // Conductivity in m/day
		maxInfiltration = gradient * K * cell.get_area();

	real
		inflow=0,
		f_wb=1;
	if (surfacewaterstorage) // If the surface water is a storage
	{
		f_wb=1-sqrt(piecewise_linear(surfacewaterstorage->h(),0,0.01)); // get a factor how dominant the inflow to the surface water, versus the state dependent outflow is
		inflow=(1-f_wb) * soilwater->get_Ksat()*soilwater->cell.get_area();		// get the state dependend outflow
	}
	//else // inflow is the sum of the inflows to surface water
	inflow += f_wb * surfacewater->water_balance(t,this);							// get the inflow dependent outflow

	return minimum(maxInfiltration,inflow);
}

real cmf::upslope::connections::CompleteInfiltration::calc_q( cmf::math::Time t )
{
	cmf::upslope::SoilLayer_ptr soilwater=m_soilwater.lock();
	cmf::river::open_water_storage_ptr surfacewaterstorage=m_surfacewaterstorage.lock();
	cmf::water::flux_node::ptr surfacewater=left_node();

	cmf::upslope::Cell& cell=soilwater->cell;
	real
		Pot_surf=surfacewater->get_potential(),																				// get_potential of the surface water				
		Pot_soil=soilwater->get_potential(),																					// get_potential of the soil water
		gradient=(Pot_surf-Pot_soil)/(0.5*soilwater->get_thickness()),								// Gradient surface->soil
		K=soilwater->get_Ksat(),										        // Conductivity in m/day
		maxInfiltration = gradient * K * cell.get_area();

	real
		inflow=0,
		f_wb=1;
	if (surfacewaterstorage) // If the surface water is a storage
	{
		f_wb=1-sqrt(piecewise_linear(surfacewaterstorage->h(),0,0.01)); // get a factor how dominant the inflow to the surface water, versus the state dependent outflow is
		inflow=(1-f_wb) * soilwater->get_Ksat()*soilwater->cell.get_area();		// get the state dependend outflow
	}
	//else // inflow is the sum of the inflows to surface water
	inflow += f_wb * surfacewater->water_balance(t,this);							// get the inflow dependent outflow

	return minimum(maxInfiltration,inflow);

//	real
// 		K=soilwater->get_Ksat(),																							// Conductivity in m/day
// 		max_inflow=K*soilwater->cell.get_area(),
// 		space_left=soilwater->get_state()-soilwater->get_capacity(),
// 		inflow=maximum(surfacewater->water_balance(t,this),0);
// 	if (!surfacewater->is_empty())
// 		inflow += surfacewaterstorage->h() * max_inflow;
// 
// 	real excess=24 * maximum((inflow + soilwater->water_balance(t,this))/24 - space_left,0);
// 	return inflow-excess;
// 
}

