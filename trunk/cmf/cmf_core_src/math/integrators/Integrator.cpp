#include "integrator.h"

real cmf::math::Integrator::error_exceedance( const num_array& compare,int * biggest_error_position/*=0 */ )
{
	real res=0;
#pragma omp parallel for shared(res)
	for (int i = 0; i < size() ; i++)
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


void cmf::math::Integrator::CopyStates( num_array & destination ) const
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

void cmf::math::Integrator::CopyStates( real * destination ) const
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
void cmf::math::Integrator::CopyDerivs( Time time,num_array & destination,real factor ) const
{
	if (destination.size()!=(int)size())
		destination.resize(size());
	if (use_OpenMP)
	{
		if (factor==1)
		{
#pragma omp parallel for schedule(guided)
			for (int i = 0; i < (int)size() ; ++i)
				destination[i]=m_States[i]->Derivate(time);
		}
		else
		{
#pragma omp parallel for schedule(guided)
			for (int i = 0; i < (int)size() ; i++)
			{
				destination[i]=m_States[i]->Derivate(time);
				destination[i]*=factor;
			}
		}
	}
	else
	{
		if (factor==1)
		{
			for (int i = 0; i < (int)size() ; ++i)
				destination[i]=m_States[i]->Derivate(time);
		}
		else
		{
			for (int i = 0; i < (int)size() ; i++)
			{
				destination[i]=m_States[i]->Derivate(time);
				destination[i]*=factor;
			}
		}
	}
}

void cmf::math::Integrator::CopyDerivs( Time time,real * destination,real factor/*=1*/ ) const
{
	if (use_OpenMP)
	{
		if (factor==1)
		{
#pragma omp parallel for schedule(guided)
			for (int i = 0; i < (int)size() ; ++i)
				destination[i]=m_States[i]->Derivate(time);
		}
		else
		{
#pragma omp parallel for schedule(guided)
			for (int i = 0; i < (int)size() ; i++)
			{
				destination[i]=m_States[i]->Derivate(time);
				destination[i]*=factor;
			}
		}
	}
	else
	{
		if (factor==1)
		{
			for (int i = 0; i < (int)size() ; ++i)
				destination[i]=m_States[i]->Derivate(time);
		}
		else
		{
			for (int i = 0; i < (int)size() ; i++)
			{
				destination[i]=m_States[i]->Derivate(time);
				destination[i]*=factor;
			}
		}
	}

}
void cmf::math::Integrator::SetStates(const num_array & newStates )
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

void cmf::math::Integrator::SetStates( real * newStates )
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

void cmf::math::Integrator::AddValuesToStates(const num_array& operands)
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
