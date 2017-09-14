

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
#include "../surfacewater.h"
#include "../../math/time.h"

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
				std::weak_ptr<cmf::upslope::SoilLayer> m_soilwater;
				std::weak_ptr<cmf::river::OpenWaterStorage> m_surfacewaterstorage;
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
				/// Creates the connection between surfacewater and first soil layer
				static void use_for_cell(cmf::upslope::Cell& c)
				{
					new MatrixInfiltration(c.get_layer(0),c.get_surfacewater());
				}
			};
			/// @ingroup infiltration
			/// Connects the surfacewater and the most upper layer using a Green-Ampt equation like infiltration
			///
			/// The Green-Ampt formula is given as:
			/// \f[q(t) = -K_s \frac{dh}{dz} A\f]
			/// where:
			/// - \f$q(t)\f$ is the infiltration rate in m3/day
			/// - \f$K_s\f$ is the saturated conductivity in m/day
			/// - \f$\frac{dh}{dz}\f$ is the hydraulic gradient in the wetting front
			/// - \f$A\f$ is the surface area of the cell
			///
			/// The gradient in the wetting front is calculated as:
			/// \f[\frac{dh}{dz} = \frac{h_f - h_0}{Z_f} = \frac{|\Psi_f| + Z_f}{Z_f}\f]
			/// where:
			/// - \f$h_f\f$ is the hydraulic head at the bottom of the wetting front in m
			/// - \f$h_0\f$ is the hydraulic head at the surface in m
			/// - \f$Z_f\f$ is the length of the wetting front in m
			///
			/// Since \f$Z_f\f$ is unknown, the depth of the wetting front can be approximated by:
			/// \f[Z_f = \frac{F}{\theta_s - \theta_i}\f]
			/// with:
			/// - \f$F\f$ the accumulated volume per area of infiltrated water
			/// - \f$\theta_s, \theta_i\f$ the volumetric water content at saturation resp. at start of the infiltration
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
			class GreenAmptInfiltration : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::SoilLayer> m_soilwater;
				std::weak_ptr<cmf::upslope::SurfaceWater> m_surfacewaterstorage;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_soilwater=cmf::upslope::SoilLayer::cast(right_node());
					m_surfacewaterstorage=cmf::upslope::SurfaceWater::cast(left_node());
				}
			public:
				GreenAmptInfiltration(cmf::upslope::SoilLayer::ptr soilwater,cmf::water::flux_node::ptr surfacewater) 
					: flux_connection(surfacewater,soilwater,"Green Ampt infiltration"){
						NewNodes();
				}
				/// Creates the connection between surfacewater and first soil layer
				static void use_for_cell(cmf::upslope::Cell& c) {
					new cmf::upslope::connections::GreenAmptInfiltration(c.get_layer(0),c.get_surfacewater());
				}
			};

			/// @ingroup infiltration
			/// Connects the surfacewater and the most upper layer using a simplified infiltration model
			/// suitable for conceptional models. 
			///
			/// \f[ q_{inf} = \left(1-e_{sat}\left(W, W_0\right)\right)  q_{inf,pot} \f]
			/// where:
			///   - \f$q_{inf}\f$ Effective infiltration from surface to first layer (soil)
			///   - \f$e_{sat}(W, W_0)\f$ Saturation excess, ranging from 0 (nowhere saturated soil layer) to 1 (fully saturated).
			///      \f$W\f$ is the average wetness calculated from the soil layer, \f$W_0\f$ is a parameter denoting the wetness, where
			///      50% of the layer is saturated using a sigmoidal function:
			///        - \f$e_{sat}(W_{soil}, W_0) = \left(1+e^{-(W-W_0)0.2(1-W_0)}\right)^{-1}\f$ 
			///   - \f$q_{inf,pot}\f$ is the potential infiltration, given by the incoming fluxes limited by the saturated conductivity:
			///        - \f$q_{inf,pot} = \min(q_{in}, K_{sat} A)\f$
			///		   - \f$q_{in}\f$ Sum of incoming fluxes to the surfacewater in \f$m^3/day\f$
			///        - \f$K_{sat}\f$ Saturated conductivity in \f$m/day\f$
			///        - \f$A\f$ Cell area in \f$m^2\f$
			class SimpleInfiltration : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::SoilLayer> m_soilwater;
				std::weak_ptr<cmf::upslope::SurfaceWater> m_surfacewaterstorage;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_soilwater=cmf::upslope::SoilLayer::cast(right_node());
					m_surfacewaterstorage=cmf::upslope::SurfaceWater::cast(left_node());
				}
			public:
				/// Wetness at which only 50% of the potential infiltration \f$p_{inf,pot}\f$
				real W0;
				/// Creates the connection between surfacewater and first soil layer
				/// @param soilwater the infiltration target
				/// @param surfacewater the infiltration source
				/// @param W0 the 50% saturation value
				SimpleInfiltration(cmf::upslope::SoilLayer::ptr soilwater,cmf::water::flux_node::ptr surfacewater,real W0=0.9);
				
				static void use_for_cell(cmf::upslope::Cell& c) {
					new cmf::upslope::connections::SimpleInfiltration(c.get_layer(0),c.get_surfacewater());
				}

			};
		}
	}
}
#endif // infiltration_h__
