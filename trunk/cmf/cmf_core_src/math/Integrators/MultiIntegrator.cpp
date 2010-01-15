

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