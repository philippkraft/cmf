#ifndef cvode_h__
#define cvode_h__
// include CVODE
#include <sundials/sundials_nvector.h>       /* serial N_Vector types, fct. and macros */

#include "Integrator.h"
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
			
		public:
			/// Type of Krylov preconditioning. Must have one of the following values:
			/// - 'N' No Krylov preconditioner
			/// - 'L' Left hand side banded preconditioning
			/// - 'R' Right hand side preconditioning
			char preconditioner;
			/// Specifies the maximum number of nonlinear solver iterations at one step
			int MaxNonLinearIterations;
			// Specifies the maximum number of nonlinear solver convergence failures at one step
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
			bool reinit_always;
			cmf::math::Time max_step;
			/// Returns the last order of the solver used, may be smaller than MaxOrder due to the number of steps already taken or to fullfill stability limit
			int GetOrder();
			void cmf::math::CVodeIntegrator::ReInit(Time initdt, real epsilon=0);
			/// Initializes the solver. Do not add or remove statevariables after initialization
			void Initialize();
			virtual int Integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep);
			virtual void Reset();
			
			/// Create a new CVODE integrator
			/// @param epsilon relative and absolute error tolerance
			/// @param tStepMin Minimal timestep
			CVodeIntegrator(real epsilon=1e-9,cmf::math::Time tStepMin=Time::Milliseconds(50)) 
			: Integrator(epsilon,tStepMin), m_y(0),cvode_mem(0),precond_mem(0),preconditioner('N'),maxl(5),LinearSolver(3),
			MaxOrder(5),MaxNonLinearIterations(3),MaxErrorTestFailures(10),MaxConvergenceFailures(7),reinit_always(false),max_step(day)
			{	
				cvode_mem=0;
			}
			CVodeIntegrator(cmf::math::StateVariableOwner& states, real epsilon=1e-9,cmf::math::Time tStepMin=Time::Milliseconds(50)) 
				: Integrator(states,epsilon,tStepMin), m_y(0),cvode_mem(0),precond_mem(0),preconditioner('N'),maxl(5),LinearSolver(3),
				MaxOrder(5),MaxNonLinearIterations(3),MaxErrorTestFailures(10),MaxConvergenceFailures(7),reinit_always(false),max_step(day)
			{
				cvode_mem=0;
			}

			/// Copy constructor, creates a new CVODE integrator similiar to the given, but without statevariables
			CVodeIntegrator(const CVodeIntegrator & templ) 
				: Integrator(templ),preconditioner(templ.preconditioner),maxl(templ.maxl),m_y(0),cvode_mem(0),precond_mem(0),LinearSolver(templ.LinearSolver),
				MaxOrder(templ.MaxOrder),MaxNonLinearIterations(templ.MaxNonLinearIterations),MaxErrorTestFailures(templ.MaxErrorTestFailures),
				MaxConvergenceFailures(templ.MaxConvergenceFailures),reinit_always(templ.reinit_always)	,max_step(templ.max_step)
			{
				cvode_mem=0;
			}
			
			CVodeIntegrator * Copy() const
			{
				return new CVodeIntegrator(*this);
			}
			virtual ~CVodeIntegrator();


		};
		
	}
	
}



#endif // cvode_h__
