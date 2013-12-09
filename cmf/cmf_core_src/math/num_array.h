

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
#include "real.h"
 namespace cmf {
 	namespace math {
		/// A valarray kind of vector implementation with OpenMP capabilities
		class num_array
		{
		private:
			real* m_data;
			int m_size;
		public:
			/// @name Constructors & Destructors
			// @{
			/// Creates a new vector with size count
			/// @param count size of new vector
			/// @param Value Value of each element (default=0)
			num_array(int count,real Value=0);
			num_array(size_t count,real Value=0);
			/// Creates a vector of size 1 with value {0}
			num_array();
			/// Copy constructor
			num_array(const num_array& Vector);
		
			/// Creates a vector from data. After creation is vector the owner of data
			num_array(double * data, size_t count);
			typedef double* iterator;
			typedef const double * const_iterator;

			/// Create a vector from an iterator range. Copies the data
			num_array(const_iterator begin,const_iterator end);

			/// Destructor
			~num_array();
			//@}
			/// Assigns the given vector to this. If the sizes differ, the size of this gets adjusted
			num_array& operator=(const num_array& vector);
			num_array& operator=(const std::vector<double>& vector);
			/// Sets each element of this to scalar
			num_array& operator=(real scalar);
			iterator begin() const {return m_data;}
			iterator end()  const {return m_data + size();}
			/// Size of the vector
			int size() const { return m_size;}
			/// Changes the size of the vector
			void resize(int count);
			void resize(size_t count) {resize(int(count));}
			/// Prevents deletion of the stored data. 
			/// Use with care, and only if you know what you are doing.
			iterator release() {
				iterator res = m_data;
				m_size=0;m_data=0;
				return res;
			}
			/// returns the element at position pos (immutable vector)
			const real& operator[](int pos) const { return m_data[pos];}
			/// returns the element at position pos (mutable vector)
			real& operator[](int pos) { return m_data[pos];}
			/// @name Operators
			/// Binary operators defined as free operators:
			///
			/// x = {+,-,*,/}
			///
			/// Defined for (x is one of the operators above):
			/// - vector = vector x vector 
			/// - vector = real x vector
			/// - vector = vector x real
			//@{
			num_array operator-() const;                  ///< Return sign toggled vector
			num_array power(const num_array&) const;      ///< Return each element to the power of each element of vector
			num_array power(real exponent) const;       ///< Return each element to the power "exponent"
			num_array& operator+=(const num_array&);      ///< Add vector to this
			num_array& operator+=(real);                ///< Add scalar to this
			num_array& operator-=(const num_array&);      ///< Subtract vector from this
			num_array& operator-=(real);                ///< Subtract scalar from this
			num_array& operator*=(const num_array&);      ///< Multiply each element of vector with each element of this
			num_array& operator*=(real);                ///< Multiply each element of vector with scalar
			num_array& operator/=(const num_array&);      ///< Divide each element of this by each element of vector
			num_array& operator/=(real);                ///< Divide each element of this by scalar
			

			/// Return funct(this)
			/// @param funct a function of signature real funct(real)
			///
			/// Example: Return the sinus of each element of vector x
			/// @code
			/// num_array sin_x = x.apply(sin);
			/// @endcode
			num_array apply(real funct(real)) const; 
			//@}

			/// @name Summarizing functions
			//@{
			real dot(const num_array&) const;           ///< Return dot product of this and vector
			real sum() const;                           ///< Return sum of vector
			real max() const;														///< Return max of vector
			real min() const;                           ///< Return min of vector
			real mean() const														///< Return mean of vector
			{
				return sum()/size();
			}
			/// Returns a norm of the vector, implemented norms:  1 - sum of abs, 2 - Euclidean distance, 0 Maximum of abs
			/// @param normtype An integer indicating the type of norm
			real norm(int normtype=0) const;              
			//@}
			num_array operator+(const num_array& _Right);
			num_array operator-(const num_array& _Right);
			num_array operator*(const num_array& _Right);
			num_array operator/(const num_array& _Right);

			num_array operator+(real _Right);
			num_array operator-(real _Right);
			num_array operator*(real _Right);
			num_array operator/(real _Right);


		};


		num_array operator+(real _Left,const num_array& _Right);
		num_array operator-(real _Left,const num_array& _Right);
		num_array operator*(real _Left,const num_array& _Right);
		num_array operator/(real _Left,const num_array& _Right);

		/// Returns the max number of threads used by OpenMP in parallel sections of the code
		int get_parallel_threads();
		/// Set the number of threads used by OpenMP in parallel sections of the code
		int set_parallel_threads(int numthreads);
	}
}

#endif // num_array_h__
