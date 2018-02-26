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

#ifndef cvode3_h__
#define cvode3_h__

#include <memory>

#include "integrator.h"
#include "../time.h"

namespace cmf {
	namespace math {

		struct CVodeOptions
		{
		public:
			int max_order;
			int max_non_linear_iterations;
			int max_error_test_failures;
			int max_convergence_failures;
			int max_num_steps;
			int max_hnil_warnings;
			
			CVodeOptions();
		};

		struct CVodeInfo
		{
		public:
			long int workspace_real;
			long int workspace_int;
			long int workspace_byte;
			long int steps;
			long int rhs_evaluations;
			long int linear_solver_setups;
			long int error_test_fails;

			std::string to_string() const;

		};

		class CVode3 : public Integrator {
		protected:
			class Impl;
			
			std::unique_ptr<Impl> _implementation;
			std::string _error_msg;
			virtual void set_solver()=0;
		public:
			/// @brief Changes the limits for the CVode solver
			///
			/// Negative values do not change the current value
			/// Uses the CVode functions CVodeSetMax...
			CVodeOptions options;
			virtual int integrate(cmf::math::Time t_max, cmf::math::Time dt);
			virtual void reset();
			CVode3(cmf::math::StateVariableOwner& states, real epsilon = 1e-9);
			void set_error_msg(std::string error);
			CVode3 * copy() const;
			std::string error_msg;
			CVodeInfo get_info() const;
			virtual std::string to_string() const=0;

			/// Error vector of the integrator
			cmf::math::num_array get_error() const;
		};

		class CVodeDense : public CVode3 {
		public:
			CVodeDense(cmf::math::StateVariableOwner& states, real epsilon = 1e-9);
			std::string to_string() const {
				return "CVodeDense()";
			}

			/// Returns a continuous 1D array representing the Jacobian oclumns concatenated
			///
			/// Convert to 2D numpy nd array: jac = solver.jacobian().reshape(solver.size(), -1)
			/// This can be implemented in ODEsystem.i but is not now.
			cmf::math::num_array jacobian() const;

		protected:
			void set_solver();
		};

		class CVodeBanded : public CVode3 {
		public:
			int bandwidth;
			CVodeBanded(cmf::math::StateVariableOwner& states, real epsilon = 1e-9, int bandwidth=5);
			std::string to_string() const;
		protected:
			void set_solver();
		};

		class CVodeDiag : public CVode3 {
		public:
			CVodeDiag(cmf::math::StateVariableOwner& states, real epsilon = 1e-9);
			std::string to_string() const {
				return "CVodeDiag()";
			}
		protected:
			void set_solver();
		};

		class CVodeKrylov : public CVode3 {
		public:
			int bandwidth;
			char preconditioner;
			CVodeKrylov(cmf::math::StateVariableOwner& states, real epsilon = 1e-9, 
				int bandwidth=5, char preconditioner='L');
			std::string to_string() const;

		protected:
			void set_solver();
		};
	}
}

#endif