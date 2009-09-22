#include "numVector.h"

#include <cmath>

#include <iostream>

#ifdef _OPENMP
#include <omp.h>
#endif

cmf::math::numVector::numVector( int count,real Value/*=0*/ ) : m_size(count)
{

	m_data=new real[m_size];
#pragma omp parallel for
	for (int i = 0; i < m_size ; ++i)
		m_data[i]=Value;
}
cmf::math::numVector::numVector( size_t count,real Value/*=0*/ ) : m_size(int(count))
{
	m_data=new real[m_size];
#pragma omp parallel for
	for (int i = 0; i < m_size ; ++i)
		m_data[i]=Value;
}

cmf::math::numVector::numVector( const numVector& Vector ) : m_size(Vector.size())
{
	m_data=new real[m_size];
#pragma omp parallel for
	for (int i = 0; i < m_size ; ++i)
		m_data[i]=Vector[i];
}

cmf::math::numVector::numVector() : m_size(1)
{
	m_data=new real[1];
	m_data[0]=0;
}

cmf::math::numVector::numVector( const std::valarray<real>& Vector )
{
	m_data=new real[m_size];
#pragma omp parallel for
	for (int i = 0; i < m_size ; ++i)
		m_data[i]=Vector[i];
}

cmf::math::numVector::numVector( double * data,int count )
{
	m_data=data;
	m_size=count;
}

cmf::math::numVector::~numVector()
{
	delete[] m_data;
}

cmf::math::numVector& cmf::math::numVector::operator=( const numVector& vector )
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

cmf::math::numVector& cmf::math::numVector::operator=( real scalar )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] = scalar;
	return (*this);
}

cmf::math::numVector& cmf::math::numVector::operator=( const std::vector<double>& vector )
{
	resize(vector.size());
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; ++i)
	{
		m_data[i]=vector[i];
	}
	return *this;
}
cmf::math::numVector& cmf::math::numVector::operator-=( real _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] -= _Right;
	return (*this);
}
cmf::math::numVector& cmf::math::numVector::operator+=( real _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] += _Right;
	return (*this);
}
cmf::math::numVector& cmf::math::numVector::operator*=( real _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] *= _Right;
	return (*this);
}
cmf::math::numVector& cmf::math::numVector::operator/=( real _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] /= _Right;
	return (*this);
}
cmf::math::numVector& cmf::math::numVector::operator+=(const numVector& _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] += _Right[i];
	return (*this);
}
cmf::math::numVector& cmf::math::numVector::operator-=(const numVector& _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] -= _Right[i];
	return (*this);
}
cmf::math::numVector& cmf::math::numVector::operator*=(const numVector& _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] *= _Right[i];
	return (*this);
}
cmf::math::numVector& cmf::math::numVector::operator/=(const numVector& _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data[i] /= _Right[i];
	return (*this);
}

 cmf::math::numVector cmf::math::numVector::operator-() const
 {
 	cmf::math::numVector result(size());
 #pragma omp parallel for
 	for (int i = 0; i < size(); ++i)
 		result[i] = -m_data[i];
 	return result;
 }

void cmf::math::numVector::resize( int count )
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

cmf::math::numVector cmf::math::numVector::apply( real funct(real) ) const
{
	cmf::math::numVector result(size());
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		result[i] = funct(m_data[i]);
	return result;
}


real cmf::math::numVector::dot( const numVector& _Right) const
{
	real res=0;
#pragma omp parallel for reduction(+ : res)
	for (int i = 0; i < size(); ++i)
		res += m_data[i] * _Right[i];
	return res;

}

cmf::math::numVector cmf::math::numVector::power( real exponent ) const
{
	cmf::math::numVector result(size());
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		result[i] = pow(m_data[i],exponent);
	return result;
}

cmf::math::numVector cmf::math::numVector::power( const numVector& exponent) const
{
	cmf::math::numVector result(size());
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		result[i] = pow(m_data[i],exponent[i]);
	return result;
}
real cmf::math::numVector::sum() const
{
  real result=0;
#pragma omp parallel for reduction(+ : result)
	for (int i = 0; i < size() ; ++i)
		result += m_data[i];
	return result;
}

