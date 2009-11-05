#include "neville_interpolator.h"
cmf::math::NevilleInterpolator::NevilleInterpolator( int NumberOfPoints=2,int VectorSize=1) : m_nValues(NumberOfPoints)
{
	m_temp=new num_array[m_nValues];
	for (int i = 0; i < m_nValues ; i++)
	{
		m_temp[i].resize(VectorSize);
	}
}

cmf::math::num_array cmf::math::NevilleInterpolator::Interpolate( const num_array* y[],const Time x[],Time t,int n )
{
	// Create new vector
	num_array _result((*y[0]).size());
	// Copy values in the new vector
	Interpolate(y,x,t,n,_result);
	// Return the new vector
	return _result;
}

void cmf::math::NevilleInterpolator::Interpolate( const num_array* y[],const Time x[],Time t,int n,num_array& _result )
{
	// Look if the position t equals one of the known values
	for (int i = 0; i < n ; i++)
	{
		// If yes, return the known value
		if (x[i]==t)
		{
			_result=*(y[i]);
			return; // We're done no interpolation needed
		}
	}
	// If the number of the given points does not equal the number of allocated temporary vectors, 
	// discard the temporary vectors and reconstruct them
	if (n>m_nValues)
	{
		delete[] m_temp;
		m_nValues=n;
		m_temp=new num_array[m_nValues];
	}

	// Copy the y values to the temporary vectors
	for (int i = 0; i < n ; i++)
		m_temp[i]=*(y[i]); // resizes the temporary vectors, if needed
	for (int j = 1; j < n ; j++)
	  for (int i = n-1; i>=j;--i)
		{
			// Neville interpolation
			// Recursive formula: y[i] = ((t-x[i-j])*y[i] - (t-x[i])*y[i-1]) / (x[i]-x[i-j])
			m_temp[i]*=(t-x[i-j]).AsDays();
			m_temp[i]-=(t-x[i]).AsDays() * m_temp[i-1];
			m_temp[i]/=(x[i]-x[i-1]).AsDays();
		}
	_result=m_temp[n-1];
}
