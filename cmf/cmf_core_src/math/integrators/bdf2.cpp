

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
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
#include <algorithm>

// Creates a new Integrator w/o states
cmf::math::BDF2::BDF2( real epsilon/*=1e-9*/,cmf::math::Time tStepMin/*=Time::Seconds(10)*/) 
: Integrator(epsilon),	order(1),stepNo(0), dt_min(tStepMin), error_position(-1),max_order(2)
{ 
	// Assessing multistep functions
	calc_newState[0] = &cmf::math::BDF2::Gear1newState; // impl. Euler
	calc_newState[1] = &cmf::math::BDF2::Gear2newState; // 2nd order Gear
	reset();
}

cmf::math::BDF2::BDF2( const Integrator& templ) :
cmf::math::Integrator(templ),order(1),stepNo(0), error_position(-1),max_order(2)
{

	// Assessing multistep functions
	calc_newState[0] = &cmf::math::BDF2::Gear1newState; // impl. Euler
	calc_newState[1] = &cmf::math::BDF2::Gear2newState; // 2nd order Gear
	reset();
}

cmf::math::BDF2::BDF2(const cmf::math::state_list &states, real epsilon/*=1e-9*/,
                      cmf::math::Time tStepMin/*=Time::Milliseconds(10)*/ )
 : Integrator(states, epsilon), order(1),stepNo(0), dt_min(tStepMin), error_position(-1),max_order(2)
{
	// Assessing multistep functions
	calc_newState[0] = &cmf::math::BDF2::Gear1newState; // impl. Euler
	calc_newState[1] = &cmf::math::BDF2::Gear2newState; // 2nd order Gear
	reset();
}

// Gear formulas
// First order: newState = statevector + h*dxdt (impl. Euler)
void cmf::math::BDF2::Gear1newState( real h )
{
	real state_i;
	if (get_system().use_OpenMP > 1)
	{
#pragma omp parallel for private(state_i)
		for (ptrdiff_t i = 0; i < ptrdiff_t(get_system().size()) ; i++)
		{
			// The formula is written so ugly to avoid internal memory allocation
			// x_n+1 = x_(n) + h dxdt
			state_i  =       dxdt[i]; 
			state_i *= h; 
			state_i +=       pastStates(0)[i];
			get_system().set_state_value(i, state_i);
		}
	}
	else
	{
		for (ptrdiff_t i = 0; i < ptrdiff_t(get_system().size()) ; i++)
		{
			// The formula is written so ugly to avoid internal memory allocation
			// x_n+1 = x_(n) + h dxdt
			state_i  =       dxdt[i]; 
			state_i *= h; 
			state_i +=       pastStates(0)[i];
			get_system().set_state_value(i, state_i);
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
	if (get_system().use_OpenMP > 1)
	{
	#pragma omp parallel for private(state_i)
		for (ptrdiff_t i = 0; i < ptrdiff_t(get_system().size()) ; i++)
		{
			// The formula is written so ugly to avoid internal memory allocation
			// x_(n+1) = (p+1)�x_(n) - p�x_(n-1) + h (p+1) dxdt
			state_i  =        dxdt[i]; 
			state_i *= h_p1;
			state_i += p1_2 * pastStates(0)[i];
			state_i -= p_2  * pastStates(1)[i];
			state_i /= 1.0 + 2.0*p;
			get_system().set_state_value(i, state_i);
		}
	}
	else
	{
		for (ptrdiff_t i = 0; i < ptrdiff_t(get_system().size()) ; i++)
		{
			// The formula is written so ugly to avoid internal memory allocation
			// x_(n+1) = (p+1)�x_(n) - p�x_(n-1) + h (p+1) dxdt
			state_i  =        dxdt[i]; 
			state_i *= h_p1;
			state_i += p1_2 * pastStates(0)[i];
			state_i -= p_2  * pastStates(1)[i];
			state_i /= 1.0 + 2.0*p;
			get_system().set_state_value(i, state_i);
		}
	}

}

real cmf::math::BDF2::error_excedance( const num_array& compare,ptrdiff_t * biggest_error_position/*=0 */ )
{
	real res=0;
#pragma omp parallel for shared(res)
	for (ptrdiff_t i = 0; i < ptrdiff_t(get_system().size()) ; i++)
	{
		real v = get_system().get_state_value(i);
		real error=fabs(compare[i]-v);
		// Calculate absolute error tolerance as: epsilon + |(x_p+x_(n+1))/2|*epsilon
		real errortol=Epsilon + fabs(v)*Epsilon;
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

	if (get_system().size() == 0)
		throw std::out_of_range("No states to integrate!");
	// h is standard name in numeric for time step size
	Time h = std::min({ MaxTime - m_t, timestep.long_time_if_zero(), (m_dt * 2).long_time_if_zero() });

	// Copies the actual states to the history as x_(n)
	get_system().copy_states(pastStates(0));
	// Count the iterations
	int iter=0;
	real old_err_ex=REAL_MAX;
	real err_ex=REAL_MAX/2;
	m_Iterations=0;
	do 
	{
		// Remember the current state for convergence criterion
		get_system().copy_states(compareStates);
		// Get derivatives at t(n+1)
		get_system().copy_dxdt(this->get_t() + h,dxdt);
		// Updates the state variables with the new states, according to the current order
		(this->*(calc_newState[order-1]))(h.AsDays());


		old_err_ex=err_ex;
		err_ex=error_excedance(compareStates, &error_position);


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
			get_system().set_states(pastStates(0));

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

void cmf::math::BDF2::reset() {
	order=1;
	stepNo = 0;
	ptrdiff_t size = get_system().size();
	if (size != pastStates(0).size() ||
		size != pastStates(1).size() ||
		size != compareStates.size() ||
		size != dxdt.size()) {

		pastStates(0).resize(size);
		pastStates(1).resize(size);
		compareStates.resize(size);
		dxdt.resize(size);
	}
	abstol.resize(ptrdiff_t(get_system().size()));
	ODEsystem& system = get_system();
	for (size_t i = 0; i < get_system().size(); ++i)
		abstol[i] = system[i]->get_abs_errtol(Epsilon * 1e-3);

}

#undef max
