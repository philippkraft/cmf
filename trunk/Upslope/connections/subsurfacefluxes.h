#ifndef subsurfacefluxes_h__
#define subsurfacefluxes_h__

#include "../../water/FluxConnection.h"
#include "../cell.h"
#include "../topology.h"
#include "../SoilWaterStorage.h"

namespace cmf {
	namespace upslope {
		namespace connections {

			/// Calculates the lateral flow using the gravitational potential gradient only
			///
			/// \f[ q_{lat} = \frac{\Delta \Psi_G}{\|C_1-C_2\|} \frac 12 (T(C_1)+T(C_2)) w \f]
			/// where
			/// - \f$ \Delta \Psi_G \f$ is the gravitational potential difference
			/// - \f$ \|C_1-C_2\| \f$ is the distance from Cell 1 to Cell 2
			/// - \f$ T(C) \f$ is the transmissivity of cell C, calculated by SoilType::Transmissivity
			/// - \f$ w \f$ is the width of the connection of the cells
			class Darcy : public cmf::water::FluxConnection {
			protected:
				cmf::upslope::SoilWaterStorage *sw1,*sw2;
				real 
					flow_width, distance;
				virtual real calc_q(cmf::math::Time t) ;
				void NewNodes()
				{
					sw1=cmf::upslope::AsSoilWater(m_left);
					sw2=cmf::upslope::AsSoilWater(m_right);
				}
				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				static const CellConnector cell_connector;
				Darcy(cmf::upslope::SoilWaterStorage & left,cmf::water::FluxNode & right,real FlowWidth=0,real Distance=0)
					: cmf::water::FluxConnection(left,right,"Darcy's law"),flow_width(FlowWidth),distance(Distance) 
				{
					NewNodes();
					if (flow_width<=0 && sw2)
						flow_width=sw1->cell.get_topology().flowwidth(sw2->cell);
					if (distance<0)
						distance=sw1->Location.distance3DTo(sw2->Location);
				}

			};
			///	Calculates the lateral flow using the topographic gradient
			///
			/// \f[ q_{lat} = \frac{z_1 - z_2}{\|C_1-C_2\|} \frac 12 (T(C_1)+T(C_2)) w \f]
			/// where
			/// - \f$ z_1 - z_2 \f$ is the topographic height difference
			/// - \f$ \|C_1-C_2\| \f$ is the distance from Cell 1 to Cell 2
			/// - \f$ T(C) \f$ is the transmissivity of cell C, calculated by SoilType::Transmissivity
			/// - \f$ w \f$ is the width of the connection of the cells
			class TopographicGradientDarcy : public cmf::water::FluxConnection {
			protected:
				cmf::upslope::SoilWaterStorage *sw1,*sw2;
				real 
					flow_width, distance;
				virtual real calc_q(cmf::math::Time t) ;
				void NewNodes()
				{
					sw1=cmf::upslope::AsSoilWater(m_left);
					sw2=cmf::upslope::AsSoilWater(m_right);
				}
				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				static const cmf::upslope::CellConnector cell_connector;
				TopographicGradientDarcy(cmf::upslope::SoilWaterStorage& left,cmf::water::FluxNode & right,real FlowWidth=0,real Distance=0)
					: FluxConnection(left,right,"Topographic gradient"),flow_width(FlowWidth),distance(Distance)
				{
					NewNodes();
					if (flow_width==0 && sw2)
						flow_width=sw1->cell.get_topology().flowwidth(sw2->cell);
					if (distance==0)
						distance=sw1->Location.distance3DTo(sw2->Location);
				}


			};
			/// A tipping bucket percolation approach similar to the approach in SWAT
			///
			/// \f{eqnarray*} q_{perc} &=& V_{H_2O,drain}\left(1-e^{-\frac 1{-TT_{perc}}}\right) \\
			/// TT_{perc} &=& \frac{V_{pores,drain} - V_{field cap.}}{K_{sat}} \\
			/// V_{x,drain} &=& V_{x,drain} - V_{field cap.} \f}
			/// 
			class SWATPercolation : public cmf::water::FluxConnection {
			protected:
				cmf::upslope::SoilWaterStorage *sw1,*sw2;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					sw1=cmf::upslope::AsSoilWater(m_left);
					sw2=cmf::upslope::AsSoilWater(m_right);
				}

