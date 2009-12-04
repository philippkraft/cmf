#ifndef ExplicitEuler_fixed_h__
#define ExplicitEuler_fixed_h__

#include "Integrator.h"
namespace cmf {
	namespace math {
		/// An explicit Euler integrator, with a fixed time step
		class ExplicitEuler_fixed : public Integrator
		{
		private:
			num_array 
				dxdt;
		public:
			void AddStatesFromOwner(cmf::math::StateVariableOwner& stateOwner)
			{
				Integrator::AddStatesFromOwner(stateOwner);
				dxdt.resize(count());
			}

			/// Constructs a new ExplicitEuler_fixed from a pointer to a vector of state variables
			/// @note The Integrator becomes the owner of states
			/// @param states Statevariables of the system
			/// @param epsilon (ignored) relative error tolerance per time step (default=1e-9)
			/// @param tStepMin (ignored) minimum time step (default=10s)
			ExplicitEuler_fixed(const StateVariableVector& states, real epsilon=1e-9,cmf::math::Time tStepMin=Time::Seconds(10))
				: Integrator(states,epsilon,tStepMin),
				dxdt(states.size())
			{}
			ExplicitEuler_fixed(StateVariableOwner& states, real epsilon=1e-9,cmf::math::Time tStepMin=Time::Seconds(10))
				: Integrator(states,epsilon,tStepMin),
				dxdt(m_States.size())
			{}

			/// Constructs a new ExplicitEuler_fixed
			/// @param epsilon (ignored) relative error tolerance per time step (default=1e-9)
			/// @param tStepMin (ignored) minimum time step (default=10s)
			ExplicitEuler_fixed(real epsilon=1e-9,cmf::math::Time tStepMin=Time::Seconds(10))
				: Integrator(epsilon,tStepMin)
			{	}

			/// Copy constructor
			ExplicitEuler_fixed(const Integrator& copy) : Integrator(copy) {}
			virtual ~ExplicitEuler_fixed() {}
			virtual Integrator * Copy() const
			{
				return new ExplicitEuler_fixed(*this);
			}

			///Integrates the vector of state variables
			/// @param MaxTime (ignored) To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed time step
			int Integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
			{
				m_TimeStep=TimeStep;
				if (m_TimeStep>MaxTime-ModelTime())
					m_TimeStep=MaxTime-ModelTime();
				// Copy derivates multipied with time step to dxdt
				States().CopyDerivs(ModelTime(),dxdt,m_TimeStep.AsDays());
				// Update time step with delta x
				States() += dxdt;
				m_Time += m_TimeStep;
				
				return 1;
			}

		};

		/// A simple predictor - corrector solver
		/// \f[ y^{n+1} = y^n + \alpha f(y^n + f(y^n)dt)dt + (1-\alpha)f(y^n)dt\f]
		class PredictCorrectSimple : public Integrator
		{
		private:
			num_array 
				dxdt0, dxdt1, old_states;
		public:
			void AddStatesFromOwner(cmf::math::StateVariableOwner& stateOwner)
			{
				Integrator::AddStatesFromOwner(stateOwner);
				dxdt0.resize(count());
				dxdt1.resize(count());
				old_states.resize(count());
			}
			real alpha;
			/// Constructs a new PredictCorrectSimple from a pointer to a vector of state variables
			/// @note The Integrator becomes the owner of states
			/// @param states Statevariables of the system
			/// @param Alpha Weight factor \f$\alpha\f$ to weight \f$f(y^n)\f$ and \f$f(y^{n+1})\f$
			PredictCorrectSimple(const StateVariableVector& states, real Alpha)
				: Integrator(states,1e-9,Time::Seconds()), alpha(Alpha)
			{}
			/// Constructs a new PredictCorrectSimple from a pointer to a vector of state variables
			/// @note The Integrator becomes the owner of states
			/// @param states Statevariable owner of the system
			/// @param Alpha Weight factor \f$\alpha\f$ to weight \f$f(y^n)\f$ and \f$f(y^{n+1})\f$
			PredictCorrectSimple(StateVariableOwner& states, real Alpha)
				: Integrator(states,1e-9,Time::Seconds()),alpha(Alpha)
			{}

			/// Constructs a new PredictCorrectSimple
			/// @param Alpha Weight factor \f$\alpha\f$ to weight \f$f(y^n)\f$ and \f$f(y^{n+1})\f$
			PredictCorrectSimple(real Alpha=0.5)
				: Integrator(1e-9,Time::Seconds()),alpha(Alpha)
			{	}

			/// Copy constructor
			PredictCorrectSimple(const Integrator& copy) : Integrator(copy) {}
			virtual Integrator * Copy() const
			{
				return new ExplicitEuler_fixed(*this);
			}

			///Integrates the vector of state variables
			/// @param MaxTime (ignored) To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed time step
			int Integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
			{
				m_TimeStep=TimeStep;
				if (m_TimeStep>MaxTime-ModelTime())
					m_TimeStep=MaxTime-ModelTime();
				States().CopyStates(old_states);
				// get f(y^n)dt
				States().CopyDerivs(ModelTime(),dxdt0,m_TimeStep.AsDays());
				// Update time step with delta x
				States() += dxdt0;
				if (alpha>0)
				{
					// get f(y^n+1)dt
					States().CopyDerivs(ModelTime(),dxdt1,m_TimeStep.AsDays() * alpha);
					// reset states to y^n
					States().SetStates(old_states);
					// update states to y^n+1
					if (alpha<1) 
					{
						dxdt0 *= (1-alpha);
						States()+=dxdt0;
					}
					States() +=dxdt1;
				}
				m_Time += m_TimeStep;

				return 1;
			}

		};

	}
}

#endif // ExplicitEuler_fixed_h__
