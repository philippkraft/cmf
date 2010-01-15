

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
#ifndef Time_h__
#define Time_h__

#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#define JULIANDAY_0_1_1900 2415019
namespace cmf {
	namespace math {
		struct Date;
		/// A time class, used to pass around current modelling times
		///
		///	<p>
		///		Timespans and dates in cmf are used with a special object, called Time. The advantage
		///		of an extra class has the advantage, that the user does not has to rememember, which
		///		unit of time he or she uses or what time unit is accepted by a specific function
		///		of the model. Arithmetic and boolean operators are supported by Time. Internally
		///		the time classes stores the time as integer milliseconds, therefore rounding issues
		///		will only appear at very small time ranges. Absolute time (like dates) are represented
		///		as milliseconds gone by from Dec, 31st 1899. Microsoft Excel dates are represented
		///		as days from that time, using floating point numbers, therefore it is very simple
		///		to convert Excel time representations to cmf time.
		///	</p>
		///	<p>
		///		Another object is Date, which is doesn't provide the operators, but has a nice printed
		///		version and some special date functions, like day of year (DOY) and provides access
		///		to the current hour of day and so on, which only applyto dates and not to time spans.
		///		You can convert Time to Date an vice versa. The printing is not culture aware and
		///		uses the European representation. If you use the Python standard library datetime,
		///		conversion between Python time and cmf time is possible</p>
		///	<h2>
		///		Creating absolute time values (dates)</h2>
		///	<div class="fragment">
		///		<pre class="fragment">import CMFlib as cmf
		///# Create the time: Jan, 5th 2001, 2:30 pm, 20s, 412 ms
		///t=cmf.Time(5,1,2001,14,30,20,412)
		///print t # Prints: '36896d:14:30:20.412h'
		///print t.AsDate() # Prints: '05.01.2001 14:30:20.412'
		///</pre>
		///	</div>
		///	<h2>
		///		Creating time spans</h2>
		///	<p>
		///		In principle, there are three ways to create time spans. One is to use one of the
		///		static functions, another is to multiply an existing time span (like one of the
		///		build in constants) or to substrate two absolute times.
		///	</p>
		///	<h3>
		///		Available static functions, the default value is always 1</h3>
		///	<ul>
		///		<li>Milliseconds(): <code>t=cmf.Time.Milliseconds(4100)</code>, create a timespan
		///			of 4.1 seconds</li>
		///		<li>Seconds(): <code>t=cmf.Time.Seconds(4.1)</code>, create a timespan of 4.1 seconds</li>
		///		<li>Minutes(): <code>t=cmf.Time.Minutes(138)</code>, create a timespan of 2.3 hours
		///			(138 min)</li>
		///		<li>Hours():<code>t=cmf.Time.Hours(2.3)</code>, create a timespan of 2.3 hours (138
		///			min)</li>
		///		<li>Days():<code>t=cmf.Time.Days(2.5)</code>, create a timespan of 60 hours</li>
		///		<li>Years():<code>t=cmf.Time.Years()</code>, create a timespan of 365 days</li>
		///	</ul>
		///	<h3>
		///		Available constants</h3>
		///	<ul>
		///		<li><code>cmf.sec * 4.1</code>: 4.1 seconds</li>
		///		<li><code>cmf.min * 138</code>: 2.3 hours (138 min)</li>
		///		<li><code>cmf.h * 2.3</code>: 2.3 hours (138 min)</li>
		///		<li><code>cmf.day * 2.5</code>: 60 hours (2.5 days)</li>
		///		<li><code>cmf.week </code>: 7 days</li>
		///		<li><code>cmf.month</code>: 365/12 days (30.4167 days)</li>
		///		<li><code>cmf.year</code>: 365 days</li>
		///	</ul>
		///	<h3>
		///		Available operators:</h3>
		///	<ul>
		///		<li>time + time = time, time - time = time</li>
		///		<li>time * float = time ,time / float = time</li>
		///		<li>time/time=float</li>
		///		<li>&gt, &lt, ==, !=</li>
		///	</ul>
		///	<h2>
		///		Conversions</h2>
		///	<h3>
		///		Converting to python datetime</h3>
		///	<div class="fragment">
		///		<pre class="fragment">
		///import CMFlib as cmf
		///pythontime = cmf.year.AsPython()
		///cmftime=cmf.AsCMFTime(pythontime)
		///print type(pythontime) # '&lt;type 'datetime.datetime'&gt;'
		///print type(cmftime)    # '&lt;class 'CMFLib.Time'&gt;'
		///</pre>
		///	</div>
		///	<h3>
		///		Converting to numbers</h3>
		///	<ul>
		///		<li>t.AsMilliseconds()</li>
		///		<li>t.AsSeconds()</li>
		///		<li>t.AsMinutes()</li>
		///		<li>t.AsHours()</li>
		///		<li>t.AsDays()</li>
		///		<li>t.AsYears()</li>
		///	</ul>
		///	<h2>
		///		Creating time ranges</h2>
		///	<div class="fragment">
		///		<pre class="fragment">
		///import CMFLib as cmf
		///start=cmf.Time(5,1,2001)
		///end=cmf.Time(6,1,2001)
		///step=cmf.h * 6
		///for t in cmf.timerange(start,end,step):
		///    print t.AsDate()
		///# Prints:
		///# 05.01.2001
		///# 05.01.2001 06:00
		///# 05.01.2001 12:00
		///# 05.01.2001 18:00
		///</pre>
		///	</div>
		class Time
		{
		private:
			long long m_time_in_ms;
			Time(long long ms):m_time_in_ms(ms) {}
		public:
			static const long long ms_per_day=86400000;
			/// @name Constructors
			//@{
			/// Conversion constructor
			Time(double days):m_time_in_ms((long long)(days*ms_per_day)) {}
			/// Construction from date and stores the time as Excel-Time (0 = 31.12.1899 0:00:00)
			Time(int day,int month,int year=2001,int hour=0,int minute=0,int second=0,int ms=0);
			/// Conversion constructor
			Time(Date date);
			/// Copy constructor
			Time(const Time& t):m_time_in_ms(t.AsMilliseconds()) {}
			/// Standard constructor
			Time():m_time_in_ms(0) {}
			//@}
			/// @name Time unit conversion
			//@{
			/// Time in days
			double AsDays() const {return m_time_in_ms/((double)ms_per_day);} 
			/// Time in hours
			double AsHours() const {return m_time_in_ms/3600000.0;}
			/// Time in minutes
			double AsMinutes() const {return m_time_in_ms/60000.0;}
			/// Time in seconds
			double AsSeconds() const {return m_time_in_ms/1000.0;}
			/// Time in milliseconds
			long long AsMilliseconds() const {return m_time_in_ms;}
			Date AsDate() const;
			std::string to_string(char seperator=':');
			bool is_not_0() const {return m_time_in_ms!=0;}
			double DOY() const;
			//@}

