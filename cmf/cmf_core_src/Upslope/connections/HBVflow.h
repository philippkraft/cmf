#ifndef HBVflow_h__
#define HBVflow_h__

#include "../../water/flux_connection.h"
#include "../cell.h"
#include "../topology.h"
#include "../SoilLayer.h"
#include "subsurfacefluxes.h"
#include "../Soil/RetentionCurve.h"

namespace cmf {
	namespace upslope {
		namespace connections {
			/// @defgroup HBV HBV like connections
			/// @ingroup connections
			/// These connections represent the governing equations of the HBV model

			/// @ingroup HBV
			/// This class holds the properties for all HBV like connections
			/// Implements the RetentionCurve interface is only partly, therefore it is not suitable for other connection types
			///
			/// \f{eqnarray*} 
			/// K(w,d) &=& PERC w \\
			/// \Psi_{M} &=& 10 - \frac{10}{w}\\
			/// w(\Psi_{M}) &=& \frac{10}{10 - \Psi_M} 
			/// \f}
			struct HBVparameters : cmf::upslope::RetentionCurve
			{
				double
					cfmax, ///< Degree-Day factor
					sfcf,  ///< Snowfall correction factor
					cfr,   ///< refreezing coefficient
					cwh,   ///< Water holding capacity (of snow)
					k0,	 ///< runoff velocity of unsaturated layer in 1/day
					k1,  ///< runoff velocity of upper box in 1/day
					k2,	 ///< runoff velocity of lower box in 1/day
					perc, ///< percolation velocity in mm/day
					fc,   ///< field capacity in m3 water/m3 soil
					uplim, ///< upper box capacity in m3 water/m3 soil
					lowlim, ///< lower box capacity in m3 water/m3 soil
					beta;   ///< Percolation curve shape parameter

				HBVparameters(double _k0=1, double _k1=0.25, double _k2=0.005, double _perc = 0.05, 
											double _fc=0.3, double _beta = 4.0, double _uplim = .35, double _lowlim = 1.0, 
											double _cfmax=2 ,double _sfcf=0.6, double _cwh=0.1, double _cfr = 0.05)
				: k0(_k0),k1(_k1),k2(_k2),
					perc(_perc),fc(_fc),uplim(_uplim),lowlim(_lowlim),beta(_beta),
					cfmax(_cfmax),sfcf(_sfcf),cfr(_cfr),cwh(_cwh)
				{}
				virtual real K(real wetness,real depth) const {return perc * wetness;}
				virtual real Porosity(real depth) const { return fc;}
				virtual real VoidVolume(real upperDepth,real lowerDepth,real Area) const {return fc * Area * (lowerDepth-upperDepth);}
				virtual real Wetness(real suction) const {return 10/(10-suction);}
				virtual real MatricPotential(real wetness) const { return 10-10/wetness;}
				virtual HBVparameters* copy() const
				{
					return new HBVparameters(*this);
				};
				static HBVparameters* cast(cmf::upslope::RetentionCurve & for_cast)
				{
					HBVparameters* res=dynamic_cast<HBVparameters*>(&for_cast);
					if (res)
						return res;
					else
					{
						return 0;
					}
				
				}
			};
			/// @ingroup HBV
			/// Calculates the percolation as in the HBV model
			///
			/// \f[ Q = \left\{Q_{in} \left(\frac{V}{FC}\right)^\beta\mbox{ if first layer} \\
			///                min(PERC, V_{lower} - V_{lower,max}) \right. \f]
			class HBVpercolation : public cmf::water::flux_connection	{
			protected:
				std::tr1::weak_ptr<cmf::upslope::SoilLayer> sw1,sw2;
				void NewNodes();
				virtual real calc_q(cmf::math::Time t);
				//std::auto_ptr<HBVparameters> params;
			public:
				HBVpercolation(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right)
					: flux_connection(left,right,"HBV percolation")
				{
					NewNodes();
				}
			};
			/// @ingroup HBV
			/// Lateral flow using a simple linear storage, can be directed to another cell (HBV D like) or to a reach (HBV like)
			///
			/// \f[ q = kV \f]
			class HBVlateral : public cmf::upslope::connections::lateral_sub_surface_flux
			{
			protected:
				std::tr1::weak_ptr<cmf::upslope::SoilLayer> sw1;
				void NewNodes();
				virtual real calc_q(cmf::math::Time t);
				static void connect_cells(cmf::upslope::Cell & cell1,cmf::upslope::Cell & cell2,int start_at_layer=0);
			public:
				static const CellConnector cell_connector;
				HBVlateral(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right, real width, real dist)
					: lateral_sub_surface_flux(left,right,"HBV lateral flow",width,dist)
				{
					NewNodes();
				}

			};


			/// Installs an HBV like model setup at a cell
			/// Reference: Bergstroem et al 1995, in Singh (1995): Computer models of watershed hydrology
			///
			/// Removes the layers of a cell and adds three layers with 1m thickness and the HBVparameters as retention curve.
			/// Installs HBVpercolation between the layers
			/// @param parameters The parameters for the HBV like setup
			/// @param cell Cell where HBV like percolation is to be calculated
			void HBVinstall(HBVparameters parameters,cmf::upslope::Cell& cell);
			
		}	
	}
	
}
#endif // HBVflow_h__
