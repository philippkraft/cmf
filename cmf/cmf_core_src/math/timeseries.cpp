

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
#include "timeseries.h"
#include <cmath>
#include "real.h"
#include <stdexcept>
#include <limits>
#include <iostream>
#include <fstream>
#include <algorithm>
inline bool isfinite(real v)
{
	typedef  std::numeric_limits<real> limit;
	return 
		v != limit::infinity() &&
		v != -limit::infinity() &&
		v == v;
}
double cmf::math::timeseries::position( Time t ) const
{
	double pos=(t-begin())/step();
	if (pos<0)
		return 0;
	else if (pos>m_data->values.size()-1)
		return double(m_data->values.size()-1);
	else
		return pos;
}

double cmf::math::timeseries::interpolate( cmf::math::Time t,double n ) const
{
	if (is_empty() || !m_data)
		throw std::runtime_error("Time series is empty");
	double pos=position(t);

	// If nearest neighbor interpolation return nearest neighbor...
	if (n==0)
	{
		return m_data->values[size_t(pos+.5)];
	}
	//If the position is very near to a saved point, return the saved point
	if (pos-ptrdiff_t(pos)<0.0001)
		return m_data->values[size_t(pos)];
	else
	{
		ptrdiff_t ipos=ptrdiff_t(pos);
		if (ipos>=ptrdiff_t(m_data->values.size())-1) 
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
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()); ++i)
		m_data->values[i] -= _Right;
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator+=( double _Right )
{
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()); ++i)
		m_data->values[i] += _Right;
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator*=( double _Right )
{
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()); ++i)
		m_data->values[i] *= _Right;
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator/=( double _Right )
{
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()); ++i)
		m_data->values[i] /= _Right;
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator+=(timeseries _Right )
{
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()); ++i) {
		Time t = time_at_position(i);
		m_data->values[i] += _Right[t];
	}
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator-=(timeseries _Right )
{
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()); ++i)
		m_data->values[i] -= _Right[time_at_position(i)];
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator*=(timeseries _Right )
{
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()); ++i)
		m_data->values[i] *= _Right[time_at_position(i)];
	return (*this);
}
cmf::math::timeseries& cmf::math::timeseries::operator/=(timeseries _Right )
{
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()); ++i)
		m_data->values[i] /= _Right[time_at_position(i)];
	return (*this);
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
  ptrdiff_t pos=0;
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
	ptrdiff_t pos=0;
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
	ptrdiff_t pos=0;
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
	ptrdiff_t pos=0;
	while (t<org.end())
	{
		double v=0;ptrdiff_t count=0;
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

cmf::math::timeseries cmf::math::timeseries::get_slice( ptrdiff_t _begin,ptrdiff_t _end,ptrdiff_t _step/*=1*/ )
{
	timeseries res(time_at_position(_begin),step()*(long long)(_step));
	for (ptrdiff_t i = (_begin<0?_begin+ptrdiff_t(size()):_begin); i < (_end > ptrdiff_t(size()) ? ptrdiff_t(size()) : _end); i+=_step)
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

void cmf::math::timeseries::set_slice( ptrdiff_t _begin,ptrdiff_t _end,cmf::math::timeseries _values )
{
	for (ptrdiff_t i = (_begin<0?_begin+ptrdiff_t(size()):_begin); i < (_end > ptrdiff_t(size()) ? ptrdiff_t(size()) : _end); ++i)
	{
		set_i(i,_values.get_t(time_at_position(i)));
	}
}

cmf::math::timeseries cmf::math::timeseries::operator+( timeseries other ) const 
{ 
	timeseries res = this->copy();      
	res += other;
	return res; 
}
cmf::math::timeseries cmf::math::timeseries::operator-( timeseries other ) const 
{ 
	timeseries res = this->copy();      
	res -= other;
	return res; 
}
cmf::math::timeseries cmf::math::timeseries::operator*( timeseries other ) const 
{ 
	timeseries res = this->copy();      
	res *= other;
	return res; 
}
cmf::math::timeseries cmf::math::timeseries::operator/( timeseries other ) const 
{ 
	timeseries res = this->copy();      
	res /= other;
	return res; 
}

cmf::math::timeseries cmf::math::timeseries::operator+( double other ) const {  
	timeseries res = this->copy();      
	res += other;
	return res; 
}																																										
cmf::math::timeseries cmf::math::timeseries::operator-( double other ) const {  
	timeseries res = this->copy();      
	res -= other;
	return res; 
}																																										
cmf::math::timeseries cmf::math::timeseries::operator*( double other ) const {  
	timeseries res = this->copy();      
	res *= other;
	return res; 
}																																										
cmf::math::timeseries cmf::math::timeseries::operator/( double other ) const {  
	timeseries res = this->copy();      
	res /= other;
	return res; 
}																																										

cmf::math::timeseries cmf::math::timeseries::operator -() const {
	timeseries res = this->copy();      
	res *= -1;
	return res; 
}

cmf::math::timeseries cmf::math::timeseries::inv() const
{
	timeseries res(begin(),step());																									
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()) ; ++i)
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
		for (Time tsub=t-half_window;tsub<=t+half_window;tsub+=step())
		{
			++i;
			sum+=get_t(tsub);
		}
		res.add(sum/i);
	}
	return res;
}
inline static double array_mean(std::vector<double>::iterator begin, size_t size) {
	double sum=0;
	for (size_t i = 0; i < size ; ++i)
		sum+=*begin++;
	return sum/size;
}
cmf::math::timeseries cmf::math::timeseries::floating_avg( size_t window_size ) const
{
	cmf::math::timeseries res(begin(),step(),interpolationpower(),size());
	std::vector<double> window_content(window_size, get_i(0));
	size_t half_size=window_size/2;
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()); ++i)
	{
		window_content[i % window_size] = get_i(i);
		res.set_i(i,array_mean(window_content.begin(),window_size));
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
		for (Time tsub=t-half_window;tsub<=t+half_window;tsub+=step())
		{
			value=std::max(value,get_t(tsub));
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
		for (Time tsub=t-half_window;tsub<=t+half_window;tsub+=step())
		{
			value=std::min(value,get_t(tsub));
		}
		res.add(value);
	}
	return res;
}

