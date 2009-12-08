#include "cvodeIntegrator.h"
#include <cvode/cvode.h>                  /* main integrator header file */
#include <cvode/cvode_spgmr.h>            /* prototypes & constants for CVSPGMR solver */
#include <cvode/cvode_spbcgs.h>
#include <cvode/cvode_bandpre.h>
#include <cvode/cvode_dense.h>
#include <cvode/cvode_band.h>
#include <cvode/cvode_diag.h>
#include <cvode/cvode_sptfqmr.h>
#include <nvector/nvector_openmp.h>       /* serial N_Vector types, fct. and macros */
#include <nvector/nvector_serial.h>       /* serial N_Vector types, fct. and macros */
#include <sundials/sundials_smalldense.h> /* use generic DENSE solver in preconditioning */
#include <sundials/sundials_types.h>      /* definition of realtype */
#include <sundials/sundials_math.h>       /* contains the macros ABS, SQR, and EXP */

#define NV_DATA(v) use_OpenMP ? NV_DATA_O(v) : NV_DATA_S(v)
// Right hand side function f in dy/dt=f(y,t)
int cmf::math::CVodeIntegrator::f( realtype t, N_Vector u, N_Vector udot, void *f_data )
{
	// Get the state variables from the user data
	StateVariableVector& state_vars=*(StateVariableVector *)(f_data);
	// Get the pointers to the data of the vectors u and udot
	realtype * udata=state_vars.use_OpenMP ? NV_DATA_O(u) : NV_DATA_S(u);
	realtype * dudata=state_vars.use_OpenMP ? NV_DATA_O(udot) : NV_DATA_S(udot);
	// Get size of the problem
	int nsize=int(state_vars.size());
	// Update the states from the state vector
	state_vars.SetStates(udata);

	Time T=cmf::math::day * t;
	// Get the derivatives at time T
	state_vars.CopyDerivs(T,dudata);
	return 0;
}


void cmf::math::CVodeIntegrator::ReInit(Time initdt, real epsilon)
{
	if (epsilon=0) epsilon=Epsilon;
	CVodeReInit(cvode_mem,cmf::math::CVodeIntegrator::f,ModelTime().AsDays(),m_y,CV_SS,epsilon,&epsilon);
	CVodeSetInitStep(cvode_mem,initdt.AsDays());
}

