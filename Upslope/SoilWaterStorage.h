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
			double m_upperboundary,m_lowerboundary;
			cmf::upslope::RCurve m_retentioncurve;
		protected:
		public:
			const int Position;
			cmf::upslope::Cell & cell;
			/// Returns the upper boundary of the water storage below ground in m
			virtual double UpperBoundary() const {return m_upperboundary;}
			/// Returns the lower boundary of the water storage below ground in m
			virtual double LowerBoundary() const {return m_lowerboundary;}
			double Thickness() const {return LowerBoundary()-UpperBoundary();}
			/// Returns the soil properties of the water storage
			virtual const cmf::upslope::RCurve& Soil() const {return m_retentioncurve;}
			virtual cmf::upslope::RCurve& Soil() {return m_retentioncurve;}
			/// Returns the actual volumetric water content of the water storage
			virtual real theta() const {return m_wet.theta;}
			virtual void theta(real Value)
			{
				State(Value*cell.Area()*Thickness());
			}
			/// Returns the actual conductivity	\f$\frac{m}{day}\f$
			double K() const {return m_wet.K;}
			double Ksat() const {return m_wet.Ksat;}
			/// Returns the wetness of the soil \f$ \frac{V_{H_2O}}{V_{pores}} \f$
			virtual double Wetness() const {return m_wet.W;}
			virtual void Wetness(double wetness)
			{
				State(wetness*Capacity());
			}
			/// Calls RCurve::Matrixpotential
			virtual real MatrixPotential() const {return m_wet.Psi_m;}
			/// Gravitational Potential in m, reference height is sea level. If the layer is saturated, it returns the saturated depth above sea level, otherwise it returns the upperboundary of the layer
			/// \f[ \Psi_G=h \f]
			real GravitationalPotential() const;
			/// Returns the capacity of the water storage in m3
			virtual real Capacity()		{return m_wet.C;}
			/// Sets the potential of this soil water storage
			virtual void SetPotential(real waterhead);
			/// Returns the total potential in m
			/// \f[ \Psi = \Psi_M + \Psi_G \f]
			virtual real Potential() const;
			/// Returns the depth for saturation
			/// \f[ z_{sat,this} = \left\{z_{cell}-\Psi_{tot} \mbox{ if } W<1 \\ z_{sat,upper layer} \right. \f]
			virtual real SaturatedDepth() const;
			/// Calculates the shared crosssectional area of this and another soil water storage.
			///
			/// If both layers belong to the same cell, the area of the cell is returned, if they belong to different cells the area of the vertical shared boundary is returned
			/// @returns   Area in m2
			/// @param target The other soil water storage
			/// @param HorizontalLayers If true, the layers are assumed to be parallel to the gravitational potential, otherwise they are assumed to be parallel to the ground topography
			real GetFlowCrosssection(const cmf::upslope::SoilWaterStorage& target,bool HorizontalLayers=false) const;
			
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

		class FlexibleSizeLayer;
		/// A soil water storage with a flexible (non constant) upper boundary, according to its fill state. 
		/// This soil water storage is always saturated
		class FlexibleSizeSaturatedZone : public SoilWaterStorage
		{
		private:
			cmf::upslope::FlexibleSizeLayer* upperLayer;
			/// Creates	two soilwaterstorages and registers them with the owning cell. The boundary between the two layers is flexible and
			/// changes with saturation. The lower layer is always saturated, the upper never. A UnsatSatconnection connects both layers
			FlexibleSizeSaturatedZone(cmf::upslope::Cell& cell,real lowerboundary,const RCurve& r_curve);

		public:
			/// Returns the current upper boundary of this layer (function of state)
			virtual double UpperBoundary() const;
			static FlexibleSizeSaturatedZone* Create(cmf::upslope::Cell& cell,real lowerboundary,const RCurve& r_curve);
			virtual real SaturatedDepth() const { return UpperBoundary();}
			virtual void SetPotential(real waterhead);
		};
		/// A soil water storage above a FlexibleSizeSaturatedLayer, can only be created by FlexibleSaturatedLayer
		class FlexibleSizeLayer : public SoilWaterStorage
		{
		private:
			cmf::upslope::SoilWaterStorage* belowLayer;
			friend class FlexibleSizeSaturatedZone;
			FlexibleSizeLayer(cmf::upslope::Cell & cell,real upperboundary,real lowerboundary,const RCurve & r_curve,cmf::upslope::FlexibleSizeSaturatedZone* LayerBelow);
		public:
			virtual real LowerBoundary() const { return belowLayer ? belowLayer->UpperBoundary() : UpperBoundary()+0.001;}
			virtual real SaturatedDepth() const { return LowerBoundary();}

		};
		cmf::upslope::SoilWaterStorage* AsSoilWater(cmf::water::FluxNode* node);


	}
}
#endif // SoilWaterStorage_h__
