

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
#ifndef num_array_h__
#define num_array_h__

#include <valarray>
#include <vector>
#include <omp.h>

#include "real.h"
 namespace cmf {
 	namespace math {
		/// A valarray kind of vector implementation with OpenMP capabilities
		class num_array
		{
    private:

      ptrdiff_t m_size;
      real *m_data;

    public:

      typedef real* iterator;
      typedef const real* const_iterator;

      /// constructors
      num_array();
      num_array(const num_array& Vector);
      num_array(const_iterator begin, const_iterator end);
#ifdef USE_ADOLC
      num_array(const real    *begin, const real    *end);
#endif
      num_array(ptrdiff_t    count, real Value = 0.0);
      num_array(size_t count, real Value = 0.0);
      num_array(size_t count, real *data);

      /// destructor
      ~num_array();

      ptrdiff_t size()  const { return this->m_size; }
      iterator begin() const { return this->m_data; }
      iterator end()   const { return this->m_data + this->m_size; }
	  iterator release() {
		  iterator res = m_data;
		  m_size=0;m_data=0;
		  return res;
	  }

      // void resize(size_t    count) { this->resize(ptrdiff_t(count)); }
      void resize(ptrdiff_t count);

      /// Prevents deletion of the stored data.
      /// Use with care, and only if you know what you are doing.
      /*iterator release() {
        iterator res = m_data;
        m_size=0;m_data=0;
        return res;
      }*/

      // this = other
      void set(const num_array& other);

      // this = fac * other
      void set(const real fac, const num_array& other);

      // this = fac * this
      void scale(const real fac);

      // this = fac * this + other
      void scale_add(const real fac, const num_array& other);

      // this = this + fac * other
      void axpy(const real fac, const num_array& other);

      bool is_nan() const;

      num_array  operator- () const;
      num_array& operator= (const num_array&);
      num_array  operator+ (const num_array&);
      num_array  operator- (const num_array&);
      num_array  operator* (const num_array&);
      num_array  operator/ (const num_array&);
      num_array& operator+=(const num_array&);
      num_array& operator-=(const num_array&);
      num_array& operator*=(const num_array&);
      num_array& operator/=(const num_array&);

      num_array power(const num_array&) const;

      num_array& operator=(const std::vector<real>& vector);
      num_array& operator=(const real scalar);

      const real& operator[](const ptrdiff_t pos) const { return this->m_data[pos]; }
            real& operator[](const ptrdiff_t pos)       { return this->m_data[pos]; }

      num_array  operator+ (const real);
      num_array  operator- (const real);
      num_array  operator* (const real);
      num_array  operator/ (const real);
      num_array& operator+=(const real);
      num_array& operator-=(const real);
#ifdef USE_ADOLC
      num_array& operator*=(const adouble);
#endif
      num_array& operator*=(const real);
      num_array& operator/=(const real);

      num_array power(real exponent) const;
      num_array apply(real funct(real)) const;

      real dot(const num_array&) const;
      real sum()  const;
      real max()  const;
      real min()  const;
      real mean() const { return this->sum() / (real)(this->m_size); }

      real norm(int normtype = 0) const;
    };


    num_array operator+(real _Left, const num_array& _Right);
    num_array operator-(real _Left, const num_array& _Right);
    num_array operator*(real _Left, const num_array& _Right);
    num_array operator/(real _Left, const num_array& _Right);

	/// Returns the max number of threads used by OpenMP in parallel sections of the code
	int get_parallel_threads();
	/// Set the number of threads used by OpenMP in parallel sections of the code
	int set_parallel_threads(int numthreads);

	}
}

#endif // num_array_h__
