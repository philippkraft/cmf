

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
#include "../statevariable.h"
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
		class Integrator : public StateVariableOwner
		{
		protected:
			///@name The state variables to integrate
			//@{
			typedef std::vector<StateVariable::ptr> state_vector;
			typedef std::vector<integratable::ptr> integratable_vector;
			state_vector m_States;
			integratable_vector m_integratables;

			int error_position;
			real error_exceedance( const num_array& compare,int * biggest_error_position=0 );
			/// Copies the states to a numeric vector using use_OpenMP
#ifndef SWIG
		public:
			void CopyStates(num_array & destination) const;
			void CopyStates(real * destination) const;
			/// Copies the new states to the actual states
			void SetStates(const num_array & newStates) ;
			void SetStates(real * newStates);
			/// Copies the derivatives at time step "time" to a numeric vector using use_OpenMP
			/// @param time Time at which the derivatives should be calculated
			/// @param destination Vector to be overwritten by the results
			/// @param factor A factor that is multiplied to the derivate (e.g. unit conversion or integration length)
			void CopyDerivs(Time time,num_array & destination, real factor=1) const;
			/// Copies the derivatives at time step "time" to an preallocated c array
			/// @param time Time at which the derivatives should be calculated
			/// @param destination Allocated c array
			/// @param factor A factor that is multiplied to the derivate (e.g. unit conversion or integration length)
			void CopyDerivs(Time time,real * destination,real factor=1) const;
			/// Returns the states in a numeric vector using :CopyStates, but is slower because of additional memory allocation
			num_array GetStates() const 
			{
				num_array result(this->size());
				CopyStates(result);
				return result;
			}
			/// Returns the derivatives at time step "time" in a numeric vector using :CopyDerivs, but is slower because of additional memory allocation
			num_array GetDerivs(Time time) const 
			{
				num_array result(this->size());
				CopyDerivs(time,result);
				return result;
			}
			void AddValuesToStates(const num_array& operands);
#endif
			/// Add state variables from a StateVariableOwner
			virtual void AddStatesFromOwner(cmf::math::StateVariableOwner& stateOwner) {
				state_queue sq = stateOwner.get_states();
				m_States.insert(m_States.end(),sq.begin(),sq.end());
			}
			virtual void AddState(cmf::math::StateVariable::ptr state) {
				m_States.push_back(state);
			}

			void AddIntegratable(cmf::math::integratable::ptr integratable) {
				m_integratables.push_back(integratable);
			}



			//@}
		public:
			/// if true (default), OpenMP is used calculate the right hand side function f(y,t) in parallel
			bool use_OpenMP;

			/// returns the number of state variables
			int size() const
			{
				return (int)m_States.size();
			}
			/// Returns the statevariable at position
#ifndef SWIG
			StateVariable::ptr operator[](int position) {
				return m_States[position];
			}

#endif
			/// Simplifies the assessment of state variables
			real get_state(int position) const
			{
				return m_States[position]->get_state();
			}
			/// Simplifies the assessment of state variables
			void set_state(int position,real newState)
			{
				m_States[position]->set_state(newState);
			}
			/// gets the state variables of the integrator
			state_queue get_states() {
				state_queue q;
				for(state_vector::const_iterator it = m_States.begin(); it != m_States.end(); ++it)
				{
				    q.push(*it);
				}
				return q;
			}
			

		protected:
		///@name Accuracy parameters
			//@{
			///Tolerable error
			const real Epsilon;
			//@}
			/// @name model time
			//@{
			/// Actual time of the solver
			cmf::math::Time m_t;
			/// last time step of the solver
			cmf::math::Time m_dt;
			/// Protected function to adjust the step width for stability reasons
			void AdjustTimestep(cmf::math::Time& TimeStep,cmf::math::Time MaxTime)
			{
				//We should not step over the maximum time
				if ( MaxTime - m_t <= TimeStep) 
					TimeStep = MaxTime-m_t;
				//If the max time is reached at the next time step, we should lower the timestep, to avoid a too short timestep on the next call
				else if	((MaxTime - m_t) < TimeStep*2.0) 
					TimeStep = (MaxTime - m_t) * 0.5;
			}
			//number of iterations
			int m_Iterations;
		public:
			///Returns the current model time
			cmf::math::Time get_t() const { return m_t; }
			///Sets the current model time
			void set_t(cmf::math::Time val) { m_t = val;Reset(); }
			///Returns the last time step
			cmf::math::Time get_dt() const { return m_dt; }
			int Iterations() const { return m_Iterations;}
			void ResetIterations() {m_Iterations=0;}
			virtual void Reset() {}
			//@}
			/// @name Constructors and Destructors
			//@{

			/// Constructs a new Integrator with a new own state vector
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			Integrator(real epsilon=1e-9) 
				: m_States(), Epsilon(epsilon),m_dt(day),m_t(day)
			{}
			Integrator(cmf::math::StateVariableOwner& states,real epsilon=1e-9)
				: m_States(),Epsilon(epsilon),m_dt(day),m_t(day*0),error_position(0) ,use_OpenMP(1)
			{
				state_queue sq = states.get_states();
				m_States.insert(m_States.end(),sq.begin(),sq.end());
			}
			void operator+=(StateVariableOwner& states) {
				AddStatesFromOwner(states);
			}
			
			/// Returns a new Integrator, based on this (without the state variables), e.g. same type, epsilon, model time etc.
			virtual cmf::math::Integrator * Copy() const=0;
			//@}

			///@name Integrate
			//@{

			///Integrates the vector of state variables
			/// @param t_max To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param dt Takes the proposed timestep, and changes it into the effictivly used timestep according to the local stiffness of the problem and MaxTime
			virtual int integrate(cmf::math::Time t_max,cmf::math::Time dt)=0;
			///Integrates the vector of state variables until t_max
			/// @param t_max Time, the solver should run to
			/// @param dt Time step (may be omitted)
			/// @param reset If true, solver is resetted before integration starts
			void integrate_until(cmf::math::Time t_max,cmf::math::Time dt=Time(),bool reset=false);
			//@}
		};
	}
}
#ifdef SWIG
#endif

#endif // Integrator_h__
