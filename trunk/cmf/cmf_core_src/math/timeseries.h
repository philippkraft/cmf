

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
#ifndef timeseries_h__
#define timeseries_h__
#include <vector>
#include <iostream>
#include "time.h"
#include <tr1/memory>
namespace cmf {
	namespace math {
		///	A timeseries is a list of values, equally distributed over time. To create one,
		///	one have to provide as start date and a step size. The end time is calculated from
		///	the number of values. Values queried for times before the start time are returned
		///	as the first item, values after the end time equal the last item. A timeseries with
		///	only one item reacts like a scalar value.
		///	<h3>
		///		Creating a time series</h3>
		///	<div class="fragment">
		///		<pre class="fragment">
		///import CMFLib as cmf
		///# Start date is the January 5th 2001 at 2:30 pm
		///start=cmf.Time(5,1,2001,14,30)                      
		///# time step of the timeseries is 20 minutes
		///step=cmf.min*20
		///# Type of interpolation between values
		///# 0 - Nearest neighbor,
		///# 1 - Linear,
		///# 2 - Squared,
		///# 3 - Cubic, etc.
		///interpolation=1                                     
		///# Create timeseries
		///timeseries=cmf.timeseries(start,step,interpolation) 
		///# add data
		///timeseries.add(0.1) # Value at 2001/5/1 2:30pm is 0.1
		///timeseries.add(0.2) # Value at 2001/5/1 2:50pm is 0.2
		///timeseries.add(0.1) # Value at 2001/5/1 3:10pm is 0.1	</pre>
		///	</div>
		///	<p>
		///		With this technic it is simple to read files or databases to fill timeseries.</p>
		///	<h3>
		///		Using a timeseries</h3>
		///	<div class="fragment">
		///		<pre class="fragment">
		///# Query every minute between 2:15 and 3:14 pm
		///for t in cmf.timerange(start,start+cmf.h,cmf.min):
		///    print "Time:",t.AsDate(),"Value:",timeseries[t]
		///# Query a specific position of the timeseries
		///print timeseries[2]</pre>
		///	</div>
		class timeseries
		{
		private:
#ifndef SWIG
			struct timeseries_data {
				std::vector<double> values;
				Time begin;
				Time step;
				int interpolationpower;
				timeseries_data(Time _begin,Time _step,int _interpolationpower) 
					: begin(_begin),step(_step),interpolationpower(_interpolationpower)
				{				}
				timeseries_data(const timeseries_data& for_copy)
					: begin(for_copy.begin), step(for_copy.step), interpolationpower(for_copy.interpolationpower), values(for_copy.values)
				{				}
			};
#endif
			typedef std::tr1::shared_ptr<timeseries_data> data_pointer;
			data_pointer m_data;
			static data_pointer make_data(Time _begin=day*0,Time _step=day,int _interpolationpower=1);
			double position(cmf::math::Time t) const;
			double interpolate(cmf::math::Time t,double n) const;
			cmf::math::Time time_at_position(double pos) const
			{
				return begin()+step()*(pos<0 ? size()+pos : pos);
			}
			cmf::math::Time time_at_position(int pos) const
			{
				return begin()+step()*(pos<0 ? size()+pos : pos);
			}
			/// Order of the interpolation
		protected:
		public:
			/// First date of measurement
			cmf::math::Time begin() const {return m_data->begin;}
			/// Time between the measurements
			cmf::math::Time step() const {return m_data->step;}
			/// Last date of measurements
			cmf::math::Time end()	const		{return begin()+step()*int(m_data->values.size());}
			/// Method for the interpolation (0 - Nearest neighbor, 1- linear, 2 - cubic spline (not implemented yet)
			int interpolationpower() const {return m_data->interpolationpower;}
			/// Appends a measurement
			void add(double Value);
			/// returns true if no values are added to the timeseries
			bool is_empty() const {return m_data->values.size()==0;}
			void clear();
			size_t adress() const
			{
				return size_t(&(m_data->values[0]));
			}
			timeseries copy() const;
			int size() const {return int(m_data->values.size());}
			/// Constructor of a time series
			/// @param _begin First date of measurement
			/// @param _step Time between measurements
			/// @param _interpolationmethod Method for the interpolation (0 - Nearest neighbor, 1- linear, 2 - cubic spline (not implemented yet)
			timeseries(cmf::math::Time _begin,cmf::math::Time _step,int _interpolationmethod=1) 
				: m_data(make_data(_begin,_step,_interpolationmethod))
			{ }
			timeseries() : m_data(make_data()) 
			{	}
			timeseries( const cmf::math::timeseries& ts ) 
				//: m_data(ts.m_data)
			{
				m_data=ts.m_data;
			}
			timeseries(double scalar) : m_data(make_data())
			{
				m_data->values.push_back(scalar);
			}


#ifndef SWIG
			/// Returns an interpolated value at time t
			double operator[](cmf::math::Time t) const {return get_t(t);}
			double operator[](cmf::math::Time t) {return get_t(t);}
			/// Returns a reference to the value at position i
			double& operator[](int i){
				return m_data->values.at(i<0 ? size()+i :i);
			}
			const double& operator[](int i) const			{
				return m_data->values.at(i<0 ? size()+i :i);
			}
#endif
			double get_t(cmf::math::Time t) const {return interpolate(t,interpolationpower());}
			double get_i(int i) const
			{
				return m_data->values.at(i<0 ? size()+i :i);
			}
			void set_t(cmf::math::Time t,double value);
			void set_i(int i,double value);
			cmf::math::timeseries get_slice(cmf::math::Time _begin,cmf::math::Time _end,cmf::math::Time _step=cmf::math::Time());
			void set_slice(cmf::math::Time _begin,cmf::math::Time _end, cmf::math::timeseries values);
			cmf::math::timeseries get_slice(int _begin,int _end,int step=1);
			void set_slice(int _begin,int _end,cmf::math::timeseries _values);