void cmf::math::CVodeIntegrator::Initialize()
{
	if (m_y!=0)
		if (use_OpenMP)
			N_VDestroy_OpenMP(m_y);      // Destroys any existent vector y
		else
			N_VDestroy_Serial(m_y);
	if (cvode_mem!=0)	
		CVodeFree(&cvode_mem); // Destroys any existent solver
	int N=int(m_States.size());              // size of problem
	m_y=N_VNew_OpenMP(N);										 // Allocate vector y
	realtype * y_data = NV_DATA_O(m_y);      // Pointer to data of vector y
	m_States.CopyStates(y_data);						 // Copy states to y
	// Create a implicit (CV_BDF) solver with Newton iteration (CV_NEWTON)
	cvode_mem = CVodeCreate(CV_BDF,CV_NEWTON);
	// Set the state vector as user data of the solver
	CVodeSetFdata(cvode_mem,(void *) &m_States);
	// Local copy of Epsilon needed for CVodeMalloc
	realtype epsilon=Epsilon;
	int flag=0;
	// Allocate memory for solver and set the right hand side function, start time and error tolerance
	flag=CVodeMalloc(cvode_mem,cmf::math::CVodeIntegrator::f,ModelTime().AsDays(),m_y,CV_SS,epsilon,&epsilon);
	// Sets the initial step size
	//flag=CVodeSetInitStep(cvode_mem,TimeStep().AsDays());
	// Sets the maximum step size
	//flag=CVodeSetMaxStep(cvode_mem,cmf::math::day.AsDays());
	// Sets the minimum step size
	//flag=CVodeSetMinStep(cvode_mem,MinTimestep().AsDays());
	// Detect stability problems when order >= 3 (Max order=5)
	if (MaxOrder>2) flag=CVodeSetStabLimDet(cvode_mem,1);
	flag=CVodeSetMaxOrd(cvode_mem,MaxOrder);
	flag=CVodeSetMaxNonlinIters(cvode_mem,MaxNonLinearIterations);
	flag=CVodeSetMaxErrTestFails(cvode_mem,MaxErrorTestFailures);
	flag=CVodeSetMaxConvFails(cvode_mem,MaxConvergenceFailures);
	flag=CVodeSetMaxNumSteps(cvode_mem,10000);
	flag=CVodeSetMaxStep(cvode_mem,max_step.AsDays());

	switch(LinearSolver)
	{
	case 0 : flag=CVDense(cvode_mem,N);		          break;
	case 1 : flag=CVBand(cvode_mem,N,maxl,maxl);		break;
	case 2 : flag=CVDiag(cvode_mem);                break;
	case 3 : if (preconditioner=='L' || preconditioner=='R')
						{
							// Create a banded preconditioner
							precond_mem = CVBandPrecAlloc(cvode_mem,N,maxl,maxl);
							// Set GMRS with banded preconditioner
							flag=CVBPSpgmr(cvode_mem,preconditioner=='L' ? PREC_LEFT : PREC_RIGHT,maxl,precond_mem);
						}
						else 
						{
							// Set GMRS as solving method, without any preconditioner
							flag=CVSpgmr(cvode_mem,PREC_NONE,maxl);
							flag=CVSpilsSetGSType(cvode_mem, MODIFIED_GS);
						}
	    break;
	case 4 : 	if (preconditioner=='L' || preconditioner=='R')
						{
							// Create a banded preconditioner
							precond_mem = CVBandPrecAlloc(cvode_mem,N,maxl,maxl);
							// Set GMRS with banded preconditioner
							flag=CVBPSpbcg(cvode_mem,preconditioner=='L' ? PREC_LEFT : PREC_RIGHT,maxl,precond_mem);
						}
						else
						{
							// Set GMRS as solving method, without any preconditioner
							flag=CVSpbcg(cvode_mem,PREC_NONE,maxl);
						}
				break;
	case 5 :	if (preconditioner=='L' || preconditioner=='R')
						{
							// Create a banded preconditioner
							precond_mem = CVBandPrecAlloc(cvode_mem,N,maxl,maxl);
							// Set GMRS with banded preconditioner
							flag=CVBPSptfqmr(cvode_mem,preconditioner=='L' ? PREC_LEFT : PREC_RIGHT,maxl,precond_mem);
						}
						else 
						{
							// Set GMRS as solving method, without any preconditioner
							flag=CVSptfqmr(cvode_mem,PREC_NONE,maxl);
						}
						break;
	default: throw std::runtime_error("Linear solver type not in 0..5");
	}
	if (flag<0) throw std::runtime_error("Could not create CVODE solver");

}

cmf::math::CVodeIntegrator::~CVodeIntegrator()
{																																																																					
	// Destroy y
	if (m_y)
		if (use_OpenMP)
			N_VDestroy_OpenMP(m_y);
		else
			N_VDestroy_Serial(m_y);
	// Destroy preconditioner
	if (precond_mem) CVBandPrecFree(&precond_mem);
	// Destroy solver
	if (cvode_mem) CVodeFree(&cvode_mem);

}
int cmf::math::CVodeIntegrator::Integrate( cmf::math::Time MaxTime,cmf::math::Time TimeStep )
{
	// If solver or y is not initialized, initialize them
	if (!cvode_mem || !m_y )
	{
		m_TimeStep=MaxTime-ModelTime();
		Initialize();
	}
	if (reinit_always)
	{
		realtype epsilon=Epsilon;
		CVodeReInit(cvode_mem,cmf::math::CVodeIntegrator::f,ModelTime().AsDays(),m_y,CV_SS,epsilon,&epsilon);
	}
	// Get data of y
	realtype * y_data=NV_DATA_O(m_y);
	// Time step, needed as return value
	realtype t=0;
	// Solver until MaxTime
	CVodeSetStopTime(cvode_mem,MaxTime.AsDays());
	int res=CVode(cvode_mem,MaxTime.AsDays(),m_y,&t,CV_NORMAL);
	// Throw if integration fails
	if (res<0) 
	{
		m_States.SetStates(y_data);
		throw std::runtime_error("CVode could not integrate due to failure (see message above)");
	}
	// Get statistics about CVode call
	long iterations;
	CVodeGetNumRhsEvals(cvode_mem,&iterations);
	m_Iterations=(int)iterations;
	CVodeGetLastStep(cvode_mem,&t);
	m_TimeStep=day*t;
	// Set new time
	m_Time=MaxTime;
	// Copy result to state variables
	m_States.SetStates(y_data);
	return res;
}

void cmf::math::CVodeIntegrator::Reset()
{
	Initialize();
}

int cmf::math::CVodeIntegrator::GetOrder()
{
	int result;
	CVodeGetLastOrder(cvode_mem,&result);
	return result;


}