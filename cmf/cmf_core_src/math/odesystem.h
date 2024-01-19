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
        public:
            state_list states;

#ifndef SWIG
            state_list::iterator begin() {return states.begin();}
            state_list::iterator end() {return states.end();}
            state_list::const_iterator begin() const {return states.begin();}
            state_list::const_iterator end() const {return states.end();}
#endif
            int use_OpenMP = -1;

            operator bool() const {return states.size()>0;}

            ODEsystem(const cmf::math::ODEsystem& for_copy);

            explicit ODEsystem() = default;
            explicit ODEsystem(const cmf::math::state_list& for_copy, int use_OpenMP=-1);
            size_t size() const {return states.size();}

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

            explicit operator cmf::math::state_list&() {
                return states;
            }

#endif
            cmf::math::num_array get_dxdt(Time time) const;
            cmf::math::num_array get_state_values() const;
            real get_state_value(ptrdiff_t index) const;
            void set_state_value(ptrdiff_t index, real value) const;



        };

    }
}



#endif //CMF_ODESYSTEM_H
