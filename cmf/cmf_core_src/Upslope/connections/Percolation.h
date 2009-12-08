#ifndef Percolation_h__
#define Percolation_h__


#include "../../water/flux_connection.h"
#include "../cell.h"
#include "../topology.h"
#include "../SoilLayer.h"

namespace cmf {
	namespace upslope {
		namespace connections {
			/// @defgroup perc Percolation
  		/// @ingroup connections
			
			/// @ingroup perc
			/// A tipping bucket percolation approach similar to the approach in SWAT
			///
			/// \f{eqnarray*} q_{perc} &=& V_{H_2O,drain}\left(1-e^{-\frac 1{-TT_{perc}}}\right) \\
			/// TT_{perc} &=& \frac{V_{pores,drain} - V_{field cap.}}{K_{sat}} \\
			/// V_{x,drain} &=& V_{x,drain} - V_{field cap.} \f}
			/// 
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
				SWATPercolation(cmf::upslope::SoilLayer::ptr upperLayer,cmf::upslope::SoilLayer::ptr lowerLayer)
					: flux_connection(upperLayer,lowerLayer,"SWAT percolation") {
						NewNodes();
				}

			};
			/// @ingroup perc
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
				Richards(cmf::upslope::SoilLayer::ptr left,cmf::water::flux_node::ptr right)
					: flux_connection(left,right,"Richards eq.")
				{
					NewNodes();
				}
				static void use_for_cell(cmf::upslope::Cell & cell,bool no_override=true);
			};


		}
	}
}
#endif // Percolation_h__
