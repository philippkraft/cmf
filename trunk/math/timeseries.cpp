#include "timeseries.h"
#include <cmath>

double cmf::math::timeseries::position( Time t ) const
{
	double pos=(t-begin)/step;
	if (pos<0)
		return 0;
	else if (pos>m_Values.size()-1)
		return m_Values.size()-1;
	else
		return pos;
}

double cmf::math::timeseries::interpolate( cmf::math::Time t,double n ) const
{
	
	double pos=position(t);

	// If nearest neighbor interpolation return nearest neighbor...
	if (n==0)
	{
		return m_Values[size_t(pos+.5)];
	}
	//If the position is very near to a saved point, return the saved point
	if (pos-int(pos)<0.0001)
		return m_Values[pos];
	else
	{
		int ipos=int(pos);
		if (ipos>=int(m_Values.size())-1) 
			return m_Values[ipos];
		else
		{
			double 
				dpos=pos-ipos,
				w1=n==1 ? 1-dpos : pow(1-dpos,n),
				w2=n==1 ? dpos   : pow(dpos,n);
			if (n!=1)  //normalize
			{
				w1/=w1+w2;
				w2/=w1+w2;
			}
			double y1=m_Values[ipos],y2=m_Values[ipos+1];
			return y1*w1+y2*w2;
		}
	}
}
cmf::math::timeseries& cmf::math::timeseries::operator-=( double _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_Values[i] -= _Right;
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator+=( double _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_Values[i] += _Right;
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator*=( double _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_Values[i] *= _Right;
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator/=( double _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_Values[i] /= _Right;
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator+=(const timeseries& _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_Values[i] += _Right[i];
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator-=(const timeseries& _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_Values[i] -= _Right[i];
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator*=(const timeseries& _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_Values[i] *= _Right[i];
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator/=(const timeseries& _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_Values[i] /= _Right[i];
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::power( double exponent)
{
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; i++)
	{
		m_Values[i]=pow(m_Values[i],exponent);
	}
	return *this;
}



void cmf::math::timeseries::Save(std::ostream& file)
{
	cmf::math::Date b=begin;
	file << b.year << " " << b.month << " " << b.day 
		<< " " << b.hour << " " << b.minute << " " << b.second << " " << b.ms << " ";
	file << step.AsDays() << " ";
	file << this->size() << " " << interpolationpower << std::endl;
	for(std::vector<double>::const_iterator it = m_Values.begin(); it != m_Values.end(); ++it)
		file << *it << " ";
	file << std::endl;
}
cmf::math::timeseries::timeseries(std::istream& file)
{
	int y,mo,d,h,m,s,ms,size;
	file >> y >> mo >> d >> h >> m >> s >> ms;
	begin=cmf::math::Time(d,mo,y,h,m,s,ms);
	double step_days;
	file >> step_days;
	step=cmf::math::Time::Days(step_days);
	file >> size >> interpolationpower;
	m_Values.resize(size);
	double value=0;
	for (int i = 0; i < size ; i++)
	{
		file >> m_Values[i];
	}
}

cmf::math::timeseries cmf::math::timeseries::reduce_min( cmf::math::Time begin,cmf::math::Time step ) const
{
	const cmf::math::timeseries& org=*this;
	if (step<org.step)
	{
		throw std::runtime_error("For reduction methods, the target step size must be greater than the source step size");
	}
	cmf::math::timeseries res(begin,step);
	cmf::math::Time t=begin;
  int pos=0;
	res.Add(org[t]);
	while (t<org.end())
	{
		double v=org[t];
		for (cmf::math::Time t2=t;t2<t+step;t2+=org.step)
			v=v<org[t2] ? v : org[t2];
		res.Add(v);
		t+=step;
	}
	return res;
}

cmf::math::timeseries cmf::math::timeseries::reduce_max( cmf::math::Time begin,cmf::math::Time step ) const
{
	const cmf::math::timeseries& org=*this;
	if (step<org.step)
	{
		throw std::runtime_error("For reduction methods, the target step size must be greater than the source step size");
	}
	cmf::math::timeseries res(begin,step);
	cmf::math::Time t=begin;
	int pos=0;
	while (t<org.end())
	{
		double v=org[t];
		for (cmf::math::Time t2=t;t2<t+step;t2+=org.step)
			v=v>org[t2] ? v : org[t2];
		res.Add(v);
		t+=step;
	}
	return res;
}

cmf::math::timeseries cmf::math::timeseries::reduce_sum( cmf::math::Time begin,cmf::math::Time step ) const
{
	const cmf::math::timeseries& org=*this;
	if (step<org.step)
	{
		throw std::runtime_error("For reduction methods, the target step size must be greater than the source step size");
	}
	cmf::math::timeseries res(begin,step);
	cmf::math::Time t=begin;
	int pos=0;
	while (t<org.end())
	{
		double v=0;
		for (cmf::math::Time t2=t;t2<t+step;t2+=org.step)
			v+=org[t2];
		res.Add(v);
		t+=step;
	}
	return res;
}

cmf::math::timeseries cmf::math::timeseries::reduce_avg( cmf::math::Time begin,cmf::math::Time step ) const
{
	const cmf::math::timeseries& org=*this;
	if (step<org.step)
	{
		throw std::runtime_error("For reduction methods, the target step size must be greater than the source step size");
	}
	cmf::math::timeseries res(begin,step);
	cmf::math::Time t=begin;
	int pos=0;
	while (t<org.end())
	{
		double v=0;int count=0;
		for (cmf::math::Time t2=t;t2<t+step;t2+=org.step)
		{
			v+=org[t2];
			++count;
		}
		res.Add(v/count);
		t+=step;
	}
	return res;

}