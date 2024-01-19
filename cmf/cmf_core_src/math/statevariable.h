

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//    cmf is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    cmf is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with cmf.  If not, see <http://www.gnu.org/licenses/>.
//    
#ifndef StateVariable_h__
#define StateVariable_h__
#include "time.h"
#include <string>
#include <deque>
#include <vector>
#include <memory>
#include <cmath>
#include "num_array.h"
#include "real.h"
namespace cmf {
    /// Contains classes for numerical solving of ODE's
  namespace math {


        /// Abstract class state variable
        ///        
        ///        Simple exponential system class header implementing a state variable:
        ///        @code
        ///        class RateGrowth
        ///        {
        ///        public:
        ///            real rate;
        ///            virtual real Derivate(const cmf::math::Time& time) {return rate*get_state();}
        ///        };
        ///        @endcode
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
            virtual bool is_connected(const cmf::math::StateVariable& other) const {
                return true;
            }
#ifndef SWIG
            typedef std::vector<StateVariable*> list;
            virtual void add_connected_states(list& states);
#endif // !SWIG

        };

        typedef std::vector<cmf::math::StateVariable::ptr> state_vector;

        class state_list: private state_vector {
        public:
            state_list();
            state_list(const state_list& other);
            void append(StateVariable::ptr sv) {
                push_back(sv);
            }
            state_list& extend(const state_list& svl);
            size_t size() const;
            operator bool() const;

            state_list& operator +=(const state_list& food);

            StateVariable::ptr operator[](ptrdiff_t index) const {
                 if (index < 0) index += size();
                 return at(index);
            }

        #ifndef SWIG
            typedef state_vector::iterator iterator;
            typedef state_vector::const_iterator const_iterator;
            iterator begin();
            iterator end() {return state_vector::end();}
            const_iterator begin() const {return state_vector::begin();}
            const_iterator end() const {return state_vector::end();}
        #endif

        };
        state_list operator +(const state_list& left, const state_list& right);
    }
}

#endif // StateVariable_h__
