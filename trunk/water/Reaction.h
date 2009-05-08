#ifndef Reaction_h__
#define Reaction_h__

namespace cmf {
	namespace water {
		namespace reaction {
			/// An abstract type to use chemical (or other) reactions of solutes.
			/// Override the ReactiveFlux function or use one of the child classes, if appropriate (Have a look at the ExternalFunctionReaction, before you start recompiling cmf)
			class Reaction
			{
			public:
				/// The solute this reaction is changing (X)
				const cmf::water::Solute& Solute;
				/// Returns the reactive flux as \f$\frac{d[X]}{dt}=f([A],[B]...) \left[\frac{mol}{m^3\ day}\right]\f$ 
				virtual double ReactiveFlux(const WaterQuality& Environment)=0;
				Reaction(const cmf::water::Solute& solute) : Solute(solute) {}
			};

			
			/// A simple reaction with a constant reaction rate
			class RateReaction : public Reaction
			{
				/// The reaction rate in \f$\frac 1{day}\f$, positive rates will lead to an exponential rise of the concentration
				double r;
				/// Reactive flux: \f$ \frac{d[X]}{dt}=r\ [X] \f$
				virtual double ReactiveFlux(const WaterQuality& Environment)
				{
					 return r*Environment[Solute];
				}
				/// Creates a rate based reaction rate
				/// @param solute The solute the reaction applies to
				/// @param ReactionRate The constant (but changeable) rate of the reaction
				RateReaction(const cmf::water::Solute& solute,double ReactionRate=0) : Reaction(solute),r(ReactionRate) {}

			};

			/// A simple reaction with a constant linear change of the concentration.
			/// If the constant range is \f$ 1\frac{mol}{m^3\ day}\f$ and the rainfall deposition is 0, the concentration
			/// in each state equals the time the water stayed in the system in days
			class LinearReaction: public Reaction
			{
				/// The linear rate in \f$\frac{mol}{m^3\ day}\f$. Can be used for mean residence time calculations 
				double r;
				/// Reactive flux: \f$ \frac{d[X]}{dt}=r V \f$, where V is the volume of water
				virtual double ReactiveFlux(const WaterQuality& Environment)
				{
					if (Environment[Solute]<=0 && r<0)
						return 0;
					else
						return r;
				}
				/// Creates a aging tracer reaction
				/// @param solute The solute the reaction applies to
				/// @param LinearRate The constant (but changeable) rate of the reaction in  \f$\frac{mol}{m^3\ day}\f$
				LinearReaction(const cmf::water::Solute& solute,double LinearRate=0) : Reaction(solute),r(LinearRate) {}
			};
			/// A Michaelis Menten reaction without inhibitors
			class MichaelisMentenReaction : public Reaction
			{
			public:
				/// Michaelis Menten constant \f$ \frac{d[X]}{dt}=0.5 \mbox{ if } [X]=K_m \f$
				double K_m;
				/// Maximum reaction rate
				double V_max;
				/// Reactive flux: \f$ \frac{d[X]}{dt}=\frac{v_{max}\ [X]}{K_m+[X]} \f$
				virtual double ReactiveFlux(const WaterQuality& Environment)
				{
					double X=Environment[Solute];
					return V_max*X/(K_m+X);
				}
			};
			/// Not yet implemented: Will take a pointer to an external function to calculate the reaction
			class ExternalFunctionReaction : public Reaction
			{
			public:


			};
			typedef std::vector<Reaction*> ReactionVector;
		}
	}
	
}
#ifdef SWIG
%template(ReactionVector) std::vector<cmf::water::Reaction*>;
#endif
#endif // Reaction_h__
