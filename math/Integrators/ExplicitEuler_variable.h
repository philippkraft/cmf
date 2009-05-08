#ifndef ExplicitEuler_variable_h__
#define ExplicitEuler_variable_h__

#include "Integrator.h"
#include <cmath>
#define max(a,b) ((a)>(b) ? (a) : (b))
#define min(a,b) ((a)<(b) ? (a) : (b))

namespace cmf {
	namespace math {
		/// An explicit Euler integrator with variable step size
		class ExplicitEuler_variable : public Integrator
		{
		private:
			numVector 
				dxdt,
				compare,
				oldStates;
		public:
			/// A public field triggering error based stepsize control
			bool WithStepsizeControl;
			void AddStatesFromOwner(cmf::math::StateVariableOwner& stateOwner)
			{
				Integrator::AddStatesFromOwner(stateOwner);
				dxdt.resize(count());
				compare.resize(count());
				oldStates.resize(count());
			}

			/// Constructs a new ExplicitEuler_variable from a pointer to a vector of state variables
			/// @note The Integrator becomes the owner of states
			/// @param states Statevariables of the system
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			ExplicitEuler_variable(const StateVariableVector& states, real epsilon=1e-9,cmf::math::Time tStepMin=Time::Seconds(10))
				: Integrator(states,epsilon,tStepMin)
			{}

			/// Constructs a new ExplicitEuler_variable
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			ExplicitEuler_variable(real epsilon=1e-9,cmf::math::Time tStepMin=Time::Seconds(10))
				: Integrator(epsilon,tStepMin)	{	}

			/// Copy constructor
			ExplicitEuler_variable(const Integrator& copy) : Integrator(copy) {}
			virtual ~ExplicitEuler_variable() {}

			virtual Integrator * Copy() const
			{
				return new ExplicitEuler_variable(*this);
			}

			///Integrates the vector of state variables
			/// @param MaxTime To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed time step, and changes it into the effectively used time step according to the local stiffness of the problem and MaxTime
			int Integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
			{
				Time h=TimeStep;
				// Copy current state to oldStates
				States().CopyStates(oldStates);
				real err_ex=0;
				do 
				{
					// Calculate states in two half time steps
					for (int i = 0; i < 2 ; i++)
					{
						States().CopyDerivs(ModelTime() + ((h*i)/2).AsDays(),dxdt,h.AsDays()/2);
						States()+=dxdt;			
					}
					// Store the result as comparison
					States().CopyStates(compare);
					// Reset the states
					States().SetStates(oldStates);
					// Make Euler step
					States().CopyDerivs(ModelTime(),dxdt,h.AsDays());
					// Get error exceedance
					err_ex=error_exceedance(compare)+1e-100;
					m_TimeStep=h;
					// Calculate new time step
					h/=max(min(err_ex,0.1),10);

					if (err_ex>1 && h>m_MinTimestep)
						// Failed: Reset the states
						States().SetStates(oldStates);
					else
						// Mission accomplished
						break;

				} while(1);
				m_NextTimeStep=h;
				m_Time += m_TimeStep;
				return 1;
			}

		};
	}
}
#undef max
#undef min

#endif // ExplicitEuler_variable_h__
