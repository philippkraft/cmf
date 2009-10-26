#include "Gears_Fixpoint.h"
#include <cmath>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <iostream>
#define min(a,b) (((a)<(b)) ? (a) : (b))
// Creates a new Integrator w states
cmf::math::Gears_Fixpoint::Gears_Fixpoint(const StateVariableVector& states, real epsilon/*=1e-9*/,cmf::math::Time tStepMin/*=Time::Seconds(10)*/ ,int maxOrder)
: Integrator(states,epsilon,tStepMin),	compareStates(m_States.size()),	dxdt(m_States.size()),order(1),max_order(maxOrder),stepNo(0),time_reduce_stage(0)
{

	// Assessing multistep functions
	calc_newState[0] = &cmf::math::Gears_Fixpoint::Gear1newState; // impl. Euler
	calc_newState[1] = &cmf::math::Gears_Fixpoint::Gear2newState; // 2nd order Gear
	calc_newState[2] = &cmf::math::Gears_Fixpoint::Gear3newState; // 3rd order Gear
	calc_newState[3] = &cmf::math::Gears_Fixpoint::Gear4newState; // 4th order Gear
	calc_newState[4] = &cmf::math::Gears_Fixpoint::Gear5newState; // 5th order Gear

}

// Creates a new Integrator w/o states
cmf::math::Gears_Fixpoint::Gears_Fixpoint( real epsilon/*=1e-9*/,cmf::math::Time tStepMin/*=Time::Seconds(10)*/,int maxOrder) 
: Integrator(epsilon,tStepMin),	order(1),max_order(maxOrder),stepNo(0),time_reduce_stage(0)
{ 
	
	// Assessing multistep functions
	calc_newState[0] = &cmf::math::Gears_Fixpoint::Gear1newState; // impl. Euler
	calc_newState[1] = &cmf::math::Gears_Fixpoint::Gear2newState; // 2nd order Gear
	calc_newState[2] = &cmf::math::Gears_Fixpoint::Gear3newState; // 3rd order Gear
	calc_newState[3] = &cmf::math::Gears_Fixpoint::Gear4newState; // 4th order Gear
	calc_newState[4] = &cmf::math::Gears_Fixpoint::Gear5newState; // 5th order Gear

}

cmf::math::Gears_Fixpoint::Gears_Fixpoint( const cmf::math::Integrator& templ) :cmf::math::Integrator(templ), max_order(4),order(1),stepNo(0),time_reduce_stage(0)
{
	const Gears_Fixpoint* pIntegrator = dynamic_cast<const Gears_Fixpoint *>(&templ);
	if(pIntegrator)
		max_order=pIntegrator->max_order;

	// Assessing multistep functions
	calc_newState[0] = &cmf::math::Gears_Fixpoint::Gear1newState; // impl. Euler
	calc_newState[1] = &cmf::math::Gears_Fixpoint::Gear2newState; // 2nd order Gear
	calc_newState[2] = &cmf::math::Gears_Fixpoint::Gear3newState; // 3rd order Gear
	calc_newState[3] = &cmf::math::Gears_Fixpoint::Gear4newState; // 4th order Gear
	calc_newState[4] = &cmf::math::Gears_Fixpoint::Gear5newState; // 5th order Gear
 
}

// Adds states to the vector of states
void cmf::math::Gears_Fixpoint::AddStatesFromOwner( cmf::math::StateVariableOwner& stateOwner )
{
	// Call base class function
	Integrator::AddStatesFromOwner(stateOwner);
	// Resize helper vectors (convergence check,derivatives and history)
	compareStates.resize(count());
	dxdt.resize(count());
	for (int i = 0; i < max_order ; i++)
		pastStatesArray[i].resize(count());
}

