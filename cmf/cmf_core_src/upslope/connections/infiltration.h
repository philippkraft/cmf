

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
#ifndef infiltration_h__
#define infiltration_h__

#include "../../water/flux_connection.h"
#include "../cell.h"
#include "../SoilLayer.h"
#include "../../reach/ManningConnection.h"
namespace cmf {
	namespace upslope {
		namespace connections {
			/// @defgroup infiltration Infiltration
			/// @ingroup connections			

			/// @ingroup infiltration			
			/// Connects the surfacewater and the most upper layer
			///
			/// If UpslopeCell::InfiltrationExcess and Cell is not saturated 
			/// \f[ K_I = \min\left(\frac{\rho_{wg} \Delta z-\Psi_M}{\Delta z \rho_{wg}} K\left(\theta\right),\sum q_{surface\ in}\right) \f]
			/// else
			/// \f[ K_I = \sum q_{surface\ in} \f]
			class MatrixInfiltration : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::upslope::SoilLayer> m_soilwater;
				std::tr1::weak_ptr<cmf::river::OpenWaterStorage> m_surfacewaterstorage;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_surfacewaterstorage=cmf::river::OpenWaterStorage::cast(left_node());
					m_soilwater=cmf::upslope::SoilLayer::cast(right_node());
				}
			public:
				MatrixInfiltration(cmf::upslope::SoilLayer::ptr soilwater,cmf::water::flux_node::ptr surfacewater) 
					: flux_connection(surfacewater,soilwater,"Richards eq. infiltration") {
						NewNodes();
				}
				static void use_for_cell(cmf::upslope::Cell& c)
				{
					new MatrixInfiltration(c.get_layer(0),c.get_surfacewater());
				}
			};
			/// @ingroup infiltration
			/// Connection for infiltration with saturated conductivity
			class CompleteInfiltration : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::upslope::SoilLayer> m_soilwater;
				std::tr1::weak_ptr<cmf::river::OpenWaterStorage> m_surfacewaterstorage;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_soilwater=cmf::upslope::SoilLayer::cast(right_node());
					m_surfacewaterstorage=cmf::river::OpenWaterStorage::cast(left_node());
				}
			public:
				CompleteInfiltration(cmf::upslope::SoilLayer::ptr soilwater,cmf::water::flux_node::ptr surfacewater) 
					: flux_connection(surfacewater,soilwater,"Complete infiltration"){
						NewNodes();
				}

			};
		}
	}
}
#endif // infiltration_h__
