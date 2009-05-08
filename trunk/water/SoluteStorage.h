#ifndef SoluteStorage_h__
#define SoluteStorage_h__

#include "Solute.h"
#include "../math/StateVariable.h"
#include "../math/real.h"

//#include "Reaction.h"
namespace cmf {
	namespace water {
		class WaterStorage;
		class SoluteStorageMap;
		/// A class for the storage of any tracer. The state is the amount (mol, kg etc. see cmf::water) of the tracer in the storage
		/// \f{eqnarray*}
		/// \frac{dX}{dt}&=&\sum_{f=1}^{F}\left( q_f [X]_f\right) + \sum_{r=1}^R\left(f_r\left([A],...,[Z]\right)\ V\right) \left[\frac{mol}{day}\right]\\
		/// F&=& \mbox{Number of fluxes in water storage} \\
		/// q_f&=& \mbox{Water flux in } \frac{m^3}{day}	\\
		/// \left[X\right]_f &=& \mbox{Concentration of solute X in flux }q_f \mbox{ in } \frac{mol}{m^3} \\
		/// R&=& \mbox{Number of reactions defined for this solute storage} \\
		/// f_r\left([A],...,[Z]\right)&=& \mbox{Reactive flux of }[X] \mbox{ in environment } [A],...,[Z] \left[\frac{mol}{m^3\ day}\right] \\
		/// V &=& \mbox{Volume of water in water storage }\left[m^3\right]
		/// \f}
		class SoluteStorage : public cmf::math::StateVariable
		{
			SoluteStorage(WaterStorage& water,const cmf::water::Solute& solute, double InitialState=0) 
				: cmf::math::StateVariable(InitialState),Storage(water),Solute(solute),AdditionalFlux(0) {}
		public:
			friend class WaterStorage;
			/// Provides an interface to other models to update the rate. This variable can be set by the += operator
			/// Unit is in \f$\frac{mol}{day}\f$, where mol is only a proxy for the unit of the solute
			real AdditionalFlux;
			SoluteStorage& operator +=(real additionalRate)
			{
				AdditionalFlux=additionalRate;
				return *this;
			}
			/// The reactions applying to this Solute(vector of pointers to Reaction objects)
			//reaction::ReactionVector Reactions;
			/// The water storage to which the concentration storage belongs
			WaterStorage const & Storage;
			/// The solute, which is stored in this
			const cmf::water::Solute& Solute;
			/// Returns the concentration of the solute
			real conc() const;
			virtual real Derivate(const cmf::math::Time& time);
		};
	}
}

#endif // SoluteStorage_h__
