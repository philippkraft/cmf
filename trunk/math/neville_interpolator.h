#ifndef neville_interpolator_h__
#define neville_interpolator_h__

#include "Time.h"
#include "num_array.h"
#include <cmath>

namespace cmf {
	namespace math {
		/// Interpolates between known values (of type num_array ) at given points in time.
		/// For a polynom interpolation, temporary variables for the y values are needed. 
		/// These variables (vectors) will be allocated beforehand in the constructor.
		/// If various interpolations for the same number of points and the same value space are done,
		/// costly new allocation of memory will be avoided.
		class NevilleInterpolator
		{
		private:
			/// Number of known points, the polynom will have a degree of m_nValues-1
			int m_nValues;
			/// Array of num_array to hold the intermediate values
			num_array* m_temp;
		public:
			/// Reserves temporary space for the temporary vectors
			NevilleInterpolator(int NumberOfPoints,int VectorSize);
			/// Interpolates the missing values at position t from the n values y[] at n positions x[] and copies them to _result
			/// No memory allocation needed (despite a bit in the stack) if the sizes of the vectors and the number of points
			/// are equal to the last call
			void Interpolate( const num_array* y[],const Time x[],Time t,int n,num_array& _result );
			/// Interpolates the missing values at position t from the n values y[] at n positions x[] and returns them
			num_array Interpolate( const num_array* y[],const Time x[],Time t,int n);
		};
	}
}
#endif // neville_interpolator_h__
