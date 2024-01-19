

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
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
#include "multiintegrator.h"
#ifdef _OPENMP
#include <omp.h>
#endif


int cmf::math::MultiIntegrator::integrate( cmf::math::Time MaxTime,cmf::math::Time TimeStep )
{
	if (get_system().use_OpenMP)
	{
#pragma omp parallel for
		for (int i = 0; i < (int)m_integrators.size() ; ++i)
		{
			m_integrators[i]->integrate_until(MaxTime,TimeStep);
		}
	}
	else
	{
		for(const auto& integ: m_integrators)
		{
			integ->integrate_until(MaxTime,TimeStep);
		}
	}
	m_t=MaxTime;
	return 1;
}

std::string cmf::math::MultiIntegrator::to_string() const {
    std::string res("MultiIntegrator(size=");
    res += std::to_string(size());
    for (auto& integ: m_integrators) {
        res += "," + integ->to_string();
    }
    res += ")";
    return res;
}
