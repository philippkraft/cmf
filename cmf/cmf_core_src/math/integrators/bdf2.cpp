

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
#include "bdf2.h"
#include <cmath>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <iostream>
// Just a macro to compare two values
#define min(a,get_b) (((a)<(get_b)) ? (a) : (get_b))


// Creates a new Integrator w/o states
cmf::math::BDF2::BDF2( real epsilon/*=1e-9*/,cmf::math::Time tStepMin/*=Time::Seconds(10)*/) 
: Integrator(epsilon),	order(1),stepNo(0), dt_min(tStepMin), error_position(-1),max_order(2)
{ 
	// Assessing multistep functions
	calc_newState[0] = &cmf::math::BDF2::Gear1newState; // impl. Euler
	calc_newState[1] = &cmf::math::BDF2::Gear2newState; // 2nd order Gear
}

cmf::math::BDF2::BDF2( const Integrator& templ) :
cmf::math::Integrator(templ),order(1),stepNo(0), error_position(-1),max_order(2)
{

	// Assessing multistep functions
	calc_newState[0] = &cmf::math::BDF2::Gear1newState; // impl. Euler
	calc_newState[1] = &cmf::math::BDF2::Gear2newState; // 2nd order Gear
}

cmf::math::BDF2::BDF2( cmf::math::StateVariableOwner& states, real epsilon/*=1e-9*/,cmf::math::Time tStepMin/*=Time::Milliseconds(10)*/ )
 : Integrator(states,epsilon), order(1),stepNo(0), dt_min(tStepMin), error_position(-1),max_order(2)
{
	// Assessing multistep functions
	calc_newState[0] = &cmf::math::BDF2::Gear1newState; // impl. Euler
	calc_newState[1] = &cmf::math::BDF2::Gear2newState; // 2nd order Gear

}
// Adds states to the vector of states
void cmf::math::BDF2::add_states( cmf::math::StateVariableOwner& stateOwner )
{
	// Call base class function
	Integrator::add_states(stateOwner);
	// Resize helper vectors (convergence check,derivatives and history)
	compareStates.resize(size());
	dxdt.resize(size());
	for (int i = 0; i < 2 ; i++)
		pastStatesArray[i].resize(size());
}


// Gear formulas
// First order: newState = statevector + h*dxdt (impl. Euler)
void cmf::math::BDF2::Gear1newState( real h )
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
			state_i +=       pastStates(0)[i];
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
			state_i +=       pastStates(0)[i];
			set_state(i, state_i);
		}

	}
}

// Second order
void cmf::math::BDF2::Gear2newState(real h)
{
	real state_i;
	const real 
		// p is the relative change of time step size
		p    = h/get_dt().AsDays(),
		p1   = 1 + p,
		h_p1 = h * p1,
		p1_2 = p1 * p1,
		p_2  = p * p;
	if (use_OpenMP)
	{
	#pragma omp parallel for private(state_i)
		for (ptrdiff_t i = 0; i < ptrdiff_t(size()) ; i++)
		{
			// The formula is written so ugly to avoid internal memory allocation
			// x_(n+1) = (p+1)�x_(n) - p�x_(n-1) + h (p+1) dxdt
			state_i  =        dxdt[i]; 
			state_i *= h_p1;
			state_i += p1_2 * pastStates(0)[i];
			state_i -= p_2  * pastStates(1)[i];
			state_i /= 1.0 + 2.0*p;
			set_state(i, state_i);
		}
	}
	else
	{
		for (ptrdiff_t i = 0; i < ptrdiff_t(size()) ; i++)
		{
			// The formula is written so ugly to avoid internal memory allocation
			// x_(n+1) = (p+1)�x_(n) - p�x_(n-1) + h (p+1) dxdt
			state_i  =        dxdt[i]; 
			state_i *= h_p1;
			state_i += p1_2 * pastStates(0)[i];
			state_i -= p_2  * pastStates(1)[i];
			state_i /= 1.0 + 2.0*p;
			set_state(i, state_i);
		}
	}

}
void cmf::math::BDF2::set_abstol()
{
	abstol.resize(ptrdiff_t(m_States.size()));
	for (ptrdiff_t i = 0; i < m_States.size(); ++i)
		abstol[i] = m_States[i]->get_abs_errtol(Epsilon * 1e-3);

}

real cmf::math::BDF2::error_exceedance( const num_array& compare,int * biggest_error_position/*=0 */ )
{
	real res=0;
#pragma omp parallel for shared(res)
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()) ; i++)
	{
		real error=fabs(compare[i]-get_state(i));
		// Calculate absolute error tolerance as: epsilon + |(x_p+x_(n+1))/2|*epsilon
		real errortol=Epsilon + fabs(get_state(i))*Epsilon;
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


int cmf::math::BDF2::integrate( cmf::math::Time MaxTime,cmf::math::Time timestep )
{
	if (m_States.size()==0)
		throw std::out_of_range("No states to integrate!");
	// h is standard name in numeric for time step size
	Time h=MaxTime-get_t();
	// Don't stretch the current timestep more the 2 times the last timestep
	if (h>get_dt()*2) 
	{
		h=get_dt()*2;
	}

	// Copies the actual states to the history as x_(n)
	copy_states(pastStates(0));
	// Count the iterations
	int iter=0;
	real old_err_ex=REAL_MAX;
	real err_ex=REAL_MAX/2;
	m_Iterations=0;
	do 
	{
		// Remember the current state for convergence criterion
		copy_states(compareStates);
		// Get derivatives at t(n+1)
		copy_dxdt(this->get_t() + h,dxdt);
		// Updates the state variables with the new states, according to the current order
		(this->*(calc_newState[order-1]))(h.AsDays());


		old_err_ex=err_ex;
		err_ex=error_exceedance(compareStates,&error_position);


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
			if (h<dt_min)
			{
				std::cerr << "No convergence! Time=" << get_t().AsDate().to_string() << " Iter: " << iter << " StateV No." << error_position << std::endl;
				throw std::runtime_error("No convergence with a time step > minimal time step");
			}
			// Restore states
			set_states(pastStates(0));

			iter=0;
			err_ex=REAL_MAX/2;
			old_err_ex=REAL_MAX;
		}

		// Loop until the iterations converge
	} while(err_ex > 1);

	m_dt=h;
	
	// If we're just using this integrator to bootstrap a
	// higher-order method, arrange to transfer control to 
	// the next order method.
	if (order<max_order) ++order;
	// Raise the number of steps
	++stepNo;
	set_t(get_t() + h);
	return iter;
}

#undef max
