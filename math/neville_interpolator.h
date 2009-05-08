#ifndef neville_interpolator_h__
#define neville_interpolator_h__

#include "Time.h"
#include "numVector.h"
#include <cmath>

namespace cmf {
	namespace math {
		/// Interpolates between known values (of type numVector ) at given points in time.
		/// For a polynom interpolation, temporary variables for the y values are needed. 
		/// These variables (vectors) will be allocated beforehand in the constructor.
		/// If various interpolations for the same number of points and the same value space are done,
		/// costly new allocation of memory will be avoided.
		class NevilleInterpolator
		{
		private:
			/// Number of known points, the polynom will have a degree of m_nValues-1
			int m_nValues;
			/// Array of numVector to hold the intermediate values
			numVector* m_temp;
		public:
			/// Reserves temporary space for the temporary vectors
			NevilleInterpolator(int NumberOfPoints,int VectorSize);
			/// Interpolates the missing values at position t from the n values y[] at n positions x[] and copies them to _result
			/// No memory allocation needed (despite a bit in the stack) if the sizes of the vectors and the number of points
			/// are equal to the last call
			void Interpolate( const numVector* y[],const Time x[],Time t,int n,numVector& _result );
			/// Interpolates the missing values at position t from the n values y[] at n positions x[] and returns them
			numVector Interpolate( const numVector* y[],const Time x[],Time t,int n);
		};
	}
}
#endif // neville_interpolator_h__
