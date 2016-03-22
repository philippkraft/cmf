

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
#ifndef irregular_timeseries_h__
#define irregular_timeseries_h__


#include <vector>
#include <algorithm>
#include <iostream>
#include "time.h"
#include "../cmfmemory.h"
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
		///import cmf
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
		class irregular_timeseries
		{
		private:
#ifndef SWIG
			struct item {
				Time t;
				double v;
				item(Time _t, double _v=0.0)
					: t(_t),v(_v) {}
				item(const item& I)
					: t(I.t),v(I.v) {}
				bool operator<(const item& I) {return t<I.t;}
				bool operator==(const item& I) {return t==I.t;}
				bool operator>(const item& I) {return t>I.t;}
				item& operator=(const item& I) {t=I.t;v=I.v;}
			};
			typedef std::vector<item> item_vector;
			class data {
			private:
				item_vector values;
			public:
				void sort() {
					std::sort(values.begin(),values.end());
				}
				double operator[](Time t) const {
					item_vector::iterator lb = std::lower_bound(values.begin(),values.end());
					
					if (lb==values.begin()) {
						// If t is smaller than first item, use first
						return lb->v;
					} else if (lb==values.end()) {
						// if t is bigger than last item, use last item
						return values.back().v;
					} else if (lb->t == t) {
						// if t equals time of an item
						return lb->v;
					} else {
						// else interpolate linear
						item& v1 = *(lb-1);
						item& v2 = *(lb);
						Time dt = v2.t - v1.t;
						double f1 = (t-v1.t)/dt;
						return v1.v * f1 + v2.v * (1-f1);
					}
				}
				double operator[](int pos) const {
					return values.at(pos<0 ? pos+values.size() : pos).v;
				}
				double get(Time t) const { return (*this)[t];}
				double get(int pos) const { return (*this)[pos];}
				double get_time(int pos) const {
					return values.at(pos<0 ? pos+values.size() : pos).t;
				}
				void add(Time t, double value) {
					item_vector::iterator lb = std::lower_bound(values.begin(),values.end());
					if (lb->t == t) {
						lb->v = value;
					} else {
						values.insert(lb,value);
					}
				}
				void set(int i,double value) {
					values.at(i<0 ? i + size() : i) = value;
				}
				void erase(int i) {
					item_vector::iterator it= values.begin() + (i<0 ? i+size() : i);
					if (it<values.end())
						values.erase(it)
					else
						throw std::out_of_range("Position not in irregular timeseries")

				}
				size_t size() const {return values.size();}
			};
			typedef std::shared_ptr<data> data_pointer;
			data_pointer m_data;
			static data_pointer make_data() {
				data* new_data = new data;
				return data_pointer(new_data);
			}
#endif
		public:
			/// First date of measurement
			cmf::math::Time begin() const {
				return m_data->get_time(0);
			}
			/// Last date of measurements
			cmf::math::Time end()	const {
				return m_data->get_time(-1);
			}
			/// Appends a measurement
			void add(Time t,double Value) {
				m_data->add(t,Value);
			}
			/// returns true if no values are added to the timeseries
			bool is_empty() const {return m_data->size()==0;}
			void clear() {make_data();}
			int size() const {return int(m_data->size());}
			
			irregular_timeseries() : m_data(make_data()) 
			{	}

			irregular_timeseries( const cmf::math::irregular_timeseries& its ) 
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
			double get_t(cmf::math::Time t) const {return (*m_data)[t];}
			double get_i(int i) const
			{
				return m_data->get(i);
			}
			void set_t(cmf::math::Time t,double value) {
				m_data->add(t,value);
			}
			void set_i(int i,double value) {
				m_data->set(i,value);
			}

			void remove_nodata(double nodata_value) {
				int i=0;
				while (i<m_data->size()) {
					if (fabs(m_data->get(i) - nodata_value)<1e-15)
						m_data->erase(i);
					else
						++i;
				}
			}
			Time avg_timestep() {
				return (end()-begin())/size();
			}
	}
}
#endif // timeseries_h__
