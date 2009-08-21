#ifndef VarLayerPair_h__
#define VarLayerPair_h__

#include "SoilWaterStorage.h"
namespace cmf {
	namespace upslope {
		class VariableLayerUnsaturated;
		/// A soil water storage with a flexible (non constant) upper boundary, according to its fill state. 
		/// This soil water storage is always saturated
		class VariableLayerSaturated : public SoilWaterStorage
		{
		private:
			cmf::upslope::VariableLayerUnsaturated* upperLayer;
			/// Creates	two soilwaterstorages and registers them with the owning cell. The boundary between the two layers is flexible and
			/// changes with saturation. The lower layer is always saturated, the upper never. A UnsatSatconnection connects both layers
			VariableLayerSaturated(cmf::upslope::Cell& cell,real lowerboundary,const RetentionCurve& r_curve);

			real calc_thickness(real state) const;
			real calc_potential(real state) const;
		public:
			/// Returns the current upper boundary of this layer (function of state)
			virtual real get_upper_boundary() const;
			VariableLayerUnsaturated& UpperLayer() {return *upperLayer;}
			static VariableLayerSaturated* Create(cmf::upslope::Cell& cell,real lowerboundary,const RetentionCurve& r_curve);
			virtual real get_saturated_depth() const { return get_upper_boundary();}
			virtual void set_potential(real waterhead);
			virtual real MaximumThickness() const;
			virtual real get_potential() const;
			real get_thickness_change_rate() const;
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
			VariableLayerUnsaturated(cmf::upslope::Cell & cell,real upperboundary,real lowerboundary,const RetentionCurve & r_curve,cmf::upslope::VariableLayerSaturated* LayerBelow);
		public:
			virtual real get_wetness() const;
			virtual real get_potential() const;

			virtual real get_lower_boundary() const;
			virtual real get_saturated_depth() const { return get_lower_boundary();}

		};


	}

}

#endif // VarLayerPair_h__

