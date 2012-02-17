

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
#include "num_array.h"

#include <cmath>

#include <iostream>

#ifdef _OPENMP
#include <omp.h>
#endif

cmf::math::num_array::num_array( int count,real Value/*=0*/ ) : m_size(count)
{

	m_data=new real[m_size];
#pragma omp parallel for
	for (int i = 0; i < m_size ; ++i)
		m_data[i]=Value;
}
cmf::math::num_array::num_array( size_t count,real Value/*=0*/ ) : m_size(int(count))
{
	m_data=new real[m_size];
#pragma omp parallel for
	for (int i = 0; i < m_size ; ++i)
		m_data[i]=Value;
}

cmf::math::num_array::num_array( const num_array& Vector ) 
: m_size(Vector.size()), m_data(new double[Vector.size()])
{
	for (int i = 0; i < size() ; ++i)
		m_data[i] = Vector[i];
}

cmf::math::num_array::num_array() : m_size(1)
{
	m_data=new real[1];
	m_data[0]=0;
}


cmf::math::num_array::num_array( double * data,size_t count )
{
	m_data=data;
	m_size=count;
}

cmf::math::num_array::num_array( const_iterator begin,const_iterator end )
{
	m_size = end - begin;
	m_data = new double[m_size];
	std::copy(begin,end,m_data);
}
cmf::math::num_array::~num_array()
{
	if (m_size)
		delete[] m_data;
}

cmf::math::num_array& cmf::math::num_array::operator=( const num_array& vector )
{
	if (this == &vector)
		;	// do nothing
	else if (size() == vector.size())
#pragma omp parallel for
		for (int i = 0; i < size(); ++i)
			m_data[i] = vector[i];
	else
	{	// resize and copy
		resize(vector.size());
#pragma omp parallel for
		for (int i = 0; i < size(); ++i)
			m_data[i] = vector[i];
	}
	return (*this);

}

cmf::math::num_array& cmf::math::num_array::operator=( real scalar )
{
	for (int i = 0; i < size(); ++i)
		m_data[i] = scalar;
	return (*this);
}

cmf::math::num_array& cmf::math::num_array::operator=( const std::vector<double>& vector )
{
	resize(vector.size());
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; ++i)
	{
		m_data[i]=vector[i];
	}
	return *this;
}

cmf::math::num_array& cmf::math::num_array::operator-=( real _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] -= _Right;
	return (*this);
}
cmf::math::num_array& cmf::math::num_array::operator+=( real _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] += _Right;
	return (*this);
}
cmf::math::num_array& cmf::math::num_array::operator*=( real _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] *= _Right;
	return (*this);
}
cmf::math::num_array& cmf::math::num_array::operator/=( real _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] /= _Right;
	return (*this);
}
cmf::math::num_array& cmf::math::num_array::operator+=(const num_array& _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] += _Right[i];
	return (*this);
}
cmf::math::num_array& cmf::math::num_array::operator-=(const num_array& _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] -= _Right[i];
	return (*this);
}
cmf::math::num_array& cmf::math::num_array::operator*=(const num_array& _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] *= _Right[i];
	return (*this);
}
cmf::math::num_array& cmf::math::num_array::operator/=(const num_array& _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] /= _Right[i];
	return (*this);
}

 cmf::math::num_array cmf::math::num_array::operator-() const
 {
 	cmf::math::num_array result(size());
 #pragma omp parallel for
 	for (int i = 0; i < size(); ++i)
 		result[i] = -m_data[i];
 	return result;
 }

void cmf::math::num_array::resize( int count )
{
	if (count==size()) return;
	real* ptr_new=new real[count];
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		ptr_new[i] = m_data[i];
	delete[] m_data;
	m_data=ptr_new;
	m_size=count;
}

cmf::math::num_array cmf::math::num_array::apply( real funct(real) ) const
{
	cmf::math::num_array result(size());
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		result[i] = funct(m_data[i]);
	return result;
}


real cmf::math::num_array::dot( const num_array& _Right) const
{
	real res=0;
#pragma omp parallel for reduction(+ : res)
	for (int i = 0; i < size(); ++i)
		res += m_data[i] * _Right[i];
	return res;

}

cmf::math::num_array cmf::math::num_array::power( real exponent ) const
{
	cmf::math::num_array result(size());
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		result[i] = pow(m_data[i],exponent);
	return result;
}

