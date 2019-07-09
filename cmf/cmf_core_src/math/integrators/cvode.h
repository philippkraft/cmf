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

#ifndef cvode_h__
#define cvode_h__

#include <memory>

#include "integrator.h"
#include "../time.h"
#include "../sparse_struct.h"

namespace cmf {
	namespace math {
		/// @brief A set of options for all CVode3 solver
		///
		/// Negative numbers indicate that this option stays on the default value.
		/// For the meaning of the options see CVODE-UD, section 4.5.6
		///
		/// See Hindmarsh, A., Serban, R. and Reynolds, D.: 
		///     User Documentation for cvode v3.1.0, 2017, UCRL-SM-208108
		///
		/// Usage example:
		/// >>>solver = CVodeDens(p, 1e-9)
		/// >>>solver.options.max_order = 2
		struct CVodeOptions
		{
		public:
			/// @brief Maximum order of the solver (default 12 for CVodeAdams and 5 for any other solver)
			int max_order;
			/// @brief  maximum  number  of  nonlinear solver iterations permitted per step
			int max_non_linear_iterations;
			/// @brief maximum number of error test failures permitted in attempting one step (default=7)
			int max_error_test_failures;
			/// @brief  maximum number of nonlinear solver convergence failures permitted during one step
			int max_convergence_failures;
			/// @brief maximum number of steps to be taken by the solver in its 
			/// attempt to reach the next output time (default=500)
			int max_num_steps;
			/// maximum number of messages issued
			/// by the solver warning that t+h=t on the next internal step, default off (-1)
			int max_hnil_warnings;
			
			CVodeOptions();
		};

		/// @brief Reports the current state of a CVode solver
		struct CVodeInfo
		{
		public:
			/// @brief number of statevariable solved by the solver
			long int size;
			/// @brief Number of real values in the workspace of the solver
			long int workspace_real;
			/// @brief Number of int values in the workspace of the solver
			long int workspace_int;
			/// @brief Number of bytes values in the workspace of the solver
			long int workspace_byte;
			/// @brief Cumulative number of internal steps
			long int steps;
			/// @brief Order to be attempted on the next step
			long int current_order;
			/// @brief Number of calls to right hand side function
			long int rhs_evaluations;
			/// @brief Number of calls to linear solver setup function
			long int linear_solver_setups;
			/// @brief Number of local error test failures that have occurred
			long int error_test_fails;
			/// @brief Number of order reductions due to stability limit detection
			long int order_reductions;
			/// @brief Number of nonlinear solver iterations
			long int nonlinear_solver_iterations;
			/// @brief Number of nonlinear convergence failures
			long int nonlinear_solver_convergence_failures;
			/// @brief Number of calls to any dxdt method of a state
			long int dxdt_method_calls;


			/// @brief contains the Sundials version of CVode used
			std::string sundials_version;

			std::string to_string() const;

		};
		/// @brief Abstract base class for different modes of the CVode solver
		///
		/// Initantiate one of the child classes to gain different modes of the CVode solver
		class CVodeBase : public Integrator {
		protected:

			class Impl;
			std::unique_ptr<Impl> _implementation;

			std::string _error_msg;
			virtual void set_solver()=0;
			CVodeBase(const cmf::math::state_list & states, real epsilon = 1e-9);
			explicit CVodeBase(real epsilon=1e-9);
			CVodeBase(const CVodeBase& other);

		public:
			/// @brief the limits for the CVode solver, see CVodeOptions
			CVodeOptions options;

			/// @brief Initialize the internal memory. 
			///
			/// Automatically called, when one starts to integrate
			int initialize();

			virtual int integrate(cmf::math::Time t_max, cmf::math::Time dt);
			/// Resets the history of the multispte solver and overwrites the internal state cache
			virtual void reset();
			/// Sets an error message
			void set_error_msg(std::string error);
			/// Returns a copy of the solver
			CVodeBase * copy() const override=0;

