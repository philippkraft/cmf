

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
#ifndef SoilLayer_h__
#define SoilLayer_h__

#include "../water/WaterStorage.h"
#include "../water/flux_connection.h"
#include "../geometry/geometry.h"
#include "Soil/RetentionCurve.h"
#include "conductable.h"
#include <memory>
namespace cmf {
	namespace upslope {
		class Cell;
		/// @ingroup storages
		/// A representation of a SoilLayer
		class SoilLayer: public cmf::water::WaterStorage, public cmf::upslope::conductable
		{
		public:
			typedef std::tr1::shared_ptr<cmf::upslope::SoilLayer> ptr;
#ifndef SWIG
			operator ptr() {return std::tr1::static_pointer_cast<SoilLayer>(shared_from_this());}
#endif

		private:
			friend class Cell;
			struct wet {real	W,Psi_m,h,V,theta,C,K,Ksat, ice_fraction;	};
			wet m_wet;
			typedef std::tr1::weak_ptr<cmf::upslope::SoilLayer> weak_ptr;
			weak_ptr m_upper;
			weak_ptr m_lower;
			real m_ice_fraction;
		public:
			/// real (Ice_fraction)
			real get_ice_fraction() const { return m_ice_fraction; }
			///@param val Ice_fraction (real)
			void set_ice_fraction(real val) { 
				m_ice_fraction = val; 
				StateChangeAction();
			}
		private:
		protected:

			real m_upperboundary,m_lowerboundary;

			/// The retention curve of the soil layer
			std::auto_ptr<cmf::upslope::RetentionCurve> m_retentioncurve;

			/// Converts a head to the volume of stored water
			virtual real head_to_volume(real head) const;
			/// Converts a volume of stored water to the head
			virtual real volume_to_head(real volume) const;

		public:

			const int Position;
			cmf::upslope::Cell & cell;
			/// Returns the upper boundary of the water storage below ground in m
			virtual real get_upper_boundary() const {return m_upperboundary;}
			/// Returns the lower boundary of the water storage below ground in m
			virtual real get_lower_boundary() const {return m_lowerboundary;}

			ptr get_upper() const {return m_upper.lock();}
			ptr get_lower() const {return m_lower.lock();}

			real get_thickness() const {return get_lower_boundary()-get_upper_boundary();}
			/// Returns the soil properties of the water storage
			virtual cmf::upslope::RetentionCurve& get_soil() const {return *m_retentioncurve;}
			virtual void set_soil(const cmf::upslope::RetentionCurve& r_curve) { 
				m_retentioncurve.reset(r_curve.copy());
				StateChangeAction();
			}
			/// Returns the actual volumetric water content of the water storage
			virtual real get_theta() const {return m_wet.theta;}
			virtual void set_theta(real Value);

			virtual real get_volume() const {
				return m_wet.V;
			}
			
			real get_abs_errtol(real rel_errtol) const {
				return rel_errtol * get_capacity();
			}
			
			/// Returns the actual isotropic conductivity using the function from soil \f$\frac{m}{day}\f$
			real get_K() const {return m_wet.K;}
			/// Returns the actual anisotropic conductivity along a direction \f$K = (k_f \cdot d) K\f$
			virtual real get_K(cmf::geometry::point direction) const;
			cmf::geometry::point anisotropic_kf;
			real get_Ksat() const {return m_wet.Ksat;}
			/// Returns the wetness of the soil \f$ \frac{V_{H_2O}}{V_{pores}} \f$
			virtual real get_wetness() const {return m_wet.W;}
			virtual void set_wetness(real wetness);
			/// Calls RetentionCurve::Matrixpotential
			virtual real get_matrix_potential() const {return m_wet.Psi_m;}
			/// Gravitational get_potential in m, reference height is sea level. If the layer is saturated, it returns the saturated depth above sea level, otherwise it returns the upperboundary of the layer
			/// \f[ \Psi_G=h \f]
			real get_gravitational_potential() const;
			/// Returns the mean porosity in the layer
			real get_porosity() const;
			/// Returns the capacity of the water storage in m3
			virtual real get_capacity()	const	{return m_wet.C;}
			/// Sets the potential of this soil water storage
			virtual void set_potential(real waterhead);
			/// Returns the total potential in m
			/// \f[ \Psi = \Psi_M + \Psi_G \f]
			virtual real get_potential() const;
			/// Returns the depth for saturation
			/// \f[ z_{sat,this} = \left\{z_{cell}-\Psi_{tot} \mbox{ if } W<1 \\ z_{sat,upper layer} \right. \f]
			virtual real get_saturated_depth() const;
			/// Calculates the shared crosssectional area of this and another soil water storage.
			///
			/// If both layers belong to the same cell, the area of the cell is returned, if they belong to different cells the area of the vertical shared boundary is returned
			/// @returns   get_area in m2
			/// @param target The other soil water storage
			/// @param HorizontalLayers If true, the layers are assumed to be parallel to the gravitational potential, otherwise they are assumed to be parallel to the ground topography
			real get_flow_crosssection(const cmf::upslope::SoilLayer& target,bool HorizontalLayers=false) const;
			
			static SoilLayer::ptr cast(cmf::water::flux_node::ptr node)
			{
				return std::tr1::dynamic_pointer_cast<SoilLayer>(node);
			}
		protected:
			SoilLayer(cmf::upslope::Cell & _cell,real lowerboundary,
				const RetentionCurve& r_curve,real saturateddepth=10);
			SoilLayer(cmf::upslope::Cell & _cell,real upperBoundary,real lowerboundary,
												const RetentionCurve& r_curve,int _Position);
			/// Invalidates the saturated depth of the cell
			virtual void StateChangeAction();

		};

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
			real m_porefraction;
		protected:
			virtual real head_to_volume(real head) const;
			virtual real volume_to_head(real volume) const;
			MacroPore(SoilLayer::ptr layer,real porefraction=0.05, real Ksat=10,real density=0.05 );
		public:
			/// Gets the soil layer (matrix water storage) for this macropore storage
			SoilLayer::ptr get_layer() const {
				return m_layer.lock();
			}
			/// The fraction of the macro pores in m3/m3. This adds to the porosity of the layer
			real get_porefraction() const {return m_porefraction;}
			/// The mean distance between the macro pores in m
			real density;
			/// The saturated conductivity of the macropores in m/day
			real Ksat;
			/// Returns the actual conductivity (until now just Ksat)
			virtual real get_K() const{
				return Ksat;
			}
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
			/// Creates a macropore water storage for a soil layer.
			///
			/// @returns a MacroPore water storage
			/// @param layer The soil layer holding the macro pore
			/// @param porefraction The relative macro pore content of the soil layer
			/// @param Ksat The saturated conductivity of the macro pores
			/// @param density The mean distance between macropores or mean aggregate size
			static MacroPore::ptr create(SoilLayer::ptr layer,real porefraction=0.05, real Ksat=10,real density=0.05);
			static MacroPore::ptr cast(cmf::water::flux_node::ptr node) {
				return std::tr1::dynamic_pointer_cast<MacroPore>(node);
			}

        };
	}
}
#endif // SoilLayer_h__
