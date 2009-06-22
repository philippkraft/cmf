#ifndef infiltration_h__
#define infiltration_h__

#include "../../water/FluxConnection.h"
#include "../cell.h"
#include "../SoilWaterStorage.h"
#include "../../reach/ManningConnection.h"
namespace cmf {
	namespace upslope {
		namespace connections {
			/// Connects the surfacewater and the most upper layer
			///
			/// If UpslopeCell::InfiltrationExcess and Cell is not saturated 
			/// \f[ K_I = \min\left(\frac{\rho_{wg} \Delta z-\Psi_M}{\Delta z \rho_{wg}} K\left(\theta\right),\sum q_{surface\ in}\right) \f]
			/// else
			/// \f[ K_I = \sum q_{surface\ in} \f]
			class MatrixInfiltration : public cmf::water::FluxConnection {
			protected:
				cmf::upslope::SoilWaterStorage* m_soilwater;
				cmf::water::FluxNode* m_surfacewater;
				cmf::river::OpenWaterStorage* m_surfacewaterstorage;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_surfacewater=m_left;
					m_surfacewaterstorage=cmf::river::AsOpenWater(m_left);
					m_soilwater=cmf::upslope::AsSoilWater(m_right);
				}
			public:
				MatrixInfiltration(cmf::upslope::SoilWaterStorage& soilwater,cmf::water::FluxNode& surfacewater) 
					: FluxConnection(surfacewater,soilwater,"Richards eq. infiltration") {
						NewNodes();
				}
				static void use_for_cell(cmf::upslope::Cell& c)
				{
					new MatrixInfiltration(c.get_layer(0),c.get_surfacewater());
				}
			};
			class CompleteInfiltration : public cmf::water::FluxConnection {
			protected:
				cmf::upslope::SoilWaterStorage* m_soilwater;
				cmf::water::FluxNode* m_surfacewater;
				cmf::river::OpenWaterStorage* m_surfacewaterstorage;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_surfacewater=m_left;
					m_soilwater=cmf::upslope::AsSoilWater(m_right);
					m_surfacewaterstorage=cmf::river::AsOpenWater(m_left);
				}
			public:
				CompleteInfiltration(cmf::upslope::SoilWaterStorage& soilwater,cmf::water::FluxNode& surfacewater) 
					: FluxConnection(surfacewater,soilwater,"Complete infiltration"){
						NewNodes();
				}

			};
			class RouteWB : public cmf::water::FluxConnection
			{
			protected:
				real calc_q(cmf::math::Time t)
				{
					real wb=m_left->water_balance(t,this);
					if (wb<0 && m_right->is_empty())
						return 0.0;
					else
						return wb;
				}
				void NewNodes()
				{

				}
			public:
				RouteWB(cmf::water::FluxNode& left,cmf::water::FluxNode& right)
					: FluxConnection(left,right,"Water balance routing")
				{
					NewNodes();
				}
			};
		}
	}
}
#endif // infiltration_h__
