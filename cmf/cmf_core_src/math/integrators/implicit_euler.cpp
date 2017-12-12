

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
#include "implicit_euler.h"
#include <iostream>
#ifdef _OPENMP
#include <omp.h>
#endif
using namespace std;
cmf::math::ImplicitEuler::ImplicitEuler(StateVariableOwner& states, 
										real epsilon/*=1e-9*/,
										cmf::math::Time tStepMin/*=10.0/(3600.0*24.0)*/ ) 
: Integrator(states,epsilon),dt_min(tStepMin)
{
	oldStates      = num_array(ptrdiff_t(m_States.size()),0);
	compareStates  = num_array(ptrdiff_t(m_States.size()),0);
	dxdt           = num_array(ptrdiff_t(m_States.size()),0);
}
cmf::math::ImplicitEuler::ImplicitEuler( real epsilon/*=1e-9*/,cmf::math::Time tStepMin/*=10.0/(3600.0*24.0)*/ ) : 
Integrator(epsilon), dt_min(tStepMin)
{
}

cmf::math::ImplicitEuler::ImplicitEuler( const cmf::math::Integrator& forCopy) 
: cmf::math::Integrator(forCopy)
{

}

real cmf::math::ImplicitEuler::error_exceedance( const num_array& compare,ptrdiff_t * biggest_error_position/*=0 */ )
{
	real res=1;
#pragma omp parallel for shared(res)
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()) ; i++)
	{
		real error=fabs(compare[i]-get_state(i));
		// Calculate absolute error tolerance as: abstol + |(x_p+x_(n+1))/2|*epsilon
		real errortol=abstol[i] + fabs(get_state(i))*Epsilon;
		if (error/errortol>res)
#pragma omp critical
		{
			if (error/errortol>res)
			{
				res=error/errortol;
				if (biggest_error_position)
					*biggest_error_position=i;
			}
		}
	}
	return res;
}

void cmf::math::ImplicitEuler::add_states( cmf::math::StateVariableOwner& stateOwner ) 
{
	Integrator::add_states(stateOwner);
	oldStates.resize(ptrdiff_t(m_States.size()));
	compareStates.resize(ptrdiff_t(m_States.size()));
	dxdt.resize(ptrdiff_t(m_States.size()));
}

void cmf::math::ImplicitEuler::set_abstol()
{
	abstol.resize(ptrdiff_t(m_States.size()));
	for (ptrdiff_t i = 0; i < m_States.size(); ++i)
		abstol[i] = m_States[i]->get_abs_errtol(Epsilon * 1e-3);

}

int cmf::math::ImplicitEuler::integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
{
	if (m_States.size()==0)
		throw std::out_of_range("No states to integrate!");

	// h is standard name in numeric for time step size
	Time h=MaxTime-get_t();
	// Don't stretch the current timestep more the 4 times the last timestep
	if (h > this->get_dt() * 2) 
	{
		h=this->get_dt() * 2;
	}

	// Copies the actual states to the history as x_(n)
	copy_states(oldStates);
	// Count the iterations
	ptrdiff_t iter=0;
	
	// Remember the last errors
	
	real err_ex=REAL_MAX/2;
	real old_err_ex=REAL_MAX;
	m_Iterations=0;
	do 
	{
		// Remember the current state for convergence criterion
		copy_states(compareStates);
		// Get derivatives at t(n+1) * h[d]
		copy_dxdt(this->get_t() + h,dxdt);
		// Updates the state variables with the new states, according to the current order
		Gear1newState(h.AsDays());

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
			if (m_dt<dt_min)
			{
				std::cerr << "No convergence! Time=" << get_t().AsDate().to_string() << " Iter: " << iter << std::endl;
				throw std::runtime_error("No convergence with a time step > minimal time step");
			}
			// Restore states
			set_states(oldStates);
			iter=0;
			err_ex=REAL_MAX/2;
			old_err_ex=REAL_MAX;
		}

		// Loop until the iterations converge
	} while(err_ex > 1);

	m_dt=h;

	set_t(get_t() + h);
	return int(iter);
}

void cmf::math::ImplicitEuler::Gear1newState( real h )
{
	real state_i;
	if (use_OpenMP)
	{
#pragma omp parallel for private(state_i)
		for (ptrdiff_t i = 0; i < ptrdiff_t(size()) ; i++)
		{
			// The formula is written so ugly to avoid internal memory allocation
			// x_n+1 = x_(n) + h dxdt
			state_i  =       dxdt[i]; 
			state_i *= h; 
			state_i +=       oldStates[i];
			set_state(i, state_i);
		}
	}
	else
	{
		for (ptrdiff_t i = 0; i < ptrdiff_t(size()) ; i++)
		{
			// The formula is written so ugly to avoid internal memory allocation
			// x_n+1 = x_(n) + h dxdt
			state_i  =       dxdt[i]; 
			state_i *= h; 
			state_i +=       oldStates[i];
			set_state(i, state_i);
		}

	}

}
