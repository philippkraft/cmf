#ifndef Jacobian_h__
#define Jacobian_h__



#include "StateVariable.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "real.h"

namespace cmf {
	namespace math {
		/// Calculates the jacobian of a system (a vector of state variables)
		///
		/// The jacobian is not used by any of the solvers, it is legacy code only kept in for certain numerical debugging purposes.
		/// This code is completely unimportant!
		///
		/// Code is inspired by RADAU5 method from Hairer E,Wanner G (1991) Solving Ordinary Differential Equations II, Springer-Verlag
	
		class Jacobian
		{
		private:
			/// The matrix storage
			real ** m_matrix;
			/// The size of the matrix (m_size x m_size)
			int m_size;
		public:
			Jacobian() : m_matrix(0),m_size(0) {}
			/// Calculates the jacobian for a given system at a given time step
			void Calculate(StateVariableVector& vector, const cmf::math::Time& timestep);
			/// Returns the values of the matrix
			real operator()(int i,int j);
			/// Saves the matrix in an undocumented file. Cols are tab seperated, rows are ended with endl
			void ToFile(std::string filename);
		
			~Jacobian();
		};
	}
}
#endif // Jacobian_h__
