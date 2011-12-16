

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
#define sqr(a) ((a)*(a))

using namespace cmf::math;
/************************************************************************/
/* Surface fluxes                                                       */
/************************************************************************/
real cmf::upslope::connections::CanopyOverflow::calc_q( cmf::math::Time t ) 
{
	// The maximum capacity of the canopy in m3
	real 
		Vmax=m_cell.vegetation.CanopyCapacityPerLAI * m_cell.vegetation.LAI,
		Vact=m_Canopy.lock()->get_volume() / m_cell.get_area() * 1e3;
	if (Vact>Vmax)
		return sqr(std::min((Vact-Vmax)/Vmax,1.0))*2400 * m_cell.get_area() * 1e-3;
	else
		return 0.0;
}


real cmf::upslope::connections::SimpleTindexSnowMelt::calc_q( cmf::math::Time t )
{
	cmf::water::WaterStorage::ptr Snow=m_Snow.lock();
	if (Snow->is_empty()>=1.0)
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

real cmf::upslope::connections::EnergyBudgetSnowMelt::calc_q( cmf::math::Time t )
{
	cmf::water::WaterStorage::ptr snow(m_Snow);
	double 
		sno_cv = m_cell.snow_coverage(),
		sno_vol = snow->get_volume(),
		area = m_cell.get_area(),
		hf = m_cell.heat_flux(t);
	if (sno_vol<=1e-15) return 0.0;
	double energy_flux = sno_cv * hf * area;
	if (energy_flux<0)	return 0.0;

	cmf::atmosphere::Weather w=m_cell.get_weather(t);
	double c_snow = 2.1 ; // J/(K g) -> MJ/(K m3)
	double lambda_f = 333.5; // MJ/Mg ~=~ MJ/m3 -> Latent energy of ice melt
	// MJ
	double dt = h / day;
	double energy_to_0degC = w.Tground * snow->get_volume() * c_snow * m_cell.get_area();
	double net_melt_energy_flux = energy_to_0degC/dt + energy_flux;
	if (net_melt_energy_flux<0) return 0.0;
	// MJ/day
	double melt_rate = net_melt_energy_flux / lambda_f;
	return melt_rate;
	
}