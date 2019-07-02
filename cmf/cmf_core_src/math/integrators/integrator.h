

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
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
#include "../odesystem.h"
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
		class Integrator
		{
#ifndef SWIG
		private:
			ODEsystem m_system;
		protected:
			const ODEsystem& get_system() const;
			ODEsystem& get_system();
#endif

		public:

			virtual ~Integrator() = default;

			void set_system(const state_list& states);

			size_t size() const;

			cmf::math::StateVariable::ptr operator[](ptrdiff_t index) const;

            explicit operator cmf::math::state_list();

            /// Desrcibes the iterator
            virtual std::string to_string() const = 0;

		protected:
			///@name Accuracy parameters
			//@{
			///Tolerable error
			const real Epsilon=1e-9;
			//@}
			/// @name model time
			//@{
			/// Actual time of the solver
			cmf::math::Time m_t;
			/// last time step of the solver
			cmf::math::Time m_dt;
			//number of iterations
			int m_Iterations=0;
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
			explicit Integrator(real epsilon=1e-9);

			explicit Integrator(const state_list &states, real epsilon = 1e-9);
			Integrator(const cmf::math::Integrator& other);
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

#endif // Integrator_h__
