
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

#include "AtmosphericFluxes.h"

real cmf::upslope::connections::snowfraction( real T )
{
	return 1-piecewise_linear(T,cmf::atmosphere::Weather::get_snow_threshold()-1,cmf::atmosphere::Weather::get_snow_threshold()+1);
}

real cmf::upslope::connections::Rainfall::calc_q( cmf::math::Time t )
{
	real snow = m_cell.get_snow() ? snowfraction(m_cell.get_weather(t).T) : 0.0;
	const cmf::upslope::vegetation::Vegetation& veg=m_cell.vegetation;
	real f=0; // Fraction of rainfall to use
	if (Throughfall) f+=1-veg.CanopyClosure;
	if (InterceptedRainfall) f+=veg.CanopyClosure;
	return (1-snow) * f * m_cell.get_rainfall(t);
}

cmf::upslope::connections::Rainfall::Rainfall( cmf::water::flux_node::ptr target,
		cmf::upslope::Cell & cell,
		bool getthroughfall/*=true*/,
		bool getintercepted/*=true*/ ) 
	:	cmf::water::flux_connection(cell.get_rain_source(),
		target,
		getthroughfall && getintercepted ? "Rainfall" : getthroughfall ? "Throughfall" : getintercepted ? "Intercepted rain" : "No rain"),
		m_cell(cell),
		Throughfall(getthroughfall),
		InterceptedRainfall(getintercepted)
{
	NewNodes();
}

real cmf::upslope::connections::Snowfall::calc_q( cmf::math::Time t )
{
	real snow = m_cell.get_snow() ? snowfraction(m_cell.get_weather(t).T) : 0.0;
	return snow * m_cell.get_rainfall(t); // Convert mm/day to m3/day
}
