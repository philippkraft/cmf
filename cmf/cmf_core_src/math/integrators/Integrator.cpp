#include "integrator.h"



void cmf::math::Integrator::copy_states( num_array & destination ) const
{
	if (destination.size()!=(int)size())
		destination.resize(size());
	if (use_OpenMP)
	{
#pragma omp parallel for
		for (int i = 0; i < (int)size() ; i++)
		{
			destination[i]=get_state(i);
		}
	}
	else
	{
		for (int i = 0; i < (int)size() ; i++)
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
		for (int i = 0; i < (int)size() ; ++i)
		{
			destination[i]=get_state(i);
		}
	}
	else
	{
		for (int i = 0; i < (int)size() ; ++i)
		{
			destination[i]=get_state(i);
		}

	}
}
void cmf::math::Integrator::copy_dxdt( Time time,num_array & destination,real factor ) const
{
	if (destination.size()!=(int)size())
		destination.resize(size());
	if (use_OpenMP)
	{
		if (factor==1)
		{
#pragma omp parallel for schedule(guided)
			for (int i = 0; i < (int)size() ; ++i)
				destination[i]=m_States[i]->dxdt(time);
		}
		else
		{
#pragma omp parallel for schedule(guided)
			for (int i = 0; i < (int)size() ; i++)
			{
				destination[i]=m_States[i]->dxdt(time);
				destination[i]*=factor;
			}
		}
	}
	else
	{
		if (factor==1)
		{
			for (int i = 0; i < (int)size() ; ++i)
				destination[i]=m_States[i]->dxdt(time);
		}
		else
		{
			for (int i = 0; i < (int)size() ; i++)
			{
				destination[i]=m_States[i]->dxdt(time);
				destination[i]*=factor;
			}
		}
	}
}

void cmf::math::Integrator::copy_dxdt( Time time,real * destination,real factor/*=1*/ ) const
{
	if (use_OpenMP)
	{
		if (factor==1)
		{
#pragma omp parallel for schedule(guided)
			for (int i = 0; i < (int)size() ; ++i)
				destination[i]=m_States[i]->dxdt(time);
		}
		else
		{
#pragma omp parallel for schedule(guided)
			for (int i = 0; i < (int)size() ; i++)
			{
				destination[i]=m_States[i]->dxdt(time);
				destination[i]*=factor;
			}
		}
	}
	else
	{
		if (factor==1)
		{
			for (int i = 0; i < (int)size() ; ++i)
				destination[i]=m_States[i]->dxdt(time);
		}
		else
		{
			for (int i = 0; i < (int)size() ; i++)
			{
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
		for (int i = 0; i < (int)size() ; i++)
		{
			m_States[i]->set_state(newStates[i]);
		}
	}
	else
	{
		for (int i = 0; i < (int)size() ; i++)
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
		for (int i = 0; i < (int)size() ; ++i)
		{
			m_States[i]->set_state(newStates[i]);
		}
	}
	else
	{
		for (int i = 0; i < (int)size() ; ++i)
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
		for (int i = 0; i < (int)size() ; i++)
		{
			m_States[i]->set_state(m_States[i]->get_state() + operands[i]);		
		}
	}
	else
	{
		for (int i = 0; i < (int)size() ; i++)
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
	for(integratable_vector::iterator it = m_integratables.begin(); it != m_integratables.end(); ++it)
		(*it)->reset(start);
	while (m_t < t_max) {
		integrate(t_max,dt);
		for(integratable_vector::iterator it = m_integratables.begin(); it != m_integratables.end(); ++it)
			(*it)->integrate(m_t);
		++i;
	}
	if (i>0) m_dt = (t_max - start)/i;
}