#ifndef subsurfacefluxes_h__
#define subsurfacefluxes_h__

#include "../../water/flux_connection.h"
#include "../cell.h"
#include "../topology.h"
#include "../SoilLayer.h"

namespace cmf {
	namespace upslope {
		namespace connections {

			class SubSurfaceFlux : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::upslope::SoilLayer> sw1,sw2;
				real 
					flow_width, distance;

				virtual real calc_q(cmf::math::Time t)=0;
				virtual void NewNodes()
				{
					sw1=cmf::upslope::SoilLayer::cast(left_node());
					sw2=cmf::upslope::SoilLayer::cast(right_node());
				}

				SubSurfaceFlux(cmf::upslope::SoilLayer_ptr left,cmf::water::flux_node::ptr right, std::string Name,real FlowWidth,real Distance=0)
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
			class Darcy : public SubSurfaceFlux {
			protected:
				std::tr1::weak_ptr<cmf::upslope::SoilLayer> sw1,sw2;
				real 
					flow_width, distance;
				virtual real calc_q(cmf::math::Time t) ;
				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				static const CellConnector cell_connector;
				Darcy(cmf::upslope::SoilLayer_ptr left,cmf::water::flux_node::ptr right,real FlowWidth,real Distance=0)
					: SubSurfaceFlux(left,right,"Darcy flow",FlowWidth,Distance) 
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
			class TopographicGradientDarcy : public SubSurfaceFlux {
			protected:
				virtual real calc_q(cmf::math::Time t) ;
				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				static const cmf::upslope::CellConnector cell_connector;
				TopographicGradientDarcy(cmf::upslope::SoilLayer_ptr left,cmf::water::flux_node::ptr right,real FlowWidth,real Distance=0)
					: SubSurfaceFlux(left,right,"Topographic gradient",FlowWidth,Distance)
				{ }


			};
			/// A tipping bucket percolation approach similar to the approach in SWAT
			///
			/// \f{eqnarray*} q_{perc} &=& V_{H_2O,drain}\left(1-e^{-\frac 1{-TT_{perc}}}\right) \\
			/// TT_{perc} &=& \frac{V_{pores,drain} - V_{field cap.}}{K_{sat}} \\
			/// V_{x,drain} &=& V_{x,drain} - V_{field cap.} \f}
			/// 
			
			/// A connection similar to OHDIS-KWMSS (OHymos-based DIStributed model - with Kinematic Wave Method for Surface and Subsurface runoff) 
			///
			/// \f{eqnarray*} 
			/// q_{u} &=& w d_m k(\theta) \frac{\delta z}{\delta x} \\ 
			///	q_{s} &=& w (h-d)k_{sat} \frac{\delta z}{\delta x} + q_{u} \\
			/// q_{o} &=& w (\frac{sqrt{\left|\frac{\delta z}{\delta x}\right|}{n} (h-d_a)^(2/3) + q_s
			/// \f}
			class OHDISflow : public SubSurfaceFlux
			{
			protected:
				virtual real calc_q(cmf::math::Time t) ;

				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				OHDISflow(cmf::upslope::SoilLayer_ptr left,cmf::water::flux_node::ptr right,real FlowWidth, real Distance=0)
					: SubSurfaceFlux(left,right,"OHDIS like connection",FlowWidth,Distance)
				{			}

				static const CellConnector cell_connector;

			};

			class SWATPercolation : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::upslope::SoilLayer> sw1,sw2;
				void NewNodes()
				{
					sw1=cmf::upslope::SoilLayer::cast(left_node());
					sw2=cmf::upslope::SoilLayer::cast(right_node());
				}
				virtual real calc_q(cmf::math::Time t);

			public:
				static void use_for_cell(cmf::upslope::Cell & cell,bool no_override=true);
				SWATPercolation(cmf::upslope::SoilLayer_ptr upperLayer,cmf::upslope::SoilLayer_ptr lowerLayer)
					: flux_connection(upperLayer,lowerLayer,"SWAT percolation") {
						NewNodes();
				}

			};
			/// Calculates flow according to the Richards equation
			///
			/// \f{eqnarray*}
			/// q_{Richards} &=& \frac{\Delta\Psi_{tot}}{d} K(\theta) A \\
			/// \Psi_{tot} &= &\Psi_{M}(\theta) + h
			/// \f}
			/// where
			/// - \f$ \Delta\Psi_{tot} [m]\f$ is the difference of the total water potentials of the two soil layers
			/// - \f$ d [m]\f$ is the distance between the two soil layers
			/// - \f$ K(\theta)\left[\frac m{day}\right]\f$ is the geometric mean conductivity (see SoilType::Kunsat)
			/// - \f$ A [m^2]\f$ is the crosssectional area of the flux
			/// - \f$ \Psi_M(\theta) [m]\f$ is the matrix potential (see SoilType::MatrixPotential)
			/// - \f$ h [m]\f$ is the height of a soil layer above sea level
			class Richards : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::upslope::SoilLayer> sw1,sw2;
				void NewNodes()
				{
					sw1=cmf::upslope::SoilLayer::cast(left_node());
					sw2=cmf::upslope::SoilLayer::cast(right_node());
				}

				virtual real calc_q(cmf::math::Time t) ;
			public:
				Richards(cmf::upslope::SoilLayer_ptr left,cmf::water::flux_node::ptr right)
					: flux_connection(left,right,"Richards eq.")
				{
					NewNodes();
				}
				static void use_for_cell(cmf::upslope::Cell & cell,bool no_override=true);
			};


			class Richards_lateral: public SubSurfaceFlux
			{
			protected:
				real flow_thickness;
				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
				virtual real calc_q(cmf::math::Time t) ;

			public:
				Richards_lateral(cmf::upslope::SoilLayer_ptr left,cmf::water::flux_node::ptr right,real FlowWidth=0,real Distance=0)
					: SubSurfaceFlux(left,right,"Richards eq.",FlowWidth,Distance)
				{
					flow_thickness=left->get_thickness();
					if (!sw2.expired())
					{
						flow_thickness=minimum(flow_thickness,sw2.lock()->get_thickness());
					}
				}
				static const CellConnector cell_connector;
			};
			/// Calculates the flux using a simplified version of the Richards equation, without using the suction potential
			///
			/// \f[q_{simpl. R} = \frac{\Delta z}{d} K(\theta) A \f]
			class UnsaturatedDarcy : public SubSurfaceFlux {
			protected:
				virtual real calc_q(cmf::math::Time t) ;
				void NewNodes()
				{
					sw1=cmf::upslope::SoilLayer::cast(left_node());
					sw2=cmf::upslope::SoilLayer::cast(right_node());
				}

				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				UnsaturatedDarcy(cmf::upslope::SoilLayer_ptr left,cmf::upslope::SoilLayer_ptr right,real FlowWidth,real Distance=0)
					: SubSurfaceFlux(left,right,"Unsaturated Darcy",FlowWidth,Distance) 
				{				}

				static void use_for_cell(cmf::upslope::Cell & cell,bool no_override=true);
				static const CellConnector cell_connector;
			};

		}

	}
}

#endif // subsurfacefluxes_h__

