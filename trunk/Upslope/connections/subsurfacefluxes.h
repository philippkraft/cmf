#ifndef subsurfacefluxes_h__
#define subsurfacefluxes_h__

#include "../../water/flux_connection.h"
#include "../cell.h"
#include "../topology.h"
#include "../SoilLayer.h"

namespace cmf {
	namespace upslope {
		namespace connections {
			/// An abstract base class for lateral subsurface fluxes
			class lateral_sub_surface_flux : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::upslope::SoilLayer> sw1,sw2;
				real 
					flow_width, distance;

				virtual real calc_q(cmf::math::Time t)=0;
				virtual void NewNodes();

				lateral_sub_surface_flux(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right, std::string Name,real FlowWidth,real Distance=0)
					: cmf::water::flux_connection(left,right,Name), flow_width(FlowWidth), distance(Distance)
				{
					NewNodes();
					if (Distance<=0)
						distance=left->get_distance_to(*right);
				}

			};


			/// Calculates the lateral flow using the gravitational potential gradient only
			///
			/// \f[ q_{lat} = \frac{\Delta \Psi_G}{\|C_1-C_2\|} \frac 12 (T(C_1)+T(C_2)) w \f]
			/// where
			/// - \f$ \Delta \Psi_G \f$ is the gravitational potential difference
			/// - \f$ \|C_1-C_2\| \f$ is the distance from Cell 1 to Cell 2
			/// - \f$ T(C) \f$ is the transmissivity of cell C, calculated by SoilType::Transmissivity
			/// - \f$ w \f$ is the width of the connection of the cells
			class Darcy : public lateral_sub_surface_flux {
			protected:
				virtual real calc_q(cmf::math::Time t) ;
				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				static const CellConnector cell_connector;
				Darcy(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right,real FlowWidth,real Distance=0)
					: lateral_sub_surface_flux(left,right,"Darcy flow",FlowWidth,Distance) 
				{}

			};
			///	Calculates the lateral flow using the topographic gradient
			///
			/// \f[ q_{lat} = \frac{z_1 - z_2}{\|C_1-C_2\|} \frac 12 (T(C_1)+T(C_2)) w \f]
			/// where
			/// - \f$ z_1 - z_2 \f$ is the topographic height difference
			/// - \f$ \|C_1-C_2\| \f$ is the distance from Cell 1 to Cell 2
			/// - \f$ T(C) \f$ is the transmissivity of cell C, calculated by SoilType::Transmissivity
			/// - \f$ w \f$ is the width of the connection of the cells
			class TopographicGradientDarcy : public lateral_sub_surface_flux {
			protected:
				virtual real calc_q(cmf::math::Time t) ;
				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				static const cmf::upslope::CellConnector cell_connector;
				TopographicGradientDarcy(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right,real FlowWidth,real Distance=0)
					: lateral_sub_surface_flux(left,right,"Topographic gradient",FlowWidth,Distance)
				{ }


			};
			
			/// A connection similar to OHDIS-KWMSS (OHymos-based DIStributed model - with Kinematic Wave Method for Surface and Subsurface runoff) 
			///
			/// \f{eqnarray*} 
			/// q_{u} &=& w d_m k(\theta) \frac{\delta z}{\delta x} \\ 
			///	q_{s} &=& w (h-d)k_{sat} \frac{\delta z}{\delta x} + q_{u} \\
			/// q_{o} &=& w (\frac{sqrt{\left|\frac{\delta z}{\delta x}\right|}{n} (h-d_a)^(2/3) + q_s
			/// \f}
			class OHDISflow : public lateral_sub_surface_flux
			{
			protected:
				virtual real calc_q(cmf::math::Time t) ;

				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				OHDISflow(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right,real FlowWidth, real Distance=0)
					: lateral_sub_surface_flux(left,right,"OHDIS like connection",FlowWidth,Distance)
				{			}

				static const CellConnector cell_connector;

			};

			class Richards_lateral: public lateral_sub_surface_flux
			{
			protected:
				real flow_thickness;
				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
				virtual real calc_q(cmf::math::Time t) ;

			public:
				Richards_lateral(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right,real FlowWidth=0,real Distance=0)
					: lateral_sub_surface_flux(left,right,"Richards eq.",FlowWidth,Distance)
				{
					flow_thickness=left->get_thickness();
					if (!sw2.expired())
					{
						flow_thickness=minimum(flow_thickness,sw2.lock()->get_thickness());
					}
				}
				static const CellConnector cell_connector;
			};

		}

	}
}

#endif // subsurfacefluxes_h__