// Gear formulas
// First order: newState = statevector + h*dxdt (impl. Euler)
void cmf::math::Gears_Fixpoint::Gear1newState(real h)
{
	real state_i;
#pragma omp parallel for 
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

// Second order
void cmf::math::Gears_Fixpoint::Gear2newState(real h)
{
	real state_i;
	const real h2=h * 2.0;
#pragma omp parallel for 
	for (int i = 0; i < count() ; i++)
	{
		// The formula is written so ugly to avoid internal memory allocation
		// x_n+1 = (4x_(n) - x_(n-1) + 2 h dxdt) / 3
		state_i  =       dxdt[i]; 
		state_i *= h2; 
		state_i += 4.0 * pastStates(0)[i];
		state_i -=       pastStates(1)[i];
		state_i /= 3.0;
		state(i, state_i);
	}
}

// Third order: 
void cmf::math::Gears_Fixpoint::Gear3newState(real h)
{
	real state_i;
	const real h6=h*6.0;
#pragma omp parallel for 
	for (int i = 0; i < count() ; i++)
	{
		// The formula is written so ugly to avoid internal memory allocation
		// x_n+1 = (18x_(n) - 9x_(n-1) + 2x_(n-2) + 6 h dxdt)/11
		state_i  =      pastStates(0)[i]; 
		state_i *= 2.0; 
		state_i -=      pastStates(1)[i];
		state_i *= 9.0; 
		state_i += 2  * pastStates(2)[i];
	  state_i += h6 * dxdt[i];
		state_i /=11.0;
		state(i, state_i);
	}
}
// Fourth order:
void cmf::math::Gears_Fixpoint::Gear4newState(real h)
{
	real state_i;
	const real h12=h * 12.0;
	#pragma omp parallel for 
	for (int i = 0; i < count() ; i++)
	{
		// The formula is written so ugly to avoid internal memory allocation
		// x_n+1 = (48x_(n) - 36x_(n-1) + 16x_(n-2) - 3x_(n-3) + 12 h dxdt)/25
		state_i  =        pastStates(0)[i]; 
		state_i *= 3.0;
		state_i +=        pastStates(2)[i];
		state_i *= 16.0; 
		state_i -= 36.0 * pastStates(1)[i];
		state_i -=  3.0 * pastStates(3)[i]; 
		state_i +=  h12 * dxdt[i];
		state_i /= 25.0;
		state(i, state_i);
	}
}
void cmf::math::Gears_Fixpoint::Gear5newState(real h)
{
	real state_i;
	const real h60=h * 60.0;
#pragma omp parallel for 
	for (int i = 0; i < count() ; i++)
	{
		// The formula is written so ugly to avoid internal memory allocation
		// x_n+1 = (48x_(n) - 36x_(n-1) + 16x_(n-2) - 3x_(n-3) + 12 h dxdt)/25
		state_i  =        pastStates(0)[i]; 
		state_i -=        pastStates(1)[i]; 
		state_i *= 1.5;
		state_i +=        pastStates(2)[i];
		state_i *= 200.0; 
		state_i -= 75.0 * pastStates(3)[i];
		state_i += 12.0 * pastStates(4)[i]; 
		state_i +=  h60 * dxdt[i];
		state_i /= 137.0;
		state(i, state_i);
	}
}



int cmf::math::Gears_Fixpoint::Integrate( cmf::math::Time MaxTime,cmf::math::Time timestep )
{
	// h is standard name in numeric for time step size
	Time h=timestep;
	// If timestep has to be reduced for sync purposes
	if (ModelTime()+h>MaxTime) 
	{
		// Set time step to the difference between now and the sync point
		h=MaxTime-ModelTime();
		// Debugging message
		std::cout << Tag << " - Time step change from Maxtime";
	}
	
	// For the next timestep use the given one, discard sync point reduction
	m_NextTimeStep=timestep;

	// If the time step has changed
	if (h!=this->TimeStep())
	{
		// Debugging message
		std::cout << Tag << " - New step size: " << h.to_string() << ", reduce stage: " << time_reduce_stage << std::endl;
		// Set current order to 1 (impl. Euler, no history)
		order=1;
		// Set number of steps with unchanged time step to zero
		stepNo=0;
		// Save the time step as the used time step
		m_TimeStep=h;
	}
	// Copies the actual states to the history as x_(n)
	States().CopyStates(pastStates(0));
	// Count the iterations
	int iter=0;
	real old_err_ex=REAL_MAX;
	real err_ex=REAL_MAX;
	do 
	{
		// Remember the current state for convergence criterion
		States().CopyStates(compareStates);
		// Get derivatives at t(n+1)
		States().CopyDerivs(this->ModelTime() + h,dxdt);
		// Updates the state variables with the new states, according to the current order
		(this->*(calc_newState[order-1]))(h.AsDays());


		old_err_ex=err_ex;
		err_ex=max_rel_error_exceedance(compareStates);


		// Count number of iterations
		++iter;
		

		// If the number of iterations exceeds the allowed number, reduce step size
		if (iter>GEAR_MAX_ITER || err_ex>old_err_ex)
		{
			// Step size reduction
			m_NextTimeStep /= 2;
			// Remember stage of reduction (for recovery)
			++time_reduce_stage;

			// If the time step becomes too small, throw exception
			if (m_NextTimeStep<MinTimestep())
			{
				throw std::runtime_error("No convergence with a time step > minimal time step");
			}
			// Restore states
#pragma omp parallel for
			for (int i = 0; i < count() ; i++)
				state(i,pastStates(0)[i]);
			// Leave function
			return iter;
			// The model time has not changed!
		}

		// Loop until the iterations converge
	} while(err_ex > 1);


	// If the model has run for a while with the current reduced step size, make the step size bigger again
	if (stepNo>GEAR_RECOVER_AFTER_STEPS && time_reduce_stage && ModelTime()==MaxTime)
	{
		m_NextTimeStep *=min(pow(2.0,time_reduce_stage),8);
		time_reduce_stage-=min(time_reduce_stage,3);
	}


	// If we're just using this integrator to bootstrap a
	// higher-order method, arrange to transfer control to 
	// the next order method.
	if (max_order > order) ++order;
  // Raise the number of steps
	++stepNo;
	ModelTime(ModelTime() + h);
	return iter;
}


// Test for convergence
bool cmf::math::Gears_Fixpoint::approx_equal_to_states( const cmf::math::num_array& compare )
{
	bool res=1;
#pragma omp parallel
	{
#pragma omp for reduction(&& : res)
		for (int i = 0; i < count() ; i++)
		{
			res = res && abs(compare[i]-state(i)) <= Epsilon + (compare[i]+state(i))*0.5*Epsilon;
		}
	}
	return res;
}

// Calculate the maximum relative error exceedance of the current iteration
real cmf::math::Gears_Fixpoint::max_rel_error_exceedance(const cmf::math::num_array& compare)
{
  real res=0;
	for (int i = 0; i < count() ; i++)
	{
		real error=abs(compare[i]-state(i));
		// Calculate absolute error tolerance as: epsilon + |(x_p+x_(n+1))/2|*epsilon
		real errortol=Epsilon + abs(compare[i]+state(i))*0.5*Epsilon;
		if (error/errortol>res)
			res=error/errortol;
	}
	return res;
}

cmf::math::Gears_Fixpoint::~Gears_Fixpoint()
{
}

