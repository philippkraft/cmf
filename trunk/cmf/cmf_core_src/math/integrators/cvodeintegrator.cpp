

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
#include "cvodeIntegrator.h"
#include <cvode/cvode.h>                  /* main integrator header file */
#include <cvode/cvode_spgmr.h>            /* prototypes & constants for CVSPGMR solver */
#include <cvode/cvode_spbcgs.h>
#include <cvode/cvode_bandpre.h>
#include <cvode/cvode_dense.h>
#include <cvode/cvode_band.h>
#include <cvode/cvode_diag.h>
#include <cvode/cvode_sptfqmr.h>
#include <nvector/nvector_serial.h>       /* serial N_Vector types, fct. and macros */
#include <sundials/sundials_dense.h> /* use generic DENSE solver in preconditioning */
#include <sundials/sundials_types.h> /* definition of realtype */
#include <sundials/sundials_math.h>  /* contains the macros ABS and SQR */

#define NV_DATA(v) use_OpenMP ? NV_DATA_O(v) : NV_DATA_S(v)
// Right hand side function f in dy/dt=f(y,t)
int cmf::math::CVodeIntegrator::f( realtype t, N_Vector u, N_Vector udot, void *f_data )
{
	// Get the state variables from the user data
	StateVariableVector& state_vars=*(StateVariableVector *)(f_data);
	// Get the pointers to the data of the vectors u and udot
	realtype * udata=NV_DATA_S(u);
	realtype * dudata=NV_DATA_S(udot);
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
	CVodeReInit(cvode_mem,ModelTime().AsDays(),m_y);
	CVodeSetInitStep(cvode_mem,initdt.AsDays());
}

void cmf::math::CVodeIntegrator::Initialize()
{
	if (m_y!=0)
		N_VDestroy_Serial(m_y);
	if (cvode_mem!=0)	
		CVodeFree(&cvode_mem); // Destroys any existent solver
	int N=int(m_States.size());              // size of problem
	m_y=N_VNew_Serial(N);										 // Allocate vector y
	realtype * y_data = NV_DATA_S(m_y);      // Pointer to data of vector y
	m_States.CopyStates(y_data);						 // Copy states to y
	// Create a implicit (CV_BDF) solver with Newton iteration (CV_NEWTON)
	cvode_mem = CVodeCreate(CV_BDF,CV_NEWTON);
	// Set the state vector as user data of the solver
	CVodeSetUserData(cvode_mem,(void *) &m_States);
	// Local copy of Epsilon needed for CVodeMalloc
	realtype epsilon=Epsilon;
	int flag=0;
	// Allocate memory for solver and set the right hand side function, start time and error tolerance
	flag=CVodeInit(cvode_mem,cmf::math::CVodeIntegrator::f,ModelTime().AsDays(),m_y);
	flag=CVodeSStolerances(cvode_mem,epsilon,epsilon);
	if (MaxOrder>2) flag=CVodeSetStabLimDet(cvode_mem,1);
	flag=CVodeSetMaxOrd(cvode_mem,MaxOrder);
	flag=CVodeSetMaxNonlinIters(cvode_mem,MaxNonLinearIterations);
	flag=CVodeSetMaxErrTestFails(cvode_mem,MaxErrorTestFailures);
	flag=CVodeSetMaxConvFails(cvode_mem,MaxConvergenceFailures);
	flag=CVodeSetMaxNumSteps(cvode_mem,10000);
	flag=CVodeSetMaxStep(cvode_mem,max_step.AsDays());
	int PREC = preconditioner=='L' ? PREC_LEFT : preconditioner=='R' ? PREC_RIGHT : PREC_NONE;
	switch(LinearSolver)
	{
	case 0 : flag=CVDense(cvode_mem,N);		          break;
	case 1 : flag=CVBand(cvode_mem,N,maxl,maxl);		break;
	case 2 : flag=CVDiag(cvode_mem);                break;
	case 3 : 
		flag=CVSpgmr(cvode_mem,PREC,maxl);
		// Create a banded preconditioner
		flag = CVBandPrecInit(cvode_mem,N,maxl,maxl);
		// Set GMRS with banded preconditioner
		break;
	case 4 : 
		flag=CVSpbcg(cvode_mem,PREC,maxl);
		// Create a banded preconditioner
		flag = CVBandPrecInit(cvode_mem,N,maxl,maxl);
		// Set GMRS with banded preconditioner
		break;
	case 5 : 
		flag=CVSptfqmr(cvode_mem,PREC,maxl);
		// Create a banded preconditioner
		flag = CVBandPrecInit(cvode_mem,N,maxl,maxl);
		// Set GMRS with banded preconditioner
		break;
	default: throw std::runtime_error("Linear solver type not in 0..5");
	}
	if (flag<0) throw std::runtime_error("Could not create CVODE solver");

}

cmf::math::CVodeIntegrator::~CVodeIntegrator()
{																																																																					
	// Destroy y
	if (m_y)
		N_VDestroy_Serial(m_y);
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
		CVodeReInit(cvode_mem,ModelTime().AsDays(),m_y);
	}
	// Get data of y
	realtype * y_data=NV_DATA_S(m_y);
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