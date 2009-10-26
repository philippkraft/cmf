#include "FixPointImplicitEuler.h"
#include <iostream>
#ifdef _OPENMP
#include <omp.h>
#endif
using namespace std;
cmf::math::ImplicitEuler::ImplicitEuler(StateVariableOwner& states, real epsilon/*=1e-9*/,cmf::math::Time tStepMin/*=10.0/(3600.0*24.0)*/ ) : 
Integrator(states,epsilon,tStepMin)
{
	oldStates		  = num_array(int(m_States.size()),0);
	compareStates	= num_array(int(m_States.size()),0);
	dxdt          = num_array(int(m_States.size()),0);
}
cmf::math::ImplicitEuler::ImplicitEuler( real epsilon/*=1e-9*/,cmf::math::Time tStepMin/*=10.0/(3600.0*24.0)*/ ) : 
Integrator(epsilon,tStepMin)
{
}

cmf::math::ImplicitEuler::ImplicitEuler( const cmf::math::Integrator& forCopy) : cmf::math::Integrator(forCopy)
{

}
void cmf::math::ImplicitEuler::AddStatesFromOwner( cmf::math::StateVariableOwner& stateOwner ) 
{
	Integrator::AddStatesFromOwner(stateOwner);
	oldStates.resize(int(m_States.size()));
	compareStates.resize(int(m_States.size()));
	dxdt.resize(int(m_States.size()));
}



int cmf::math::ImplicitEuler::Integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
{
	// h is standard name in numeric for time step size
	Time h=MaxTime-ModelTime();
	// Don't stretch the current timestep more the 4 times the last timestep
	if (h > this->TimeStep() * 2) 
	{
		h=this->TimeStep() * 2;
	}

	// Copies the actual states to the history as x_(n)
	States().CopyStates(oldStates);
	// Count the iterations
	int iter=0;
	
	// Remember the last errors
	
	real err_ex=REAL_MAX/2;
	real old_err_ex=REAL_MAX;
	m_Iterations=0;
	do 
	{
		// Remember the current state for convergence criterion
		States().CopyStates(compareStates);
		// Get derivatives at t(n+1) * h[d]
		States().CopyDerivs(this->ModelTime() + h,dxdt,h.AsDays());
		//// Updates the state variables with the new states, according to the current order
		States().SetStates(oldStates);
		States() += dxdt;

		old_err_ex=err_ex;
		err_ex=error_exceedance(compareStates);


		// Count number of iterations
		++iter;
		++m_Iterations;


		// If the number of iterations exceeds the allowed number, reduce step size
		if (iter>200 || err_ex>=old_err_ex)
		{
			// Step size reduction
			h /= 2;
			//std::cout << '-';
			// If the time step becomes too small, throw exception
			if (m_NextTimeStep<MinTimestep())
			{
				std::cerr << "No convergence! Time=" << ModelTime().AsDate().to_string() << " Iter: " << iter << " Reach:" << Tag << std::endl;
				throw std::runtime_error("No convergence with a time step > minimal time step");
			}
			// Restore states
			States().SetStates(oldStates);
			iter=0;
			err_ex=REAL_MAX/2;
			old_err_ex=REAL_MAX;
		}

		// Loop until the iterations converge
	} while(err_ex > 1);

	m_TimeStep=h;

	ModelTime(ModelTime() + h);
	return iter;
}
