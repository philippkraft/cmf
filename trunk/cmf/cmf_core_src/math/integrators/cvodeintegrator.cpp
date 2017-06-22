

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
#include "cvodeintegrator.h"
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
	cmf::math::CVodeIntegrator* integ= static_cast<cmf::math::CVodeIntegrator*>(f_data);
	// Get the pointers to the data of the vectors u and udot
	realtype * udata=NV_DATA_S(u);
	realtype * dudata=NV_DATA_S(udot);
	// Get size of the problem
	int nsize=int(integ->size());
	// Update the states from the state vector
	try {
		integ->set_states(udata);
	}	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		integ->error_msg=e.what();
		return -8;
	}
	Time T=cmf::math::day * t;
	Time T_model = integ->get_t();
	real sec_local = (T - T_model)/sec;
	// Get the derivatives at time T
	try {
		integ->copy_dxdt(T,dudata);
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		integ->error_msg=e.what();
		return -8;
	}
	return 0;
}

void cmf::math::CVodeIntegrator::release()
{
	// If saved vector exists, destroy it
	if (m_y!=0) N_VDestroy_Serial(m_y);
	// Destroys any existent solver
	if (cvode_mem!=0) CVodeFree(&cvode_mem); 
}

void cmf::math::CVodeIntegrator::initialize()
{
	release();
	// size of problem
	int N=int(m_States.size());              
	// Allocate vector y
	m_y=N_VNew_Serial(N);
	// Pointer to data of vector y
	realtype * y_data = NV_DATA_S(m_y);      
	// Copy states to y
	copy_states(y_data);					 
	// Create a implicit (CV_BDF) solver with Newton iteration (CV_NEWTON)
	cvode_mem = CVodeCreate(CV_BDF,CV_NEWTON);
	// Set this wrapper as user data of the solver
	CVodeSetUserData(cvode_mem,(void *) this);
	// Local copy of Epsilon needed for CVodeSVtolerances
	realtype reltol=Epsilon;
	N_Vector abstol=N_VNew_Serial(N);
	realtype * abstol_data=NV_DATA_S(abstol);
	for (int i = 0; i < N ; ++i)
		abstol_data[i] = m_States[i]->get_abs_errtol(reltol*1e-3);
	int flag=0;
	// Allocate memory for solver and set the right hand side function, start time and error tolerance
	flag=CVodeInit(cvode_mem,cmf::math::CVodeIntegrator::f,get_t().AsDays(),m_y);
	flag=CVodeSVtolerances(cvode_mem,reltol,abstol);
	if (MaxOrder>2) flag=CVodeSetStabLimDet(cvode_mem,1);
	flag=CVodeSetMaxOrd(cvode_mem,MaxOrder);
	flag=CVodeSetMaxNonlinIters(cvode_mem,MaxNonLinearIterations);
	flag=CVodeSetMaxErrTestFails(cvode_mem,MaxErrorTestFailures);
	flag=CVodeSetMaxConvFails(cvode_mem,MaxConvergenceFailures);
	flag=CVodeSetMaxNumSteps(cvode_mem,10000);
	flag=CVodeSetMaxStep(cvode_mem,max_step.AsDays());
	// Disable warnings about 0-length timesteps (they are all right)
	flag=CVodeSetMaxHnilWarns(cvode_mem,-1);
	int PREC = preconditioner=='L' ? PREC_LEFT : preconditioner=='R' ? PREC_RIGHT : PREC_NONE;
	switch(LinearSolver)
	{
	case 0 : flag=CVDense(cvode_mem,N);		          break;
	case 1 : flag=CVBand(cvode_mem,N,maxl,maxl);	  break;
	case 2 : flag=CVDiag(cvode_mem);                  break;
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
int cmf::math::CVodeIntegrator::integrate( cmf::math::Time MaxTime,cmf::math::Time TimeStep )
{
	if (m_States.size()==0)
		throw std::out_of_range("No states to integrate!");

	// If solver or y is not initialized, initialize them
	if (!cvode_mem || !m_y )
	{
		m_dt=MaxTime-get_t();
		initialize();
	}
	// Get data of y
	realtype * y_data=NV_DATA_S(m_y);
	// Time step, needed as return value
	realtype t_ret=this->get_t().AsDays(),
		t_step=0;
	CVodeSetStopTime(cvode_mem,MaxTime.AsDays());
	error_msg="";
	int res = CVode(cvode_mem,MaxTime.AsDays(),m_y, &t_ret, CV_ONE_STEP);
	if (res<0) {
		set_states(y_data);
		if (error_msg!="")
			throw std::runtime_error(error_msg + " - " + (day*t_ret).AsDate().to_string());
		else
			throw std::runtime_error("CVode could not integrate due to failure (see message above) at t=" + (day*t_ret).AsDate().to_string());
	}
	long iterations;
	CVodeGetNumRhsEvals(cvode_mem,&iterations);
	m_Iterations=(int)iterations;
	CVodeGetLastStep(cvode_mem,&t_step);
	m_dt=day*t_step;
	real sec_step = t_step * 24*60*60;
	double sec_to_go = (MaxTime - t_ret*day)/sec;
	if (res == CV_TSTOP_RETURN) {
		if ((day*t_ret - MaxTime).AsMilliseconds()>5 || (day*t_ret - MaxTime).AsMilliseconds()<-5) {
			throw std::runtime_error("Time returned by CVODE ("+(day*t_ret).to_string() 
				+") differs from given end time(" 
				+ MaxTime.to_string() + "). Please inform cmf maintainer");
		}
		m_t = MaxTime;
	} else {
		m_t = t_ret*day;
	}


	// Copy result to state variables
	set_states(y_data);
	return res;
}

void cmf::math::CVodeIntegrator::reset()
{
	if (cvode_mem) {
		realtype* y_data = NV_DATA_S(m_y);
		this->copy_states(y_data);
		CVodeReInit(cvode_mem,get_t()/day,m_y);
	}
}

int cmf::math::CVodeIntegrator::get_order() const
{
	int result;
	CVodeGetLastOrder(cvode_mem,&result);
	return result;
}

cmf::math::num_array cmf::math::CVodeIntegrator::get_error() const
{
	N_Vector ele = N_VNew_Serial(long(size()));
	N_Vector eweight = N_VNew_Serial(long(size()));
	CVodeGetEstLocalErrors(cvode_mem,ele);
	CVodeGetErrWeights(cvode_mem,eweight);
	N_Vector result = N_VNew_Serial(long(size()));
	N_VProd(ele,eweight,result);
	num_array res(NV_DATA_S(result),NV_DATA_S(result)+size());
	N_VDestroy_Serial(ele);
	N_VDestroy_Serial(eweight);
	return res;

}

int cmf::math::CVodeIntegrator::get_nonlinear_iterations() const
{
	if (cvode_mem) {
		long result;
		CVodeGetNumNonlinSolvIters(cvode_mem,&result);
		return result;
	} else {
		throw std::runtime_error("CVode solver is not initialized");
	}
}

int cmf::math::CVodeIntegrator::get_rhsevals() const
{
	if (cvode_mem) {
		long result;
		CVodeGetNumRhsEvals(cvode_mem,&result);
		return result;
	} else {
		throw std::runtime_error("CVode solver is not initialized");
	}
}
