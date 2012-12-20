

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
		/// Connects the surfacewater and the most upper layer using a Richards equation like infiltration
		/// model.
		///
		/// The potential infiltration is calculated according to the Richards equation.
		/// The gradient is from the cell surface to the center of the first layer and the
		/// conductivity is the geometric mean of the wetted surface (\f$K_{sat}\f$) and the conductivity
		/// of the layer center (\f$K(\theta_{layer})\f$
		/// \f{eqnarray*}
		/// q_{max} &=& \frac{\Psi_{surface} - \Psi_{soil}}{\Delta z} K A_{cell} \\
		/// K &=& \sqrt{K\left(\theta_{layer}\right)K_{sat}} \\
		/// \Delta z &=& z_{cell} - z_{layer center}
		/// \f}
		///
		/// If the surface water is modeled by a distinct water storage, the actual infiltration is
		/// given as the product of the potential infiltration with the coverage of the surface water
		/// cmf::upslope::Cell::surface_water_coverage
		/// \f[q_{act} = q_{max} \frac{A_{water}}{A_{cell}}\f]
		///
		/// If the surface water is no storage on its own, but just a water distribution node,
		/// the actual infiltration is the minimum of the potential infiltration and the current
		/// inflow (rain, snow melt) to the surface
		/// \f[q_{act} = \min\left(q_{max}, \sum{q_{in,surfacewater}}\right)\f]


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
			/// Connects the surfacewater and the most upper layer using a Richards equation like infiltration
			/// model but assuming saturated conductivity as the potential infiltration rate into the first layer.
			///
			/// The potential infiltration is calculated according to the Richards equation.
			/// The gradient is from the cell surface to the center of the first layer and the
			/// conductivity is \f$K_{sat}\f$
			/// \f{eqnarray*}
			/// q_{max} &=& \frac{\Psi_{surface} - \Psi_{soil}}{\Delta z} K A_{cell} \\
			/// K &=& \sqrt{K\left(\theta_{layer}\right)K_{sat}} \\
			/// \Delta z &=& z_{cell} - z_{layer center}
			/// \f}
			///
			/// If the surface water is modeled by a distinct water storage, the actual infiltration is
			/// given as the product of the potential infiltration with the coverage of the surface water
			/// cmf::upslope::Cell::surface_water_coverage
			/// \f[q_{act} = q_{max} \frac{A_{water}}{A_{cell}}\f]
			///
			/// If the surface water is no storage on its own, but just a water distribution node,
			/// the actual infiltration is the minimum of the potential infiltration and the current
			/// inflow (rain, snow melt) to the surface
			/// \f[q_{act} = \min\left(q_{max}, \sum{q_{in,surfacewater}}\right)\f]


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
