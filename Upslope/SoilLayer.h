#ifndef SoilLayer_h__
#define SoilLayer_h__

#include "../water/WaterStorage.h"
#include "../water/flux_connection.h"
#include "../Geometry/geometry.h"
#include "Soil/RetentionCurve.h"
#include "cell.h"
#include <memory>
namespace cmf {
	namespace upslope {
		class SoilLayer;
		typedef std::tr1::shared_ptr<cmf::upslope::SoilLayer> SoilLayer_ptr;

		/// A representation of a SoilLayer
		class SoilLayer: public cmf::water::WaterStorage
		{
		private:
			friend class Cell;
			struct wet {real	W,Psi_m,theta,C,K,Ksat;	};
			wet m_wet;
		protected:
			real m_upperboundary,m_lowerboundary;
			std::auto_ptr<cmf::upslope::RetentionCurve> m_retentioncurve;
		protected:
		public:
			const int Position;
			cmf::upslope::Cell & cell;
			/// Returns the upper boundary of the water storage below ground in m
			virtual real get_upper_boundary() const {return m_upperboundary;}
			/// Returns the lower boundary of the water storage below ground in m
			virtual real get_lower_boundary() const {return m_lowerboundary;}
			real get_thickness() const {return get_lower_boundary()-get_upper_boundary();}
			/// Returns the soil properties of the water storage
			virtual cmf::upslope::RetentionCurve& get_soil() const {return *m_retentioncurve;}
			virtual void set_soil(const cmf::upslope::RetentionCurve& r_curve) { m_retentioncurve.reset(r_curve.copy());}
			/// Returns the actual volumetric water content of the water storage
			virtual real get_theta() const {return m_wet.theta;}
			virtual void set_theta(real Value)
			{
				set_state(Value*cell.get_area()*get_thickness());
			}
			/// Returns the actual conductivity	\f$\frac{m}{day}\f$
			real get_K() const {return m_wet.K;}
			real get_Ksat() const {return m_wet.Ksat;}
			/// Returns the wetness of the soil \f$ \frac{V_{H_2O}}{V_{pores}} \f$
			virtual real get_wetness() const {return m_wet.W;}
			virtual void set_wetness(real wetness)
			{
				set_state(wetness*get_capacity());
			}
			/// Calls RetentionCurve::Matrixpotential
			virtual real get_matrix_potential() const {return m_wet.Psi_m;}
			/// Gravitational get_potential in m, reference height is sea level. If the layer is saturated, it returns the saturated depth above sea level, otherwise it returns the upperboundary of the layer
			/// \f[ \Psi_G=h \f]
			real get_gravitational_potential() const;
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
			
			static SoilLayer_ptr cast(cmf::water::flux_node::ptr node)
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


	}
}
#endif // SoilLayer_h__
