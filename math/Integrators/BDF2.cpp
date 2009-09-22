#include "BDF2.h"
#include <cmath>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <iostream>
// Just a macro to compare two values
#define min(a,b) (((a)<(b)) ? (a) : (b))

// Creates a new Integrator w states
cmf::math::BDF2::BDF2(const StateVariableVector& states, real epsilon/*=1e-9*/,cmf::math::Time tStepMin/*=Time::Seconds(10)*/ )
: Integrator(states,epsilon,tStepMin),	compareStates(m_States.size()),	dxdt(m_States.size()),order(1),stepNo(0)
{

	// Assessing multistep functions
	calc_newState[0] = &cmf::math::BDF2::Gear1newState; // impl. Euler
	calc_newState[1] = &cmf::math::BDF2::Gear2newState; // 2nd order Gear

}

// Creates a new Integrator w/o states
cmf::math::BDF2::BDF2( real epsilon/*=1e-9*/,cmf::math::Time tStepMin/*=Time::Seconds(10)*/) 
: Integrator(epsilon,tStepMin),	order(1),stepNo(0)
{ 
	// Assessing multistep functions
	calc_newState[0] = &cmf::math::BDF2::Gear1newState; // impl. Euler
	calc_newState[1] = &cmf::math::BDF2::Gear2newState; // 2nd order Gear
}

cmf::math::BDF2::BDF2( const Integrator& templ) :
cmf::math::Integrator(templ),order(1),stepNo(0)
{

	// Assessing multistep functions
	calc_newState[0] = &cmf::math::BDF2::Gear1newState; // impl. Euler
	calc_newState[1] = &cmf::math::BDF2::Gear2newState; // 2nd order Gear
}

cmf::math::BDF2::BDF2( cmf::math::StateVariableOwner& states, real epsilon/*=1e-9*/,cmf::math::Time tStepMin/*=Time::Milliseconds(10)*/ )
 : Integrator(states,epsilon,tStepMin), order(1),stepNo(0)
{
	// Assessing multistep functions
	calc_newState[0] = &cmf::math::BDF2::Gear1newState; // impl. Euler
	calc_newState[1] = &cmf::math::BDF2::Gear2newState; // 2nd order Gear

}
// Adds states to the vector of states
void cmf::math::BDF2::AddStatesFromOwner( cmf::math::StateVariableOwner& stateOwner )
{
	// Call base class function
	Integrator::AddStatesFromOwner(stateOwner);
	// Resize helper vectors (convergence check,derivatives and history)
	compareStates.resize(count());
	dxdt.resize(count());
	for (int i = 0; i < 2 ; i++)
		pastStatesArray[i].resize(count());
}


// Gear formulas
// First order: newState = statevector + h*dxdt (impl. Euler)
void cmf::math::BDF2::Gear1newState( real h )
{
	real state_i;
	if (use_OpenMP)
	{
#pragma omp parallel for private(state_i)
		for (int i = 0; i < count() ; i++)
		{
			// The formula is written so ugly to avoid internal memory allocation
			// x_n+1 = x_(n) + h dxdt
			state_i  =       dxdt[i]; 
			state_i *= h; 
			state_i +=       pastStates(0)[i];
			state(i, state_i);
		}
	}
	else
	{
		for (int i = 0; i < count() ; i++)
		{
			// The formula is written so ugly to avoid internal memory allocation
			// x_n+1 = x_(n) + h dxdt
			state_i  =       dxdt[i]; 
			state_i *= h; 
			state_i +=       pastStates(0)[i];
			state(i, state_i);
		}

	}
}

// Second order
void cmf::math::BDF2::Gear2newState(real h)
{
	real state_i;
	const real 
		// p is the relative change of time step size
		p    = h/TimeStep().AsDays(),
		p1   = 1 + p,
		h_p1 = h * p1,
		p1_2 = p1 * p1,
		p_2  = p * p;
	if (use_OpenMP)
	{
	#pragma omp parallel for private(state_i)
		for (int i = 0; i < count() ; i++)
		{
			// The formula is written so ugly to avoid internal memory allocation
			// x_(n+1) = (p+1)²x_(n) - p²x_(n-1) + h (p+1) dxdt
			state_i  =        dxdt[i]; 
			state_i *= h_p1;
			state_i += p1_2 * pastStates(0)[i];
			state_i -= p_2  * pastStates(1)[i];
			state_i /= 1.0 + 2.0*p;
			state(i, state_i);
		}
	}
	else
	{
		for (int i = 0; i < count() ; i++)
		{
			// The formula is written so ugly to avoid internal memory allocation
			// x_(n+1) = (p+1)²x_(n) - p²x_(n-1) + h (p+1) dxdt
			state_i  =        dxdt[i]; 
			state_i *= h_p1;
			state_i += p1_2 * pastStates(0)[i];
			state_i -= p_2  * pastStates(1)[i];
			state_i /= 1.0 + 2.0*p;
			state(i, state_i);
		}
	}

}




int cmf::math::BDF2::Integrate( cmf::math::Time MaxTime,cmf::math::Time timestep )
{
	// h is standard name in numeric for time step size
	Time h=MaxTime-ModelTime();
	// Don't stretch the current timestep more the 2 times the last timestep
	if (h>TimeStep()*2) 
	{
		h=TimeStep()*2;
	}

	// Copies the actual states to the history as x_(n)
	States().CopyStates(pastStates(0));
	// Count the iterations
	int iter=0;
	real old_err_ex=REAL_MAX;
	real err_ex=REAL_MAX/2;
	m_Iterations=0;
	do 
	{
		// Remember the current state for convergence criterion
		States().CopyStates(compareStates);
		// Get derivatives at t(n+1)
		States().CopyDerivs(this->ModelTime() + h,dxdt);
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
			if (h<MinTimestep())
			{
				if (UseEulerAtTmin) // If an explicit euler step is to be taken at Tmin, do it
				{
					h=MinTimestep();
					std::cerr << "No convergence! Time=" << ModelTime().AsDate().ToString() << " Euler step taken for " << h.ToString() << std::endl;
					dxdt*=h.AsDays(); 
					States() += dxdt;
					order=1;
					break;
				}
				else
				{
					std::cerr << "No convergence! Time=" << ModelTime().AsDate().ToString() << " Iter: " << iter << " StateV No." << error_position << std::endl;
					throw std::runtime_error("No convergence with a time step > minimal time step");
				}
			}
			// Restore states
			States().SetStates(pastStates(0));

			iter=0;
			err_ex=REAL_MAX/2;
			old_err_ex=REAL_MAX;
		}

		// Loop until the iterations converge
	} while(err_ex > 1);

	m_TimeStep=h;
	
	// If we're just using this integrator to bootstrap a
	// higher-order method, arrange to transfer control to 
	// the next order method.
	if (order<2) ++order;
	// Raise the number of steps
	++stepNo;
	ModelTime(ModelTime() + h);
	return iter;
}

#undef max
