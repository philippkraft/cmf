

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
		/// @brief A time class, used to pass around current modelling times
		///
		/// Timespans and dates in cmf are used with a special object, called Time. An extra 
		/// class has the advantage, that the user does not have to remember, which
		/// unit of time he or she uses or what time unit is accepted by a specific function
		/// of the model. Arithmetic and boolean operators are supported by Time. Internally
		/// the time classes stores the time as integer milliseconds, therefore rounding issues
		/// will only appear at very small time ranges. Absolute time (like dates) are represented
		/// as milliseconds gone by from Dec, 31st 1899. Microsoft Excel dates are represented
		/// as days from that time, using floating point numbers, therefore it is very simple
		/// to convert Excel time representations to cmf time.
		/// 
		/// Another object is Date, which is doesn't provide the operators, but has a nice printed
		/// version and some special date functions, like day of year (DOY) and provides access
		/// to the current hour of day and so on, which only applyto dates and not to time spans.
		/// You can convert Time to Date an vice versa. The printing is not culture aware and
		/// uses the European representation. If you use the Python standard library datetime,
		/// conversion between Python time and cmf time is possible
		/// 
		/// Creating absolute time values (dates)
		///@code{.py}
		///import cmf
		///# Create the time: Jan, 5th 2001, 2:30 pm, 20s, 412 ms
		///t=cmf.Time(5,1,2001,14,30,20,412)
		///print t # Prints: '36896d:14:30:20.412h'
		///print t.AsDate() # Prints: '05.01.2001 14:30:20.412'
		///@endcode
		///	Creating time spans
		///
		/// In principle, there are three ways to create time spans. One is to use one of the
		/// static functions, another is to multiply an existing time span (like one of the
		/// build in constants) or to substrate two absolute times.
		///
		/// Available constants
		///  - @code cmf.sec * 4.1 @endcode : 4.1 seconds 
		///  - @code cmf.min * 138 @endcode : 2.3 hours (138 min)
		///  - @code cmf.h * 2.3 @endcode : 2.3 hours (138 min)
		///  - @code cmf.day * 2.5 @endcode : 60 hours (2.5 days)
		///  - @code cmf.week @endcode : 7 days
		///  - @code cmf.month @endcode : 365/12 days (30.4167 days)
		///  - @code cmf.year @endcode : 365 days
		///	
		/// Available operators:
		///	
		///  - time + time = time, time - time = time
		///  - time * float = time ,time / float = time
		///  - time/time=float
		///  - &gt;, &lt;, ==, !=
		///	
		///	
		/// Conversions
		///	
		/// Converting to python datetime
		///@code{.py}
		///import cmf
		///pythontime = cmf.year.AsPython()
		///cmftime=cmf.AsCMFTime(pythontime)
		///print type(pythontime) # type 'datetime.datetime'
		///print type(cmftime)    # '<class 'cmf.Time'>
		///@endcode
		///	
		/// Converting to numbers
		///	
		///  - t.AsMilliseconds()
		///  - t.AsSeconds()
		///  - t.AsMinutes()
		///  - t.AsHours()
		///  - t.AsDays()
		///  - t.AsYears()
		///	
		///	
		/// Creating time ranges
		///	@code
		///import cmf
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
		///@endcode

		class Time
		{
		private:
			long long m_time_in_ms;
#ifndef SWIG
			friend Time timespan(long long);
#endif
		public:
			static const long long ms_per_day=86400000;
			/// @name Constructors
			//@{

			/// Construction from date and stores the time as Excel-Time (0 = 31.12.1899 0:00:00)
			Time(int day,int month,int year,int hour=0,int minute=0,int second=0,int ms=0);
			/// Conversion constructor
			Time(Date date);
			/// Copy constructor
			Time(const Time& t):m_time_in_ms(t.AsMilliseconds()) {}
			/// Standard constructor
			Time():m_time_in_ms(0) {}
			//@}
#ifndef SWIG
			explicit Time(long long ms):m_time_in_ms(ms) {}
			Time& operator=(const Time& right);
			
			operator bool() const {
				return m_time_in_ms!=0;
			}
#endif
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
			Time operator+=(const Time& t1)			{m_time_in_ms+=t1.AsMilliseconds(); return *this;}
			/// Time -= Time
			Time operator-=(const Time& t1)			{m_time_in_ms-=t1.AsMilliseconds(); return *this;}
			/// Time = Time * double
			Time operator*(double x) const			{return Time((long long)(m_time_in_ms*x));}
			/// Time = Time / double
			Time operator/(double x) const			{return Time((long long)(m_time_in_ms/x));}
			/// Time = Time % Time
			Time operator%(const Time& t1) const 	{return Time(m_time_in_ms % t1.AsMilliseconds());}
			/// Time *= double
			Time& operator*=(double x)				{m_time_in_ms= (long long)(m_time_in_ms * x); return *this;}
			/// Time /= double
			Time& operator/=(double x)				{m_time_in_ms= (long long)(m_time_in_ms / x); return *this;}
			/// Time = Time * int
			Time operator*(long long x) const		{return Time((long long)(m_time_in_ms*x));}
			/// Time = Time / int
			Time operator/(long long x) const		{return Time((long long)(m_time_in_ms/x));}
			/// Time *= int
			Time& operator*=(long long x)			{m_time_in_ms*= x; return *this;}
			/// Time /= int
			Time& operator/=(long long x)			{m_time_in_ms/= x; return *this;}
			/// Time = Time * int
			Time operator*(int x) const				{return Time((long long)(m_time_in_ms*x));}
			/// Time = Time / int
			Time operator/(int x) const				{return Time((long long)(m_time_in_ms/x));}
			/// Time *= int
			Time& operator*=(int x)					{m_time_in_ms*= x; return *this;}
			/// Time /= int
			Time& operator/=(int x)					{m_time_in_ms/= x; return *this;}
			/// Time %= Time
			Time& operator%=(const Time& t1)		{m_time_in_ms%= t1.AsMilliseconds(); return *this; }
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
			
		};
#ifndef SWIG
		Time operator*(double f,Time t);
		Time operator*(int f,Time t);
		Time timespan(long long ms);
#endif

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
			Date(int _day,int _month,int _year,int _hour=0,int _minute=0,int _second=0,int _ms=0):
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
		const Time ms   = timespan(1);	///< 1 milli second
		const Time sec  = 1000 * ms;	///< 1 second
		const Time min  = 60 * sec;		///< 1 minute
		const Time h    = 60 * min;		///< 1 hour
		const Time day  = 24 * h;		///< 1 day
		const Time week = 7 * day;		///< 1 week
		const Time year = 365 * day;	///< 1 year  = 365*day
		const Time month= year/12;		///< 1 month = 30*day+10*h
		const Time never=Time(0,0,-9999); ///< A date long before our time to indicate 'never'
	}
}

#ifndef SWIG
		std::ostream& operator<<(std::ostream& os,const cmf::math::Date& date);
		std::ostream& operator<<(std::ostream& os,const cmf::math::Time& time);
#endif

#endif // Time_h__
