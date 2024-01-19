

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
#ifndef ExplicitEuler_fixed_h__
#define ExplicitEuler_fixed_h__

#include "integrator.h"
namespace cmf {
	namespace math {
		/// An explicit Euler integrator, with a fixed time step
		class ExplicitEuler_fixed : public Integrator
		{
		private:
			num_array 
				dxdt;
		public:

			/// Constructs a new ExplicitEuler_fixed from a pointer to a vector of state variables
			/// @note The Integrator becomes the owner of states
			/// @param states Statevariables of the system
			explicit ExplicitEuler_fixed(const cmf::math::state_list & states);

			/// Constructs a new ExplicitEuler_fixed
			ExplicitEuler_fixed();

			/// copy constructor
			ExplicitEuler_fixed(const Integrator& copy) : Integrator(copy) {}
			virtual ~ExplicitEuler_fixed() {}
			virtual Integrator * copy() const;
			std::string to_string() const override {
				return std::string("ExplicitEuler_fixed(size=") + std::to_string(size()) + std::string(")");
			}


			///Integrates the vector of state variables
			/// @param MaxTime (ignored) To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed time step
			int integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep);

		};

		/// A simple predictor - corrector solver. Not tested and very experimentally
		/// \f$ y^{n+1} = y^n + \alpha f(y^n + f(y^n)dt)dt + (1-\alpha)f(y^n)dt \f$
		class HeunIntegrator : public Integrator
		{
		private:
			num_array 
				dxdt0, dxdt1, old_states;
		public:
			/// Alpha Weight factor \f$\alpha\f$ to weight \f$f(y^n)\f$ and \f$f(y^{n+1})\f$
			real alpha;
			/// Constructs a new PredictCorrectSimple from a pointer to a vector of state variables
			/// @note The Integrator becomes the owner of states
			/// @param states Statevariable owner of the system
			/// @param Alpha Weight factor \f$\alpha\f$ to weight \f$f(y^n)\f$ and \f$f(y^{n+1})\f$
			explicit HeunIntegrator(const cmf::math::state_list & states, real Alpha = 0.5);

			/// Constructs a new Heun integrator
			/// @param Alpha Weight factor \f$\alpha\f$ to weight \f$f(y^n)\f$ and \f$f(y^{n+1})\f$
			HeunIntegrator(real Alpha=0.5);

			/// copy constructor
			HeunIntegrator(const Integrator& copy);
			virtual ~HeunIntegrator() {}
			virtual Integrator * copy() const;
			std::string to_string() const override {
				return std::string("HeunIntegrator(size=") + std::to_string(size()) + std::string(")");
			}

			///Integrates the vector of state variables
			/// @param MaxTime (ignored) To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed time step
			int integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep);
			void reset() override;

		};


	}
}

#endif // ExplicitEuler_fixed_h__
