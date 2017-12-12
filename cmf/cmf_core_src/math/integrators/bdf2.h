

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
#ifndef BDF2_h__
#define BDF2_h__

#include "integrator.h"
#include "../real.h"
namespace cmf {
	namespace math {

		/// An order 2 BDF-Method with fixed-point iteration and variable step size
		///
		///
		/// Derived from 
		///  - Roussel C. and Roussel M. (2003) "Generic Object-Oriented Differential Equation Integrators", 
		/// C/C++ User Journal, Nov. 2003, http://www.ddj.com/cpp/184401724?pgno=8 and
		///  - Eckert S., Baaser H., Gross D. and Scherf O. (2004) "A BDF2 integration method with step size control for elasto-plasticity",
		/// Computational Mechanics 34, 377 - 386, DOI: 10.1007/s00466-004-0581-1
		///
		/// Most important function: Integrate

		class BDF2 : public Integrator
		{
		private:
			///@name Internal data storages
			//@{
			/// Array to store the states for test of convergence
			num_array compareStates;
			/// Array to store the derivatives
			num_array dxdt;

			///History of the last states
			num_array pastStatesArray[2];
			///Number of steps taken
			int stepNo;
			/// Current order of Gear formula
			int order;
			Time dt_min;
			/// Gets one of the last states
			/// @returns   the n<sup>th</sup> state in history
			/// @param toBack Number of steps back (ranging from 1..4, no runtime check)
			num_array& pastStates(int toBack=1)
			{
				int pos = (stepNo-toBack) % 2;
				return pastStatesArray[pos];
			}
			/// Absolute error tolerance
			num_array abstol;

			// Refreshes the absolute tolerance vector
			void set_abstol();

			//@}


			/// @name Gear functions: The following functions evaluate the Gear formulas of specified order, loading the results into compareStates.
			//@{
			/// Gear formulas through an array of pointers.
			typedef void (BDF2::*newState_functions)(real);
			newState_functions calc_newState[2];

			void Gear1newState(real h);
			void Gear2newState(real h);

			//@}
			real error_exceedance( const num_array& compare,int * biggest_error_position=0);
			int error_position;
		public:

			/// Add state variables from a StateVariableOwner
			void add_states(cmf::math::StateVariableOwner& stateOwner);
			/// Returns the position of the biggest error
			int get_error_position() const
			{
				return error_position;
			}
			/// Maximum order, can be 1 (implicit euler) or 2 (BDF2)
			int max_order;
			virtual void reset() {
				order=1;
				stepNo = 0;
			}

			/// Constructs a new BDF2 integrator
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			BDF2(real epsilon=1e-9,cmf::math::Time tStepMin=cmf::math::timespan(10));
			/// Constructs a new Gears_var_Step
			/// @param states States to be added to the solver
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			BDF2(cmf::math::StateVariableOwner& states, real epsilon=1e-9,cmf::math::Time tStepMin=cmf::math::timespan(10));
			
			/// Constructs a new BDF2 integrator
			/// @param templ Template to be used to construct a BDF2 method
			BDF2(const Integrator & templ);
			virtual Integrator * copy() const
			{
				BDF2* newBDF2 = new BDF2(*this);
				newBDF2->max_order = max_order;
				return newBDF2;
			}



			/// Integrates the vector of state variables
			///
			///  - if \f$ t_{max}-t_{now}\leq 2h_n \f$ then \f$ h_{n+1}= t_{max}-t_{now} \f$ else \f$ h_{n+1}= 2h_n \f$
			///  - Uses pastStatesArray to store \f$ y_n,y_{n-1} \f$
			///  - if more than one step is taken start iterating:
			///    - \f$ y_{n+1}^i=\frac{1}{1+2\rho }\left( (\rho +1)^{2}y_{n}-\rho ^{2}y_{n-1}+(\rho +1)h\frac{dy}{dt}(y_{n+1}^{i-1},t_{n+1} \right), y_{n+1}^0=y_n \f$ 
			///   with \f$ \rho = \frac{h_{n+1}}{h_n} \f$
			///  - until \f$ y_{n+1}^{i} - y_{n+1}^{i-1} \leq \ \|\epsilon + \epsilon y_{n+1}\|_{\infty} \f$
			///  - If too many iterations are needed, or the error is rising, repeat iteration with \f$ h_{n+1} = \frac{h_{n+1}}{2} \f$
			/// @param MaxTime To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed timestep, ignored by this solver
			int integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep);
		};
	}
}
#endif // BDF2_h__
