

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
#ifndef macropore_h__
#define macropore_h__

#include "../water/WaterStorage.h"
#include "../water/flux_connection.h"
#include "../geometry/geometry.h"
#include "conductable.h"
#include <memory>
#include "../water/flux_connection.h"

namespace cmf {
	namespace upslope {
		class Cell;
		class SoilLayer;
		/// @ingroup storages
		/// @brief An additional water storage for a soil layer to model matrix water and macro pore water seperately. 
		/// @deprecated The MacroPore model is still very experimental and not stable. Only for tryouts!
		///
		/// If present, the soil layer water storage holds the matrix water and the MacroPore holds the 
		/// water in the macro pore. Use cmf::upslope::Macropore::create to create a macropore storage.
		///
		/// Use cmf::upslope::connections::GradientMacroFlow or cmf::upslope::connections::KinematicMacroFlow
		/// to model water flow between macro pores and a lateral connection (\ref latflux) like cmf::upslope::connections::Richards_lateral 
		/// to connect the macro pore with the matrix.
		///
		/// @code{.py}
		/// import cmf
		/// p=cmf.project()
		/// c=p.NewCell(0,0,0,1000,True)
		/// # Add two layers
		/// c.add_layer(0.1, cmf.VanGenuchtenMualem())
		/// c.add_layer(0.2, cmf.VanGenuchtenMualem())
		/// # Create Macropore storages
		/// macropores = [cmf.MacroPore.create(l,porefraction=0.05,Ksat=10.,density=0.05) for l in c.layers]
		/// # Macro pore infiltration
		/// cmf.KinematicMacroFlow(c.surfacewater, macropores[0])
		/// # Macro pore percolation
		/// cmf.KinematicMacroFlow(macropores[0], macropores[1])
		/// # Macro / Micro exchange
		/// for mp in macropores:
		///     cmf.Richards_lateral(mp.layer, mp, FlowWidth = mp.get_flowwidth(), Distance = mp.density/2)
		/// @endcode
		class MacroPore : public cmf::water::WaterStorage //, public cmf::upslope::conductable 
		{
		public:
			typedef std::tr1::shared_ptr<cmf::upslope::MacroPore> ptr;
#ifndef SWIG
			operator ptr() {return std::tr1::static_pointer_cast<MacroPore>(shared_from_this());}
#endif

		private:
			std::tr1::weak_ptr<SoilLayer> m_layer;
			real m_porefraction_min;
			real m_porefraction_max;
		protected:
			virtual real head_to_volume(real head) const;
			virtual real volume_to_head(real volume) const;
			MacroPore(SoilLayer::ptr layer,real porefraction=0.05, real Ksat=10,real density=0.05,real porefraction_wilt = -1., real K_shape=0.0 );
			real exp_of_matrixpotential(real factor=1.) const {
				return exp(minimum(get_layer()->get_matrix_potential(),0.0) * factor);
			}
		public:
			/// Gets the soil layer (matrix water storage) for this macropore storage
			SoilLayer::ptr get_layer() const {
				return m_layer.lock();
			}
			/// The fraction of the macro pores in m3/m3. This adds to the porosity of the layer
			real get_porefraction() const {
				return m_porefraction_min + (1-exp_of_matrixpotential()) * (m_porefraction_max-m_porefraction_min);
			}
			/// The mean distance between the macro pores in m
			real density;
			/// The saturated conductivity of the macropores in m/day
			real Ksat;
			/// Returns the actual conductivity
			virtual real get_K() const;
			/// Returns the actual anisotropic conductivity along a direction \f$K = (k_f \cdot d) K\f$
			virtual real get_K(cmf::geometry::point direction) const {
				return get_K();				
			}
			/// The cell of this macropore
			cmf::upslope::Cell& get_cell() const {
				return get_layer()->cell;
			}
			/// Returns the actual water level in the macropore in m above reference
			real get_potential() const;
			/// Returns the actual stored volume in this macropore in m3
			real get_volume() const;
			/// Returns the capacity of the macropores in m3
			///
			/// \f[V_{max} = \Phi_{macro} A \Delta z\f]
			/// where:
			///  - \f$V_{max}\f$ is the water capacity of the macropore
			///  - \f$\Phi_{macro}\f$ is the fraction of macro pores in the soil in \f$m^3 macro pores/m^3 soil\f$
			///  - \f$A \Delta z\f$ is the total volume of the soil layer (including all pores) in \f$m^3\f$
			real get_capacity() const;
			/// Sets the volume of stored water in m3
			void set_volume(real volume);
			/// Sets the water level in the macropore. Be aware of not setting it below the lower boundary
			void set_potential(real waterhead);
			/// Get the relative water content in the macro pore \f$\theta_{macro} = V_{macro}/V_{max}\f$
			real get_filled_fraction() const;
			/// The approximate length of the aggregate boundaries.
			///
			/// \f[l = \frac{2}{d_{macro}} A\f]
			/// where:
			/// - \f$l\f$ is the length of the aggregate boundaries (in m)
			/// - \f$2\f$ is the number of directions
			/// - \f$d_{macro}\f$ is the mean distance between macropores (density) in m
			/// - \f$A\f$ is the area of the cell
			real get_flowwidth() const;