			/// @name Arithmetic operators
			//@{
			/// Time = Time + Time
			Time operator+(const Time& t1) const	{return Time(m_time_in_ms+t1.AsMilliseconds());} 
			/// Time = Time - Time
			Time operator-(const Time& t1) const	{return Time(m_time_in_ms-t1.AsMilliseconds());}
			/// Time += Time
			Time operator+=(const Time& t1)				{m_time_in_ms+=t1.AsMilliseconds(); return *this;}
			/// Time -= Time
			Time operator-=(const Time& t1)				{m_time_in_ms-=t1.AsMilliseconds(); return *this;}
			/// Time = Time * double
			Time operator*(double x) const				{return Time((long long)(m_time_in_ms*x));}
			/// Time = Time / double
			Time operator/(double x) const				{return Time((long long)(m_time_in_ms/x));}
			/// Time = Time % Time
			Time operator%(const Time& t1) const {return Time(m_time_in_ms % t1.AsMilliseconds());}
			/// Time *= double
			Time& operator*=(double x)							{m_time_in_ms= (long long)(m_time_in_ms * x); return *this;}
			/// Time /= double
			Time& operator/=(double x)							{m_time_in_ms= (long long)(m_time_in_ms / x); return *this;}
			/// Time = Time * int
			Time operator*(int x) const           {return Time((long long)(m_time_in_ms*x));}
			/// Time = Time / int
			Time operator/(int x) const           {return Time((long long)(m_time_in_ms/x));}
			/// Time *= int
			Time& operator*=(int x)						  	{m_time_in_ms*= x; return *this;}
			/// Time /= int
			Time& operator/=(int x)						  	{m_time_in_ms/= x; return *this;}
			/// Time %= Time
			Time& operator%=(const Time& t1)			{m_time_in_ms%= t1.AsMilliseconds(); return *this; }
			/// double = Time / Time
			double operator/(const Time& t1) const  {return ((double)m_time_in_ms)/((double)t1.AsMilliseconds());}
			/// Returns the number of times this is included in t1
			long long times_in(const Time& t1) const {return t1.AsMilliseconds()/m_time_in_ms;}
			//@}
			/// @name Boolean Operators
			//@{
			/// bool = Time < Time
			bool operator<(const Time& t1) const	{return m_time_in_ms < t1.AsMilliseconds();}
			/// bool = Time > Time
			bool operator>(const Time& t1) const	{return m_time_in_ms > t1.AsMilliseconds();}
			/// bool = Time <= Time
			bool operator<=(const Time& t1) const	{return m_time_in_ms <= t1.AsMilliseconds();}
			/// bool = Time >= Time
			bool operator>=(const Time& t1)	const	{return m_time_in_ms >= t1.AsMilliseconds();}
			/// bool = Time == Time
			bool operator==(const Time& t1)	const	{return m_time_in_ms==t1.AsMilliseconds();}
			/// bool = Time != Time
			bool operator!=(const Time& t1)	const	{return m_time_in_ms!=t1.AsMilliseconds();}
			//@}
			
