

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   cmf is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
//   
#ifndef Integrator_h__
#define Integrator_h__

#include "../num_array.h"
#include "../StateVariable.h"
#include "../real.h"
#include <stdexcept>
namespace cmf {
	namespace math {
		/// Base class for any kind of integrator
		///
		/// Pure virtual functions:
		/// - Integrate
		/// - Copy
		/// Please provide a custom copy constructor
		/// @todo Put the methods of StateVariableVector here, and delete StateVariableVector
		class Integrator
		{
		protected:
			///@name The state variables to integrate
			//@{
			StateVariableVector m_States;
			int error_position;
			real error_exceedance( const num_array& compare,int * biggest_error_position=0 )
			{
				real res=0;
#pragma omp parallel for shared(res)
				for (int i = 0; i < count() ; i++)
				{
					real error=fabs(compare[i]-state(i));
					// Calculate absolute error tolerance as: epsilon + |(x_p+x_(n+1))/2|*epsilon
					real errortol=Epsilon + fabs(state(i))*Epsilon;
					if (error/errortol>res)
#pragma omp critical
					{
						if (error/errortol>res)
						{
							res=error/errortol;
							if (biggest_error_position)
								*biggest_error_position=i;
						}
					}
				}
				return res;
			}
			
			//@}
		public:
			/// A public variable to identify the solver
			char Tag;
			bool UseEulerAtTmin;
			bool use_OpenMP;

			/// returns the number of state variables
			int count() const
			{
				return (int)m_States.size();
			}
			/// Simplifies the assessment of state variables
			real state(int position) const
			{
				return m_States[position]->get_state();
			}
			/// Simplifies the assessment of state variables
			void state(int position,real newState)
			{
				m_States[position]->set_state(newState);
			}
			
			/// @name Assessment of state variables for integration
			//@{ 
			/// Adds the state variables of a StateVariableOwner to the state variables of the solver
			virtual void AddStatesFromOwner(cmf::math::StateVariableOwner& stateOwner)
			{
				stateOwner.AddStateVariables(m_States);
			}
			virtual void AddState(cmf::math::StateVariable& statevar)
			{
				m_States.push_back(&statevar);
			}
			//@}

		protected:
			/// Returns the vector of StateVariable pointers 
			cmf::math::StateVariableVector& States() {return m_States;}
			const cmf::math::StateVariableVector& States() const {return m_States;}
			///@name Accuracy parameters
			//@{
			///Tolerable error
			const real Epsilon;
			///Minimal timestep
			const cmf::math::Time m_MinTimestep;
			//@}
			/// @name model time
			//@{
			/// Actual time of the solver
			cmf::math::Time m_Time;
			/// last time step of the solver
			cmf::math::Time m_TimeStep;
			/// Proposed step width for the next time step
			cmf::math::Time m_NextTimeStep;
			/// Protected function to adjust the step width for stability reasons
			void AdjustTimestep(cmf::math::Time& TimeStep,cmf::math::Time MaxTime)
			{
				//We should not step over the maximum time
				if ( MaxTime - m_Time <= TimeStep) 
					TimeStep = MaxTime-m_Time;
				//If the max time is reached at the next time step, we should lower the timestep, to avoid a too short timestep on the next call
				else if	((MaxTime - m_Time) < TimeStep*2.0) 
					TimeStep = (MaxTime - m_Time) * 0.5;
			}
			//number of iterations
			int m_Iterations;
		public:
			///Returns the current model time
			cmf::math::Time ModelTime() const { return m_Time; }
			///Sets the current model time
			void ModelTime(cmf::math::Time val) { m_Time = val;Reset(); }
			///Returns the last time step
			cmf::math::Time TimeStep() const { return m_TimeStep; }
			///Returns the next time step width
			cmf::math::Time NextTimeStep() const { return m_NextTimeStep; }
			///Sets the next time step width
			void NextTimeStep(cmf::math::Time val) { m_NextTimeStep = val; }
			///Returns The minimal allowed time step length
			const cmf::math::Time MinTimestep() const { return m_MinTimestep; }
			int Iterations() const { return m_Iterations;}
			void ResetIterations() {m_Iterations=0;}
			virtual void Reset() {}
			//@}
			/// @name Constructors and Destructors
			//@{
			/// Constructs a new FixPointImplicitEuler from a pointer to a vector of state variables
			/// @note The RKF Integrator becomes the owner of states
			/// @param states Statevariables of the system
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			Integrator(const StateVariableVector& states, real epsilon=1e-9,cmf::math::Time tStepMin=10.0/(3600.0*24.0)) 
				: m_States(states), Epsilon(epsilon),m_MinTimestep(tStepMin),m_TimeStep(1.0),m_Time(0.0),m_NextTimeStep(1.0),error_position(0),UseEulerAtTmin(0),use_OpenMP(1)			{			}
			
			/// Constructs a new Integrator with a new own state vector
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			Integrator(real epsilon=1e-9,cmf::math::Time tStepMin=Time::Seconds(10)) 
				: m_States(), Epsilon(epsilon),m_MinTimestep(tStepMin),m_TimeStep(1.0),m_Time(0.0),m_NextTimeStep(1.0),UseEulerAtTmin(0)
			{}
			Integrator(cmf::math::StateVariableOwner& states,real epsilon=1e-9,cmf::math::Time tStepMin=cmf::math::sec * 10.0)
				: m_States(),Epsilon(epsilon),m_MinTimestep(tStepMin),m_TimeStep(1.0),m_Time(0.0),m_NextTimeStep(1.0),error_position(0),UseEulerAtTmin(0) ,use_OpenMP(1)
			{
				states.AddStateVariables(m_States);
			}
			/// Copy constructor, does not copy the state variables
			Integrator(const Integrator& forCopy)
				: Epsilon(forCopy.Epsilon), m_MinTimestep(forCopy.m_MinTimestep),m_NextTimeStep(forCopy.m_NextTimeStep),m_Time(forCopy.m_Time),m_TimeStep(forCopy.m_TimeStep),
				Tag(forCopy.Tag),m_States(),UseEulerAtTmin(forCopy.UseEulerAtTmin),use_OpenMP(forCopy.use_OpenMP)
			{}
			
			/// Returns a new Integrator, based on this (without the state variables), e.g. same type, epsilon, model time etc.
			virtual cmf::math::Integrator * Copy() const=0;
			//@}

			///@name Integrate
			//@{

			///Integrates the vector of state variables
			/// @param MaxTime To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed timestep, and changes it into the effictivly used timestep according to the local stiffness of the problem and MaxTime
			virtual int Integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)=0;
			int Integrate( cmf::math::Time MaxTime)
			{		return Integrate(MaxTime,m_NextTimeStep);	}
			///
			void IntegrateUntil(cmf::math::Time MaxTime)
			{ IntegrateUntil(MaxTime,m_NextTimeStep);	}
			///Integrates the vector of state variables until MaxTime
			void IntegrateUntil(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
			{
				m_Iterations=0;
				m_NextTimeStep=TimeStep;
				while (m_Time < MaxTime)
					Integrate(MaxTime);
			}

			//@}


		};
	}
}
#ifdef SWIG
#endif

#endif // Integrator_h__
