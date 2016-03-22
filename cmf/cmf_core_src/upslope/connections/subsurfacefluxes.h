

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
#ifndef subsurfacefluxes_h__
#define subsurfacefluxes_h__

#include "../../water/flux_connection.h"
#include "../cell.h"
#include "../Topology.h"
#include "../SoilLayer.h"

namespace cmf {
	namespace upslope {
		/// Contains different kinds of connections between the water storages of a cell
		namespace connections {
			/// @defgroup latflux lateral subsurface fluxes
			/// @ingroup connections

			/// @ingroup latflux
			/// An abstract base class for lateral subsurface fluxes
			class lateral_sub_surface_flux : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::SoilLayer> sw1,sw2;
				std::weak_ptr<cmf::upslope::conductable> c2;
				real 
					flow_width, distance;

				virtual real calc_q(cmf::math::Time t)=0;
				virtual void NewNodes();

				lateral_sub_surface_flux(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right, std::string Name,real FlowWidth,real Distance=0);

			};

			/// @ingroup latflux
			/// Calculates the lateral flow using the gravitational potential gradient only
			///
			/// \f[ q_{lat} = \frac{\Delta \Psi_G}{\|C_1-C_2\|} \frac 12 (T(C_1)+T(C_2)) w \f]
			/// where
			/// - \f$ \Delta \Psi_G \f$ is the gravitational potential difference
			/// - \f$ \|C_1-C_2\| \f$ is the distance from Cell 1 to Cell 2
			/// - \f$ T(C) \f$ is the transmissivity of cell C, calculated by SoilType::Transmissivity
			/// - \f$ w \f$ is the width of the connection of the cells
			class Darcy : public lateral_sub_surface_flux {
			protected:
				virtual real calc_q(cmf::math::Time t) ;
				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,ptrdiff_t start_at_layer=0);
			public:
				static const CellConnector cell_connector;
				Darcy(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right,real FlowWidth,real Distance=0)
					: lateral_sub_surface_flux(left,right,"Darcy flow",FlowWidth,Distance) 
				{}

			};
			/// @ingroup latflux
			///	Calculates the lateral flow using the topographic gradient
			///
			/// \f[ q_{lat} = \frac{z_1 - z_2}{\|C_1-C_2\|} \frac 12 (T(C_1)+T(C_2)) w \f]
			/// where
			/// - \f$ z_1 - z_2 \f$ is the topographic height difference
			/// - \f$ \|C_1-C_2\| \f$ is the distance from Cell 1 to Cell 2
			/// - \f$ T(C) \f$ is the transmissivity of cell C, calculated by SoilType::Transmissivity
			/// - \f$ w \f$ is the width of the connection of the cells
			class TopographicGradientDarcy : public lateral_sub_surface_flux {
			protected:
				virtual real calc_q(cmf::math::Time t) ;
				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,ptrdiff_t start_at_layer=0);
			public:
				static const cmf::upslope::CellConnector cell_connector;
				/// @brief Creates the connection
				///
				/// @param left Left node of the connection (needs to be soil water storage)
				/// @param right Right node of the connection (can be any node)
				/// @param FlowWidth the width of the connection - is multiplied by layer thickness to get the interface area
				/// @param Distance the length of the connection. If 0, the distance is calculated from the position of the nodes
				TopographicGradientDarcy(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right,real FlowWidth,real Distance=0)
					: lateral_sub_surface_flux(left,right,"Topographic gradient",FlowWidth,Distance)
				{ }


			};


			/// @ingroup latflux
			/// A simple kinemtic wave model for subsurface flux
			///
			/// \f[ q = \frac{\Delta z_{surface}}{d} K(\theta)_{source} A_{cross} \f]
			class DarcyKinematic : public lateral_sub_surface_flux
			{
			protected:
				virtual real calc_q(cmf::math::Time t) ;

				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,ptrdiff_t start_at_layer=0);
			public:
				/// @brief Creates the connection
				///
				/// @param left Left node of the connection (needs to be soil water storage)
				/// @param right Right node of the connection (can be any node)
				/// @param FlowWidth the width of the connection - is multiplied by layer thickness to get the interface area
				/// @param Distance the length of the connection. If 0, the distance is calculated from the position of the nodes
				DarcyKinematic(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right,real FlowWidth, real Distance=0)
					: lateral_sub_surface_flux(left,right,"A Darcian kinematic wave approach",FlowWidth,Distance)
				{			}

				static const CellConnector cell_connector;

			};

			/// @ingroup latflux
			/// Calculates the flux using Richard's equation for adjacent layers
			/// \f[
			/// 	q_{lat} = \frac{\Psi_1 - \Psi_2}{\|C_1-C_2\|}
			/// 	K(\theta) A
			/// \f]
			/// where:
			/// - \f$q_{lat}\f$ the lateral flow in \f$m^3/day\f$
			/// - \f$\Psi_i\f$ the head of node i
			/// - \f$ \|C_1-C_2\| \f$ is the distance from Cell 1 to Cell 2
			/// - \f$K(\theta_{1,2}) = \sqrt{K(\theta_1) K(\theta_2)}\f$
			/// - \f$A\f$ the crosssectional area of the interface between storages 1 and 2
			class Richards_lateral: public lateral_sub_surface_flux
			{
			protected:
				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,ptrdiff_t start_at_layer=0);
				virtual real calc_q(cmf::math::Time t) ;

			public:
				real flow_thickness;
				bool wet_right_node;
				/// @brief Creates the connection
				///
				/// @param left Left node of the connection (needs to be soil water storage)
				/// @param right Right node of the connection (can be any node)
				/// @param FlowWidth the width of the connection - is multiplied by layer thickness to get the interface area
				/// @param Distance the length of the connection. If 0, the distance is calculated from the position of the nodes
				Richards_lateral(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right,real FlowWidth=0,real Distance=0)
					: lateral_sub_surface_flux(left,right,"Richards eq. (lat)",FlowWidth,Distance), wet_right_node(false)
				{
					flow_thickness=left->get_thickness();
					if (!sw2.expired())
					{
						flow_thickness=minimum(flow_thickness,sw2.lock()->get_thickness());
					}
				}
				static void usebaseflow(bool use);
				static const CellConnector cell_connector;
			};

			/// @ingroup latflux
			/// Calculates a flux from a soil layer using TOPMODELs (Beven & Kirkby 1979) exponential transmissivity concept
			///
			/// \f[ T = T_0 \exp(-D_i/m) \f]
			/// where:
			/// - \f$T\f$ is the actual transmissivity of the profile in \f$m^2/day\f$
			/// - \f$T0\f$ is the transmissivity of the profile at saturation
			/// - \f$D_i\f$ is the drained depth in m, calculated as \f$(C-V)/A\f$, the capacity of the layer - volume per area
			/// - \f$m\f$ a scaling factor in m
			///
			/// By using the transmissivity in Darcy's law and assuming the GW gradient to be parallel to the topographic slope
			/// we get for the flow:
			/// \f[ q = T_0 \exp(-D_i/m) w s\f]
			/// where:
			/// - \f$q\f$ is the flux in \f$m^3/day\f$
			/// - \f$w\f$ is the flow width (unit contour length)
			/// - \f$s\f$ is the topographic slope between layer and outlet
			///
			/// @note TOPMODEL is based on the concept of drained depth, not, as cmf on the concept of stored volume.
			/// Hence, negative volumes can occur if
			class TOPModelFlow : public cmf::water::flux_connection {
			protected:
				virtual real calc_q(cmf::math::Time t);
				std::weak_ptr<cmf::upslope::SoilLayer> sw1;
				virtual void NewNodes() {
					sw1 = cmf::upslope::SoilLayer::cast(this->left_node());
				}
			public:
				real 
					flow_width, distance,T0,m;
				TOPModelFlow(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right,real T0,real m,real flowwidth, real distance=0);
		};


	}
  }
}

#endif // subsurfacefluxes_h__