			/// Creates a time representing y years
			static Time Years(double y=1) {return Time((long long)(y*365*24*60*60*1000));}
			/// Creates a time representing y days
			static Time Days(double d=1) {return Time((long long)(d*24*60*60*1000));}
			/// Creates a time representing h hours
			static Time Hours(double h=1) {return Time((long long)(h*60*60*1000));}
			/// Creates a time representing min minutes
			static Time Minutes(double min=1) {return Time((long long)(min*60*1000));}
			/// Creates a time representing secs seconds
			static Time Seconds(double secs=1) {return Time((long long)(secs*1000));}
			/// Creates a time representing ms milliseconds
			static Time Milliseconds(long long ms=1) {return Time(ms);}
		};
		Time operator*(double f,Time t);
		Time operator*(int f,Time t);

		Time minimum_t(Time t1,Time t2);
		Time maximum_t(Time t1,Time t2);

		/// An absolute time, not for calculation. Date and Time are interchangable 
		struct Date
		{
			int year;  ///<Year
			int month; ///<Month
			int day;   ///<Day of Month
			int hour;  ///<Hour of day
			int minute;///<Minute of hour
			int second;///<Second of minute
			int ms;    ///< millisecond of second

			/// Creates a new date
			Date(int _day,int _month,int _year=2001,int _hour=0,int _minute=0,int _second=0,int _ms=0):
			day(_day),month(_month),year(_year),hour(_hour),minute(_minute),second(_second),ms(_ms)
			{	}

			/// Creates a new date from time (based on the 31.12.1899, like in Excel(TM))
			Date(const Time& time)
			{
				Date tdate=time.AsDate();
				day=tdate.day;
				month=tdate.month;
				year=tdate.year;
				hour=tdate.hour;
				minute=tdate.minute;
				second=tdate.second;
				ms=tdate.ms;
			}

			/// Converts a date to Time (based on the 31.12.1899, like in Excel(TM)
			Time ToTime()
			{
				return Time(*this);
			}
			/// Returns the day of year
			double DOY()
			{
				Time Jan1(1,1,year);
				return (ToTime() - Jan1).AsDays();
			}

			/// Returns a string representing the date
			std::string to_string();
		};
		const Time ms=Time::Milliseconds();   ///< 1 milli second
		const Time sec=Time::Seconds();       ///< 1 second
		const Time min=Time::Minutes();	      ///< 1 minute
		const Time h=Time::Hours();           ///< 1 hour
		const Time day=Time::Days();		      ///< 1 day
		const Time week=Time::Days(7);	      ///< 1 week
		const Time month=Time::Days(365/12.0);///< 1 month = 30*day+10*h
		const Time year=Time::Years();				///< 1 year  = 365*day
	}
}

#ifndef SWIG
		std::ostream& operator<<(std::ostream& os,const cmf::math::Date& date);
		std::ostream& operator<<(std::ostream& os,const cmf::math::Time& time);
#endif

#endif // Time_h__
