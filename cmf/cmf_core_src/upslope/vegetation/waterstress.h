#ifndef waterstress_h__
#define waterstress_h__

#include "../../math/real.h"


namespace cmf {
	namespace upslope {
		namespace ET {

			class stressedET;
			/// An abstract class to calculate the actual transpiration from potential transpiration.
			///
			/// Implementations of WaterStressFunction are used by ET connections derived from cmf::upslope::ET::stressedET
			///
			class RootUptakeStessFunction {
			protected:
				stressedET* connection;
			public:
				/// Calculates the water stress for a layer
				///
				/// @param connection The stressedET connection this stress function belongs to
				/// @param Tpot Potential Transpiration in mm/day (for the full profile)
				/// @returns Actual flux from layer in m3/day
				virtual real Tact(const stressedET* connection,real Tpot) const = 0;
				/// Creates a new copy of this wetness
				virtual cmf::upslope::ET::RootUptakeStessFunction* copy() const = 0;
				virtual std::string to_string() const = 0;
				virtual ~RootUptakeStessFunction()
				{

				}
			};

			/// The classical suction depending transpiration Stress curve after Feddes
			///
			/// The ability of roots to take water from the soil up is limited by the suction pressure (matrix potential) of the 
			/// soil. The stress is defined using a trapezoid function, with P0 being the pressure, where water uptake is prohibited
			/// by saturation. Between P0 and P1 the saturation stress gets lower. Between P1 and P2, no stress occurs (ET=ETpot) and
			/// with a pressure below P2, water uptake is limited by drought. P3 indicates the highest suction (lowest pressure) to which 
			/// plants can extract water (wilting point). 
			///
			/// Default values:
			/// \f$P_0 = \infty, P_1=\infty, P_2=-5m, P_3=-160m\f$, resulting in no saturation stress and draught stress starting at 
			/// -5m and a wilting point of -160m.
			///
			/// If one would like to include saturation stress, typical values for P0 and P1 are 0.0m and 0.1m. By changing
			/// P2 and P3, you can account for different drought sensibility for different plant types.
			///
			class SuctionStress : public RootUptakeStessFunction {
			private:
				real fT(real potential) const;
			public:
				real Tact(const stressedET* connection,real Tpot) const;
				real 
					P0, ///< Above this pressure, ET=0.0 due to saturation (typical value: 0.0m)
					P1, ///< Between P0 and P1 ET raises linear from 0.0 to ETpot (typical value: -0.1m)
					P2, ///< Between P1 and P2, ET is unstressed, ET=ETpot (typical value: -5 m)
					P3; ///< Between P2 and P3, ET drops linear with the pressure head to zero due to drought (typical value: -160m)
				
				/// Creates a new suction stress function. 
				SuctionStress(real P0=1e308,real P1=1e307,real P2=-5,real P3=-160);

				SuctionStress(const SuctionStress& other);
				cmf::upslope::ET::SuctionStress* copy() const;
				std::string to_string() const {
					return "suction based stress";
				}
				virtual ~SuctionStress()
				{

				}

			};

			/// A simple water content based stress model based on Feddes (1978)
			///
			/// stress a is piecewise linear function based on water content
			///
			///  - if \f$\theta>\theta_d \rightarrow ET = ET_{pot}\f$
			///  - if \f$\theta_d>\theta > \theta_{wilt} \rightarrow ET = \frac{\theta_d - \theta}{\theta_d - \theta_{wilt}} ET_{pot}\f$
			///  - if \f$\theta_{wilt}>\theta \rightarrow ET = 0.0 \f$
			///
			/// The parameters \f$\theta_{d,w}\f$ can be omitted (or set to a negative value) to use default values derived from the layer
			/// properties. The critical water content (where stress starts) is then defined (conf. Feddes 1978) as the center between 
			/// field capacity (\f$\theta_{fc}\f$) and the wilting point (\f$\theta_{wp}\f$).
			///   - \f$\theta_w = \theta(pF=4.2)\f$: Water content at pF=4.2
			///   - \f$\theta_d = 1/2 (\theta(pF=1.8) + \theta(pF=4.2))\f$
			class ContentStress : public RootUptakeStessFunction {
			public:
				real theta_d;
				real theta_w;
				real Tact(const stressedET* connection,real Tpot) const;
				cmf::upslope::ET::ContentStress* copy() const;
				std::string to_string() const {
					return "water content based stress (Feddes 1978)";
				}
				/// Creates a new ContentStress for critical water content theta_d in vol Water/vol Soil and wilting point theta_w.
				ContentStress(real theta_d = -1, real theta_w = -1);
				virtual ~ContentStress()
				{

				}
			};
			/// A WaterStressFunction based on the stored water volume of a layer
			///
			/// If the layer contains more water than V1, ET is not limited (ET=ETpot). Below V1
			/// ET goes linear to 0.0 at V0
			class VolumeStress : public RootUptakeStessFunction {
			public:
				real 
					V1, ///< Upper volume threshold in \f$m^3\f$. If the layer contains more water than V1, ET=ETpot
					V0; ///< Lower volume threshold in \f$m^3\f$. If the layer contains less water than V0, ET=0.0
				real Tact(const stressedET* connection,real Tpot) const;

				/// Creates a new water stress function for water volume
				///
				/// @param V1 Water content in \f$m^3\f$ without stress (ET=ETpot)
				/// @param V0 Water content in \f$m^3\f$ with full stress (ET=0.0)
				VolumeStress(real V1,real V0);
				VolumeStress(const VolumeStress& other);
				cmf::upslope::ET::VolumeStress* copy() const;
				std::string to_string() const {
					return "volume based stress";
				}
				virtual ~VolumeStress()
				{

				}
			};

		}
	}
}
#endif // waterstress_h__
