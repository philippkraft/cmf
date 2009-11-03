#ifndef VarLayerPair_h__
#define VarLayerPair_h__

#include "SoilLayer.h"
namespace cmf {
	namespace upslope {
		class VariableLayerUnsaturated;
		/// A soil water storage with a flexible (non constant) upper boundary, according to its fill state. 
		/// This soil water storage is always saturated
		class VariableLayerSaturated : public SoilLayer
		{
		public:
			typedef std::tr1::shared_ptr<cmf::upslope::VariableLayerSaturated> ptr;
#ifndef SWIG
			operator ptr() {return std::tr1::static_pointer_cast<VariableLayerSaturated>(shared_from_this());}
#endif
		private:
			friend class Cell;
			std::tr1::weak_ptr<cmf::upslope::VariableLayerUnsaturated> m_upperLayer;
			/// Creates	two SoilLayers and registers them with the owning cell. The boundary between the two layers is flexible and
			/// changes with saturation. The lower layer is always saturated, the upper never. A UnsatSatconnection connects both layers
			VariableLayerSaturated(cmf::upslope::Cell& cell,real lowerboundary,const RetentionCurve& r_curve);
			static std::tr1::shared_ptr<VariableLayerSaturated> Create(cmf::upslope::Cell& cell,real lowerboundary,const RetentionCurve& r_curve);

			real calc_thickness(real state) const;
			real calc_potential(real state) const;
		public:
			/// Returns the current upper boundary of this layer (function of state)
			virtual real get_upper_boundary() const;

			std::tr1::shared_ptr<VariableLayerUnsaturated> UpperLayer() {return m_upperLayer.lock();}
			
			
			virtual real get_saturated_depth() const { return get_upper_boundary();}
			
			virtual void set_potential(real waterhead);
			
			virtual real MaximumThickness() const;
			
			virtual real get_potential() const;
			
			real get_thickness_change_rate() const;
			
			static std::tr1::shared_ptr<VariableLayerSaturated> get_from_cell(cmf::upslope::Cell& cell);
		};
		/// A soil water storage above a FlexibleSizeSaturatedLayer, can only be created by FlexibleSaturatedLayer
		class VariableLayerUnsaturated : public SoilLayer
		{
		public:
			typedef std::tr1::shared_ptr<cmf::upslope::VariableLayerUnsaturated> ptr;
#ifndef SWIG
			operator ptr() {return std::tr1::static_pointer_cast<VariableLayerUnsaturated>(shared_from_this());}
#endif
		private:
			
			std::tr1::weak_ptr<cmf::upslope::VariableLayerSaturated> belowLayer;
			
			friend class VariableLayerSaturated;
			
			VariableLayerUnsaturated(cmf::upslope::Cell & cell,real upperboundary,real lowerboundary,
																const RetentionCurve & r_curve,
																std::tr1::shared_ptr<cmf::upslope::VariableLayerSaturated> LayerBelow);
		public:
			
			virtual real get_wetness() const;
			
			virtual real get_potential() const;

			virtual real get_lower_boundary() const;
			
			virtual real get_saturated_depth() const { return get_lower_boundary();}

		};
	}
}

#endif // VarLayerPair_h__