void cmf::math::timeseries::add( double Value )
{
	m_data->values.push_back(Value);
}

void cmf::math::timeseries::clear()
{
	m_data->values.clear();
}


double cmf::math::timeseries::mean() const
{
	double sum=0;
	ptrdiff_t count=size();
	double v=0;
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()) ; ++i)
	{
		v = m_data->values[i];
		if (isfinite(v))
			sum += v;
		else
			--count;
	}
	return sum/count;
}

double cmf::math::timeseries::min() const
{
	double _min=get_i(0);
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()) ; ++i)
	{
		if (_min>m_data->values[i] && isfinite(m_data->values[i]))
			_min=m_data->values[i];
	}
	return _min;
}

double cmf::math::timeseries::max() const
{
	double _max=get_i(0);
	for (ptrdiff_t i = 0; i < ptrdiff_t(size()) ; ++i)
	{
		if (_max<m_data->values[i] && isfinite(m_data->values[i]))
			_max=m_data->values[i];
	}
	return _max;

}

cmf::math::timeseries cmf::math::timeseries::copy() const
{
	timeseries res;
	timeseries_data* new_data = new timeseries_data(*m_data);
	res.m_data.reset(new_data);
	return res;
}

cmf::math::timeseries cmf::math::timeseries::log() const
{
	timeseries res(begin(),step());
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
		res.add(std::log(get_i(i)));
	return res;
}
cmf::math::timeseries cmf::math::timeseries::log10() const
{
	timeseries res(begin(),step());
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
		res.add(std::log10(get_i(i)));
	return res;
}
cmf::math::timeseries cmf::math::timeseries::exp() const
{
	timeseries res(begin(),step());
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
		res.add(std::exp(get_i(i)));
	return res;
}
cmf::math::timeseries cmf::math::timeseries::power(double exponent) const
{
	timeseries res(begin(),step());
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
		res.add(pow(get_i(i),exponent));
	return res;
}
inline bool is_nodata(double value,double nodata_value) { return fabs(value-nodata_value)<1e-15;}
void cmf::math::timeseries::remove_nodata( double nodata_value )
{
	size_t i =0;
	while (i<size())	{
		double current_value = this->get_i(i);
		double next_value=nodata_value;
		double first_value=nodata_value;
		if (is_nodata(current_value,nodata_value))	{
			size_t j=i;
			while (j<size() && is_nodata(get_i(j),nodata_value)) ++j;
			if (j<size()) 
				next_value = get_i(j);
			else if (i) 
				next_value = get_i(i-1);
			else
				throw std::runtime_error("Timeseries contains only no data values. Can't remove them.");

			if (i) // use value before nodat occurance as left side value for interpolation
				first_value = get_i(i-1);
			else // use constant right value for interpolation
				first_value = next_value;
			// interpolate linear
			for (size_t k = i; k < j; ++k) {
				double f_next = double(k-(i-1))/double(j-(i-1));
				set_i(k, f_next * next_value + (1-f_next) * first_value);
			}
			i=j;
		} else {
			// Goto next value
			++i;
		}
	}
}

