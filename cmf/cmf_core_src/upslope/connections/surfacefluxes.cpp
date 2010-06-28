

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   cmf is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
//   
#include "surfacefluxes.h"
#include "../../math/real.h"
/************************************************************************/
/* Surface fluxes                                                       */
/************************************************************************/
real cmf::upslope::connections::CanopyOverflow::calc_q( cmf::math::Time t ) 
{
	// The maximum capacity of the canopy in m3
	real 
		Vmax=m_cell.vegetation.CanopyCapacityPerLAI * m_cell.vegetation.LAI * m_cell.get_area() * 1e-3,
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