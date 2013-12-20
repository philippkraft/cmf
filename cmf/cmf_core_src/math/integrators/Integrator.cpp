#include "integrator.h"
#ifdef _OPENMP
#include <omp.h>
#endif


void cmf::math::Integrator::copy_states( num_array & destination ) const
{
	if (destination.size()!=(ptrdiff_t)size())
		destination.resize(size());
	if (use_OpenMP)
	{
#pragma omp parallel for
		for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
		{
			destination[i]=get_state(i);
		}
	}
	else
	{
		for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
		{
			destination[i]=get_state(i);
		}
	}

}

void cmf::math::Integrator::copy_states( real * destination ) const
{
	if (use_OpenMP)
	{
#pragma omp parallel for
		for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
		{
			destination[i]=get_state(i);
		}
	}
	else
	{
		for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
		{
			destination[i]=get_state(i);
		}

	}
}
void cmf::math::Integrator::copy_dxdt( Time time,num_array & destination,real factor ) const
{
	if (destination.size()!=(ptrdiff_t)size())
		destination.resize(size());
	std::string err_msg;
	bool err=false;
	if (use_OpenMP)
	{
		if (factor==1)	{
			#pragma omp parallel for schedule(guided)
			for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i) {
				if (!err)  
				{
					try {
						#pragma omp flush(err)
						destination[i]=m_States[i]->dxdt(time);
					} catch(std::exception& e) {
						#pragma omp critical 
						{
							err_msg = e.what();
							err = true;
							#pragma omp flush(err)
						}
					}
				}
			}
		} else {
			#pragma omp parallel for schedule(guided)
			for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
			{
				#pragma omp flush(err)
				if (!err)  {
					try {
						destination[i]  = m_States[i]->dxdt(time);
						destination[i] *= factor;
					} catch(std::exception& e) {
						#pragma omp critical
						err_msg = e.what();
						err = true;
						#pragma omp flush(err)
					}
				}
			}
		}
		if (err) {
			throw std::runtime_error(err_msg);
		}
	} else {
		if (factor==1)
		{
			for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
				destination[i]=m_States[i]->dxdt(time);
		}
		else
		{
			for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
			{
				destination[i]=m_States[i]->dxdt(time);
				destination[i]*=factor;
			}
		}
	}
}

void cmf::math::Integrator::copy_dxdt( Time time,real * destination,real factor/*=1*/ ) const
{
	std::string err_msg;
	bool err=false;
	if (use_OpenMP)		{
		if (factor==1)	{
			#pragma omp parallel for schedule(guided)
			for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i) {
				if (!err)  
				{
					try {
						#pragma omp flush(err)
						destination[i]=m_States[i]->dxdt(time);
					} catch(std::exception& e) {
						#pragma omp critical 
						{
							err_msg = e.what();
							err = true;
							#pragma omp flush(err)
						}
					}
				}
			}
		} else {
			#pragma omp parallel for schedule(guided)
			for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
			{
				#pragma omp flush(err)
				if (!err)  {
					try {
						destination[i]  = m_States[i]->dxdt(time);
						destination[i] *= factor;
					} catch(std::exception& e) {
						#pragma omp critical
						err_msg = e.what();
						err = true;
						#pragma omp flush(err)
					}
				}
			}
		}
		if (err) {
			throw std::runtime_error(err_msg);
		}
	}	else	{ // use_OpenMP=false
		if (factor==1)	{
			for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
				destination[i]=m_States[i]->dxdt(time);
		} else {
			for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)	{
				destination[i]=m_States[i]->dxdt(time);
				destination[i]*=factor;
			}
		}
	}
}
void cmf::math::Integrator::set_states(const num_array & newStates )
{
	if (use_OpenMP)
	{
#pragma omp parallel for
		for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
		{
			m_States[i]->set_state(newStates[i]);
		}
	}
	else
	{
		for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
		{
			m_States[i]->set_state(newStates[i]);
		}
	}

}

void cmf::math::Integrator::set_states( real * newStates )
{
	if (use_OpenMP)
	{
#pragma omp parallel for
		for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
		{
			m_States[i]->set_state(newStates[i]);
		}
	}
	else
	{
		for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
		{
			m_States[i]->set_state(newStates[i]);
		}
	}
}

void cmf::math::Integrator::add_values_to_states(const num_array& operands)
{
	if (use_OpenMP)
	{
#pragma omp parallel for
		for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
		{
			m_States[i]->set_state(m_States[i]->get_state() + operands[i]);		
		}
	}
	else
	{
		for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
		{
			m_States[i]->set_state(m_States[i]->get_state() + operands[i]);		
		}
	}
}

void cmf::math::Integrator::integrate_until( cmf::math::Time t_max,cmf::math::Time dt/*=Time()*/,bool reset_solver/*=false*/ )
{
	m_Iterations=0;
	int i=0;
	Time start = m_t;
	if (reset_solver) reset();
	if (!dt) dt=m_dt;
	if (reset_integratables) integratables.reset(start);
	while (m_t < t_max) {
		
		integrate(t_max,dt);
		integratables.integrate(m_t);
		++i;
	}
	if (i>0) m_dt = (t_max - start)/i;
}
