

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
#include "../cmfmemory.h"
namespace cmf {
	namespace upslope {
		class Cell;
		/// @ingroup storages
		/// A representation of a SoilLayer
		class SoilLayer: public cmf::water::WaterStorage, public cmf::upslope::conductable
		{
		public:
			typedef std::shared_ptr<cmf::upslope::SoilLayer> ptr;
#ifndef SWIG
			operator ptr() {return std::static_pointer_cast<SoilLayer>(shared_from_this());}
#endif

		private:
			friend class Cell;
			struct wet {real	W,Psi_m,h,V,theta,C,K,Ksat, ice_fraction;	};
			wet m_wet;
			typedef std::weak_ptr<cmf::upslope::SoilLayer> weak_ptr;
			weak_ptr m_upper;
			weak_ptr m_lower;
			real m_ice_fraction;
			real m_rootfraction;
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
			std::unique_ptr<cmf::upslope::RetentionCurve> m_retentioncurve;

			/// Converts a head to the volume of stored water
			virtual real head_to_volume(real head) const;
			/// Converts a volume of stored water to the head
			virtual real volume_to_head(real volume) const;

		public:

			const ptrdiff_t Position;
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

			/// Returns the root fraction of the layer. If it is not explicitly set, it uses the parameters of the vegetation object of the cell
			real get_rootfraction() const;
			/// Sets the root fraction in this layer explicitly
			void set_rootfraction(real rootfraction) {
				m_rootfraction = rootfraction;
			}
			
			/// Calculates the shared crosssectional area of this and another soil water storage.
			///
			/// If both layers belong to the same cell, the area of the cell is returned, if they belong to different cells the area of the vertical shared boundary is returned
			/// @returns   get_area in m2
			/// @param target The other soil water storage
			/// @param HorizontalLayers If true, the layers are assumed to be parallel to the gravitational potential, otherwise they are assumed to be parallel to the ground topography
			real get_flow_crosssection(const cmf::upslope::SoilLayer& target,bool HorizontalLayers=false) const;
			
			static SoilLayer::ptr cast(cmf::water::flux_node::ptr node)
			{
				return std::dynamic_pointer_cast<SoilLayer>(node);
			}
		protected:
			SoilLayer(cmf::upslope::Cell & _cell,real lowerboundary,
				const RetentionCurve& r_curve,real saturateddepth=10);
			SoilLayer(cmf::upslope::Cell & _cell,real upperBoundary,real lowerboundary,
												const RetentionCurve& r_curve,ptrdiff_t _Position);
			/// Invalidates the saturated depth of the cell
			virtual void StateChangeAction();

		};
	}
}
#endif // SoilLayer_h__
