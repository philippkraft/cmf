#ifndef SoilWaterStorage_h__
#define SoilWaterStorage_h__

#include "../water/WaterStorage.h"
#include "../water/FluxConnection.h"
#include "../Geometry/geometry.h"
#include "Soil/RetentionCurve.h"
#include "cell.h"
#include <memory>
#include <sstream>
namespace cmf {
	namespace upslope {
		/// A representation of a Layer
		class SoilWaterStorage: public cmf::water::WaterStorage
		{
		private:
			struct wet {real	W,Psi_m,theta,C,K,Ksat;	};
			wet m_wet;
		protected:
			real m_upperboundary,m_lowerboundary;
			cmf::upslope::RCurve m_retentioncurve;
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
			virtual const cmf::upslope::RCurve& get_soil() const {return m_retentioncurve;}
			virtual cmf::upslope::RCurve& get_soil() {return m_retentioncurve;}
			/// Returns the actual volumetric water content of the water storage
			virtual real get_theta() const {return m_wet.theta;}
			virtual void set_theta(real Value)
			{
				State(Value*cell.get_area()*get_thickness());
			}
			/// Returns the actual conductivity	\f$\frac{m}{day}\f$
			real get_K() const {return m_wet.K;}
			real get_Ksat() const {return m_wet.Ksat;}
			/// Returns the wetness of the soil \f$ \frac{V_{H_2O}}{V_{pores}} \f$
			virtual real get_wetness() const {return m_wet.W;}
			virtual void set_wetness(real wetness)
			{
				State(wetness*Capacity());
			}
			/// Calls RCurve::Matrixpotential
			virtual real get_matrix_potential() const {return m_wet.Psi_m;}
			/// Gravitational get_potential in m, reference height is sea level. If the layer is saturated, it returns the saturated depth above sea level, otherwise it returns the upperboundary of the layer
			/// \f[ \Psi_G=h \f]
			real get_gravitational_potential() const;
			/// Returns the capacity of the water storage in m3
			virtual real Capacity()		{return m_wet.C;}
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
			real get_flow_crosssection(const cmf::upslope::SoilWaterStorage& target,bool HorizontalLayers=false) const;
			
			SoilWaterStorage* copy()
			{
				return new SoilWaterStorage(cell,m_upperboundary,m_lowerboundary,m_retentioncurve,Position);
			}
			static SoilWaterStorage * Create(cmf::upslope::Cell & _cell,real lowerboundary,const RCurve& r_curve,real saturateddepth=-10)
			{
				return new SoilWaterStorage(_cell,lowerboundary,r_curve,saturateddepth);
			}
		protected:
			SoilWaterStorage(cmf::upslope::Cell & _cell,real lowerboundary,const RCurve& r_curve,real saturateddepth=-10);
			SoilWaterStorage(cmf::upslope::Cell & _cell,real upperBoundary,real lowerboundary,const RCurve& r_curve,int _Position);
			/// Invalidates the saturated depth of the cell
			virtual void StateChangeAction();

		};

		class VariableLayerUnsaturated;
		/// A soil water storage with a flexible (non constant) upper boundary, according to its fill state. 
		/// This soil water storage is always saturated
		class VariableLayerSaturated : public SoilWaterStorage
		{
		private:
			cmf::upslope::VariableLayerUnsaturated* upperLayer;
			/// Creates	two soilwaterstorages and registers them with the owning cell. The boundary between the two layers is flexible and
			/// changes with saturation. The lower layer is always saturated, the upper never. A UnsatSatconnection connects both layers
			VariableLayerSaturated(cmf::upslope::Cell& cell,real lowerboundary,const RCurve& r_curve);

		public:
			/// Returns the current upper boundary of this layer (function of state)
			virtual real get_upper_boundary() const;
			VariableLayerUnsaturated& UpperLayer() {return *upperLayer;}
			static VariableLayerSaturated* Create(cmf::upslope::Cell& cell,real lowerboundary,const RCurve& r_curve);
			virtual real get_saturated_depth() const { return get_upper_boundary();}
			virtual void set_potential(real waterhead);
			virtual real MaximumThickness() const;
			virtual real get_potential() const;
			static VariableLayerSaturated* get_from_cell(cmf::upslope::Cell& cell)
			{
				for (int i = 0; i < cell.layer_count() ; ++i)
				{
					VariableLayerSaturated *p_result = dynamic_cast<VariableLayerSaturated *>(&cell.get_layer(i));
					if(p_result)
					{
						 return p_result;
					}
				}
				return 0;
			}
		};
		/// A soil water storage above a FlexibleSizeSaturatedLayer, can only be created by FlexibleSaturatedLayer
		class VariableLayerUnsaturated : public SoilWaterStorage
		{
		private:
			cmf::upslope::SoilWaterStorage* belowLayer;
			friend class VariableLayerSaturated;
			VariableLayerUnsaturated(cmf::upslope::Cell & cell,real upperboundary,real lowerboundary,const RCurve & r_curve,cmf::upslope::VariableLayerSaturated* LayerBelow);
		public:
			virtual real get_lower_boundary() const { return belowLayer ? belowLayer->get_upper_boundary() : get_upper_boundary()+0.001;}
			virtual real get_saturated_depth() const { return get_lower_boundary();}

		};
		cmf::upslope::SoilWaterStorage* AsSoilWater(cmf::water::FluxNode* node);


	}
}
#endif // SoilWaterStorage_h__