			void remove_nodata(double nodata_value);

			
			/// @name Operators
			/// Binary operators defined as free operators:
			///
			/// x = {+,-,*,/}
			///
			/// Defined for (x is one of the operators above):
			/// - timeseries = timeseries x timeseries 
			/// - timeseries = double x timeseries
			/// - timeseries = timeseries x double
			//@{
			timeseries& operator+=(timeseries);      ///< add timeseries to this
			timeseries& operator+=(double);                ///< add scalar to this
			timeseries& operator-=(timeseries);      ///< Subtract timeseries from this
			timeseries& operator-=(double);                ///< Subtract scalar from this
			timeseries& operator*=(timeseries);      ///< Multiply each element of timeseries with each element of this
			timeseries& operator*=(double);                ///< Multiply each element of timeseries with scalar
			timeseries& operator/=(timeseries);      ///< Divide each element of this by each element of timeseries
			timeseries& operator/=(double);                ///< Divide each element of this by scalar

			timeseries operator+(timeseries) const;
			timeseries operator-(timeseries other) const;
			timeseries operator*(timeseries other) const;
			timeseries operator/(timeseries other) const;

			timeseries operator+(double other) const;
			timeseries operator-(double other) const;
			timeseries operator*(double other) const;
			timeseries operator/(double other) const;

			timeseries operator-() const;
			timeseries inv() const;


			timeseries reduce_min(cmf::math::Time begin,cmf::math::Time step) const; /// Creates a timeseries with a bigger timestep, containing the minimum
			timeseries reduce_max(cmf::math::Time begin,cmf::math::Time step) const; /// Creates a timeseries with a bigger timestep, containing the maximum
			timeseries reduce_sum(cmf::math::Time begin,cmf::math::Time step) const; /// Creates a timeseries with a bigger timestep, containing the sum
			timeseries reduce_avg(cmf::math::Time begin,cmf::math::Time step) const; /// Creates a timeseries with a bigger timestep, containing the average

			timeseries floating_avg(cmf::math::Time window_width) const;
			timeseries floating_max(cmf::math::Time window_width) const;
			timeseries floating_min(cmf::math::Time window_width) const;

			double mean() const;
			double min() const;
			double max() const;

			timeseries log() const;
			timeseries log10() const;
			timeseries power(double exponent) const;
			timeseries exp() const;



		};
		double nash_sutcliff(const cmf::math::timeseries& model,const cmf::math::timeseries& observation);
		double R2(const cmf::math::timeseries& model,const cmf::math::timeseries& observation);

	}
}
#endif // timeseries_h__
