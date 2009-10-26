#ifndef infiltration_h__
#define infiltration_h__

#include "../../water/flux_connection.h"
#include "../cell.h"
#include "../SoilLayer.h"
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
			class MatrixInfiltration : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::upslope::SoilLayer> m_soilwater;
				std::tr1::weak_ptr<cmf::river::OpenWaterStorage> m_surfacewaterstorage;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_surfacewaterstorage=cmf::river::OpenWaterStorage::cast(left_node());
					m_soilwater=cmf::upslope::SoilLayer::cast(right_node());
				}
			public:
				MatrixInfiltration(cmf::upslope::SoilLayer_ptr soilwater,cmf::water::flux_node::ptr surfacewater) 
					: flux_connection(surfacewater,soilwater,"Richards eq. infiltration") {
						NewNodes();
				}
				static void use_for_cell(cmf::upslope::Cell& c)
				{
					new MatrixInfiltration(c.get_layer(0),c.get_surfacewater());
				}
			};
			class CompleteInfiltration : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::upslope::SoilLayer> m_soilwater;
				std::tr1::weak_ptr<cmf::river::OpenWaterStorage> m_surfacewaterstorage;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_soilwater=cmf::upslope::SoilLayer::cast(right_node());
					m_surfacewaterstorage=cmf::river::OpenWaterStorage::cast(left_node());
				}
			public:
				CompleteInfiltration(cmf::upslope::SoilLayer_ptr soilwater,cmf::water::flux_node::ptr surfacewater) 
					: flux_connection(surfacewater,soilwater,"Complete infiltration"){
						NewNodes();
				}

			};
		}
	}
}
#endif // infiltration_h__
