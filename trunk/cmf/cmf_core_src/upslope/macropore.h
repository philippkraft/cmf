

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
#include "../cmfmemory.h"
#include "../water/flux_connection.h"

namespace cmf {
	namespace upslope {
		class Cell;
		class SoilLayer;
		/// @defgroup MacroPore Macro pore water storage and its fluxes for a dual porosity system
		/// @ingroup storages
		/// For certain applications, modelling the soil solution in one soil layer as a homogeneous mixed
		/// solution is not sufficient. In the presence of macropores and other preferential flow paths
		/// at least an dual porosity model is needed. To use dual porosity setups in cmf, existing soil layers
		/// are extended by a MacroPore storage. This group contains the macropore water storage and serveral
		/// connections to calculate the flux between macropores and between macropores and the soil matrix.
		///
		/// @note The macropore part of cmf is still under development. Use it only if you are willing to 
		/// check the model behaviour down to the C++ source. Suggestions, patches and test cases are more than welcome

		/// @ingroup MacroPore
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
			typedef std::shared_ptr<cmf::upslope::MacroPore> ptr;
#ifndef SWIG
			operator ptr() {return std::static_pointer_cast<MacroPore>(shared_from_this());}
#endif

		private:
			std::weak_ptr<SoilLayer> m_layer;
		protected:
			virtual real head_to_volume(real head) const;
			virtual real volume_to_head(real volume) const;
			MacroPore(SoilLayer::ptr layer,real porefraction_min=0.05, real Ksat=10,real density=0.05,real porefraction_max = -1., real K_shape=0.0 );
			real exp_of_matrixpotential(real factor=1.) const {
				return exp(minimum(get_layer()->get_matrix_potential(),0.0) * factor);
			}
		public:
			real porefraction_min;
			real porefraction_max;
			/// Gets the soil layer (matrix water storage) for this macropore storage
			SoilLayer::ptr get_layer() const {
				return m_layer.lock();
			}
			/// The fraction of the macro pores in m3/m3. This adds to the porosity of the layer
			real get_porefraction() const;
			/// The mean distance between the macro pores in m
			real density;
			/// The saturated conductivity of the macropores in m/day
			real Ksat;
			/// The layer's cracking wetness, crack dynamic starts below this value. The value is ignored if porefraction_min = porefraction_max
			real crack_wetness;
			/// Returns the actual conductivity
			virtual real get_K() const;
			/// Returns the actual anisotropic conductivity along a direction \f$K = (k_f \cdot d) K\f$
			virtual real get_K(cmf::geometry::point direction) const {
				return get_K();				
			}

			/// Returns the crack width for a prismatic crackstructure
			///
			/// For a prismatic crack structure, the porefraction in m3/m3 equals the vertical crack area in m2/m2.
			/// The length of equally spaced cracks is in one direction the inverse of the density and twice the
			/// length for two directions.
			/// \f[ l_{crack} [m/m^2]= 2 \frac {1}{d[m]}\f]
			/// If we again ignore the fact that the spacing of the cracking crossings is counted double, the crack width is:
			/// \f[ w_{crack}[m] = \frac{A_{crack}[m^2/m^2]}{l_{crack}[m/m^2]} \f]
			/// Combining both eq. above:
			/// \f[ w_{crack}[m] = A_{crack}[m^2/m^2]\frac{d[m]}{2} \f]
			virtual real get_crackwidth() const {
				return get_porefraction() * density / 2.;
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
				return std::dynamic_pointer_cast<MacroPore>(node);
			}

		};
		namespace connections {
			class BaseMacroFlow  : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::MacroPore> mp1,mp2;
				std::weak_ptr<cmf::upslope::conductable> c2;
				void NewNodes();
				BaseMacroFlow(cmf::water::WaterStorage::ptr left,cmf::water::flux_node::ptr right,std::string connectionname)
					: flux_connection(left,right,connectionname)
				{
					NewNodes();
				}
			};
			///@ingroup MacroPore
			///
			/// @brief Gradient based flux from macro pore to macro pore.
			/// @deprecated The MacroPore model is still very experimental and not stable. Only for tryouts!
			///
			/// \f[
			/// q = K(\theta) \frac{\Delta \Psi}{\Delta z}
			/// \f]
			class GradientMacroFlow : public BaseMacroFlow {
				virtual real calc_q(cmf::math::Time t) ;
			public:
				GradientMacroFlow(cmf::upslope::MacroPore::ptr left,cmf::water::flux_node::ptr right)
					: BaseMacroFlow(left,right,"Gradient based macro pore flow")
				{
				}
			};
			/// @ingroup MacroPore
			///
			/// @brief Linear storage based flux from macro pore to macro pore.
			/// @deprecated The MacroPore model is still very experimental and not stable. Only for tryouts!
			///
			/// \f[
			/// q = A_{cell} K_{macro} \left(\frac{V_{upper}}{C_{upper}}\right)^\beta \left(1-\frac{V_{lower}}{C_{lower}}\right)
			/// \f]
			/// where:
			/// - \f$A_{cell}\f$ is the area of the owning cell in m2
			/// - \f$K_{macro}\f$ is the conductivity of the macro pore storage
			/// - \f$V\f$ is the actual stored water volume in the upper resp. lower macro pore storage
			/// - \f$C\f$ is the capacity of the upper resp. lower macro pore storage
			class KinematicMacroFlow : public BaseMacroFlow {
			protected:
				virtual real calc_q(cmf::math::Time t) ;
			public:
				///
				/// Creates the connection
				///
				/// @param left,right the nodes between the connection should be created.
				/// @param beta a conceptional curve shape parameter for the relation between storage and outflow
				/// @note Either left or right needs to be a MacroPore, left needs to be a water storage
				KinematicMacroFlow(cmf::water::WaterStorage::ptr left,cmf::water::flux_node::ptr right, real beta=1.)
					: BaseMacroFlow(left,right,"Kinematic based macro pore flow")
				{}
			};

