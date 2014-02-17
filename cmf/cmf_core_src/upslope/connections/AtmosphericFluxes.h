

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
#include "../../atmosphere/precipitation.h"
namespace cmf {
	namespace upslope {

		namespace connections {
			/// @ingroup connections
			
			/// A function to calculate the snow fraction of the precipitation according to the air temperature.
			///
			/// Returns 0.0 for T>+1 degC, 1.0 for T<-1degC and interpolates linear between. 
			/// Values between 0 and 1 can be interpreted as sleet
			real snowfraction(real T);

			/// A connection routing rainfall to surface water and to an eventually existing canopy storage
			class Rainfall : public cmf::water::flux_connection
			{
			protected:
				cmf::upslope::Cell & m_cell;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes() {}
			public:
				/// Is true, if this connection routes throughfall
				bool Throughfall;
				/// Is true, if this connection routes interception
				bool InterceptedRainfall;
				/// Creates a new Rainfall connection
				Rainfall(cmf::water::flux_node::ptr target,cmf::upslope::Cell & cell,bool getthroughfall=true,bool getintercepted=true);
			};
			/// @ingroup connections
			/// A connection routing snowfall (precipitation below freezing Temp) to the snow pack
			class Snowfall : public cmf::water::flux_connection
			{
			protected:
				cmf::upslope::Cell & m_cell;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes() {}

			public:
				Snowfall(cmf::water::flux_node::ptr target,cmf::upslope::Cell & cell) 
					: cmf::water::flux_connection(cell.get_rain_source(),target,"Snowfall"),m_cell(cell) {
						NewNodes();
				}


			};
		}
	}
}
#endif // AtmosphericFluxes_h__