			std::string error_msg;

			/// Returns the current solver statistics
			CVodeInfo get_info() const;

			/// Returns a string representation of the solver
			std::string to_string() const override =0;

			/// Error vector of the integrator
			cmf::math::num_array get_error() const;
			
			/// Returns a continuous 1D array representing the Jacobian columns concatenated
			///
			/// In Python, get_jacobian returns the Jacobian as a 2D array
			virtual cmf::math::num_array _get_jacobian() const;
			
			~CVodeBase() override;
		};
		/// @brief implicit BDF CVode solver with full Jacobian approximation
		///
		/// Use this solver for small but stiff systems (<20 state variables)
		///
		/// The solver calculates for each step the full Jacobian matrix of the system
		/// using a difference quotient approximation of the real Jacobian
		class CVodeDense : public CVodeBase {
		public:
			/// @brief Creates a new implicit dense CVode solver 
			explicit CVodeDense(const cmf::math::state_list & states, real epsilon = 1e-9);
			explicit CVodeDense(real epsilon = 1e-9);
			std::string to_string() const override {
				return "CVodeDense()";
			}
			cmf::math::num_array _get_jacobian() const override;
			cmf::math::CVodeDense* copy() const override;

		protected:
			void set_solver();
		};

		/// @brief Explizit multistep solver using CVode
		class CVodeAdams : public CVodeBase {
		public:
		    explicit CVodeAdams(real epsilon = 1e-9);
			explicit CVodeAdams(const cmf::math::state_list & states, real epsilon = 1e-9);
			std::string to_string() const override;
            cmf::math::CVodeAdams* copy() const override;

		protected:
			void set_solver() override;
		};

		/// @brief implicit BDF CVode solver with a banded Jacobian approximation
		class CVodeBanded : public CVodeBase {
		public:
			/// @brief Width of the band to both sides of the diagonal
			int bandwidth;
            explicit CVodeBanded(real epsilon = 1e-9, int w = 5);
            explicit CVodeBanded(const cmf::math::state_list & states, real epsilon = 1e-9, int w = 5);
			std::string to_string() const override;
            cmf::math::CVodeBanded * copy() const override;

        protected:
			void set_solver() override;
		};

		/// @brief implicit BDF CVode solver with a one line diagonal Jacobian approximation
		class CVodeDiag : public CVodeBase {
		public:
            explicit CVodeDiag(const cmf::math::state_list & states, real epsilon = 1e-9);
            explicit CVodeDiag(real epsilon = 1e-9);
			std::string to_string() const {
				return "CVodeDiag()";
			}
            cmf::math::CVodeDiag*  copy() const;

        protected:
			void set_solver();
		};

		/// @brief implicit BDF CVode solver with a Krylov preconditioner
		class CVodeKrylov : public CVodeBase {
		public:
			/// @brief Band width of the preconditioner (both sides of the diagonal)
			int bandwidth;
			/// @brief Type of the preconditioner 'L'->left, 'R'->right, 'B'->both, 'N'->None, default 'L'
			char preconditioner;
            explicit CVodeKrylov(const cmf::math::state_list & states, real epsilon = 1e-9,
                        int w = 5, char p = 'L');
            explicit CVodeKrylov(real epsilon = 1e-9,
                        int w = 5, char p = 'L');
			std::string to_string() const;
            cmf::math::CVodeKrylov* copy() const;


        protected:
			void set_solver();
		};

		class CVodeKLU : public CVodeBase {
		public:
            explicit CVodeKLU(real epsilon = 1e-9);
			explicit CVodeKLU(const cmf::math::state_list & states, real epsilon = 1e-9);
			std::string to_string() const;
			virtual cmf::math::num_array _get_jacobian() const;
            cmf::math::CVodeKLU* copy() const override;

        protected:
			void set_solver();
		private:
			friend class CVodeBase::Impl;
			sparse_structure sps;

		};
	}
}

#endif