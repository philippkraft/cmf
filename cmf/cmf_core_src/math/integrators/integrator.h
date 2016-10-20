

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
		/// - copy
		/// Please provide a custom copy constructor
		class Integrator : public StateVariableOwner
		{
		protected:
			typedef std::vector<StateVariable::ptr> state_vector;
			state_vector m_States;

			/// Copies the states to a numeric vector using use_OpenMP
		public:
#ifndef SWIG
			void copy_states(num_array & destination) const;
			void copy_states(real * destination) const;
			/// Copies the new states to the actual states
			void set_states(const num_array & newStates) ;
			void set_states(real * newStates);
			/// Copies the derivatives at time step "time" to a numeric vector using use_OpenMP
			/// @param time Time at which the derivatives should be calculated
			/// @param destination Vector to be overwritten by the results
			/// @param factor A factor that is multiplied to the derivate (e.g. unit conversion or integration length)
			void copy_dxdt(Time time,num_array & destination, real factor=1) const;
			/// Copies the derivatives at time step "time" to an preallocated c array
			/// @param time Time at which the derivatives should be calculated
			/// @param destination Allocated c array
			/// @param factor A factor that is multiplied to the derivate (e.g. unit conversion or integration length)
			void copy_dxdt(Time time,real * destination,real factor=1) const;
			
			/// Adds the values in operands to the current states
			void add_values_to_states(const num_array& operands);
			StateVariable::ptr operator[](int position) {
				return m_States[position];
			}

#endif
			cmf::math::num_array get_dxdt(Time time) const 
			{
				num_array result(this->size());
				copy_dxdt(time,result);
				return result;
			}
			cmf::math::num_array get_states() const 
			{
				num_array result(this->size());
				copy_states(result);
				return result;
			}

			/// Add state variables from a StateVariableOwner
			virtual void add_states(cmf::math::StateVariableOwner& stateOwner) {
				StateVariableList sq = stateOwner.get_states();
				m_States.insert(m_States.end(),sq.begin(),sq.end());
			}
			/// Adds a single state variable to the integrator
			virtual void add_single_state(cmf::math::StateVariable::ptr state) {
				m_States.push_back(state);
			}
			
			/// Public access to integratables
			integratable_list integratables;
			/// If true, the integratables of this solver are reset
			bool reset_integratables;




			//@}
			/// if true (default), OpenMP is used calculate the right hand side function f(y,t) in parallel
			bool use_OpenMP;

			/// returns the number of state variables
			size_t size() const
			{
				return m_States.size();
			}
			/// Returns the statevariable at position
			/// Simplifies the assessment of state variables
			real get_state(ptrdiff_t position) const
			{
				return m_States[position]->get_state();
			}
			/// Simplifies the assessment of state variables
			void set_state(ptrdiff_t position,real newState)
			{
				m_States[position]->set_state(newState);
			}
			/// gets the state variables of the integrator
			StateVariableList get_states() {
				StateVariableList q;
				for(state_vector::const_iterator it = m_States.begin(); it != m_States.end(); ++it)
				{
				    q.append(*it);
				}
				return q;
			}
			virtual ~Integrator() {}
			

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
			void set_t(cmf::math::Time val) { m_t = val;reset(); }
			///Returns the last time step
			cmf::math::Time get_dt() const { return m_dt; }
			/// Resets any saved history (for multistep methods)
			virtual void reset() {}
			//@}
			/// @name Constructors and Destructors
			//@{

			/// Constructs a new Integrator with a new own state vector
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			Integrator(real epsilon=1e-9) 
				: m_States(), Epsilon(epsilon),m_dt(day),m_t(day),
				  use_OpenMP(true), reset_integratables(true)
			{}
			Integrator(cmf::math::StateVariableOwner& states,real epsilon=1e-9)
				: m_States(),Epsilon(epsilon),m_dt(day),m_t(day*0),
				  use_OpenMP(true), reset_integratables(true)
			{
				this->add_states(states);
			}
			Integrator(const cmf::math::Integrator& other) 
				: m_States(), Epsilon(other.Epsilon), m_dt(other.m_dt), m_t(other.m_t), 
				  reset_integratables(other.reset_integratables), use_OpenMP(other.use_OpenMP)
			{}
			/// Polymorphic copy constructor
			virtual Integrator * copy() const=0;
			//@}

			///@name Integrate
			//@{

			///Integrates the vector of state variables
			/// @param t_max To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param dt Takes the proposed time step, and changes it into the effectively used time step according to the local stiffness of the problem and MaxTime
			virtual int integrate(cmf::math::Time t_max,cmf::math::Time dt)=0;
			///Integrates the vector of state variables until t_max
			/// @param t_max Time, the solver should run to
			/// @param dt Time step (may be omitted)
			/// @param reset If true, solver is reseted before integration starts
			void integrate_until(cmf::math::Time t_max,cmf::math::Time dt=Time(),bool reset=false);
			//@}
		};
	}
}
#ifdef SWIG
#endif

#endif // Integrator_h__
