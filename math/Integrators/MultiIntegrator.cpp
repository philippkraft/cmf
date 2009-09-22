#include "MultiIntegrator.h"
#ifdef _OPENMP
#include <omp.h>
#endif


int cmf::math::MultiIntegrator::Integrate( cmf::math::Time MaxTime,cmf::math::Time TimeStep )
{
	if (use_OpenMP)
	{
#pragma omp parallel for
		for (int i = 0; i < (int)m_integrators.size() ; ++i)
		{
			m_integrators[i]->IntegrateUntil(MaxTime,TimeStep);
		}
	}
	else
	{
		for(integ_vector::iterator it = m_integrators.begin(); it != m_integrators.end(); ++it)
		{
			(**it).IntegrateUntil(MaxTime,TimeStep);
		}
	}
	m_Time=MaxTime;
	return 1;
}