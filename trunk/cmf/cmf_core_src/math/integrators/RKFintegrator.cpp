

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
#include "RKFintegrator.h"
#ifdef _OPENMP
#include <omp.h>
#endif
using namespace std;
cmf::math::RKFIntegrator::RKFIntegrator(StateVariableOwner& states, real epsilon/*=1e-9*/,cmf::math::Time _dt_min/*=10.0/(3600.0*24.0)*/ ) : 
Integrator(states,epsilon),oldStates(m_States.size()),dt_min(_dt_min)
{	
	for (int i = 0; i < 6 ; i++)
		kValues[i]= num_array(size());
}
cmf::math::RKFIntegrator::RKFIntegrator( real epsilon/*=1e-9*/,cmf::math::Time _dt_min/*=10.0/(3600.0*24.0)*/ ) : 
Integrator(epsilon), dt_min(_dt_min)
{
	
}

cmf::math::RKFIntegrator::RKFIntegrator( const cmf::math::RKFIntegrator& forCopy ) 
: Integrator(forCopy),dt_min(forCopy.dt_min)
{
	
}
void cmf::math::RKFIntegrator::add_states( cmf::math::StateVariableOwner& stateOwner )
{
	Integrator::add_states(stateOwner);
	oldStates.resize(size());
	for (int i = 0; i < 6 ; i++)
		kValues[i].resize(size());
}
	const real 
// Runge-Kutta-Fehlberg formula of 4-th order
cmf::math::RKFIntegrator::c[6] = { 0.0, 1.0/4.0,  3.0/8.0,  12.0/13.0,  1.0,  1.0/2.0 },
// Runge-Kutta-Fehlberg weights of the k-Values
cmf::math::RKFIntegrator::A[6][5] = { 
	{  0.0,            0.0,           0.0,            0.0,            0.0       },
	{  1.0/4.0,        0.0,           0.0,            0.0,            0.0       },
	{  3.0/32.0,       9.0/32.0,      0.0,            0.0,            0.0       },
	{  1932.0/2197.0, -7200.0/2197.0, 7296.0/2197.0,  0.0,            0.0       },
	{  439.0/216.0,   -8.0,           3680.0/513.0,  -845.0/4104.0,   0.0       },
	{ -8.0/27.0,       2.0,          -3544.0/2565.0,  1859.0/4104.0, -11.0/40.0 }
},
// weight of intermediate solutions for the end solution (RK5)
cmf::math::RKFIntegrator::b5[6] = { 16.0/135.0, 0.0, 6656.0/12825.0, 28561.0/56430.0, -9.0/50.0, 2.0/55.0 },
// local error term
cmf::math::RKFIntegrator::e[6] = { -1.0/360.0,  0.0, 128.0/4275.0, 2197.0/75240.0, -1.0/50.0, -2.0/55.0 };

//Returns the proposed timestep for the next integration

int cmf::math::RKFIntegrator::integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
{
	if (m_States.size()==0)
		throw std::out_of_range("No states to integrate!");

	if (TimeStep<dt_min)
		TimeStep=dt_min;
	//If we are nearly at the maximum time, we do a (small) Euler step)
	if (MaxTime - m_t < dt_min)
	{
		//Use as time step the difference between MaxTime and Time
		m_dt = MaxTime - m_t;
		//Calculate new states with an eulerian step : x(t+h) = x(t) + timeStep * dx/dt
		// Copy derivates multipied with time step to dxdt
		copy_dxdt(get_t(),kValues[0],TimeStep.AsDays());
		// Update time step with delta x
		add_values_to_states(kValues[0]);

		//Step to MaxTime
		m_t = MaxTime;
		
	  return 1;
	}
	//Trying the complete timestep
	//Adjust the timestep to avoid the eularian solution above
	AdjustTimestep(TimeStep,MaxTime);
	copy_states(oldStates);


	//h is the normal name of the variable TimeStep
	Time h=TimeStep;

	//The global error between the RK5 and RK4 method
	real error=0;
	int Loops=0;
	//While the error between RK5 and RK4 is greater than the maximum error
	do
	{
		error=0;
		//Calculate k[0] (the current derivate)	
		
		copy_dxdt(m_t,k(0));

		//For each higher order of the Runge-Kutta integrator
		for (int order = 1; order <= 5 ; order++)
		{
			//Update the state variables with the A-weighted k-Values
			real kSum=0;
			int m;
#pragma omp parallel for private(kSum,m) firstprivate(order)
			for (int i = 0; i < size() ; i++)
			{
				//Weighted sum of k-values
				//Sum up the k-Values weighted by the j'th row
				kSum=0.0;
				for (m = 0; m < order ; m++)
					kSum += A[order][m] * k(order,i);

				//Update the state value for that order, to calculate the next k value
				set_state(i,kSum * h.AsDays() + oldStates[i]); 
			} //Done with system equations
			const Time subTimestep = m_t + h * c[order];
			//For each differential equation of the system, calculate the derivate from the new state
			copy_dxdt(subTimestep,k(order));
		} // next order

		//Sum up the k - Values to obtain the final state for the 5th order RK-solution
		//and the local error
			//Weighted sum of k-values
			real kSum=0;
			real localError=0;
#pragma omp parallel for private(kSum,localError) shared(error)
		for (int i = 0; i < size() ; i++)
		{
			kSum=0;
			localError=0;
			//Sum up the k-Values weighted by the j'th row
			for (unsigned int order = 0; order < 6 ; order++)
			{
				kSum += b5[order] * k(order,i);
				localError += e[order] * k(order,i);
			}
			//Get the absolute of the local error
			localError = fabs(localError);
			//Update the state value for that order
			set_state(i,kSum * h.AsDays() + oldStates[i]);
			//Calculates the local error weighted with h
			if (localError>error)
			{
#pragma omp critical
				if (localError>error)
				{
					error=localError;
				}
			}
		} //Done with system equations

		//Calculate the new timestep

		//Remember the time step h as the last used time step
		TimeStep = h;

		//Scale factor for the new timestep, shouldn't be greater than 5
		real s=5;
		if (error>Epsilon * pow(s,-4)/2) //If the error leads to a smaller scale factor than the initial one
			s = pow(real(Epsilon / (2 * error)), real(1.0 / 4.0));
		//Also the new time step should not be smaller than 10% of the old
		s = s < 0.1 ? 0.1 : s;
		//Last but not least, no timesteps smaller than minTimestep are allowed, 
		//maybe in this case the model should exit with an error "No stable solution possible"
		h = h*s < dt_min ? dt_min : h*s;
		//Count the number of tries
		Loops++;
		//If we have to try again:
		if (error > Epsilon && Loops<100 && TimeStep>dt_min)
		{
			//Reset the states to the old states, and try again with the new time step
			set_states(oldStates);
		}
		//In case of a good solution or if no solution after 100 tries is found, we exit the trying
	} while (error > Epsilon && Loops<100 && TimeStep>dt_min);
	// A stable solution for the timestep TimeStep is found, h is the proposal for the next time step
	//Add the last timestep to the system time
	m_t +=TimeStep;
	m_dt=TimeStep;
	//return the proposed time step for the next integration
	++m_Iterations;
	return Loops;

}


