#include "infiltration.h"
#include "../../math/Time.h"
/************************************************************************/
/* Infiltration                                                         */
/************************************************************************/
real cmf::upslope::connections::MatrixInfiltration::calc_q( cmf::math::Time t )
{
	cmf::upslope::SoilLayer::ptr soilwater=m_soilwater.lock();
	cmf::river::OpenWaterStorage::ptr surfacewaterstorage=m_surfacewaterstorage.lock();
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
		f_wb=1-sqrt(piecewise_linear(surfacewaterstorage->get_depth(),0,0.01)); // get a factor how dominant the inflow to the surface water, versus the state dependent outflow is
		inflow=(1-f_wb) * soilwater->get_Ksat()*soilwater->cell.get_area();		// get the state dependend outflow
	}
	//else // inflow is the sum of the inflows to surface water
	inflow += f_wb * surfacewater->water_balance(t,this);							// get the inflow dependent outflow

	return minimum(maxInfiltration,inflow);
}

real cmf::upslope::connections::CompleteInfiltration::calc_q( cmf::math::Time t )
{
	cmf::upslope::SoilLayer::ptr soilwater=m_soilwater.lock();
	cmf::river::OpenWaterStorage::ptr surfacewaterstorage=m_surfacewaterstorage.lock();
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
		f_wb=1-sqrt(piecewise_linear(surfacewaterstorage->get_depth(),0,0.01)); // get a factor how dominant the inflow to the surface water, versus the state dependent outflow is
		inflow=(1-f_wb) * soilwater->get_Ksat()*soilwater->cell.get_area();		// get the state dependend outflow
	}
	//else // inflow is the sum of the inflows to surface water
	inflow += f_wb * surfacewater->water_balance(t,this);							// get the inflow dependent outflow

	return minimum(maxInfiltration,inflow);

}