real cmf::math::numVector::max() const
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

real cmf::math::numVector::min() const
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

real cmf::math::numVector::norm( int normtype ) const
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

size_t cmf::math::numVector::adress() const
{
	return size_t(m_data);
}
//
// Binary Operators
//
cmf::math::numVector cmf::math::numVector::operator+(const numVector& _Right )
{
	cmf::math::numVector result(size());
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		result[i] = (*this)[i] + _Right[i];
	return result;
}
cmf::math::numVector cmf::math::numVector::operator-(const numVector& _Right )
{
	cmf::math::numVector result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] - _Right[i];
	return result;
}
cmf::math::numVector cmf::math::numVector::operator*(const numVector& _Right )
{
	cmf::math::numVector result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] * _Right[i];
	return result;
}
cmf::math::numVector cmf::math::numVector::operator/(const numVector& _Right )
{
	cmf::math::numVector result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] / _Right[i];
	return result;
}

cmf::math::numVector cmf::math::numVector::operator+(real _Right )
{
	cmf::math::numVector result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] + _Right;
	return result;
}
cmf::math::numVector cmf::math::numVector::operator-(real _Right )
{
	cmf::math::numVector result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] - _Right;
	return result;
}
cmf::math::numVector cmf::math::numVector::operator*(real _Right )
{
	cmf::math::numVector result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] * _Right;
	return result;
}
cmf::math::numVector cmf::math::numVector::operator/(real _Right )
{
	cmf::math::numVector result((*this).size());
#pragma omp parallel for
	for (int i = 0; i < (*this).size(); ++i)
		result[i] = (*this)[i] / _Right;
	return result;
}

void cmf::math::numVector::set_data_from_adress( long long data_adress,size_t count )
{
	resize(count);
	double * data=(double *)data_adress;
#pragma omp parallel for
	for (int i = 0; i < (int)count ; ++i)
	{
		m_data[i]=data[i];		
	}
}
cmf::math::numVector cmf::math::operator+( real _Left,const numVector& _Right )
{
	cmf::math::numVector result(_Right.size());
#pragma omp parallel for
	for (int i = 0; i < _Right.size(); ++i)
		result[i] = _Left + _Right[i];
	return result;
}
cmf::math::numVector cmf::math::operator-( real _Left,const numVector& _Right )
{
	cmf::math::numVector result(_Right.size());
#pragma omp parallel for
	for (int i = 0; i < _Right.size(); ++i)
		result[i] = _Left - _Right[i];
	return result;
}
cmf::math::numVector cmf::math::operator*( real _Left,const numVector& _Right )
{
	cmf::math::numVector result(_Right.size());
#pragma omp parallel for
	for (int i = 0; i < _Right.size(); ++i)
		result[i] = _Left * _Right[i];
	return result;
}
cmf::math::numVector cmf::math::operator/( real _Left,const numVector& _Right )
{
	cmf::math::numVector result(_Right.size());
#pragma omp parallel for
	for (int i = 0; i < _Right.size(); ++i)
		result[i] = _Left / _Right[i];
	return result;
}

// static numVector* convert_array_interface_to_numvector(PyObject* obj)
// {
// 	if (PyObject_HasAttrString(obj,"__array_interface__"))
// 	{
// 		PyObject* ai=PyObject_GetAttrString(obj,"__array_interface__");
// 		if (PyDict_Check(ai))
// 		{
// 			 PyObject* data=PyDict_GetItemString(obj,"data");
// 			 PyObject* adress=PyLong_AsLongLong(PyTuple_GetItem(data,0));
// 			 PyObject* shape=PyDict_GetItemString(obj,"shape");
// 			 long long size=0;
// 			 for (Py_ssize_t i = 0; i < PyTuple_Size(shape) ; ++i)
// 			 {
// 				 size*=PyTuple_GetItem(shape,i);
// 			 }
// 			 PyObject* typestr=PyDict_GetItemString(obj,"typestr");
// 			 PyObject* version=PyDict_GetItemString(obj,"version");
// 			 		
// 			 
// 
// 		}
// 		Py_XDECREF(ai);
// 	}
// 	return 0;
// }
	
