#include "StateVariable.h"
#include "numVector.h"
#include <cmath>
#ifdef _OPENMP
#include <omp.h>
#endif


void cmf::math::StateVariableVector::CopyStates( numVector & destination ) const
{
	if (destination.size()!=(int)size())
		destination.resize(size());
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; i++)
	{
		destination[i]=(*this)[i]->State();
	}
}

void cmf::math::StateVariableVector::CopyStates( real * destination ) const
{
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; ++i)
	{
		destination[i]=(*this)[i]->State();
	}
}
void cmf::math::StateVariableVector::CopyDerivs( Time time,numVector & destination,real factor ) const
{
	if (destination.size()!=(int)size())
		destination.resize(size());
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

void cmf::math::StateVariableVector::CopyDerivs( Time time,real * destination,real factor/*=1*/ ) const
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
void cmf::math::StateVariableVector::SetStates(const numVector & newStates )
{
	#pragma omp parallel for
	for (int i = 0; i < (int)size() ; i++)
	{
		(*this)[i]->State(newStates[i]);
	}
}

void cmf::math::StateVariableVector::SetStates( real * newStates )
{
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; ++i)
	{
		(*this)[i]->State(newStates[i]);
	}
}

void cmf::math::StateVariableVector::AddValuesToStates(const numVector& operands)
{
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; i++)
	{
		(*this)[i]->State((*this)[i]->State() + operands[i]);		
	}
}