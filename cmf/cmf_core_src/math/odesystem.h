//
// Created by kraft-p on 01.04.2019.
//

#ifndef CMF_ODESYSTEM_H
#define CMF_ODESYSTEM_H

#include <vector>
#include "num_array.h"
#include "statevariable.h"

namespace cmf {
    namespace math {
        class ODEsystem {
        private:
            state_list m_states;
        public:

#ifndef SWIG
            state_list::iterator begin() {return m_states.begin();}
            state_list::iterator end() {return m_states.end();}
            state_list::const_iterator begin() const {return m_states.begin();}
            state_list::const_iterator end() const {return m_states.end();}
#endif
            int use_OpenMP;

            void append(StateVariable::ptr sv) {
                m_states.push_back(sv);
            }
            void extend(const cmf::math::state_list& svl) {
                (*this) += svl;
            }

            operator bool() const {return m_states.size()>0;}

            ODEsystem& operator +=(const ODEsystem& food) {
                m_states.insert(m_states.end(),food.m_states.begin(),food.m_states.end());
                return *this;
            }
            ODEsystem& operator +=(const cmf::math::state_list& food) {
                m_states.insert(m_states.end(),food.begin(),food.end());
                return *this;
            }

            explicit ODEsystem(int use_OpenMP=0);

            ODEsystem(const cmf::math::ODEsystem& for_copy);

            explicit ODEsystem(const cmf::math::state_list& for_copy, int use_OpenMP=0);
            size_t size() const {return m_states.size();}

            ~ODEsystem() = default;

            StateVariable::ptr operator[](ptrdiff_t index) const;

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

#endif
            cmf::math::num_array get_dxdt(Time time) const;
            cmf::math::num_array get_state_values() const;
            real get_state_value(ptrdiff_t index) const;



        };

    }
}



#endif //CMF_ODESYSTEM_H
