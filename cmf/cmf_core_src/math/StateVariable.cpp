

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
#include "StateVariable.h"
#include "num_array.h"
#include <cmath>
#ifdef _OPENMP
#include <omp.h>
#endif


void cmf::math::StateVariableVector::CopyStates( num_array & destination ) const
{
	if (destination.size()!=(int)size())
		destination.resize(size());
	if (use_OpenMP)
	{
#pragma omp parallel for
		for (int i = 0; i < (int)size() ; i++)
		{
			destination[i]=(*this)[i]->get_state();
		}
	}
	else
	{
		for (int i = 0; i < (int)size() ; i++)
		{
			destination[i]=(*this)[i]->get_state();
		}
	}

}

void cmf::math::StateVariableVector::CopyStates( real * destination ) const
{
	if (use_OpenMP)
	{
#pragma omp parallel for
		for (int i = 0; i < (int)size() ; ++i)
		{
			destination[i]=(*this)[i]->get_state();
		}
	}
	else
	{
		for (int i = 0; i < (int)size() ; ++i)
		{
			destination[i]=(*this)[i]->get_state();
		}

	}
}
void cmf::math::StateVariableVector::CopyDerivs( Time time,num_array & destination,real factor ) const
{
	if (destination.size()!=(int)size())
		destination.resize(size());
	if (use_OpenMP)
	{
		if (factor==1)
		{
#pragma omp parallel for
			for (int i = 0; i < (int)size() ; ++i)
				destination[i]=(*this)[i]->Derivate(time);
		}
		else
		{
#pragma omp parallel for
			for (int i = 0; i < (int)size() ; i++)
			{
				destination[i]=(*this)[i]->Derivate(time);
				destination[i]*=factor;
			}
		}
	}
	else
	{
		if (factor==1)
		{
			for (int i = 0; i < (int)size() ; ++i)
				destination[i]=(*this)[i]->Derivate(time);
		}
		else
		{
			for (int i = 0; i < (int)size() ; i++)
			{
				destination[i]=(*this)[i]->Derivate(time);
				destination[i]*=factor;
			}
		}
	}
}

void cmf::math::StateVariableVector::CopyDerivs( Time time,real * destination,real factor/*=1*/ ) const
{
	if (use_OpenMP)
	{
		if (factor==1)
		{
#pragma omp parallel for
			for (int i = 0; i < (int)size() ; ++i)
				destination[i]=(*this)[i]->Derivate(time);
		}
		else
		{
#pragma omp parallel for
			for (int i = 0; i < (int)size() ; i++)
			{
				destination[i]=(*this)[i]->Derivate(time);
				destination[i]*=factor;
			}
		}
	}
	else
	{
		if (factor==1)
		{
			for (int i = 0; i < (int)size() ; ++i)
				destination[i]=(*this)[i]->Derivate(time);
		}
		else
		{
			for (int i = 0; i < (int)size() ; i++)
			{
				destination[i]=(*this)[i]->Derivate(time);
				destination[i]*=factor;
			}
		}
	}

}
void cmf::math::StateVariableVector::SetStates(const num_array & newStates )
{
	if (use_OpenMP)
	{
#pragma omp parallel for
		for (int i = 0; i < (int)size() ; i++)
		{
			(*this)[i]->set_state(newStates[i]);
		}
	}
	else
	{
		for (int i = 0; i < (int)size() ; i++)
		{
			(*this)[i]->set_state(newStates[i]);
		}
	}

}

void cmf::math::StateVariableVector::SetStates( real * newStates )
{
	if (use_OpenMP)
	{
#pragma omp parallel for
		for (int i = 0; i < (int)size() ; ++i)
		{
			(*this)[i]->set_state(newStates[i]);
		}
	}
	else
	{
		for (int i = 0; i < (int)size() ; ++i)
		{
			(*this)[i]->set_state(newStates[i]);
		}
	}
}

void cmf::math::StateVariableVector::AddValuesToStates(const num_array& operands)
{
	if (use_OpenMP)
	{
#pragma omp parallel for
		for (int i = 0; i < (int)size() ; i++)
		{
			(*this)[i]->set_state((*this)[i]->get_state() + operands[i]);		
		}
	}
	else
	{
		for (int i = 0; i < (int)size() ; i++)
		{
			(*this)[i]->set_state((*this)[i]->get_state() + operands[i]);		
		}
	}
}