cmf::math::num_array cmf::math::num_array::power( const num_array& exponent) const
{
	cmf::math::num_array result(size());
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		result[i] = pow(m_data[i],exponent[i]);
	return result;
}
real cmf::math::num_array::sum() const
{
  real result=0;
#pragma omp parallel for reduction(+ : result)
	for (int i = 0; i < size() ; ++i)
		result += m_data[i];
	return result;
}

real cmf::math::num_array::max() const
{
	real result=0;
#pragma omp parallel for shared(result)
	for (int i = 0; i < size() ; ++i)
	{
		if (m_data[i]>result)
		{
#pragma omp critical
			{
				if (m_data[i]>result) result=m_data[i];
			}
		}
	}
	return result;

}

real cmf::math::num_array::min() const
{
	real result=0;
#pragma omp parallel for shared(result)
	for (int i = 0; i < size() ; ++i)
	{
		if (m_data[i]<result)
		{
#pragma omp critical
			{
				if (m_data[i]<result) result=m_data[i];
			}
		}
	}
	return result;

}

real cmf::math::num_array::norm( int normtype ) const
{
  if (normtype==1) // Sum of absolute
	{
		real result=0;
#pragma omp parallel for reduction(+ : result)
		for (int i = 0; i < size() ; ++i)
			result += abs(m_data[i]);
		return result;
	}
	else if (normtype==2) // Euclidean length
	{
		real result=0;
#pragma omp parallel for reduction(+ : result)
		for (int i = 0; i < size() ; ++i)
			result += m_data[i]*m_data[i];
		return sqrt(result);
	}
	else // Maximum of absolute
	{
		real result=0;
#pragma omp parallel for shared(result)
		for (int i = 0; i < size() ; ++i)
		{
			if (abs(m_data[i])>result)
			{
#pragma omp critical
				{
					if (abs(m_data[i])>result) result=abs(m_data[i]);
				}
			}
		}
		return result;
	}
}

//
// Binary Operators
//
cmf::math::num_array cmf::math::num_array::operator+(const num_array& _Right )
{
	cmf::math::num_array result(size());
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		result[i] = (*this)[i] + _Right[i];
	return result;
}
cmf::math::num_array cmf::math::num_array::operator-(const num_array& _Right )
{
	cmf::math::num_array result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] - _Right[i];
	return result;
}
cmf::math::num_array cmf::math::num_array::operator*(const num_array& _Right )
{
	cmf::math::num_array result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] * _Right[i];
	return result;
}
cmf::math::num_array cmf::math::num_array::operator/(const num_array& _Right )
{
	cmf::math::num_array result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] / _Right[i];
	return result;
}

cmf::math::num_array cmf::math::num_array::operator+(real _Right )
{
	cmf::math::num_array result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] + _Right;
	return result;
}
cmf::math::num_array cmf::math::num_array::operator-(real _Right )
{
	cmf::math::num_array result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] - _Right;
	return result;
}
cmf::math::num_array cmf::math::num_array::operator*(real _Right )
{
	cmf::math::num_array result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] * _Right;
	return result;
}
cmf::math::num_array cmf::math::num_array::operator/(real _Right )
{
	cmf::math::num_array result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] / _Right;
	return result;
}

cmf::math::num_array cmf::math::operator+( real _Left,const num_array& _Right )
{
	cmf::math::num_array result(_Right.size());
#pragma omp parallel for
	for (int i = 0; i < _Right.size(); ++i)
		result[i] = _Left + _Right[i];
	return result;
}
cmf::math::num_array cmf::math::operator-( real _Left,const num_array& _Right )
{
	cmf::math::num_array result(_Right.size());
#pragma omp parallel for
	for (int i = 0; i < _Right.size(); ++i)
		result[i] = _Left - _Right[i];
	return result;
}
cmf::math::num_array cmf::math::operator*( real _Left,const num_array& _Right )
{
	cmf::math::num_array result(_Right.size());
#pragma omp parallel for
	for (int i = 0; i < _Right.size(); ++i)
		result[i] = _Left * _Right[i];
	return result;
}
cmf::math::num_array cmf::math::operator/( real _Left,const num_array& _Right )
{
	cmf::math::num_array result(_Right.size());
#pragma omp parallel for
	for (int i = 0; i < _Right.size(); ++i)
		result[i] = _Left / _Right[i];
	return result;
}

int cmf::math::count_parallel_threads()
{
#ifdef _OPENMP
	return omp_get_max_threads();
#else
	return 1;
#endif
}
