#include "cvode3.h"

#include <cvode/cvode.h>
#include <nvector/nvector_serial.h>
#include <cvode/cvode_direct.h>
#include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype      */

#include <cvode/cvode_direct.h>        /* access to CVDls interface            */
// Special includes for dense solver
#include <sunmatrix/sunmatrix_dense.h> /* access to dense SUNMatrix            */
#include <sunlinsol/sunlinsol_dense.h> /* access to dense SUNLinearSolver      */

// Special includes for banded solver
#include <sunmatrix/sunmatrix_band.h> /* access to band SUNMatrix            */
#include <sunlinsol/sunlinsol_band.h> /* access to band SUNLinearSolver      */

#include <cvode/cvode_spils.h>

using namespace cmf::math;



int f(realtype t, N_Vector u, N_Vector udot, void *f_data)
{
	CVode3* integ = static_cast<CVode3*>(f_data);
	// Get the pointers to the data of the vectors u and udot
	realtype * udata = NV_DATA_S(u);
	realtype * dudata = NV_DATA_S(udot);
	// Get size of the problem
	int nsize = int(integ->size());
	// Update the states from the state vector
	try {
		integ->set_states(udata);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		integ->set_error_msg(e.what());
		return -8;
	}
	Time T = cmf::math::day * t;
	Time T_model = integ->get_t();
	real sec_local = (T - T_model) / sec;
	// Get the derivatives at time T
	try {
		integ->copy_dxdt(T, dudata);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		integ->set_error_msg(e.what());
		return -8;
	}
	return 0;
}



class cmf::math::CVode3::Impl {
private:
	CVode3 * _integrator;

	int set_dense_solver() {
		if (cvode_mem == 0) {
			throw std::runtime_error("Tried to create dense solver for uninitialized cvode");
		}
		J = SUNDenseMatrix(N, N);
		LS = SUNDenseLinearSolver(y, J);
		// Check for LS / J = NULL
		int flag = 0;
		return CVDlsSetLinearSolver(cvode_mem, LS, J);
		// Check flag and raise an error
	}



	int set_banded_solver(int bandwidth = 5) {
		if (cvode_mem == 0) {
			throw std::runtime_error("Tried to create banded solver for uninitialized cvode");
		}
		J = SUNBandMatrix(N, bandwidth, bandwidth, 2 * bandwidth);
		LS = SUNBandLinearSolver(y, J);
		return CVDlsSetLinearSolver(cvode_mem, LS, J);

	}


public:
	Impl(CVode3 * integrator) : 
		_integrator(integrator)
	{		}
	
	// State vector
	N_Vector y = 0;
	// Jacobian matrix
	SUNMatrix J = 0;
	// Linear solver
	SUNLinearSolver LS = 0;
	int N=0;
	void * cvode_mem=0;
	
	void release()
	{
		// If saved vector exists, destroy it
		if (y != 0) N_VDestroy_Serial(y);
		if (cvode_mem != 0) CVodeFree(&cvode_mem);
		if (LS != 0) SUNLinSolFree(LS);
		if (J != 0) SUNMatDestroy(J);
	}
	int intitialize() {
		release();
		// size of problem
		N = int(_integrator->m_States.size());
		// Allocate vector y
		y = N_VNew_Serial(N);
		// Pointer to data of vector y
		realtype * y_data = NV_DATA_S(y);
		// Copy states to y
		_integrator->copy_states(y_data);
		// Create a implicit (CV_BDF) solver with Newton iteration (CV_NEWTON)
		cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
		// Set this wrapper as user data of the solver
		CVodeSetUserData(cvode_mem, (void *)this);
		// Local copy of Epsilon needed for CVodeSVtolerances
		realtype reltol = _integrator->Epsilon;
		N_Vector abstol = N_VNew_Serial(N);
		realtype * abstol_data = NV_DATA_S(abstol);
		for (int i = 0; i < N; ++i)
			abstol_data[i] = _integrator->m_States[i]->get_abs_errtol(reltol*1e-3);
		int flag = 0;
		// Allocate memory for solver and set the right hand side function, start time and error tolerance
		flag = CVodeInit(cvode_mem, f, _integrator->get_t().AsDays(), y);
		flag = CVodeSVtolerances(cvode_mem, reltol, abstol);
		
		/* Fine tuning of solver settings
		if (_integrator->MaxOrder>2) flag = CVodeSetStabLimDet(cvode_mem, 1);

		flag = CVodeSetMaxOrd(cvode_mem, _integrator->MaxOrder);
		flag = CVodeSetMaxNonlinIters(cvode_mem, _integrator->MaxNonLinearIterations);
		flag = CVodeSetMaxErrTestFails(cvode_mem, _integrator->MaxErrorTestFailures);
		flag = CVodeSetMaxConvFails(cvode_mem, MaxConvergenceFailures);
		flag = CVodeSetMaxNumSteps(cvode_mem, 10000);
		flag = CVodeSetMaxStep(cvode_mem, max_step.AsDays());
		*/
		// Disable warnings about 0-length timesteps (they are all right)
		flag = CVodeSetMaxHnilWarns(cvode_mem, -1);

		flag = set_banded_solver(5);

		

		if (flag<0) throw std::runtime_error("Could not create CVODE solver");

	}
};

int cmf::math::CVode3::integrate(cmf::math::Time t_max, cmf::math::Time dt)
{

	return 0;
}

void cmf::math::CVode3::reset()
{
}

cmf::math::CVode3::CVode3(cmf::math::StateVariableOwner & states, real epsilon)
	: Integrator(states, epsilon)
{
	_implementation = std::make_unique<cmf::math::CVode3::Impl>(this);
}

void cmf::math::CVode3::set_error_msg(std::string error)
{
	_error_msg = error;
}

CVode3 * cmf::math::CVode3::copy() const
{
	return nullptr;
}

