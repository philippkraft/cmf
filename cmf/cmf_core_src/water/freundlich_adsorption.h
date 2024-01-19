#include "../math/real.h"
#include "adsorption.h"

namespace cmf {
	namespace water {

        #ifdef CMF_DOCUMENT_FREUNDLICH
        /// @brief BROKEN: This class calculates the adsorption equilibrium between sorbat and sorbent using the Freundlich isotherme.
        ///
        /// Freundlich isotherme:
        ///
        /// \f[\frac{x_{ad}}{m} = K c^n\f]
        /// where
        ///  - \f$x_{ad} = x_{tot} - x_{free}\f$ is the adsorbed tracer mass
        ///     - \f$x_{tot}\f$ is the total tracer mass
        ///     - \f$x_{free}\f$ is the dissolved tracer mass
        ///  - \f$m\f$ is the mass of the sorbent in the same unit as the tracer mass
        ///  - \f$K\f$ is the Freundlich sorption coefficient
        ///  - \f$c = \frac{x_{free}}{V}\f$ is the concentration of the tracer in tracer mass per m3
        ///  - \f$n\f$ is the Freundlich exponent
        ///
        /// CMF stores in a solute storage the total mass of a tracer and needs to calculate the free tracer mass.
        /// The eq. above can not be rearanged to get \f$x_{free}\f$ from \f$x_{tot}\f$. Instead, the value is iterated
        /// using [regula falsi](http://en.wikipedia.org/wiki/False_position_method). If n is near to 1,
        /// using LinearAdsorption will speed up your calculations.
        ///
        /// The simplest physically based adsorption model by Langmuir (LangmuirAdsorption) has also a analytical solution
        /// and is hence calculated faster then Freundlich.
        #endif
        class FreundlichAdsorbtion: public Adsorption {
        public:
            /// Freundlich half saturation
            real K;
            /// Freundlich n
            real n;
            /// Mass of sorbent (CEC, clay mass etc.)
            real m;
            /// Tolerable error for Newton iteration
            real epsilon;
            /// Maximum number of iterations
            int maxiter;
            real freesolute(real xt,real V) const;
            real totalsolute(real xf, real V) const;
            /// @param K,n Freundlich coefficents
            /// @param m Mass of sorbent in units of tracer
            /// @param epsilon Tolerance of regula falsi iteration for the calculation of dissolved tracer from total trace, default = 1e-12
            /// @param maxiter Maximum number of iterations, default = 100
            FreundlichAdsorbtion(real K,real n, real m, real epsilon=1e-12, int maxiter=100);
            FreundlichAdsorbtion(const FreundlichAdsorbtion& other);
            FreundlichAdsorbtion* copy(real m=-1) const;
            virtual ~FreundlichAdsorbtion()
            {

            }
        };
    }
}
