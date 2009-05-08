#ifndef ExplicitEuler_fixed_h__
#define ExplicitEuler_fixed_h__

#include "Integrator.h"
namespace cmf {
	namespace math {
		/// An explicit Euler integrator, with a fixed time step
		class ExplicitEuler_fixed : public Integrator
		{
		private:
			numVector 
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

	}
}

#endif // ExplicitEuler_fixed_h__
