#include "surfacefluxes.h"
#include "../../math/real.h"
/************************************************************************/
/* Surface fluxes                                                       */
/************************************************************************/
real cmf::upslope::connections::CanopyOverflow::calc_q( cmf::math::Time t ) 
{
	// The maximum capacity of the canopy in m3
	real 
		Vmax=m_cell.get_vegetation().CanopyCapacityPerLAI * m_cell.get_vegetation().LAI * m_cell.get_area() * 1e-3,
		Vact=m_Canopy->get_state();
	return maximum((Vact-Vmax)/math::min.AsDays(),0);
}

real cmf::upslope::connections::HBVSnowMelt::calc_q( cmf::math::Time t )
{
	if (m_Snow->is_empty())
		return 0.0;
	else 
	{
		real T=m_cell.get_weather(t).T;
		real ThresholdTemp=cmf::atmosphere::Weather::snow_threshold;
		if (T>ThresholdTemp)
		{
			real f=piecewise_linear(m_Snow->get_state()/m_cell.get_area(),0,0.001);
			return f*SnowMeltRate*(T-ThresholdTemp)*m_cell.get_area()*0.001;
		}
		else if (m_SnowWater->is_empty())
			return 0.0;
		else
			return RefreezeRate*SnowMeltRate*(T-ThresholdTemp)*m_cell.get_area()*0.001;
	}
}

real cmf::upslope::connections::SnowWaterOverflow::calc_q( cmf::math::Time t )
{
	if (m_SnowWater->is_empty()) 
		return 0.0;
	else if (m_Snow->is_empty())
		return SnowConductivity;
	else
	{
		real sn_to_snw=m_SnowWater->get_state()/(RelCapacity*m_Snow->get_state());
		real f=boltzmann(sn_to_snw,0.1,0.05);
		return SnowConductivity*f*m_cell.get_area();
	}
}
