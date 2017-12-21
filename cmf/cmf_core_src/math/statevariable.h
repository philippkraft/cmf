

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
#include <vector>
#include "../cmfmemory.h"
#include <cmath>
#include "num_array.h"
#include "real.h"
namespace cmf {
	/// Contains classes for numerical solving of ODE's
  namespace math {
#ifndef SWIG
	  class precalculatable {
	  public:
		  virtual void do_action(Time t, bool use_OpenMP = true)=0;
	  };

#endif


	  /// integratable is a functionality for different classes for integrating values over time.
	  ///
	  /// Main usage of an integratable is the calculation of average fluxes over time e.g.
	  /// \f[ \int_{t_0}^{t_{end}}q\left(t,V_i,V_j\right)dt \f]
	  /// 
	  class integratable {
	  public:
		  typedef std::shared_ptr<integratable> ptr;
		  /// Integrates the variable until time t
		  virtual void integrate(Time t)=0;
		  /// Sets the start time of the integral
		  virtual void reset(Time t)=0;
		  /// Get the integral from the last reset until the last call of integrate. 
		  virtual double sum() const =0;
		  /// Returns average of the integrated variable (eg. flux) from the last reset until the last call of integrate
		  virtual double avg() const =0;
	  };
	  /// A list of cmf::math::integratable objects
	  ///
	  /// @todo TODO: Complete collection interface (getitem with slicing etc.)
	  class integratable_list {
	  private:
		  typedef std::vector<integratable::ptr> integ_vector;
		  integ_vector m_items;
	  public:
		  /// Adds an integratable to the list
		  void append(cmf::math::integratable::ptr add);
		  /// Removes an integratable from the list
		  void remove(cmf::math::integratable::ptr rm);

		  integratable::ptr operator[](int index) const {
			  return m_items.at(index < 0 ? index+size() : index);
		  }

		  /// Number of integratables in the list
		  size_t size() const {return m_items.size();}
		  cmf::math::num_array avg() const;
		  cmf::math::num_array sum() const;
		  void reset(Time t);
		  void integrate(Time t);
		  integratable_list() {}
		  integratable_list(const integratable_list& for_copy)
		  : m_items(for_copy.m_items)
		  {	  }

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
			/// Returns if the state was currently updated
			bool StateIsChanged() {return m_StateIsNew;}
			// A scale to handle abstol issues with the CVode Solver and with the is_empty function
			// By default, the scale is 1. Absolute tolerance is scale times rel tolerance
			real m_Scale;

		public:
			typedef std::shared_ptr<StateVariable> ptr;
			/// Returns the derivate of the state variable at time @c time
			virtual real dxdt(const cmf::math::Time& time)=0;
			/// Returns the current state of the variable
			real get_state() const {return m_State;}
			/// Gives access to the state variable
			void set_state(real newState);

			virtual real get_abs_errtol(real rel_errtol) const{
				return rel_errtol * m_Scale;
			}
			virtual std::string to_string() const=0;
			/// ctor
			StateVariable(real InitialState=0, real scale=1) 
				: m_State(InitialState),m_StateIsNew(true), m_Scale(scale)
			{}

			virtual ~StateVariable() {}
		};

		class StateVariableList;
		/// An abstract class, that owns one or more state variables, that can add them to a vector of state variables in a certain order
		class StateVariableOwner
		{
		public:
			/// Add the state variables, owned by an object derived from StateVariableOwner, to the given vector
			virtual StateVariableList get_states()=0;
			virtual ~StateVariableOwner() {}
		};

		class StateVariableList : public StateVariableOwner {
			typedef std::vector<StateVariable::ptr> state_vector;
			state_vector m_vector;
		public:

#ifndef SWIG
			state_vector::iterator begin() {return m_vector.begin();}
			state_vector::iterator end() {return m_vector.end();}
			state_vector::const_iterator begin() const {return m_vector.begin();}
			state_vector::const_iterator end() const {return m_vector.end();}
#endif

			void append(StateVariable::ptr sv) {
				m_vector.push_back(sv);
			}
			void extend(StateVariableOwner& svo) {
				(*this) += svo.get_states();
			}
			StateVariableList get_states() {
				return *this;
			}
#ifndef SWIG
			operator bool() const {return m_vector.size()>0;}
#endif
			StateVariableList& operator +=(const StateVariableList& food) {
				m_vector.insert(m_vector.end(),food.m_vector.begin(),food.m_vector.end());
				return *this;
			}
			StateVariableList& operator +=(StateVariableOwner& svo) {
				return (*this)+=svo.get_states();
			}
			size_t size() const {return m_vector.size();}
			virtual ~StateVariableList() {}
		};

	}
}

#endif // StateVariable_h__