			public:
				static void use_for_cell(cmf::upslope::Cell & cell,bool no_override=true);
				SWATPercolation(cmf::upslope::SoilWaterStorage& upperLayer,cmf::upslope::SoilWaterStorage & lowerLayer)
					: FluxConnection(upperLayer,lowerLayer,"SWAT percolation") {
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
			class Richards : public cmf::water::FluxConnection {
			protected:
				cmf::upslope::SoilWaterStorage *sw1,*sw2;
				real 
					flow_area, distance;
				virtual real calc_q(cmf::math::Time t) ;
				void NewNodes()
				{
					sw1=cmf::upslope::AsSoilWater(m_left);
					sw2=cmf::upslope::AsSoilWater(m_right);
				}

				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				Richards(cmf::upslope::SoilWaterStorage& left,cmf::water::FluxNode & right,real FlowArea=0,real Distance=0)
					: FluxConnection(left,right,"Richards eq."),flow_area(FlowArea),distance(Distance)
				{
					NewNodes();
					if (flow_area==0 && sw2)
						flow_area=sw1->get_flow_crosssection(*sw2);
					if (distance==0)
						distance=sw1->Location.distance3DTo(sw2->Location);
				}
				static const CellConnector cell_connector;
				static void use_for_cell(cmf::upslope::Cell & cell,bool no_override=true);
			};
			class Richards_lateral: public Richards
			{
			public:
				Richards_lateral(cmf::upslope::SoilWaterStorage& left,cmf::water::FluxNode & right,real FlowWidth=0,real Distance=0)
					: Richards(left,right,FlowWidth,Distance)
				{
					flow_area=left.get_thickness()*FlowWidth;
					if (sw2)
					{
						flow_area=minimum(flow_area,sw2->get_thickness()*FlowWidth);
					}
				}
			};
			/// Calculates the flux using a simplified version of the Richards equation, without using the suction potential
			///
			/// \f[q_{simpl. R} = \frac{\Delta z}{d} K(\theta) A \f]
			class UnsaturatedDarcy : public cmf::water::FluxConnection {
			protected:
				cmf::upslope::SoilWaterStorage *sw1,*sw2;
				real 
					flow_area, distance,flow_width;
				virtual real calc_q(cmf::math::Time t) ;
				void NewNodes()
				{
					sw1=cmf::upslope::AsSoilWater(m_left);
					sw2=cmf::upslope::AsSoilWater(m_right);
				}

				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				UnsaturatedDarcy(cmf::upslope::SoilWaterStorage& left,cmf::upslope::SoilWaterStorage & right,real FlowArea=0,real Distance=0)
					: FluxConnection(left,right,"Unsaturated Darcy"),flow_area(FlowArea),distance(Distance) 
				{
					NewNodes();
					if (flow_area==0 && sw2)
						flow_area=sw1->get_flow_crosssection(*sw2);
					if (distance==0)
						distance=sw1->Location.distance3DTo(sw2->Location);
				}

				static void use_for_cell(cmf::upslope::Cell & cell,bool no_override=true);
				static const CellConnector cell_connector;
			};

			/// A connection similar to OHDIS-KWMSS (OHymos-based DIStributed model - with Kinematic Wave Method for Surface and Subsurface runoff) 
			///
			/// \f{eqnarray*} 
			/// q_{u} &=& w d_m k(\theta) \frac{\delta z}{\delta x} \\ 
			///	q_{s} &=& w (h-d)k_{sat} \frac{\delta z}{\delta x} + q_{u} \\
			/// q_{o} &=& w (\frac{sqrt{\left|\frac{\delta z}{\delta x}\right|}{n} (h-d_a)^(2/3) + q_s
			/// \f}
			class OHDISflow : public cmf::water::FluxConnection
			{
			protected:
				cmf::upslope::SoilWaterStorage *sw1,*sw2;
				real 
					flow_width,distance;
				virtual real calc_q(cmf::math::Time t) ;
				void NewNodes()
				{
					sw1=cmf::upslope::AsSoilWater(m_left);
					sw2=cmf::upslope::AsSoilWater(m_right);
				}

				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				OHDISflow(cmf::upslope::SoilWaterStorage& left,cmf::water::FluxNode& right,real FlowWidth, real Distance)
					: cmf::water::FluxConnection(left,right,"OHDIS like connection"),flow_width(FlowWidth),distance(Distance)
				{
					NewNodes();
				}

				static const CellConnector cell_connector;

			};
		}

	}
}

#endif // subsurfacefluxes_h__

