#ifndef VarLayerPercolation_h__
#define VarLayerPercolation_h__

#include "../../water/FluxConnection.h"
#include "../VarLayerPair.h"
namespace cmf {
	namespace upslope {
		namespace connections {

			/// Calculates the flux from the unsaturated zone to the saturated zone
			/// by vertical outflow from the unsaturated zone and 
			/// water exchange by water table changes
			///
			/// \f{eqnarray*}
			/// K_u &=& K(\theta) A \\
			/// Ex_w &=& \frac{dz_{sat}}{dt\ \Delta Z_{layer}} V_{layer} \\
			/// \frac{dz_{sat}}{dt} &=& \frac{ \sum q_{Saturated}}{ A \Phi} \\
			/// \f}
			/// where:
			/// - \f$ A \f$ is the area of the cell
			/// - \f$ K(\theta) \f$ is the unsaturated conductivity (see SoilType::Kunsat )
			/// - \f$ \Delta Z \f$ is the thickness of a layer
			/// - \f$ V \f$ is the volume of water in a layer
			/// - \f$ layer \f$ is the unsaturated zone if \f$ \frac{dz_{sat}}{dt} \f$ is positive, otherwise layer is the saturated zone
			/// - \f$ \sum q \f$ is the water balance of the saturated zone
			/// - \f$ \Phi \f$ is the porosity
			class VarLayerPercolationRichards : public cmf::water::FluxConnection {
			protected:
				cmf::upslope::VariableLayerUnsaturated* m_unsat;
				cmf::upslope::VariableLayerSaturated* m_sat;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_unsat=dynamic_cast<cmf::upslope::VariableLayerUnsaturated*>(m_left);
					m_sat=dynamic_cast<cmf::upslope::VariableLayerSaturated*>(m_right);
				}

			public:
				bool Only_Exw;
				/// Creates a connection between unsaturated and saturated zone
				VarLayerPercolationRichards(cmf::upslope::VariableLayerUnsaturated& unsat,cmf::upslope::VariableLayerSaturated& sat,bool only_Exw=false) 
					: FluxConnection(unsat,sat,"Richards like variable layer percolation"),Only_Exw(only_Exw) {
						NewNodes();
				}
			};

			class VarLayerPercolationSimple : public cmf::water::FluxConnection {
				cmf::upslope::VariableLayerUnsaturated* m_unsat;
				cmf::upslope::VariableLayerSaturated* m_sat;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_unsat=dynamic_cast<cmf::upslope::VariableLayerUnsaturated*>(m_left);
					m_sat=dynamic_cast<cmf::upslope::VariableLayerSaturated*>(m_right);
				}
			public:
				real pF_field_cap;
				/// Creates a connection between unsaturated and saturated zone
				VarLayerPercolationSimple(cmf::upslope::VariableLayerUnsaturated& unsat,cmf::upslope::VariableLayerSaturated& sat,real _pF_field_cap=1.8) 
					: FluxConnection(unsat,sat,"Simple variable layer percolation") , pF_field_cap(_pF_field_cap)
				{NewNodes();	}
			};

		}
	}
}
#endif // VarLayerPercolation_h__
