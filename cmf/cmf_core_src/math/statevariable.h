

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
#ifndef StateVariable_h__
#define StateVariable_h__
#include "time.h"
#include <string>
#include <deque>
#include <tr1/memory>
#include <cmath>
#include "num_array.h"
#include "real.h"
namespace cmf {
	/// Contains classes for numerical solving of ODE's
  namespace math {
	  /// integration_variable is a functionality for different classes for integrating values over time.
	  ///
	  /// Main usage of an integration_variable is the calculation of average fluxes
	  class integratable {
	  public:
		  virtual void integrate(Time t)=0;
		  virtual void reset(Time t)=0;
	  };

		/// Abstract class state variable
		///		
		///		Simple exponential system class header implementing a state variable:
		///		@code
		///		class RateGrowth
		///		{
		///		public:
		///			real rate;
		///			virtual real Derivate(const cmf::math::Time& time) {return rate*get_state();}
		///		};
		///		@endcode
		///
	  class StateVariable {
		private:
			bool m_StateIsNew;
			/// Holds the value of the Statevariable
			real m_State;
		protected:
			virtual void StateChangeAction() {}
			/// Sets the updated flag (m_StateIsNew) to false
			void MarkStateChangeHandled() {m_StateIsNew=false;}
		public:
			typedef std::tr1::shared_ptr<StateVariable> ptr;
			/// Returns the derivate of the state variable at time @c time
			virtual real Derivate(const cmf::math::Time& time)=0;
			/// Returns the current state of the variable
			real get_state() const {return m_State;}
			/// Gives access to the state variable
			void set_state(real newState) {
				//m_StateIsNew=m_State!=newState;
				m_State=newState;
				m_StateIsNew=true;
				if (m_StateIsNew) StateChangeAction();
			}
			virtual real get_abs_errtol(real rel_errtol) const{
				return rel_errtol;
			}
			/// Returns if the state was currently updated
			bool StateIsChanged() {return m_StateIsNew;}
			/// ctor
			StateVariable(real InitialState=0) : m_State(InitialState),m_StateIsNew(true) {}
		};

		class state_queue;
		/// An abstract class, that owns one or more state variables, that can add them to a vector of state variables in a certain order
		class StateVariableOwner
		{
		public:
			/// Add the state variables, owned by an object derived from StateVariableOwner, to the given vector
			virtual state_queue get_states()=0;
		};

		class state_queue : public StateVariableOwner {
			typedef std::deque<StateVariable::ptr> state_deque;
			state_deque	m_queue;
		public:

#ifndef SWIG
			state_deque::iterator begin() {return m_queue.begin();}
			state_deque::iterator end() {return m_queue.end();}
			state_deque::const_iterator begin() const {return m_queue.begin();}
			state_deque::const_iterator end() const {return m_queue.end();}
#endif

			void push(StateVariable::ptr sv) {
				m_queue.push_back(sv);
			}
			void push(StateVariableOwner& svo) {
				(*this) += svo.get_states();
			}
			StateVariable::ptr pop() {
				StateVariable::ptr res = m_queue.front();
				m_queue.pop_front();
				return res;
			}
			StateVariable::ptr front() const {
				return m_queue.front();
			}
			state_queue get_states() {
				return *this;
			}
			void eat(state_queue& food) {
				while (food) 
					push(food.pop());
			}
			operator bool() const {return m_queue.size()>0;}
			state_queue& operator +=(const state_queue& food) {
				m_queue.insert(m_queue.end(),food.m_queue.begin(),food.m_queue.end());
				return *this;
			}
			state_queue& operator +=(StateVariableOwner& svo) {
				return (*this)+=svo.get_states();
			}
			size_t size() const {return m_queue.size();}
		};

	}
}

#endif // StateVariable_h__
