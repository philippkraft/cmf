

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
#ifndef FixpointImplicitEuler_h__
#define FixpointImplicitEuler_h__




#include "integrator.h"
#include "../real.h"

namespace cmf {
	namespace math {
		/// An implicit (backward) Euler integrator using fixpoint iteration
		class ImplicitEuler : public Integrator
		{
			//@}
		private:
			///@name Internal data storages
			//@{
			/// Array to store the new states
			num_array compareStates;
			///Vector of the state variable in the beginning of the integration
			num_array oldStates;
			///Vector of derivatives
			num_array dxdt;
			/// Absolute error tolerance
			num_array abstol;

			//@}

			real error_exceedance( const num_array& compare,ptrdiff_t * biggest_error_position=0 );
			void Gear1newState(real h);
			// Refreshes the absolute tolerance vector
			void set_abstol();
		public:
			/// The shortest allowed timestep
			Time dt_min;
			/// Add state variables from a StateVariableOwner
			void add_states(cmf::math::StateVariableOwner& stateOwner);
			/// Constructs a new FixPointImplicitEuler from a pointer to a vector of state variables
			/// @note The Integrator becomes the owner of states
			/// @param states Statevariables of the system
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			ImplicitEuler(StateVariableOwner& states, real epsilon=1e-9,cmf::math::Time tStepMin=cmf::math::timespan(10));
			/// Constructs a new FixPointImplicitEuler
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			ImplicitEuler(real epsilon=1e-9,cmf::math::Time tStepMin=cmf::math::timespan(10));

			/// copy constructor
			ImplicitEuler(const Integrator&);

			virtual Integrator * copy() const
			{
				return new ImplicitEuler(*this);
			}
			///Integrates the vector of state variables
			/// @param MaxTime To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed timestep, and changes it into the effictivly used timestep according to the local stiffness of the problem and MaxTime
			int integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep);

		};
	}
}

#endif // FixpointImplicitEuler_h__
