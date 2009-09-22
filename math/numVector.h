#ifndef numVector_h__
#define numVector_h__
#include <valarray>
#include <vector>
#include "real.h"
 namespace cmf {
 	namespace math {
		/// A valarray kind of vector implementation with OpenMP capabilities
		class numVector
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
			numVector(int count,real Value=0);
			numVector(size_t count,real Value=0);
			/// Creates a vector of size 1 with value {0}
			numVector();
			/// Copy constructor
			numVector(const numVector& Vector);

			numVector(double * data,int count);

			void set_data_from_adress(long long data_adress,size_t count);

			size_t adress() const;

#ifndef SWIG
			/// Convert from valarray<real>
			numVector(const std::valarray<real>& Vector);
#endif
			/// Destructor
			~numVector();
			//@}
#ifndef SWIG
			/// Assigns the given vector to this. If the sizes differ, the size of this gets adjusted
			numVector& operator=(const numVector& vector);
			numVector& operator=(const std::vector<double>& vector);
			/// Sets each element of this to scalar
			numVector& operator=(real scalar);
#endif
			/// Size of the vector
			int size() const { return m_size;}
			/// Changes the size of the vector
			void resize(int count);
			void resize(size_t count) {resize(int(count));}
#ifndef SWIG
			/// returns the element at position pos (immutable vector)
			real operator[](int pos) const { return m_data[pos];}
			/// returns the element at position pos (mutable vector)
			real& operator[](int pos) { return m_data[pos];}
#endif			
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
			numVector operator-() const;                  ///< Return sign toggled vector
			numVector power(const numVector&) const;      ///< Return each element to the power of each element of vector
			numVector power(real exponent) const;       ///< Return each element to the power "exponent"
			numVector& operator+=(const numVector&);      ///< Add vector to this
			numVector& operator+=(real);                ///< Add scalar to this
			numVector& operator-=(const numVector&);      ///< Subtract vector from this
			numVector& operator-=(real);                ///< Subtract scalar from this
			numVector& operator*=(const numVector&);      ///< Multiply each element of vector with each element of this
			numVector& operator*=(real);                ///< Multiply each element of vector with scalar
			numVector& operator/=(const numVector&);      ///< Divide each element of this by each element of vector
			numVector& operator/=(real);                ///< Divide each element of this by scalar
			

			/// Return funct(this)
			/// @param funct a function of signature real funct(real)
			///
			/// Example: Return the sinus of each element of vector x
			/// @code
			/// numVector sin_x = x.apply(sin);
			/// @endcode
			numVector apply(real funct(real)) const; 
			//@}

			/// @name Summarizing functions
			//@{
			real dot(const numVector&) const;           ///< Return dot product of this and vector
			real sum() const;                           ///< Return sum of vector
			real max() const;														///< Return max of vector
			real min() const;                           ///< Return min of vector
			
			/// Returns a norm of the vector, implemented norms:  1 - sum of abs, 2 - Euclidean distance, 0 Maximum of abs
			/// @param normtype An integer indicating the type of norm
			real norm(int normtype=0) const;              
			//@}
			numVector operator+(const numVector& _Right);
			numVector operator-(const numVector& _Right);
			numVector operator*(const numVector& _Right);
			numVector operator/(const numVector& _Right);

			numVector operator+(real _Right);
			numVector operator-(real _Right);
			numVector operator*(real _Right);
			numVector operator/(real _Right);

	
		};


		numVector operator+(real _Left,const numVector& _Right);
		numVector operator-(real _Left,const numVector& _Right);
		numVector operator*(real _Left,const numVector& _Right);
		numVector operator/(real _Left,const numVector& _Right);

	}
}

#endif // numVector_h__