void cmf::math::timeseries::set_i( ptrdiff_t i,double value )
{
	ptrdiff_t ndx = i;
	if (ndx<0) ndx = size() + i;
	if (ndx>=ptrdiff_t(size())) throw std::out_of_range("Index is out of range of the timeseries");
	double & d=m_data->values[ndx];
	d=value;
}

void cmf::math::timeseries::set_t( cmf::math::Time t,double value )
{
	ptrdiff_t pos=ptrdiff_t(position(t)+0.5);
	set_i(pos,value);
}

cmf::math::timeseries::timeseries( cmf::math::Time _begin,cmf::math::Time _step,int _interpolationmethod/*=1*/,size_t size/*=0*/ ) 
: m_data(make_data(_begin,_step,_interpolationmethod))
{
	if (size) 
		m_data->values = std::vector<double>(size,0.0);
}

cmf::math::timeseries cmf::math::timeseries::from_array( cmf::math::Time _begin, cmf::math::Time _step, const cmf::math::num_array& _data )
{
	timeseries result(_begin,_step);
	result.m_data->values = std::vector<double>(_data.begin(),_data.end());
	return result;
}

size_t cmf::math::timeseries::count_values() const
{
	size_t count=0;
	for (size_t i = 0; i <  size(); ++i)
		if (isfinite(m_data->values[i])) ++count;
	return count;
}

cmf::math::timeseries cmf::math::timeseries::from_file( std::string filename )
{
	std::ifstream file;
	file.open(filename.c_str(),std::ios::binary);
	// 4 int64 describe the metadata of the timeseries (size,begin,step and interpolation shape)
	long long metadata[4];
	file.read((char*)metadata,32);
	double buf;
	// Create timeseries from meta data
	timeseries ts(ms*metadata[1], // begin
		ms*metadata[2], // step
		int(metadata[3]), // interpolation shape 
		ptrdiff_t(metadata[0])); // size
	// Read the data from file
	for (ptrdiff_t i = 0; i < metadata[0] ; ++i)
	{
		file.read((char*)(&buf),8);
		ts[i] = buf;
	}
	return ts;
}
double cmf::math::nash_sutcliffe(const cmf::math::timeseries& model,const cmf::math::timeseries& observation)
{
	double mean_obs=observation.mean();
	double sq_sum_diff=0,sq_sum_ind=0;
	cmf::math::Time begin=std::max(model.begin(),observation.begin());
	cmf::math::Time end=std::min(model.end(),observation.end());
	cmf::math::Time step=observation.step();
	double model_t, obs_t;
	for (cmf::math::Time t=begin;t<=end;t+=step)
	{
		model_t = model[t];
		obs_t  = observation[t];
		if (isfinite(model_t) && isfinite(obs_t))
		{
			sq_sum_diff+=square(model_t-obs_t);
			sq_sum_ind+=square(obs_t-mean_obs);
		}
	}
	return 1-sq_sum_diff/sq_sum_ind;
}

