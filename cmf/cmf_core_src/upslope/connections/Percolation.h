

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
#ifndef Percolation_h__
#define Percolation_h__


#include "../../water/flux_connection.h"
#include "../cell.h"
#include "../Topology.h"
#include "../SoilLayer.h"
#include "../conductable.h"

namespace cmf {
	namespace upslope {
		namespace connections {
			/// @defgroup perc Percolation
  			/// @ingroup connections
			
			/// @ingroup perc
			/// A tipping bucket percolation approach similar to the approach in SWAT
			///
			/// \f{eqnarray*} q_{perc} &=& V_{H_2O,drain}\left(1-e^{-\frac 1{-TT_{perc}}}\right) \\
			/// TT_{perc} &=& \frac{V_{pores,drain} - V_{field cap.}}{K_{sat}} \\
			/// V_{x,drain} &=& V_{x,drain} - V_{field cap.} \f}
			/// 
			class SWATPercolation : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::SoilLayer> sw1,sw2;
				void NewNodes()
				{
					sw1=cmf::upslope::SoilLayer::cast(left_node());
					sw2=cmf::upslope::SoilLayer::cast(right_node());
				}
				virtual real calc_q(cmf::math::Time t);

			public:
				/// Uses this connection for a layers of the cell
				static void use_for_cell(cmf::upslope::Cell & cell,bool no_override=true);
				SWATPercolation(cmf::upslope::SoilLayer::ptr upperLayer,cmf::upslope::SoilLayer::ptr lowerLayer)
					: flux_connection(upperLayer,lowerLayer,"SWAT percolation") {
						NewNodes();
				}

			};
			/// @ingroup perc
			/// Calculates flow according to the Richards equation
			///
			/// \f{eqnarray*}
			/// q_{Richards} &=& \frac{\Delta\Psi_{tot}}{d} K(\theta) A \\
			/// \Psi_{tot} &= &\Psi_{M}(\theta) + h
			/// \f}
			/// where
			/// - \f$ \Delta\Psi_{tot} [m]\f$ is the difference of the total water potentials of the two soil layers
			/// - \f$ d [m]\f$ is the distance between the two soil layers
			/// - \f$ K(\theta)\left[\frac m{day}\right]\f$ is the geometric mean conductivity (see SoilType::Kunsat)
			/// - \f$ A [m^2]\f$ is the crosssectional area of the flux
			/// - \f$ \Psi_M(\theta) [m]\f$ is the matrix potential (see SoilType::MatrixPotential)
			/// - \f$ h [m]\f$ is the height of a soil layer above sea level
			class Richards : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::SoilLayer> sw1,sw2;
				std::weak_ptr<cmf::upslope::conductable> c2;
				void NewNodes()
				{
					sw1=cmf::upslope::SoilLayer::cast(left_node());
					sw2=cmf::upslope::SoilLayer::cast(right_node());
					c2=cmf::upslope::conductable::cast(right_node());
				}

				virtual real calc_q(cmf::math::Time t) ;
			public:
				/// Creates a Richards equation connection between two soil layers (left and right, rather top and bottom) of the same cell
				Richards(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right)
					: flux_connection(left,right,"Richards eq.")
				{
					NewNodes();
				}
				/// @brief Connects all layers of the cell with the Richards equation. 
				/// 
				/// Connects all layers of the cell with the Richards equation. If no_override=true (default), then only unconnected 
				/// layers will be connected. Existing connections will be replaced by Richards otherwise
				static void use_for_cell(cmf::upslope::Cell & cell,bool no_override=true);
			};

