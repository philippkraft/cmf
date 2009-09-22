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
			/// Calculates the percolation and capillar rise between an unsaturated and a saturated zone, using the approach used in the PIHM( Qu & Duffy 2007)
			/// \f[ q_{perc} = K_s \frac{1-e^{-\alpha(z_s-h_g)}-\alpha h_u}{\alpha(z_s-h_g)-\left(1-e^{-\alpha(z_s-h_g)}\right)} \f]
			/// where:
			/// - \f$K_s\f$ is the saturated conductivity in \f$\frac m {day}\f$
			/// - \f$\alpha\f$ is a curve shape parameter in \f$m^{-1}\f$
			/// - \f$z_s\f$ is the soil depth in m
			/// - \f$h_g\f$ is the thickness of the saturated zone
			/// - \f$h_u\f$ is the equivalent thickness of the unsaturated zone, calculated as \f$h_u = (z_s - h_g)\frac{\theta}{\phi}\f$
			class PIHMpercolation : public cmf::water::FluxConnection {
				cmf::upslope::VariableLayerUnsaturated* m_unsat;
				cmf::upslope::VariableLayerSaturated* m_sat;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_unsat=dynamic_cast<cmf::upslope::VariableLayerUnsaturated*>(m_left);
					m_sat=dynamic_cast<cmf::upslope::VariableLayerSaturated*>(m_right);
				}
			public:
				real alpha;
				/// Creates a connection between unsaturated and saturated zone
				PIHMpercolation(cmf::upslope::VariableLayerUnsaturated& unsat,cmf::upslope::VariableLayerSaturated& sat,real _alpha=1) 
					: FluxConnection(unsat,sat,"Percolation as in PIHM (Qu & Duffy 2007)") , alpha(_alpha)
				{NewNodes();	}

			};
			/// Calculates lateral flow between saturated zones (with variable head) using the approach used in the PIHM( Qu & Duffy 2007)
			///	\f[q_{lat} = w K_{eff} \frac{\Delta \Psi}{d} \frac{z_{sat,1} + z_{sat,2}}{2} \f]
			/// where
			/// - \f$w\f$ is the width of the shared boundary
			/// - \f$K_{eff}\f$ is the harmonic mean of the saturated conductivities
			/// - \f$\Delta\Psi\f$ is the difference between the saturated water heads
			/// - \f$z_{sat,i}\f$ is the thickness of the saturated layer in cell i
			class PIHMlateral : public cmf::water::FluxConnection {
				cmf::upslope::SoilWaterStorage * m_soil_left, * m_soil_right;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_soil_left=dynamic_cast<cmf::upslope::VariableLayerSaturated*>(m_left);
					m_soil_right=AsSoilWater(m_right);
				}
			private:
				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				static const CellConnector cell_connector;
				real flow_width,distance;
				PIHMlateral(cmf::upslope::VariableLayerSaturated& left, cmf::water::FluxNode& right, real _width,real _distance)
					: cmf::water::FluxConnection(left,right,"Lateral flow as in PIHM (Qu & Duffy 2007)"), flow_width(_width), distance(_distance)
				{NewNodes();	}
			};
		}
	}
}
#endif // VarLayerPercolation_h__
