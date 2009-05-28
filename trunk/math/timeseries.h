#ifndef timeseries_h__
#define timeseries_h__
#include <vector>
#include <iostream>
#include "Time.h"
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
		///# Add data
		///timeseries.Add(0.1) # Value at 2001/5/1 2:30pm is 0.1
		///timeseries.Add(0.2) # Value at 2001/5/1 2:50pm is 0.2
		///timeseries.Add(0.1) # Value at 2001/5/1 3:10pm is 0.1	</pre>
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
				{
				}
			};
#endif
			typedef std::tr1::shared_ptr<timeseries_data> data_pointer;
			data_pointer m_data;
			static data_pointer make_data(Time _begin=day*0,Time _step=day,int _interpolationpower=1);
			double position(cmf::math::Time t) const;
			double interpolate(cmf::math::Time t,double n) const;
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
			double interpolationpower() const {return m_data->interpolationpower;}
			/// Appends a measurement
			void Add(double Value)
			{
				m_data->values.push_back(Value);
			}
			/// returns true if no values are added to the timeseries
			bool isempty() const {return m_data->values.size()==0;}
			void clear()
			{
				m_data->values.clear();
			}

			int size() const {return int(m_data->values.size());}
			/// Constructor of a time series
			/// @param _begin First date of measurement
			/// @param _step Time between measurements
			/// @param _interpolationmethod Method for the interpolation (0 - Nearest neighbor, 1- linear, 2 - cubic spline (not implemented yet)
			timeseries(cmf::math::Time _begin,cmf::math::Time _step,int _interpolationmethod=1) 
				: m_data(make_data(_begin,_step,_interpolationmethod))
			{
			}
			timeseries() : m_data(make_data()) 
			{			}
			timeseries( const cmf::math::timeseries& ts ) : m_data(ts.m_data)
			{	}
			timeseries(double scalar) : m_data(make_data())
			{
				m_data->values.push_back(scalar);
			}


#ifndef SWIG
			/// Returns an interpolated value at time t
			double operator[](cmf::math::Time t) const {return interpolate(t,interpolationpower());}
			/// Returns a reference to the value at position i
			double& operator[](int i)			{
				int ndx=i<0 ? int(m_data->values.size())+i : i;
				return m_data->values.at(ndx);
			}
			double operator[](int i) const			{
				int ndx=i<0 ? int(m_data->values.size())+i : i;
				return m_data->values.at(ndx);
			}
#endif
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
			timeseries& operator+=(const timeseries&);      ///< Add timeseries to this
			timeseries& operator+=(double);                ///< Add scalar to this
			timeseries& operator-=(const timeseries&);      ///< Subtract timeseries from this
			timeseries& operator-=(double);                ///< Subtract scalar from this
			timeseries& operator*=(const timeseries&);      ///< Multiply each element of timeseries with each element of this
			timeseries& operator*=(double);                ///< Multiply each element of timeseries with scalar
			timeseries& operator/=(const timeseries&);      ///< Divide each element of this by each element of timeseries
			timeseries& operator/=(double);                ///< Divide each element of this by scalar
			timeseries& power(double);											 ///< raises the timeseries to a power

			timeseries reduce_min(cmf::math::Time begin,cmf::math::Time step) const; /// Creates a timeseries with a bigger timestep, containing the minimum
			timeseries reduce_max(cmf::math::Time begin,cmf::math::Time step) const; /// Creates a timeseries with a bigger timestep, containing the maximum
			timeseries reduce_sum(cmf::math::Time begin,cmf::math::Time step) const; /// Creates a timeseries with a bigger timestep, containing the sum
			timeseries reduce_avg(cmf::math::Time begin,cmf::math::Time step) const; /// Creates a timeseries with a bigger timestep, containing the average

			//@}
			/// @name I/O
			//@{
			/// Save the Meteorology data to an ASCII File with fixed format
			void Save(std::ostream& file);
			/// Creates a Meterology from a File in fixed format
			/// @code
			/// begin <year> <month> <day> <hour> <minute> <second> <millisecond>
			/// step <year> <month> <day> <hour> <minute> <second> <millisecond>
			/// size <size>	 interpolationpower <interpolationpower>
			/// values <val1> <val2> ...
			/// @endcode
			explicit timeseries(std::istream& file);
			//@}
		};


	}
}
#ifdef SWIG
	%extend cmf::math::timeseries
	{
		double __size__()
		{
			return $self->size();
		}
		double __getitem__(cmf::math::Time t)
		{
			return (*$self)[t];
		}
		double __getitem__(int i)
		{
			return (*$self)[i];
		}
		void __setitem__(int i,double value)
		{
			 (*$self)[i]=value;
		}
		%pythoncode
    {
        def AddList(list) :
            """ Adds the values of a list to the timeseries"""
            for item in list :
                self.Add(float(item))
    }
	}
#endif
#endif // timeseries_h__