			/// @ingroup perc
			/// Calculates flow according to a simplified Richards equation
			///
			/// \f{eqnarray*}
			/// q_{Richards} &=& (K(\theta) - K(\theta_r)) A \\
			/// \f}
			/// where
			/// - \f$ d [m]\f$ is the distance between the two soil layers
			/// - \f$ K(\theta)\left[\frac m{day}\right]\f$ is the geometric mean conductivity (see SoilType::Kunsat)
			/// - \f$ A [m^2]\f$ is the crosssectional area of the flux
			class SimplRichards : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::SoilLayer> sw1,sw2;
				void NewNodes()
				{
					sw1=cmf::upslope::SoilLayer::cast(left_node());
					sw2=cmf::upslope::SoilLayer::cast(right_node());
				}

				virtual real calc_q(cmf::math::Time t) ;
			public:

				SimplRichards(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right)
					: flux_connection(left,right,"Richards eq.")
				{
					NewNodes();
				}
				/// @brief Connects all layers of the cell with the SimplRichards equation. 
				/// 
				/// Connects all layers of the cell with the SimplRichards equation. If no_override=true (default), then only unconnected 
				/// layers will be connected. Existing connections will be replaced by SimplRichards otherwise
				static void use_for_cell(cmf::upslope::Cell & cell,bool no_override=true);
			};


			/// @ingroup perc
			/// Calculates a free drainage (unit gradient) from a layer to somewhere else
			///
			/// \f[ q = K(\theta) A\f]
			/// where:
			/// - \f$q\f$ Flux from the layer to the other side of the connection in \f$m^3/day\f$
			/// - \f$K(\theta)\f$ Actual conductivity in \f$m/day\f$ depending on the water content of the layer \f$\theta\f$
			/// - \f$A\f$ Cell area in \f$m^2\f$
			class FreeDrainagePercolation : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::SoilLayer> sw1;
				void NewNodes()
				{
					sw1=cmf::upslope::SoilLayer::cast(left_node());
				}
				virtual real calc_q(cmf::math::Time t);
			public:
				FreeDrainagePercolation(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right)
					: flux_connection(left,right,"Free drainage percolation")
				{
					NewNodes();
				}
			};


			///@ingroup perc
			///
			/// @brief A simplification of macro pore flux for swelling soils
			/// 
			/// Connects the surfacewater of the cell with deeper layers, assuming the presence of cracks.
			/// At saturation level of the target layer, the cracks are closed
			/// \f[q_{crack} = K_{max,crack} \left(1-\left(\frac{w-w_0}{1-w_0}\right)^\beta\right) A\f]
			/// where:
			///  - \f$q_{crack}\f$ is the flux from the surface water to the target layer in m3/day
			///  - \f$K_{max,crack}\f$ is the maximum conductivity of the cracks in m/day
			///  - \f$w\f$ is the actual wetness of the target layer
			///  - \f$w_0\f$ is saturation, where the shrinkage of the cracks starts
			///  - \f$\beta\f$ is an empirical shape parameter of the crack size/wetness relation
			///  - \f$A\f$ is the area of the cell
			class LayerBypass: public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::SoilLayer> Sl;
				void NewNodes() {
					Sl = cmf::upslope::SoilLayer::cast(right_node());
				}
				virtual real calc_q(cmf::math::Time t);
			public:
				/// maximum conductivity of the cracks in m/day
				real Kmax;
				/// is the actual wetness of the target layer
				real w0;
				/// is an empirical shape parameter of the crack size/wetness relation
				real beta;
				
				/// @returns the actual crack conductivity
				/// @param w the wetness of the target layer
				real K(real w);

				/// @brief Creates a layer bypass connection
				///
				/// @param left The source of the flux, usually the surfacewater of the cell
				/// @param right The target soil layer
				/// @param Kmax maximum conductivity of the cracks in m/day
				/// @param w0 is the actual wetness of the target layer
				/// @param beta is an empirical shape parameter of the crack size/wetness relation
				LayerBypass(cmf::water::flux_node::ptr left,cmf::upslope::SoilLayer::ptr right, real Kmax=100.,real w0=0.0,real beta=1.0);
			};



		}
	}
}
#endif // Percolation_h__
