

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

#include <cmath>
#include <iostream>

#include "num_array.h"
#include <cassert>
/// constructors

cmf::math::num_array::num_array()
: m_size(1)
{
  this->m_data = new real[1];
  this->m_data[0] = 0.0;
}

cmf::math::num_array::num_array(const num_array& Vector )
: m_size(Vector.size()), m_data(new real[Vector.size()])
{
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < m_size ; ++i)
    this->m_data[i] = Vector[i];
}

cmf::math::num_array::num_array(const_iterator begin, const_iterator end)
{
  this->m_size = end - begin;

  this->m_data = new real[this->m_size];
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; i++)
    this->m_data[i] = begin[i];
}

#ifdef USE_ADOLC
cmf::math::num_array::num_array(const real *begin, const real *end)
{
  this->m_size = end - begin;
  assert(this->m_size >= 0);

  this->m_data = new real[this->m_size];
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; i++)
    this->m_data[i] = begin[i];
}
#endif

cmf::math::num_array::num_array(ptrdiff_t count, real Value)
: m_size(count)
{

  this->m_data = new real[this->m_size];
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size ; ++i)
    this->m_data[i] = Value;
}

cmf::math::num_array::num_array(size_t count, real Value)
: m_size(ptrdiff_t(count))
{

  this->m_data = new real[this->m_size];
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size ; ++i)
    this->m_data[i] = Value;
}

cmf::math::num_array::num_array(size_t count, real *data)
{
  this->m_size = count;

  this->m_data = data;
}

/// END constructors

/// destructor
cmf::math::num_array::~num_array()
{
  if (this->m_size)
    delete[] this->m_data;
}

void cmf::math::num_array::resize(const ptrdiff_t count)
{

  if (count == this->m_size) return;

  ptrdiff_t min = count < this->m_size ? count : this->m_size;
  real* ptr_new = new real[count];
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < min; ++i)
    ptr_new[i] = this->m_data[i];

  delete[] this->m_data;
  this->m_data = ptr_new;
  this->m_size = count;
}

