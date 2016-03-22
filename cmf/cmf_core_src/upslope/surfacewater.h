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

#ifndef surfacewater_h__
#define  surfacewater_h__

#include "../reach/OpenWaterStorage.h"
#include "../reach/ReachType.h"
#include "../math/real.h"
#include "../water/simple_connections.h"
#include "cell.h"

namespace cmf {
	namespace upslope {
		/// A child class of OpenWaterStorage to model surface water on a cell
		///
		/// Specially created to connect a SurfaceWater with other nodes, is
		/// the flux connection cmf::upslope::connections::KinematicSurfaceRunoff
		class SurfaceWater : public cmf::river::OpenWaterStorage {
		public:
			friend class Cell;
			typedef std::shared_ptr<cmf::upslope::SurfaceWater> ptr;
#ifndef SWIG
			operator ptr() {return std::static_pointer_cast<SurfaceWater>(shared_from_this());}
#endif
		private:
			cmf::upslope::Cell& m_cell;
			real m_nManning;
			cmf::river::Prism* m_height_function;
			static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,ptrdiff_t start_at_layer=0);
		protected:
			SurfaceWater(cmf::upslope::Cell& cell);

		public:
			/// Gets the height function (a cmf::river::Prism) for further reference
			virtual const cmf::river::Prism& get_height_function() const {
				return *m_height_function;
			}
			/// Prevents the change of the height function. A SurfaceWater storage has always a Prism height function
			virtual void set_height_function(const cmf::river::IVolumeHeightFunction& val){
				throw std::runtime_error("Cannot set the height function of a surface water. Change the parameters");
			}

			/// get Manning roughness (n) of the surface
			///
			/// From Python use this as a property:
			/// @code{.py}
			/// cell.surfacewater.nManning = 0.1
			/// print cell.surfacewater.nManning
			/// @endcode
			real get_nManning() const {
				return m_nManning;
			}
			/// set Manning roughness (n) of the surface
			///
			/// From Python use this as a property:
			/// @code{.py}
			/// cell.surfacewater.nManning = 0.1
			/// print cell.surfacewater.nManning
			/// @endcode
			void set_nManning(real n) {
				m_nManning = n;
			}
			/// Get water depth at which runoff starts
			///
			/// From Python use this as a property:
			/// @code{.py}
			/// cell.surfacewater.puddledepth = 0.01
			/// print cell.surfacewater.puddledepth
			/// @endcode
			real get_puddledepth() const {
				return m_height_function->RoughThickness;
			}

			/// Set water depth at which runoff starts
			///
			/// From Python use this as a property:
			/// @code{.py}
			/// cell.surfacewater.puddledepth = 0.01
			/// print cell.surfacewater.puddledepth
			/// @endcode
			void set_puddledepth(real puddledepth) {
				m_height_function->RoughThickness = puddledepth;
			}

			/// Get the cell of the surface
			Cell& get_cell() const {return m_cell;}


			static ptr cast(cmf::water::flux_node::ptr node);
		};