			/// The shape of the conductivity \f$f_K\f$ in relation to the matric potential of the micropore system
			///
			/// \f[K = K_{macro} \exp(f_K \Psi_M)\f]
			real K_shape;

			/// Creates a macropore water storage for a soil layer.
			///
			/// @returns a MacroPore water storage
			/// @param layer The soil layer holding the macro pore
			/// @param porefraction The relative macro pore content of the soil layer
			/// @param Ksat The saturated conductivity of the macro pores
			/// @param density The mean distance between macropores or mean aggregate size
			/// @param porefraction_wilt The fraction of macropore space at wilting point. Use this parameter for swelling soils
			/// @param K_shape A parameter to change the conductivity of the macropore with the matrix potential
			static MacroPore::ptr create(SoilLayer::ptr layer,real porefraction=0.05, real Ksat=10,real density=0.05,real porefraction_wilt = -1., real K_shape=0.0 );
			static MacroPore::ptr cast(cmf::water::flux_node::ptr node) {
				return std::tr1::dynamic_pointer_cast<MacroPore>(node);
			}

		};
		namespace connections {
			///@ingroup perc
			///
			/// @brief Gradient based flux from macro pore to macro pore.
			/// @deprecated The MacroPore model is still very experimental and not stable. Only for tryouts!
			///
			/// \f[
			/// q = K(\theta) \frac{\Delta \Psi}{\Delta z}
			/// \f]
			class GradientMacroFlow : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::upslope::MacroPore> mp1,mp2;
				std::tr1::weak_ptr<cmf::upslope::conductable> c2;
				void NewNodes()
				{
					mp1=cmf::upslope::MacroPore::cast(left_node());
					mp2=cmf::upslope::MacroPore::cast(right_node());
					c2=cmf::upslope::conductable::cast(right_node());
				}

				virtual real calc_q(cmf::math::Time t) ;
			public:
				GradientMacroFlow(cmf::upslope::MacroPore::ptr left,cmf::water::flux_node::ptr right)
					: flux_connection(left,right,"Gradient based macro pore flow")
				{
					NewNodes();
				}
			};
			///@ingroup perc
			///
			/// @brief Linear storage based flux from macro pore to macro pore.
			/// @deprecated The MacroPore model is still very experimental and not stable. Only for tryouts!
			///
			/// \f[
			/// q = K_{macro} \frac{V_{upper}}{C_{upper}} \left(1-\frac{V_{lower}}{C_{lower}}\right)
			/// \f]
			/// where:
			/// - \f$K_{macro}\f$ is the conductivity of the macro pore storage
			/// - \f$V\f$ is the actual stored water volume in the upper resp. lower macro pore storage
			/// - \f$C\f$ is the capacity of the upper resp. lower macro pore storage
			class KinematicMacroFlow : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::upslope::MacroPore> mp1,mp2;
				std::tr1::weak_ptr<cmf::upslope::conductable> c2;
				void NewNodes();
				virtual real calc_q(cmf::math::Time t) ;
			public:
				/// Creates the connection
				///
				/// @param left,right the nodes between the connection should be created.
				/// @note Either left or right needs to be a MacroPore, left needs to be a water storage
				KinematicMacroFlow(cmf::water::WaterStorage::ptr left,cmf::water::flux_node::ptr right)
					: flux_connection(left,right,"Kinematic based macro pore flow")
				{
					NewNodes();
				}
			};

			/// A gradient based exchange term between macropores and micropores, using a fixed potential for macropores
			///
			/// \f[q = K \frac{\Delta\Psi}{d/2}  A \f]
			/// where:
			/// - \f$K\f$ The conductivity of the aggregate boundary
			/// - \f$\Delta\Psi\f$ The potential difference. Using the air potential as the constant potential for the macro pores, 
			///   you get: \f$\Delta\Psi = \Psi_M(\theta_{micro})\f$ 
			/// - \f$d\f$ the mean aggregate size in m
			/// - \f$A\f$ the crosssection area, given as the flow width (cmf::upslope::MacroPore::get_flowwidth) times layer thickness
			class DiffusiveMacroMicroExchange : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::upslope::MacroPore> mp;
				std::tr1::weak_ptr<cmf::upslope::SoilLayer> sl;
				void NewNodes();
				virtual real calc_q(cmf::math::Time t);
			public:
				DiffusiveMacroMicroExchange(cmf::upslope::SoilLayer::ptr left, cmf::upslope::MacroPore::ptr right) 
					: flux_connection(left,right,"Macro to micro pores exchange")
				{
					NewNodes();
				}


			};
		}
	}
}
#endif
