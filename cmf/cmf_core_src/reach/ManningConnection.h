

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
#ifndef ManningConnection_h__
#define ManningConnection_h__

#include "../water/flux_connection.h"
#include "OpenWaterStorage.h"
#include "ReachType.h"
#include "../math/real.h"
#include "../upslope/cell.h"
namespace cmf {
	namespace river {

		/// @defgroup manning Surface water connections
		/// @ingroup connections
		
		/// @ingroup manning
		/// @brief Calculates the flux between two open water bodies, using Manning's equation
        ///
		///
		/// This is the base class for a kinematic wave approach (topography driven)
		/// and a diffusive wave approach (water table driven). The only difference
		/// between both approaches is the calculation of the flux driving slope.
		/// For the model, one of ManningKinematic or ManningDiffusive connection 
		/// is selected
		///
        /// \f{eqnarray*}
		/// q_{Manning}&=& A R^{\frac 2 3} \sqrt{\frac {\Delta_z} n} \\
		/// A &=& \frac V l \mbox{, (Crosssectional area of the wetted crossection, Volume per length)} \\
		/// R &=& \frac A {P(d)} \\
		/// P(d) &=& \mbox{ the perimeter of the wetted crosssection, a function of reach depth} \\
		/// d(V) &=& \mbox{ the depth of the reach, a function of the volume} \\
		/// \Delta_z &=& \frac{\|z_1 - z_2\|}{l} \mbox{ Slope of the reach} \\
		/// n&=&\mbox{Manning friction number}
		/// \f}
		///
		/// For the kinematic wave the slope of the river bed is used as slope \f$\Delta_z = \frac{|z_1 - z_2\|}{l}\f$, 
		///
		/// while for the diffusive wave the slope is calculated from the actual water head. \f$\Delta_z = \|\frac{h_1 - h_2}{l}\f$
		class Manning : public cmf::water::flux_connection
		{
		protected:

			virtual real get_slope(cmf::water::flux_node::ptr lnode, cmf::water::flux_node::ptr rnode, real d)=0;
			static void connect_cells(cmf::upslope::Cell& c1,cmf::upslope::Cell& c2,bool is_diffusive_wave);
			std::weak_ptr<cmf::river::OpenWaterStorage> w1,w2;
			virtual real calc_q(cmf::math::Time t);
			void NewNodes()
			{
				w1=cmf::river::OpenWaterStorage::cast(left_node());
				w2=cmf::river::OpenWaterStorage::cast(right_node());
			}
			Manning(cmf::river::OpenWaterStorage::ptr left,cmf::water::flux_node::ptr right,
				const cmf::river::IVolumeHeightFunction& reachtype,bool diffusive_wave)
				: flux_connection(left,right,diffusive_wave ? "Manning (diff. wave)" : "Manning (kin. wave)"),	flux_geometry(reachtype), is_diffusive_wave(diffusive_wave) {
					NewNodes();
			}

		public:
			bool is_diffusive_wave;
			cmf::river::volume_height_function flux_geometry;

			typedef std::shared_ptr<Manning> ptr;


		};
        
		/// @ingroup manning
		/// Connecting surface water bodies using a diffusive wave. 
		/// This approach might not be numerical stable for deep water with small gradient
		/// cmf for experimental reasons
		/// \f{eqnarray*}
		/// q_{Manning}&=& A R^{\frac 2 3} \sqrt{\frac {\Delta_z} n} \\
		/// A &=& \frac V l \mbox{, (Crosssectional area of the wetted crossection, Volume per length)} \\
		/// R &=& \frac A {P(d)} \\
		/// P(d) &=& \mbox{ the perimeter of the wetted crosssection, a function of reach depth} \\
		/// d(V) &=& \mbox{ the depth of the reach a function of the volume} \\
		/// \Delta_z = \|\frac{h_1 - h_2}{l} \mbox{ Slope of the reach waterlevels} \\
        /// n&=&\mbox{Manning friction number}
		/// \f}

