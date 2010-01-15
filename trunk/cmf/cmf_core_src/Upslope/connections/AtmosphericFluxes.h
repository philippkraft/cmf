

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
#ifndef AtmosphericFluxes_h__
#define AtmosphericFluxes_h__

#include "../../water/flux_connection.h"
#include "../cell.h"
#include "../../project.h"
#include "../../Atmosphere/Precipitation.h"
namespace cmf {
	namespace upslope {

		namespace connections {
			/// @ingroup connections
			/// A connection routing rainfall to surface water and to an eventually existing canopy storage
			class Rainfall : public cmf::water::flux_connection
			{
			protected:
				cmf::upslope::Cell & m_cell;
				virtual real calc_q(cmf::math::Time t)
				{
					bool snow=(m_cell.get_snow()!=0) && m_cell.get_weather(t).T<cmf::atmosphere::Weather::snow_threshold;
					if (snow)
						return 0.0;
					else
					{
						cmf::upslope::vegetation::Vegetation veg=m_cell.get_vegetation();
						real f=0; // Fraction of rainfall to use
						if (Throughfall) f+=1-veg.CanopyClosure;
						if (InterceptedRainfall) f+=veg.CanopyClosure;
						return f * (*m_cell.get_rainfall())(t); 
					}
				}
				void NewNodes() {}
			public:
				bool Throughfall;
				bool InterceptedRainfall;
				Rainfall(cmf::water::flux_node::ptr target,cmf::upslope::Cell & cell,bool getthroughfall=true,bool getintercepted=true) 
					: cmf::water::flux_connection(cell.get_rainfall(),target,
					getthroughfall && getintercepted ? "Rainfall" : getthroughfall ? "Throughfall" : getintercepted ? "Intercepted rain" : "No rain"),
					m_cell(cell),Throughfall(getthroughfall),InterceptedRainfall(getintercepted) {
						NewNodes();
				}
			};
			/// @ingroup connections
			/// A connection routing snowfall (precipitation below freezing Temp) to the snow pack
			class Snowfall : public cmf::water::flux_connection
			{
			protected:
				cmf::upslope::Cell & m_cell;
				virtual real calc_q(cmf::math::Time t)
				{
					bool snow=(m_cell.get_snow()!=0) && m_cell.get_weather(t).T<cmf::atmosphere::Weather::snow_threshold;
					if (snow)
						return (*m_cell.get_rainfall())(t); // Convert mm/day to m3/day
					else
						return 0.0;
				}
				void NewNodes() {}

			public:
				Snowfall(cmf::water::flux_node::ptr target,cmf::upslope::Cell & cell) 
					: cmf::water::flux_connection(cell.get_rainfall(),target,"Snowfall"),m_cell(cell) {
						NewNodes();
				}


			};
		}
	}
}
#endif // AtmosphericFluxes_h__
