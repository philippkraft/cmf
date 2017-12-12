

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
	// Get current temperature
	real T = m_cell.get_weather(t).T;
	// Get snowmelt threshold temperature
	real ThresholdTemp=cmf::atmosphere::Weather::get_snow_threshold();
	// If we have snowmelt...
	if (T>ThresholdTemp)
	{
		real potential_meltrate_m3 = SnowMeltRate * (T - ThresholdTemp) * m_cell.get_area() * 0.001;
		// limit snow meltrate to current snow/(10 min)
		return std::min(Snow->get_volume() * 24 * 6, potential_meltrate_m3);
	}
	else // too cold, no melting
		return 0.0;
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

real cmf::upslope::connections::RutterInterception::calc_q( cmf::math::Time t )
{
	// Get canopy storage and rainfall boundary condition
	cmf::water::WaterStorage::ptr canopy= m_Canopy.lock();
	cmf::water::flux_node::ptr rain = m_cell.get_rain_source();
	real 
		// Maximum storage capacity in mm
		Vmax=m_cell.vegetation.CanopyCapacityPerLAI * m_cell.vegetation.LAI,
		// Actual storage capacity in mm
		Vact=canopy->get_volume() / m_cell.get_area() * 1e3,
		// Actual rainfall in m3/day
		P_t = rain->flux_to(*canopy,t),
		// When the Capacity is exceeded (which can happen due to numerical errors and at harvest (LAI=0), 
		// remove exceeding water with a linear storage with k=1/h
		overflow = Vact > Vmax? (Vact - Vmax) * 24. : 0.0;
	if (Vmax <= 0.0) // Prevent nan through 0-division
		return P_t ;//+ overflow;
	else 
		// Return fraction of Vact/Vmax of actual rainfall
		// for very small Vmax, Vact can be easily magnitudes higher then Vmax, 
		// however the flux is bounded to not exceed P_t to prevent extreme fluxes for small values
		return P_t * minmax(Vact/Vmax,0,2);// + overflow;
}
