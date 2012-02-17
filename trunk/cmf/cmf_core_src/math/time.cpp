

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
#include "time.h"
#include <sstream>
namespace cmf {
	namespace math {
		cmf::math::Time operator*(double f,Time t) {return t*f;}
		cmf::math::Time operator*(int f,Time t) {return t*f;}

		cmf::math::Time timespan( long long ms )
		{
			return Time(ms);
		}

	}
}

long long ms_from_Date(cmf::math::Date date)
{
	// Calculation of julian day
	int a         = (14-date.month)/12;
	int y         = (date.year + 4800) - a;
	int m         = date.month + 12*a - 3;
	int julianday = date.day + (153*m+2)/5 + 365*y + y/4 - y/100 + y/400 - 32045;
	long long res=0;
	res  = (julianday - JULIANDAY_0_1_1900) * cmf::math::Time::ms_per_day;
	res += date.hour * 3600 * 1000;
	res += date.minute * 60 * 1000;
	res += date.second      * 1000;
	res += date.ms;
	return res;

}
cmf::math::Time::Time( int day,int month,int year/*=2001*/,int hour/*=0*/,int minute/*=0*/,int second/*=0*/,int ms/*=0*/ )
{
	m_time_in_ms=ms_from_Date(Date(day,month,year,hour,minute,second,ms));
}

cmf::math::Time::Time( cmf::math::Date date )
{
	m_time_in_ms=ms_from_Date(date);
}
cmf::math::Date cmf::math::Time::AsDate() const
{
	int																
		j  = int(
		m_time_in_ms/ms_per_day // Days since 1.1.1900
		+ JULIANDAY_0_1_1900 // Days from JD=0 (4712 BC) to 1900
		+ 32044   // Days to Greogorian epoch start (4800 BC)
		),
		g  = j / 146097,
		dg = j % 146097,
		c  = ((dg/36524 + 1)*3)/4,
		dc = dg - c * 36524,
		b  = dc / 1461,
		db = dc % 1461,
		a  = ((db/365 + 1)*3)/4,
		da = db - a * 365,
		y  = g*400 + c*100 + b*4 + a,
		m  = (da*5 + 308)/153 - 2,
		d  = da - (m+4)*153 / 5 + 122,
		Y  = y - 4800 + (m+2)/12,
		M  = (m + 2) % 12 + 1,
		D  = d + 1,
		ms_since_midnight = int(m_time_in_ms % ms_per_day),
		h = ms_since_midnight / 3600000,
		min = (ms_since_midnight % 3600000)/60000,
		s = (ms_since_midnight % 60000)/1000,
		ms  = (ms_since_midnight % 1000);
	return Date(D,M,Y,h,min,s,ms);

}

std::string cmf::math::Time::to_string( char seperator/*=':'*/ )
{
	std::stringstream str;
	str << (*this);
	return str.str();
}

double cmf::math::Time::DOY() const
{
	return AsDate().DOY();
}

cmf::math::Time& cmf::math::Time::operator=( const cmf::math::Time& other )
{
	this->m_time_in_ms = other.m_time_in_ms;
	return *this;
}
std::string cmf::math::Date::to_string()
{
	std::stringstream str;
	str << (*this);
	return str.str();
}

	std::ostream& operator<<(std::ostream& os,const cmf::math::Date& date)
	{
		if (date.day<10) os << "0";
		os << date.day << "." ;
		if (date.month<10) os << "0";
		os << date.month << "." << date.year << " " ;
		if (date.hour || date.minute || date.second || date.ms)
		{
			if (date.hour<10) os << "0";
			os << date.hour << ":";
			if (date.minute<10) os << "0";
			os	 << date.minute;
			if (date.second || date.ms)
			{
				os << ":";
				if (date.second<10) os << "0";
				if (date.ms)
					os << date.second + (date.ms*0.001);
				else os << date.second;

			}
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os,const cmf::math::Time& time)
	{
		long long 
			time_in_ms=time.AsMilliseconds(),
			d=time_in_ms / cmf::math::Time::ms_per_day,
			h=(time_in_ms % cmf::math::Time::ms_per_day)/3600000 ,
			m=(time_in_ms % 3600000)/60000,
			s=(time_in_ms % 60000)/1000,
			ms=(time_in_ms % 1000);
		double sec=s + ms/1000.0;
		std::string unit="h  ";
		if (d) os << d << "d" << ':';
		if (h || d)
		{
			if (h<10) os << "0";
			os << h << ':';
		}
		else unit="min";

		if (m || h || d)
		{
			if (m<10) os << "0";
			os	<< m << ':';
		}
		else unit="s  ";
		if (sec<10) os << "0";
		os << sec << unit;
		return os;
	}

