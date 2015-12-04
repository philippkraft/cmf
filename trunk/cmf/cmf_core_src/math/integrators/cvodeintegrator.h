

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
#ifndef cvode_h__
#define cvode_h__
// include CVODE
#include <cvode/cvode.h>
#include <sundials/sundials_nvector.h>       /* serial N_Vector types, fct. and macros */
#include "integrator.h"
namespace cmf {
	namespace math {
		/// A wrapper class for the CVODE integrator from the SUNDIALS library
		///
		/// https://computation.llnl.gov/casc/sundials/main.html
		class CVodeIntegrator : public Integrator
		{
		private:
			N_Vector m_y;
			void * cvode_mem, * precond_mem;
			/// The right handside function f in eq. \f$\frac{dy}{dt}=f(t,y)\f$
			static int f(realtype t, N_Vector u, N_Vector udot, void *f_data);
			std::string error_msg;
		public:
			/// Type of Krylov preconditioning. Must have one of the following values:
			/// - 'N' No Krylov preconditioner
			/// - 'L' Left hand side banded preconditioning
			/// - 'R' Right hand side preconditioning
			char preconditioner;
			/// Specifies the maximum number of nonlinear solver iterations at one step
			int MaxNonLinearIterations;
			/// Specifies the maximum number of nonlinear solver convergence failures at one step
			int MaxConvergenceFailures;
			/// the maximum number of error test failures in attempting one step
			int MaxErrorTestFailures;
			/// Thickness of jacobian band used for the solver
			int maxl;
			/// Type of linear solver:
			/// - 0 = Dense Jacobian
			/// - 1 = Banded Jacobian
			/// - 2 = Diagonal Jacobian
			/// - 3 = Iterative Krylov solver with scaled preconditioned GMRES
			/// - 4 = Iterative Krylov solver with scaled preconditioned Bi-CGStab
			/// - 5 = Iterative Krylov solver with scaled preconditioned TFQMR
			int LinearSolver;
			/// Maximal order of the solver
			int MaxOrder;
			/// Maximal size of a time step
			cmf::math::Time max_step;
			/// Returns the last order of the solver used, may be smaller than MaxOrder due to the number of steps already taken or to fullfill stability limit
			int get_order() const;
			/// Initializes the solver. Do not add or remove state variables after initialization. The solver is automatically intialized when integrating
			void initialize();
			/// Releases the internal solver. Call release before you add state variables or to change properties.
			void release();

			virtual int integrate(cmf::math::Time t_max,cmf::math::Time dt);
			/// Resets the multi-step integrator. External changes to the state variables get recognized by the solver. Parameter changes of solver are ignored.
			virtual void reset();
			
			/// Create a new CVODE integrator
			/// @param epsilon relative and absolute error tolerance
			/// @param _preconditioner [R]ight, [L]eft, [B]oth side Krylov preconditioner or [N]o preconditioner
			CVodeIntegrator(real epsilon=1e-9,char _preconditioner='R') 
			:	Integrator(epsilon), 
				m_y(0),cvode_mem(0),precond_mem(0),
				preconditioner(_preconditioner),maxl(5),LinearSolver(3),
				MaxOrder(5),MaxNonLinearIterations(3),MaxErrorTestFailures(10),MaxConvergenceFailures(7),
				max_step(day)
			{	
				cvode_mem=0;
			}
			/// Create a new CVODE integrator
			/// @param states State variables to be added to the integrator
			/// @param epsilon relative and absolute error tolerance
			/// @param _preconditioner [R]ight, [L]eft, [B]oth side Krylov preconditioner or [N]o preconditioner
			CVodeIntegrator(cmf::math::StateVariableOwner& states, real epsilon=1e-9,char _preconditioner='R') 
				: Integrator(states,epsilon), m_y(0),cvode_mem(0),precond_mem(0),preconditioner(_preconditioner),maxl(5),LinearSolver(3),
				MaxOrder(5),MaxNonLinearIterations(3),MaxErrorTestFailures(10),MaxConvergenceFailures(7), error_msg("")
			{
				if (epsilon<=0.0 || epsilon>1e-3) {
					throw std::runtime_error("CVodeIntegrator: 0.0 < epsilon < 1e-3 not fullfilled");
				}
				cvode_mem=0;
			}

			/// copy constructor, creates a new CVODE integrator similiar to the given, but without statevariables
			CVodeIntegrator(const CVodeIntegrator & templ) 
				: Integrator(templ),preconditioner(templ.preconditioner),maxl(templ.maxl),m_y(0),cvode_mem(0),precond_mem(0),LinearSolver(templ.LinearSolver),
				MaxOrder(templ.MaxOrder),MaxNonLinearIterations(templ.MaxNonLinearIterations),MaxErrorTestFailures(templ.MaxErrorTestFailures),
				MaxConvergenceFailures(templ.MaxConvergenceFailures),max_step(templ.max_step),error_msg("")
			{
				cvode_mem=0;
			}
			/// Error vector of the integrator
			cmf::math::num_array get_error() const;

			/// Returns the number of non-linear iterations performed. Calls CVodeGetNumNonlinSolvIters
			int get_nonlinear_iterations() const;
			/// Returns the number of evaluations of the right hand side of the ODE. Calls CVodeGetNumRhsEvals
			int get_rhsevals() const;

			CVodeIntegrator * copy() const
			{
				return new CVodeIntegrator(*this);
			}
			virtual ~CVodeIntegrator();


		};
		
	}
	
}



#endif // cvode_h__
