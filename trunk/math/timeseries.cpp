#include "timeseries.h"
#include <cmath>
#include "real.h"
#include <stdexcept>
double cmf::math::timeseries::position( Time t ) const
{
	double pos=(t-begin())/step();
	if (pos<0)
		return 0;
	else if (pos>m_data->values.size()-1)
		return m_data->values.size()-1;
	else
		return pos;
}

double cmf::math::timeseries::interpolate( cmf::math::Time t,double n ) const
{
	if (is_empty() || !m_data)
		throw std::out_of_range("Time series is empty");
	double pos=position(t);

	// If nearest neighbor interpolation return nearest neighbor...
	if (n==0)
	{
		return m_data->values[size_t(pos+.5)];
	}
	//If the position is very near to a saved point, return the saved point
	if (pos-int(pos)<0.0001)
		return m_data->values[pos];
	else
	{
		int ipos=int(pos);
		if (ipos>=int(m_data->values.size())-1) 
			return m_data->values[ipos];
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
			double y1=m_data->values[ipos],y2=m_data->values[ipos+1];
			return y1*w1+y2*w2;
		}
	}
}
cmf::math::timeseries& cmf::math::timeseries::operator-=( double _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data->values[i] -= _Right;
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator+=( double _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data->values[i] += _Right;
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator*=( double _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data->values[i] *= _Right;
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator/=( double _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data->values[i] /= _Right;
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator+=(timeseries _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data->values[i] += _Right[i];
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator-=(timeseries _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data->values[i] -= _Right[i];
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator*=(timeseries _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data->values[i] *= _Right[i];
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator/=(timeseries _Right )
{
#pragma omp parallel for
	for (int i = 0; i < size(); ++i)
		m_data->values[i] /= _Right[i];
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::power( double exponent)
{
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; i++)
	{
		m_data->values[i]=pow(m_data->values[i],exponent);
	}
	return *this;
}



void cmf::math::timeseries::Save(std::ostream& file)
{
	cmf::math::Date b=begin();
	file << b.year << " " << b.month << " " << b.day 
		<< " " << b.hour << " " << b.minute << " " << b.second << " " << b.ms << " ";
	file << step().AsDays() << " ";
	file << this->size() << " " << interpolationpower() << std::endl;
	for(std::vector<double>::const_iterator it = m_data->values.begin(); it != m_data->values.end(); ++it)
		file << *it << " ";
	file << std::endl;
}
cmf::math::timeseries::timeseries(std::istream& file)
{
	int y,mo,d,h,m,s,ms,size;
	file >> y >> mo >> d >> h >> m >> s >> ms;
	Time begin=cmf::math::Time(d,mo,y,h,m,s,ms);
	double step_days;
	file >> step_days;
	Time step=cmf::math::Time::Days(step_days);
	int interpolationpower;
	file >> size >> interpolationpower;
	m_data=make_data(begin,step,interpolationpower);
	double value=0;
	for (int i = 0; i < size ; i++)
	{
		file >> m_data->values[i];
	}
}

cmf::math::timeseries cmf::math::timeseries::reduce_min( cmf::math::Time begin,cmf::math::Time step ) const
{
	const cmf::math::timeseries& org=*this;
	if (step<org.step())
	{
		throw std::runtime_error("For reduction methods, the target step size must be greater than the source step size");
	}
	cmf::math::timeseries res(begin,step);
	cmf::math::Time t=begin;
  int pos=0;
	res.add(org[t]);
	while (t<org.end())
	{
		double v=org[t];
		for (cmf::math::Time t2=t;t2<t+step;t2+=org.step())
			v=v<org[t2] ? v : org[t2];
		res.add(v);
		t+=step;
	}
	return res;
}

cmf::math::timeseries cmf::math::timeseries::reduce_max( cmf::math::Time begin,cmf::math::Time step ) const
{
	const cmf::math::timeseries& org=*this;
	if (step<org.step())
	{
		throw std::runtime_error("For reduction methods, the target step size must be greater than the source step size");
	}
	cmf::math::timeseries res(begin,step);
	cmf::math::Time t=begin;
	int pos=0;
	while (t<org.end())
	{
		double v=org[t];
		for (cmf::math::Time t2=t;t2<t+step;t2+=org.step())
			v=v>org[t2] ? v : org[t2];
		res.add(v);
		t+=step;
	}
	return res;
}

cmf::math::timeseries cmf::math::timeseries::reduce_sum( cmf::math::Time begin,cmf::math::Time step ) const
{
	const cmf::math::timeseries& org=*this;
	if (step<org.step())
	{
		throw std::runtime_error("For reduction methods, the target step size must be greater than the source step size");
	}
	cmf::math::timeseries res(begin,step);
	cmf::math::Time t=begin;
	int pos=0;
	while (t<org.end())
	{
		double v=0;
		for (cmf::math::Time t2=t;t2<t+step;t2+=org.step())
			v+=org[t2];
		res.add(v);
		t+=step;
	}
	return res;
}

cmf::math::timeseries cmf::math::timeseries::reduce_avg( cmf::math::Time begin,cmf::math::Time step ) const
{
	const cmf::math::timeseries& org=*this;
	if (step<org.step())
	{
		throw std::runtime_error("For reduction methods, the target step size must be greater than the source step size");
	}
	cmf::math::timeseries res(begin,step);
	cmf::math::Time t=begin;
	int pos=0;
	while (t<org.end())
	{
		double v=0;int count=0;
		for (cmf::math::Time t2=t;t2<t+step;t2+=org.step())
		{
			v+=org[t2];
			++count;
		}
		res.add(v/count);
		t+=step;
	}
	return res;

}

cmf::math::timeseries::data_pointer cmf::math::timeseries::make_data( Time _begin/*=day*0*/,Time _step/*=day*/,int _interpolationpower/*=1*/ )
{
	timeseries_data* new_data=new timeseries_data(_begin,_step,_interpolationpower);
	return data_pointer(new_data);
}

cmf::math::timeseries cmf::math::timeseries::get_slice( cmf::math::Time _begin,cmf::math::Time _end,cmf::math::Time _step )
{
	if (!_step.is_not_0())
		_step=step();
	timeseries res(_begin,_step);
	for (Time t=_begin;t<_end;t+=_step)
		res.add(this->get_t(t));

	return res;
}

cmf::math::timeseries cmf::math::timeseries::get_slice( int _begin,int _end,int _step/*=1*/ )
{
	timeseries res(time_at_position(_begin),step()*_step);
	for (int i = (_begin<0?_begin+size():_begin); i < (_end > size() ? size() : _end); i+=_step)
	{
		res.add(this->get_i(i));
	}
	return res;
}
void cmf::math::timeseries::set_slice( cmf::math::Time _begin,cmf::math::Time _end,cmf::math::timeseries _values )
{
	for (Time t=_begin;t<_end;t+=_values.step() < step() ? _values.step() : step())
		set_t(t,_values.get_t(t));
}

void cmf::math::timeseries::set_slice( int _begin,int _end,cmf::math::timeseries _values )
{
	for (int i = (_begin<0?_begin+size():_begin); i < (_end > size() ? size() : _end); ++i)
	{
		set_i(i,_values.get_t(time_at_position(i)));
	}
}
double cmf::math::timeseries::mean() const
{
	double sum=0;
#pragma omp parallel for reduction(+ : sum)
	for (int i = 0; i < size() ; ++i)
		sum += m_data->values[i];
	return sum/size();
}

cmf::math::timeseries cmf::math::timeseries::operator+( timeseries other ) const 
{ 
	timeseries res(minimum_t(begin(),other.begin()),minimum_t(step(),other.step()));      
	for (Time t=res.begin();t<=maximum_t(end(),other.end());t+=res.step())						
		res.add(get_t(t) + other.get_t(t));																						
	return res; 
}
cmf::math::timeseries cmf::math::timeseries::operator-( timeseries other ) const 
{ 
	timeseries res(minimum_t(begin(),other.begin()),minimum_t(step(),other.step()));      
	for (Time t=res.begin();t<=maximum_t(end(),other.end());t+=res.step())						
		res.add(get_t(t) - other.get_t(t));																						
	return res; 
}
cmf::math::timeseries cmf::math::timeseries::operator*( timeseries other ) const 
{ 
	timeseries res(minimum_t(begin(),other.begin()),minimum_t(step(),other.step()));      
	for (Time t=res.begin();t<=maximum_t(end(),other.end());t+=res.step())						
		res.add(get_t(t) * other.get_t(t));																						
	return res; 
}
cmf::math::timeseries cmf::math::timeseries::operator/( timeseries other ) const 
{ 
	timeseries res(minimum_t(begin(),other.begin()),minimum_t(step(),other.step()));      
	for (Time t=res.begin();t<=maximum_t(end(),other.end());t+=res.step())						
		res.add(get_t(t) / other.get_t(t));																						
	return res; 
}

cmf::math::timeseries cmf::math::timeseries::operator+( double other ) const {  
	timeseries res(begin(),step());																								
	for (Time t=res.begin();t<=end();t+=res.step())						
		res.add(get_t(t) + other);	
	return res; 
}																																										
cmf::math::timeseries cmf::math::timeseries::operator-( double other ) const {  
	timeseries res(begin(),step());																								
	for (Time t=res.begin();t<=end();t+=res.step())						
		res.add(get_t(t) - other);	
	return res; 
}																																										
cmf::math::timeseries cmf::math::timeseries::operator*( double other ) const {  
	timeseries res(begin(),step());																								
	for (Time t=res.begin();t<=end();t+=res.step())						
		res.add(get_t(t) * other);	
	return res; 
}																																										
cmf::math::timeseries cmf::math::timeseries::operator/( double other ) const {  
	timeseries res(begin(),step());																								
	for (Time t=res.begin();t<=end();t+=res.step())						
		res.add(get_t(t) / other);	
	return res; 
}																																										

cmf::math::timeseries cmf::math::timeseries::operator -() const {
	timeseries res(begin(),step());																									
	for (int i = 0; i < size() ; ++i)
	{
		res.add(-get_i(i));
	}
	return res;
}

cmf::math::timeseries cmf::math::timeseries::inv() const
{
	timeseries res(begin(),step());																									
	for (int i = 0; i < size() ; ++i)
	{
		res.add(1/get_i(i));
	}
	return res;

}

cmf::math::timeseries cmf::math::timeseries::floating_avg( cmf::math::Time window_width ) const
{
	cmf::math::timeseries res(begin(),step());
	cmf::math::Time half_window=window_width/2;
	for (Time t=res.begin();t<=end();t+=res.step())
	{
		double sum=0;
		size_t i=0;
		for (Time tsub=t-half_window;tsub<=t+half_window;t+=step())
		{
			++i;
			sum+=get_t(tsub);
		}
		res.add(sum/i);
	}
	return res;
}

cmf::math::timeseries cmf::math::timeseries::floating_max( cmf::math::Time window_width ) const
{
	cmf::math::timeseries res(begin(),step());
	cmf::math::Time half_window=window_width/2;
	for (Time t=res.begin();t<=end();t+=res.step())
	{
		double value=get_t(t);
		for (Time tsub=t-half_window;tsub<=t+half_window;t+=step())
		{
			value=maximum(value,get_t(tsub));
		}
		res.add(value);
	}
	return res;

}
cmf::math::timeseries cmf::math::timeseries::floating_min( cmf::math::Time window_width ) const
{
	cmf::math::timeseries res(begin(),step());
	cmf::math::Time half_window=window_width/2;
	for (Time t=res.begin();t<=end();t+=res.step())
	{
		double value=get_t(t);
		for (Time tsub=t-half_window;tsub<=t+half_window;t+=step())
		{
			value=minimum(value,get_t(tsub));
		}
		res.add(value);
	}
	return res;
}



double cmf::math::nash_sutcliff(const cmf::math::timeseries& model,const cmf::math::timeseries& observation)
{
	double mean_2=observation.mean();
	double sq_sum_diff=0,sq_sum_ind=0;
	cmf::math::Time begin=maximum_t(model.begin(),observation.begin());
	cmf::math::Time end=minimum_t(model.begin(),observation.begin());
	cmf::math::Time step=maximum_t(model.begin(),observation.begin());
	for (cmf::math::Time t=begin;t<=end;t+=step)
	{
		sq_sum_diff+=square(model[t]-observation[t]);
		sq_sum_ind+=square(observation[t]-mean_2);
	}
	return 1-sq_sum_diff/sq_sum_ind;
}

double cmf::math::R2( const cmf::math::timeseries& model,const cmf::math::timeseries& observation )
{
	double mean_y=observation.mean();
	double numerator=0,denominator=0;
	cmf::math::Time begin=maximum_t(model.begin(),observation.begin());
	cmf::math::Time end=minimum_t(model.begin(),observation.begin());
	cmf::math::Time step=maximum_t(model.begin(),observation.begin());
	for (cmf::math::Time t=begin;t<=end;t+=step)
	{
		numerator+=square(model[t]-mean_y);
		denominator=square(observation[t]-mean_y);
	}
	return numerator/denominator;


}
