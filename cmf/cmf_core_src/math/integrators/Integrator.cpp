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

cmf::math::Integrator::Integrator(const StateVariableList &states, real epsilon)
        : m_States(),Epsilon(epsilon),m_dt(day),m_t(day*0),
          use_OpenMP(true), reset_integratables(true)
{
    this->add_states(states);
}

cmf::math::Integrator::Integrator(const cmf::math::Integrator &other)
		: m_States(), Epsilon(other.Epsilon), m_dt(other.m_dt), m_t(other.m_t),
		  reset_integratables(other.reset_integratables), use_OpenMP(other.use_OpenMP)
{}

void cmf::math::Integrator::add_states(const cmf::math::StateVariableList &states) {
	m_States.insert(m_States.end(), states.begin(), states.end());
}

cmf::math::StateVariable::ptr cmf::math::Integrator::operator[](ptrdiff_t position) {
	if (position < 0) position += m_States.size();
	if (position < m_States.size()) return m_States[position];
	throw std::out_of_range("state not in integrator");
}

cmf::math::num_array cmf::math::Integrator::get_dxdt(cmf::math::Time time) const {
	num_array result(this->size());
	copy_dxdt(time,result);
	return result;
}

cmf::math::num_array cmf::math::Integrator::get_state_values() const {
	num_array result(this->size());
	copy_states(result);
	return result;
}