		class Manning_Diffusive: public Manning
		{
		private:
			static void connect_cells(cmf::upslope::Cell& c1,cmf::upslope::Cell& c2,ptrdiff_t dummy)
			{		cmf::river::Manning::connect_cells(c1,c2,true);		}
		protected:
			virtual real get_slope(cmf::water::flux_node::ptr lnode, cmf::water::flux_node::ptr rnode, real d);
		public:
			typedef std::shared_ptr<Manning_Diffusive> ptr;
			static const cmf::upslope::CellConnector cell_connector;
			/// A parameter to linearize the dependency of q on slope during levelling out
			real linear_slope_width;
			/// Creates a diffusive wave connection between to open water storages.
			///
			/// @param left,right The nodes to be connected by the diffusive wave. Left needs to be an open water storage
			/// @param reachtype The channel geometry
			Manning_Diffusive(cmf::river::OpenWaterStorage::ptr left,cmf::water::flux_node::ptr right,const cmf::river::IChannel& reachtype)
				: Manning(left,right,reachtype,true), linear_slope_width(0.0)
			{			}
		};

		/// @ingroup manning
		/// Connecting surface water bodies using a kinematic wave. 
		/// Note the fixed gradient \f$\Delta_z\f$
		/// \f{eqnarray*}
		/// q_{Manning}&=& A R^{\frac 2 3} \sqrt{\frac {\Delta_z} n} \\
		/// A &=& \frac V l \mbox{, (Crosssectional area of the wetted crossection, Volume per length)} \\
		/// R &=& \frac A {P(d)} \\
		/// P(d) &=& \mbox{ the perimeter of the wetted crosssection, a function of reach depth} \\
		/// d(V) &=& \mbox{ the depth of the reach a function of the volume} \\
		/// \Delta_z &=& \frac{\|z_1 - z_2\|}{l} \mbox{ Slope of the reach} \\
        /// n&=&\mbox{Manning friction number}
		/// \f}

		
		class Manning_Kinematic: public Manning
		{
		private:
			static void connect_cells(cmf::upslope::Cell& c1,cmf::upslope::Cell& c2,ptrdiff_t dummy)
			{		cmf::river::Manning::connect_cells(c1,c2,false);		}
		protected:
			virtual real get_slope(cmf::water::flux_node::ptr lnode, cmf::water::flux_node::ptr rnode, real d);
		public:
			typedef std::shared_ptr<Manning_Kinematic> ptr;
			static const cmf::upslope::CellConnector cell_connector;
			/// Creates a kinematic wave connection between to open water storages.
			///
			/// @param left,right The nodes to be connected by the kinematic wave. Left needs to be an open water storage
			/// @param reachtype The channel geometry
			Manning_Kinematic(cmf::river::OpenWaterStorage::ptr left,cmf::water::flux_node::ptr right,const cmf::river::IChannel& reachtype)
				: Manning(left,right,reachtype,false)
			{			}
		};
		/*
		// Just a try is in concurrence with KinematicSurfaceRunoff
		class KinematicSheetFlow: public cmf::water::flux_connection {
		protected:
			static void connect_cells(cmf::upslope::Cell& c1,cmf::upslope::Cell& c2, int i);
			std::weak_ptr<cmf::river::OpenWaterStorage> w1,w2;
			virtual real calc_q(cmf::math::Time t);
			void NewNodes()
			{
				w1=cmf::river::OpenWaterStorage::cast(left_node());
				w2=cmf::river::OpenWaterStorage::cast(right_node());
			}

		public:
			typedef std::shared_ptr<Manning> ptr;
			static const cmf::upslope::CellConnector cell_connector;
			KinematicSheetFlow(cmf::river::OpenWaterStorage::ptr left,cmf::water::flux_node::ptr right)
				: flux_connection(left,right,"Kinematic sheet overland flow")
			{
				NewNodes();
			}

		};
		*/
	}
}
#endif // ManningConnection_h__
