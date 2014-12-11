#include "RK23integrator.h"

#include <cmath>
#include <sstream>
// #include "AdaptiveTimeStepInfo.h"

using namespace cmf::math;

// Butcher tableau, reference:
// P. Bogacki, L.F. Shampine: A 3(2) Pair of Runge-Kutta Formulas
// Appl. Math. Lett. (1989), Vol. 2 (4), pp. 321-325
// DOI: http://dx.doi.org/10.1016/0893-9659(89)90079-7

//                  c1 = 0.0
const real RK23Integrator::c2 = 1./2.,
           RK23Integrator::c3 = 3./4.,
           RK23Integrator::c4 = 1.;

const real RK23Integrator::a21 = 1./2.,
           //       a31 = 0.,
           RK23Integrator::a32 = 3./4.,
           RK23Integrator::a41 = 2./9.,
           RK23Integrator::a42 = 1./3.,
           RK23Integrator::a43 = 4./9.;

// 3rd order method:
// first same as last (FSAL), i.e. a4i = bi

// 2nd order embedded method
const real RK23Integrator::b1e = 7./24.,
           RK23Integrator::b2e = 1./ 4.,
           RK23Integrator::b3e = 1./ 3.,
           RK23Integrator::b4e = 1./ 8.;

// step size control parameters
const double cmf::math::RK23Integrator::fac     = 0.9;
const double cmf::math::RK23Integrator::facmax_ = 4.0;
const double cmf::math::RK23Integrator::facmin  = 0.125; // = 1/8

RK23Integrator::RK23Integrator(StateVariableOwner& states,
                 double abstol/*=1e-6*/, double reltol/*=1e-6*/,
                 Time dt_min/*=cmf::math::sec*/)
	: Integrator(states, reltol), abstol(abstol), reltol(reltol), dt_min(dt_min),
	  facmax(facmax_), initialized(false)
{}

cmf::math::RK23Integrator::RK23Integrator( const cmf::math::RK23Integrator& other )
	: Integrator(other), abstol(other.abstol),reltol(other.reltol), dt_min(dt_min),
	facmax(other.facmax), initialized(false)

{}

void cmf::math::RK23Integrator::reset()
{
	initialized = false;
}


RK23Integrator::~RK23Integrator()
{}

void RK23Integrator::add_states(cmf::math::StateVariableOwner& stateOwner) {
	// Call base class function
	Integrator::add_states(stateOwner);
	// Resize helper vectors (convergence check,derivatives and history)
	k1.resize(size());
	k2.resize(size());
	k3.resize(size());
	k4.resize(size());
	old_states.resize(size());
	y.resize(size());
	y_e.resize(size());
	initialized = false;
}

int RK23Integrator::integrate(cmf::math::Time t_max, cmf::math::Time dt)
{
	// no states => no need to integrate
	if (m_States.empty())
		return 0;

	// choose appropriate step size with respect to m_t and t_max
	//m_dt = tau * math::day;
	AdjustTimestep(m_dt, t_max);
	double tau = m_dt.AsDays();


	bool accept  = false;

	if (!initialized)
	// this function is called for the first time
	{
#ifdef MPI
		update_ghost_states();
#endif
		copy_states(old_states);
		copy_dxdt(m_t, k4);
		initialized = true;
	}
	else
	// not the first call to this function
	{
		// y holds the last state
		old_states.set(y);
	}

	// first same as last (FSAL):
	// k4 of the last step equals k1 of the new step
	k1.set(k4);

	// loop until an appropriate step size has been found
	do {
		// second stage
	    y.set (a21, k1);
		y.scale_add(tau, old_states);
		set_states(y);
#ifdef MPI
		update_ghost_states();
#endif
		copy_dxdt(m_t + c2 * m_dt, k2);

		// third stage
		// a31 = 0.0
		y.set (a32, k2);
		y.scale_add(tau, old_states);
		set_states(y);
#ifdef MPI
		update_ghost_states();
#endif
		copy_dxdt(m_t + c3 * m_dt, k3);

		// result of 3rd order method:
		// first same as last (FSAL), i.e. a4i = bi
		y.set (a41, k1);
		y.axpy(a42, k2);
		y.axpy(a43, k3);
		y.scale_add(tau, old_states);

		// fourth stage
		set_states(y);
#ifdef MPI
		update_ghost_states();
#endif
		copy_dxdt(m_t + c4 * m_dt, k4);

		// result of 2nd order embedded method
		y_e.set (b1e, k1);
		y_e.axpy(b2e, k2);
		y_e.axpy(b3e, k3);
		y_e.axpy(b4e, k4);
		y_e.scale_add(tau, old_states);

		// estimate error
		const double err = estimate_error();

		// check for NaN
		if (err!=err)
		{
			tau *= 0.5;
			m_dt = tau * math::day; // may go below dt_min
			continue;
		}

		double step_factor = fac * pow(1.0/err, 1.0/3.0);
		step_factor = std::min(facmax, step_factor);
		step_factor = std::max(facmin, step_factor);

	    // accept if error is small enough or if minimum step size is reached
		accept = (err <= 1.0) || (m_dt <= dt_min);

		if (accept)
		{
	        // accept state y: nothing to do, since y was already set
		    // increment time with accepted step
			m_t += m_dt;

	        // don't decrease if we already hit dt_min
		    if (m_dt <= dt_min) step_factor = std::max(step_factor, 1.0);

			// predict next step
			m_dt *= step_factor;

			// if previous step was rejected, facmax was modified
			facmax = facmax_;

			// finish current time step info object
        
		}
		else
		{
			 //if (rank == 0) std::cout << "reject with m_dt = " << m_dt << " and err = " << err << "\n";


			// predict next step to try
			// assert(step_factor < 1.0);
			m_dt *= step_factor;
			m_dt = std::max(m_dt, dt_min); // don't go below dt_min
			tau = m_dt.AsDays();

			// don't increase too much after rejection
			facmax = 2.0;

	    }

    } while(!accept);

	return 0;
}


double RK23Integrator::estimate_error()
{
  double local_a = 0.0, local_b = 0.0;
  double a = 0.0, b = 0.0, help = 0.0;
#ifndef MPI
  ptrdiff_t nb_local_states = y.size();
  ptrdiff_t nb_global_states = y.size();
#endif
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for private(help) reduction(+:local_a) reduction(+:local_b)
  for (int i = 0; i < nb_local_states; ++i)
  {
    help = y[i].getValue();
    local_b += help * help;

    help -= y_e[i].getValue();
    local_a += help * help;
  }
#else
#pragma omp parallel for private(help) reduction(+:local_a) reduction(+:local_b)
  for (int i = 0; i < nb_local_states; ++i)
  {
    help = y[i];
    local_b += help * help;

    help -= y_e[i];
    local_a += help * help;
  }
#endif
#ifdef MPI
  MPI_Allreduce(&local_a, &a, 1, MPI_DOUBLE, MPI_SUM, comm);
  MPI_Allreduce(&local_b, &b, 1, MPI_DOUBLE, MPI_SUM, comm);
//  if (rank == 0) std::cout << "a = " << a << " | b = " << b << "\n";
#else
  a = local_a;
  b = local_b;
#endif
  return sqrt(a / ((b * reltol + abstol) * static_cast<double>(nb_global_states)));


}



