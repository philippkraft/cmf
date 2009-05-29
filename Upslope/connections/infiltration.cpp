#include "infiltration.h"
#include "../../math/Time.h"
/************************************************************************/
/* Infiltration                                                         */
/************************************************************************/
real cmf::upslope::connections::MatrixInfiltration::calc_q( cmf::math::Time t )
{
	real
		f_Ponding=square(piecewise_linear(m_soilwater->Wetness(),0.95,1)),	// Ponding factor (0 complete infiltration,	1 only ponding)
		sw_b=maximum(m_soilwater->Waterbalance(t,this),0),													// Positive soil water balance
		Pot_surf=m_surfacewater->Potential(),																				// Potential of the surface water				
		Pot_soil=m_soilwater->Potential(),																					// Potential of the soil water
		gradient=(Pot_surf-Pot_soil)/(0.5*m_soilwater->Thickness()),								// Gradient surface->soil
		K=geo_mean(m_soilwater->K(),m_soilwater->Ksat()),										        // Conductivity in m/day
		
		maxInfiltration = 
			  (1-f_Ponding)*gradient*K*m_soilwater->cell.get_area() // Maximal current infiltration dP/dz * K * get_area in m3/day
			-	f_Ponding*sw_b;																		// Ponded flux

	real
		inflow=0,
		f_wb=1;
	if (m_surfacewaterstorage) // If the surface water is a storage
	{
		f_wb=1-sqrt(piecewise_linear(m_surfacewaterstorage->h(),0,0.01)); // get a factor how dominant the inflow to the surface water, versus the state dependent outflow is
		inflow=(1-f_wb) * m_soilwater->Ksat()*m_soilwater->cell.get_area();		// get the state dependend outflow
	}
	//else // inflow is the sum of the inflows to surface water
	inflow += f_wb * m_surfacewater->Waterbalance(t,this);							// get the inflow dependent outflow

	return minimum(maxInfiltration,inflow);
}
real cmf::upslope::connections::CompleteInfiltration::calc_q( cmf::math::Time t )
{
	real
		f_Ponding=square(piecewise_linear(m_soilwater->Wetness(),0.95,1)),	// Ponding factor (0 complete infiltration,	1 only ponding)
		sw_b=maximum(m_soilwater->Waterbalance(t,this),0),									// Positive soil water balance
		K=m_soilwater->Ksat(),																							// Conductivity in m/day

		maxInfiltration = 
		(1-f_Ponding)*K*m_soilwater->cell.get_area()														// Maximal current infiltration dP/dz * K * get_area in m3/day
		-	f_Ponding*sw_b;																										// Ponded flux

	real
		inflow=0,
		f_wb=1;
	if (m_surfacewaterstorage) // If the surface water is a storage
	{
		f_wb=1-sqrt(piecewise_linear(m_surfacewaterstorage->h(),0,0.01)); // get a factor how dominant the inflow to the surface water, versus the state dependent outflow is
		inflow=(1-f_wb) * m_soilwater->Ksat()*m_soilwater->cell.get_area();		// get the state dependend outflow
	}
	//else // inflow is the sum of the inflows to surface water
	inflow += f_wb * m_surfacewater->Waterbalance(t,this);							// get the inflow dependent outflow
	return minimum(maxInfiltration,inflow);	
}