			/// @ingroup MacroPore
			/// @brief A physically based macropore to macropore connection according to Jarvis & Leeds-Harrison 1987, JSS
			///
			/// \f[q_{i->j} [m/s]= \rho \frac G {12\eta} w^2 \frac{e_v-e_r}{1-e_r} S_{c,i}^\beta\ (1-S_{c,j})\f]
			/// where:
			/// - \f$q_{i->j}\f$ the flow from macro pore layer i to macropore layer j
			/// - \f$\rho=10^{-3} kg/m^3\f$ - the density of water
			/// - \f$G=9.81 m/s^2\f$ the earth acceleration
			/// - \f$\eta=1.0 kg/(m s)\f$ the viscosity of water (at 20 degC)
			/// - \f$w [m]\f$ the crack width, a function of water content and crack distance
			/// - \f$e_v [-]\f$ the crack porosity
			/// - \f$e_r [-]\f$ crack por
			/// - \f$S_c [-]\f$ the crack saturation of layer i resp. j
			/// - \f$\beta [-]\f$ a conceptional exponent to shape the flow reaction
			class JarvisMacroFlow : public BaseMacroFlow {
			protected:
				virtual real calc_q(cmf::math::Time t) ;
			public:
				real beta;
				real porefraction_r;
				/// @brief Constructs the connection
				///
				/// @param left,right the connected macropores
				/// @param beta User defined parameter for the swelling reaction
				/// @param porefraction_r Porefraction at which flow starts. For swelling soils that are closing completely th
				JarvisMacroFlow(cmf::water::WaterStorage::ptr left,cmf::water::flux_node::ptr right, real beta=1., real porefraction_r=0.0);
			};
			/// @ingroup MacroPore
			/// @brief A gradient based exchange term between macropores and micropores, 
			/// using a fixed (air-) potential for macropores
			///
			/// \f[q = K \frac{\Delta\Psi}{d/2}  A \f]
			/// where:
			/// - \f$K\f$ The conductivity of the aggregate boundary
			/// - \f$\Delta\Psi\f$ The potential difference. Using the air potential as the constant potential for the macro pores, 
			///   you get: \f$\Delta\Psi = \Psi_M(\theta_{micro})\f$ 
			/// - \f$d\f$ the mean aggregate size in m
			/// - \f$A\f$ the crosssection area, given as the flow width (cmf::upslope::MacroPore::get_flowwidth) times layer thickness
			class GradientMacroMicroExchange : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::MacroPore> mp;
				std::weak_ptr<cmf::upslope::SoilLayer> sl;
				void NewNodes();
				virtual real calc_q(cmf::math::Time t);
			public:
				GradientMacroMicroExchange(cmf::upslope::SoilLayer::ptr left, cmf::upslope::MacroPore::ptr right) 
					: flux_connection(left,right,"Macro to micro pores exchange")
				{
					NewNodes();
				}
			};

			/// @ingroup MacroPore
			/// @brief A simple first order diffusive water exchange 
			/// between MacroPore and matrix (SoilLayer)
			///
			/// \f[ q = \omega (W_{ma} - W_{mi,eff}) V_{soil}\f]
			/// where:
			///  - \f$\omega\f$ is the exchange rate in \f$day^{-1}\f$
			///  - \f$W_{ma}\f$ is the filled fraction of the macropore system [-]
			///  - \f$W_{mi,eff}\f$ is the water filled pore space of the micropores above the residual pF value [-], default 4.2
			///  - \f$V_{soil} = A_{cell} d_{layer}\f$ is the total volume of the soil layer [\f$m^3\f$]
			///
			/// The residual micropore pF is used to determine a residual water content of the micropores. Residual in this case means, that above this pF value, 
			/// water is not draining to the macro pores, even if they are empty. Although the default value is at wilting point, lower pF values are much more sensible,
			/// and should be rather lower than field capacity (pF=1.8 - 2.5). However, since this equation is rather conceptual than physical, this value can only be estimated or
			/// calibrated.
			///
			/// cf. Simunek et al J. of Hydr. 2003
			class DiffusiveMacroMicroExchange : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::MacroPore> mp;
				std::weak_ptr<cmf::upslope::SoilLayer> sl;
				void NewNodes();
				virtual real calc_q(cmf::math::Time t);
			public:
				/// \f$\omega\f$ is the exchange rate in \f$day^{-1}\f$
				real omega;
				/// 
				real pFrmi;
				DiffusiveMacroMicroExchange(cmf::upslope::MacroPore::ptr left,cmf::upslope::SoilLayer::ptr right, real omega,real pFrmi=4.2);
			};
			/// @ingroup MacroPore
			///
			/// @brief This connection models the water exchange between macropores and micropores
			/// as in the MACRO Model (Larsbo & Jarvis, 2003), which follows Gerke & van Genuchten 1996.
			///
			/// @warning @deprecated This connection uses the diffusivity of a soil given by its retention curve.
			/// Since no retention curve provides a valid value for Diffusivity in case of saturation
			/// this connection will blow up the numerical solution for sure.
			///
			/// The exchange between Macropore and matrix is defined as follows: (MACRO 5 Tech report, Larsbo & Jarvis 2003)
			///
			/// \f[q = \frac{G_f D_w \gamma_w}{d^2}(\theta_b - \theta_{mi}) V_{layer}\f]
			/// where:
			/// - \f$G_f\f$ is the geometry factor. Use 3 for a rectangular slab geometry 
			/// - \f$gamma_w\f$ A scaling factor to fit analytical and numerical solution (0.4)
			/// - \f$d\f$ is an effective diffusive path length related to aggregate size and 
			///    the influence of coatings on the aggregate surfaces in m
			/// - \f$\theta_b\f$ the saturated water content of the matrix
			/// - \f$\theta_{mi}\f$ the actual water content of the matrix
			/// - \f$D_w = \frac12(D(\theta_b)+D(\theta_{mi})W_{ma})\f$ is the effective water diffusivity in m2/day, as defined below
			///    - \f$W_{ma}\f$ is the saturation of the macropores
			class MACROlikeMacroMicroExchange : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::MacroPore> mp;
				std::weak_ptr<cmf::upslope::SoilLayer> sl;
				void NewNodes() {
					mp = cmf::upslope::MacroPore::cast(right_node());
					sl = cmf::upslope::SoilLayer::cast(left_node());
				}
				virtual real calc_q(cmf::math::Time t);
			public:
				/// Geometry factor. 3 means a rectangular slab geometry (see MACRO 5 tech report)
				/// Other values may be obtained from Gerke & van Genuchten 1996
				real Gf;
				/// \f$\gamma_w\f$ scaling factor set to 0.4. See Gerke & van Genuchten 1993 and van Genuchten 1985 for reference
				real gamma_w;
				MACROlikeMacroMicroExchange(cmf::upslope::SoilLayer::ptr left, cmf::upslope::MacroPore::ptr right, real _gamma_w = 0.4, real _Gf = 3) 
					: flux_connection(left,right,"Macro to micro pores exchange similar to MACRO"), gamma_w(_gamma_w), Gf(_Gf)
				{
					NewNodes();
				}

			};

		} // namespace connections

	} // namespace upslope
} // namespace cmf
#endif

