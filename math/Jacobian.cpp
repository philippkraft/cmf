#include "Jacobian.h"
#include <omp.h>
#define max(a,b) (((a)>(b)) ? (a):(b))


cmf::math::Jacobian::~Jacobian()
{
	if (m_matrix)
	{
		for (int i = 0; i < m_size ; i++)
			delete[] m_matrix[i];
		delete[] m_matrix;
	}
	m_matrix=0;
}

void cmf::math::Jacobian::ToFile( std::string filename )
{
	std::ofstream file;
	file.open(filename.c_str());
	for (int row = 0; row < m_size ; row++)
	{	
		for (int col = 0; col < m_size ; col++)
			file << m_matrix[row][col] << "\t";
		file << std::endl;
	}
	file.close();

}

real cmf::math::Jacobian::operator()( int i,int j )
{
	if (i<m_size && j<m_size)
		return m_matrix[i][j];
	else
		return 0.0;
}

void cmf::math::Jacobian::Calculate( StateVariableVector& vector, const cmf::math::Time& timestep)
{	
	// If the size of the sitevector differs from the last calculation
	if (m_size != (int)vector.size())
	{
		// If matrix exists, delete it
		if (m_matrix)
		{
			for (int i = 0; i < m_size ; i++)
				delete[] m_matrix[i];
			delete[] m_matrix;
		}
		// Set new size
		m_size=(int)vector.size();
		// Initialize new matrix
		m_matrix=new real* [m_size];
		for (int i=0;i<(int)m_size;++i) 
			m_matrix[i] = new real[m_size];
	}
  

	real 
		delt,					// small difference to set value
		ysafe,				// temp storage for state
		uround=real(1e-16); // Base for delt
	int n=m_size;

	//Derivates at current state
	real * deriv=new real[n];
#pragma omp parallel for
	for (int i = 0; i < n; i++) 
		deriv[i]=vector[i]->Derivate(timestep);

	// Change each state a little bit (the cols of the jacobian)
	for (int i = 0; i < n; i++) 
	{
		// Remember last state
		ysafe = vector[i]->State();
		// Calculate difference
		delt = sqrt(uround*max(1.0e-5, fabs(ysafe)));
		// Change the state for the small difference
		vector[i]->State(ysafe + delt);

		// Observe the results of the small change of one state (rows of jacobian)
#pragma omp parallel for 
		for (int j = 0; j < n; j++)
			m_matrix[j][i] = (vector[j]->Derivate(timestep) - deriv[j])/delt;

		//Restore previous state
		vector[i]->State(ysafe);
	}
	delete[] deriv;
}