		namespace connections {
			/// A connection to route water from a SurfaceWater storage to another node following a topographic gradient
			///
			/// \f[q_{runoff} = A_{cross} d_{eff}^{2/3} \frac{\sqrt{S}}{n}\f]
			/// where:
			/// - \f$q_{runoff}\f$ is the surface runoff
			/// - \f$A_{cross}\f$ is the wetted crossectional flux area, given as \f$d_{eff} \cdot w\f$
			/// - \f$w\f$ is the width of the shared boundary between the surface water storage and the target node
			/// - \f$d_{eff}\f$ is the effective flow depth of the surface water.The effective flow depth is defined as
			///   \f[d_{eff} = \begin{cases} V/A-d_{puddle}\ & V/A>d_{puddle} \\ 0.0 & V/A<=d_{puddle} \end{cases}\f]
			/// - \f$V\f$ the volume of stored water in the surface in \f$m^3\f$
			/// - \f$A\f$ the area of the cell in \f$m^2\f$
			/// - \f$d_{puddle}=V_{puddle}/A\f$ the average depth of water in the surface water needed to start run off
			/// - \f$S = \|\frac{\Delta z\|}{d}\f$ the slope between surfacewater center and the target node
			/// - \f$n\f$ the manning roughness
			///
			/// The KinematicSurfaceRunoff can be used as a cell connecting flux as in:
			/// @code{.py}
			/// cmf.connect_cells_with_flux(p, cmf.KinematicSurfaceRunoff)
			/// @endcode
			/// This results in a connection of the surfacewater storage of each cell with the surface water storages of its neighbors [see](/wiki/CmfTutCell)
			class KinematicSurfaceRunoff : public cmf::water::flux_connection {
			private:
				static void connect_cells(cmf::upslope::Cell& c1,cmf::upslope::Cell& c2,ptrdiff_t dummy);
				std::weak_ptr<cmf::upslope::SurfaceWater> wleft;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes();
				real m_distance, m_flowwidth;
			public:
				/// Creates a KinematicSurfaceRunoff between a SurfaceWater (left) with another (right) node.
				///
				/// @param left A surfacewater storage
				/// @param right The target node
				/// @param flowwidth the length of the shared boundary between left and right in m
				/// @param distance the distance between left and right in m. If d<=0m, the distance is calculated according to the position of _left_ and _right_
				KinematicSurfaceRunoff(cmf::upslope::SurfaceWater::ptr left,cmf::water::flux_node::ptr right,real flowwidth,real distance=-1)
					: flux_connection(left,right,"Kinematic surface runoff"), m_distance(distance), m_flowwidth(flowwidth) {

						NewNodes();
						if (m_distance<=0) {
							m_distance = left->position.distanceTo(right->position);
						}
				}
				static const CellConnector cell_connector;
				typedef std::shared_ptr<KinematicSurfaceRunoff> ptr;

			};
			/// A connection to route water from a SurfaceWater storage to another node following the gradient of the water level
			///
			/// \f[q_{runoff} = A_{cross} d_{eff}^{2/3} \frac{\sqrt{S}}{n}\f]
			/// where:
			/// - \f$q_{runoff}\f$ is the surface runoff
			/// - \f$A_{cross}\f$ is the wetted crossectional flux area, given as \f$d_{eff} \cdot w\f$
			/// - \f$w\f$ is the width of the shared boundary between the surface water storage and the target node
			/// - \f$d_{eff}\f$ is the effective flow depth of the surface water.The effective flow depth is defined as
			///    either the mean of the effective depth of the left and the right node (when use_depthmax=false) or the maximum of 
			///    the efficitve depth. The effective depth for a surfacewater is always defined as:
			///    \f[d_{eff} = \begin{cases} V/A-d_{puddle}\ & V/A>d_{puddle} \\ 0.0 & V/A<=d_{puddle} \end{cases}\f]
			///    The right node might be not a surfacewater. If the right node is an OpenWaterStorage, then the effective depth is 
			///    the depth of the OWS above the cell height of the left surfacewater, given by:
			///    \f[d_{eff,ows} = \Psi_{ows} - z_{cell}\f]
			///    In case of another node, the right node depth equals the effective depth of the left node.
			/// - \f$V\f$ the volume of stored water in the surface in \f$m^3\f$
			/// - \f$A\f$ the area of the cell in \f$m^2\f$
			/// - \f$d_{puddle}=V_{puddle}/A\f$ the average depth of water in the surface water needed to start run off
			/// - \f$S = \|\frac{\Delta h\|}{d}\f$ the slope between surfacewater center potential and the target node potential
			/// - \f$n\f$ the manning roughness
			///
			/// The DiffusiveSurfaceRunoff can be used as a cell connecting flux as in:
			/// @code{.py}
			/// cmf.connect_cells_with_flux(p, cmf.DiffusiveSurfaceRunoff)
			/// @endcode
			/// This results in a connection of the surfacewater storage of each cell with the surface water storages of its neighbors [see](/wiki/CmfTutCell)
			class DiffusiveSurfaceRunoff : public cmf::water::flux_connection {
			private:
				static void connect_cells(cmf::upslope::Cell& c1,cmf::upslope::Cell& c2,ptrdiff_t dummy);
				std::weak_ptr<cmf::upslope::SurfaceWater> wleft;
				std::weak_ptr<cmf::river::OpenWaterStorage> owright;
				std::weak_ptr<cmf::upslope::SurfaceWater> wright;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes();
				real m_distance, m_flowwidth;
			public:
				static void set_linear_slope(real width);
				static real get_linear_slope();
				DiffusiveSurfaceRunoff(cmf::upslope::SurfaceWater::ptr left,cmf::water::flux_node::ptr right,real flowwidth,real distance=-1)
					: flux_connection(left,right,"Diffusive surface runoff"), m_distance(distance), m_flowwidth(flowwidth) {

						NewNodes();
						if (m_distance<=0) {
							m_distance = left->position.distanceTo(right->position);
						}
				}
				static const CellConnector cell_connector;
				typedef std::shared_ptr<DiffusiveSurfaceRunoff> ptr;

			};
		}

	}
}

#endif
