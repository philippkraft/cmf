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
		Vact=m_Canopy.lock()->get_state();
	return maximum((Vact-Vmax)/24,0);
}


real cmf::upslope::connections::SimpleTindexSnowMelt::calc_q( cmf::math::Time t )
{
	cmf::water::WaterStorage::ptr Snow=m_Snow.lock();
	if (Snow->is_empty())
		return 0.0;
	else 
	{
		real T=m_cell.get_weather(t).T;
		real ThresholdTemp=cmf::atmosphere::Weather::snow_threshold;
		if (T>ThresholdTemp)
		{
			real f=piecewise_linear(Snow->get_state()/m_cell.get_area(),0,0.001);
			return f*SnowMeltRate*(T-ThresholdTemp)*m_cell.get_area()*0.001;
		}
		else
			return 0.0;
	}
}