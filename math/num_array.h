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
		
			/// Creates a new vector using data. Not for external use
			num_array(double * data,int count);

			void set_data_from_adress(size_t data_adress,size_t count);

			size_t adress() const;

#ifndef SWIG
			/// Convert from valarray<real>
			num_array(const std::valarray<real>& Vector);
#endif
			/// Destructor
			~num_array();
			//@}
#ifndef SWIG
			/// Assigns the given vector to this. If the sizes differ, the size of this gets adjusted
			num_array& operator=(const num_array& vector);
			num_array& operator=(const std::vector<double>& vector);
			/// Sets each element of this to scalar
			num_array& operator=(real scalar);
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

	}
}

#endif // num_array_h__