// this = other
void cmf::math::num_array::set(const num_array& other)
{
  assert(this->m_size == other.size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    this->m_data[i] = other[i];
}

// this = fac * other
void cmf::math::num_array::set(const real fac, const num_array& other)
{
  assert(this->m_size == other.size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    this->m_data[i] = fac * other[i];
}

// this = fac * this
void cmf::math::num_array::scale(const real fac)
{
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    this->m_data[i] *= fac;
}

// this = fac * this + other
void cmf::math::num_array::scale_add(const real fac, const num_array& other)
{
  assert(this->m_size == other.size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    this->m_data[i] = fac * (this->m_data[i]) + other[i];
}

// this = this + fac * other
void cmf::math::num_array::axpy(const real fac, const num_array& other)
{
  assert(this->m_size == other.size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    this->m_data[i] += (fac * other[i]);
}

bool cmf::math::num_array::is_nan() const
{
  bool is_nan = false;
  double data_val;
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for reduction(.or.:is_nan)
#else
#pragma omp parallel for reduction(|| : is_nan)
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i){
#ifdef USE_ADOLC
    data_val = this->m_data[i].getValue();
#else
    data_val = this->m_data[i];
#endif
    is_nan = is_nan || (data_val!=data_val);
  }

  return is_nan;
}


cmf::math::num_array cmf::math::num_array::operator-() const
{
  cmf::math::num_array result(this->m_size);
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    result[i] = -this->m_data[i];
  return result;
}

cmf::math::num_array& cmf::math::num_array::operator=(const num_array& vector)
{
  if (this == &vector)
    ; // do nothing
  else if (this->m_size == vector.size())
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
    for (ptrdiff_t i = 0; i < this->m_size; ++i)
      this->m_data[i] = vector[i];
  else
  { // resize and copy
    resize(vector.size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
    for (ptrdiff_t i = 0; i < this->m_size; ++i)
      this->m_data[i] = vector[i];
  }
  return (*this);
}

cmf::math::num_array cmf::math::num_array::operator+(const num_array& _Right)
{
  cmf::math::num_array result(this->m_size);
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < size(); ++i)
    result[i] = (*this)[i] + _Right[i];
  return result;
}

cmf::math::num_array cmf::math::num_array::operator-(const num_array& _Right)
{
  cmf::math::num_array result(this->m_size);
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    result[i] = (*this)[i] - _Right[i];
  return result;
}

cmf::math::num_array cmf::math::num_array::operator*(const num_array& _Right)
{
  cmf::math::num_array result(this->m_size);
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    result[i] = (*this)[i] * _Right[i];
  return result;
}

cmf::math::num_array cmf::math::num_array::operator/(const num_array& _Right)
{
  cmf::math::num_array result(this->m_size);
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    result[i] = (*this)[i] / _Right[i];
  return result;
}


cmf::math::num_array& cmf::math::num_array::operator+=(const num_array& _Right)
{
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    this->m_data[i] += _Right[i];
  return (*this);
}

cmf::math::num_array& cmf::math::num_array::operator-=(const num_array& _Right)
{
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    this->m_data[i] -= _Right[i];
  return (*this);
}

cmf::math::num_array& cmf::math::num_array::operator*=(const num_array& _Right)
{
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    this->m_data[i] *= _Right[i];
  return (*this);
}

cmf::math::num_array& cmf::math::num_array::operator/=(const num_array& _Right)
{
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    this->m_data[i] /= _Right[i];
  return (*this);
}

cmf::math::num_array cmf::math::num_array::power(const num_array& exponent) const
{
  cmf::math::num_array result(this->m_size);
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->m_size; ++i)
    result[i] = pow(this->m_data[i],exponent[i]);
  return result;
}

cmf::math::num_array& cmf::math::num_array::operator=(const std::vector<real>& vector)
{
  resize(vector.size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->size() ; ++i)
    this->m_data[i] = vector[i];

  return (*this);
}

cmf::math::num_array& cmf::math::num_array::operator=(const real scalar)
{
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    this->m_data[i] = scalar;

  return (*this);
}

cmf::math::num_array cmf::math::num_array::operator+(const real _Right)
{
  cmf::math::num_array result(this->size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    result[i] = (*this)[i] + _Right;

  return result;
}

cmf::math::num_array cmf::math::num_array::operator-(const real _Right)
{
  cmf::math::num_array result(this->size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    result[i] = (*this)[i] - _Right;

  return result;
}

cmf::math::num_array cmf::math::num_array::operator*(const real _Right)
{
  cmf::math::num_array result(this->size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    result[i] = (*this)[i] * _Right;

  return result;
}

cmf::math::num_array cmf::math::num_array::operator/(const real _Right)
{
  cmf::math::num_array result(this->size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    result[i] = (*this)[i] / _Right;

  return result;
}

cmf::math::num_array& cmf::math::num_array::operator+=(const real _Right)
{
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    this->m_data[i] += _Right;

  return (*this);
}

cmf::math::num_array& cmf::math::num_array::operator-=(const real _Right)
{
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    this->m_data[i] -= _Right;

  return (*this);
}

#ifdef USE_ADOLC
cmf::math::num_array& cmf::math::num_array::operator*=(const adouble _Right)
{
#pragma omp ADOLC_OPENMP parallel for
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    this->m_data[i] *= _Right;

  return (*this);
}
#endif

cmf::math::num_array& cmf::math::num_array::operator*=(const real _Right)
{
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    this->m_data[i] *= _Right;

  return (*this);
}

cmf::math::num_array& cmf::math::num_array::operator/=(const real _Right)
{
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    this->m_data[i] /= _Right;

  return (*this);
}

cmf::math::num_array cmf::math::num_array::power(real exponent) const
{
  cmf::math::num_array result(this->size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    result[i] = pow(this->m_data[i],exponent);

  return result;
}

cmf::math::num_array cmf::math::num_array::apply(real funct(real)) const
{
  cmf::math::num_array result(this->size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    result[i] = funct(this->m_data[i]);

  return result;
}

real cmf::math::num_array::dot(const num_array& _Right) const
{
  real res(0.0);
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for reduction(+ : res)
#else
#pragma omp parallel for reduction(+ : res)
#endif
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    res += this->m_data[i] * _Right[i];

  return res;

}

real cmf::math::num_array::sum() const
{
  real res(0.0);
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for reduction(+ : res)
#else
#pragma omp parallel for reduction(+ : res)
#endif
  for (ptrdiff_t i = 0; i < this->size() ; ++i)
    res += this->m_data[i];
  return res;
}

real cmf::math::num_array::max() const
{
  real result(0.0);
#ifdef USE_ADOLC
  for (ptrdiff_t i = 0; i < this->size(); ++i)
    result = fmax(result, this->m_data[i]);
#else
#pragma omp parallel for shared(result)
  for (ptrdiff_t i = 0; i < this->size() ; ++i)
  {
    if (this->m_data[i] > result)
    {
#pragma omp critical
       if (this->m_data[i] > result)
         result=this->m_data[i];
    }
  }
#endif
  return result;
}

real cmf::math::num_array::min() const
{
  real result(0.0);
#ifdef USE_ADOLC
  for (ptrdiff_t i = 0; i < this->size(); i++)
    result = fmin(result, this->m_data[i]);
#else
#pragma omp parallel for shared(result)
  for (ptrdiff_t i = 0; i < this->size() ; ++i)
  {
    if (this->m_data[i] < result)
    {
#pragma omp critical
      if (this->m_data[i] < result)
        result = this->m_data[i];
    }
  }
#endif
  return result;
}

real cmf::math::num_array::norm(int normtype) const
{
  real result(0.0);

  if (normtype == 1) // Sum of absolute
  {
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for reduction(+ : result)
#else
#pragma omp parallel for reduction(+ : result)
#endif
    for (ptrdiff_t i = 0; i < this->size() ; ++i)
      result += fabs(this->m_data[i]);

    return result;
  }
  else if (normtype == 2) // Euclidean length
  {
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for reduction(+ : result)
#else
#pragma omp parallel for reduction(+ : result)
#endif
    for (ptrdiff_t i = 0; i < this->size() ; ++i)
      result += this->m_data[i] * this->m_data[i];

    return sqrt(result);
  }
  else // Maximum of absolute
  {
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for shared(result)
    for (ptrdiff_t i = 0; i < this->size() ; ++i)
    {
      if (fabs(this->m_data[i]) > result.getValue())
      {
#pragma omp ADOLC_OPENMP critical
        if (fabs(this->m_data[i]) > result.getValue())
          result = fabs(this->m_data[i]);
      }
    }
#else
#pragma omp parallel for shared(result)
    for (ptrdiff_t i = 0; i < this->size() ; ++i)
    {
      if (fabs(this->m_data[i]) > result)
      {
#pragma omp critical
        if (fabs(this->m_data[i]) > result)
          result = fabs(this->m_data[i]);
      }
    }
#endif
    return result;
  }
}

cmf::math::num_array cmf::math::operator+(real _Left, const num_array& _Right)
{
  cmf::math::num_array result(_Right.size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < _Right.size(); ++i)
    result[i] = _Left + _Right[i];

  return result;
}

cmf::math::num_array cmf::math::operator-(real _Left, const num_array& _Right)
{
  cmf::math::num_array result(_Right.size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < _Right.size(); ++i)
    result[i] = _Left - _Right[i];

  return result;
}

cmf::math::num_array cmf::math::operator*(real _Left, const num_array& _Right)
{
  cmf::math::num_array result(_Right.size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < _Right.size(); ++i)
    result[i] = _Left * _Right[i];

  return result;
}

cmf::math::num_array cmf::math::operator/(real _Left, const num_array& _Right)
{
  cmf::math::num_array result(_Right.size());
#ifdef USE_ADOLC
#pragma omp ADOLC_OPENMP parallel for
#else
#pragma omp parallel for
#endif
  for (ptrdiff_t i = 0; i < _Right.size(); ++i)
    result[i] = _Left / _Right[i];

  return result;
}

int cmf::math::get_parallel_threads()
{
#ifdef _OPENMP
  return omp_get_max_threads();
#else
  return 1;
#endif
}
int cmf::math::set_parallel_threads( int numthreads )
{
#ifdef _OPENMP
	omp_set_num_threads(numthreads);
	return omp_get_max_threads();
#else
	return 1;
#endif
}
