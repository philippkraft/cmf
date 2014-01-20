

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
#include "meteorology.h"
#include <cmath>
#include "../math/timeseries.h"
#include "../math/num_array.h"
#include "../upslope/cell.h"
#include "../project.h"
#include <fstream>
#define min(a,get_b) ((a)<(get_b)) ? (a) : (get_b)
#define max(a,get_b) ((a)>(get_b)) ? (a) : (get_b)
#define clip(v,mn,mx) (max(min(v,mx),mn))

using namespace cmf::atmosphere;
using namespace cmf::math;
using namespace cmf::geometry;


cmf::atmosphere::Weather cmf::atmosphere::MeteoStation::get_data( cmf::math::Time t,double height ) const
{
	Weather A;
	if (Tmax.is_empty())
		throw std::runtime_error("MeteoStation: Minimal dataset is to have values for Tmax, however Tmin should be provided too");
	double height_correction= T_lapse.is_empty() ? 0.0 : T_lapse[t]*(height-z);
	
	A.Tmax=Tmax[t]+height_correction;
	A.Tmin=Tmin.is_empty() ? A.Tmax-2 : Tmin[t]+height_correction;
	if (T.is_empty())
	{
		A.T=0.5*(A.Tmax+A.Tmin);
		A.e_s=0.5*(vapour_pressure(A.Tmax)+vapour_pressure(A.Tmin));
	}
	else
	{
		A.T=T[t]+height_correction;
		A.e_s=vapour_pressure(A.T);
	}
	if (Tground.is_empty()) A.Tground=A.T;
	else A.Tground=Tground[t];
	A.instrument_height=InstrumentHeight;
	A.Windspeed=Windspeed.is_empty() ? 
		2. 	// If windspeed is missing, use 2m/s as default
		: max(Windspeed[t],0.5); 	// Windspeed shoud not be smaller than 0.5m/s

	if (!Tdew.is_empty()) // If dew point is available, use dew point
		A.e_a=vapour_pressure(Tdew[t]);
	else if (!rHmax.is_empty()) // If dew point is not available but rHmax use rHmax (if possible in combination with rHmin)
		if (!rHmin.is_empty())
			A.e_a=0.5*(vapour_pressure(A.Tmin)*rHmax[t]/100.+vapour_pressure(A.Tmax)*rHmin[t]/100.);
		else
			A.e_a=vapour_pressure(A.Tmin)*rHmax[t]/100.;
	else if (!rHmean.is_empty()) // If only rHmean is available, use this
		A.e_a=rHmean[t]/100.*A.e_s;
	else // no humidity data available, assume Tmin=Tdew
		A.e_a=vapour_pressure(A.Tmin);
	A.sunshine=Sunshine.is_empty() ? 0.5 : Sunshine[t];
	if (Rs.is_empty())
		A.Rs=global_radiation(t,height,A.sunshine,Longitude,Latitude,Timezone,daily);
	else
		A.Rs=Rs[t];
	return A;
}



cmf::atmosphere::Weather operator+(const cmf::atmosphere::Weather& left,const cmf::atmosphere::Weather& right)
{
	cmf::atmosphere::Weather res;
	res.T=left.T+right.T;
	res.Tmax=left.Tmax+right.Tmax;
	res.Tmin=left.Tmin+right.Tmin;
	res.Tground=left.Tground+right.Tground;
	res.e_a=left.e_a+right.e_a;
	res.e_s=left.e_s+right.e_s;
	res.Rs=left.Rs+right.Rs;
	res.sunshine=left.sunshine+right.sunshine;
	res.Windspeed=left.Windspeed+right.Windspeed;
	return res;
}
cmf::atmosphere::Weather operator*(const cmf::atmosphere::Weather& left,double right)
{
	cmf::atmosphere::Weather res;
	res.T=left.T*right;
	res.Tmax=left.Tmax*right;
	res.Tmin=left.Tmin*right;
	res.Tground=left.Tground*right;
	res.e_a=left.e_a*right;
	res.e_s=left.e_s*right;
	res.Rs=left.Rs*right;
	res.sunshine=left.sunshine*right;
	res.Windspeed=left.Windspeed*right;
	return res;

}

void cmf::atmosphere::MeteoStation::SetSunshineFraction(cmf::math::timeseries sunshine_duration ) 
{
	Sunshine=cmf::math::timeseries(sunshine_duration.begin(),sunshine_duration.step(),sunshine_duration.interpolationpower());
	cmf::math::Time t=sunshine_duration.begin();
	for (size_t i = 0; i < sunshine_duration.size() ; i++)
	{
		double
			DOY=t.AsDate().DOY()+t.AsDays()-ptrdiff_t(t.AsDays()),
			phi=Latitude*PI/180,	 // Latitude [rad]
			decl=0.409*sin(2*PI/365*DOY-1.39), // Declination [rad]
			sunset_angle=acos(-tan(phi)*tan(decl)),	// Sunset hour angle [rad]
			N=24/PI*sunset_angle;
		Sunshine.add(sunshine_duration[i]/N),
		t+=sunshine_duration.step();
	}

}

cmf::atmosphere::MeteoStation::MeteoStation( double latitude/*=51*/,double longitude/*=8*/,double timezone/*=1*/,double elevation/*=0*/, cmf::math::Time startTime/*=cmf::math::Time(1,1,2001)*/,cmf::math::Time timestep/*=cmf::math::day*/,std::string name/*=""*/ ) 
: Latitude(latitude),Longitude(longitude),Timezone(timezone),z(elevation),daily(timestep>=cmf::math::day),
	Tmax(startTime,timestep,1),Tmin(startTime,timestep,1), T(startTime,timestep,1),
	Windspeed(startTime,timestep,1),
	rHmean(startTime,timestep,1),rHmax(startTime,timestep,1),rHmin(startTime,timestep,1),
	Tdew(startTime,timestep,1),Sunshine(startTime,timestep,1),Tground(startTime,timestep,1),
	Rs(startTime,timestep,1),T_lapse(startTime,timestep,1), InstrumentHeight(2),x(0),y(0)
{
	Name=name;
}

cmf::atmosphere::MeteoStation::MeteoStation( const cmf::atmosphere::MeteoStation& other )
:	Latitude(other.Latitude),Longitude(other.Longitude),Timezone(other.Timezone),daily(other.daily),
	Tmax(other.Tmax),Tmin(other.Tmin),T(other.T),
	Windspeed(other.Windspeed),
	rHmean(other.rHmean),rHmax(other.rHmax),rHmin(other.rHmin),
	Tdew(other.Tdew),Sunshine(other.Sunshine),Rs(other.Rs),Tground(other.Tground),
	T_lapse(other.T_lapse),InstrumentHeight(other.InstrumentHeight),x(other.x),y(other.y),z(other.z)
{
	Name=other.Name;
}

void cmf::atmosphere::MeteoStation::use_for_cell( cmf::upslope::Cell& c )
{
	MeteoStation::ptr sh_this;
	const MeteoStationList& msl =  c.get_project().meteo_stations;
	for (size_t i = 0; i < msl.size() ; ++i)
	{
		if (msl[i].get() == this) {
			MeteoStationReference mref(msl[i],c.get_position());
			c.set_meteorology(mref);
			break;
		}
	}

}

double cmf::atmosphere::MeteoStationList::calculate_Temp_lapse( cmf::math::Time begin, cmf::math::Time step,cmf::math::Time end )
{
	using namespace cmf::math;
	ptrdiff_t n = size();
	ptrdiff_t steps=0;
	timeseries temp_lapse(begin,step);
	double avg_lapse=0;
	double inv_size = 1./(n-1.);
	for (Time t=begin;t<=end;t+=step)
	{
		real SShT=0,SShh=0,sum_T=0,sum_h=0;
		for(vector::iterator it = m_stations.begin(); it != m_stations.end(); ++it)
		{
			MeteoStation& station=**it;
			real
				h = station.z,
				T = station.T[t];
			SShT+=h*T;
			SShh+=h*h;
			sum_h+=h;
			sum_T+=T;
		}
		SShT=inv_size * (SShT-sum_h*sum_T);
		SShh=inv_size * (SShh-sum_h*sum_T);
		temp_lapse.add(SShT/SShh);
		++steps;
		for(vector::iterator it = m_stations.begin(); it != m_stations.end(); ++it)
		{
			MeteoStation& station=**it;
			station.T_lapse=temp_lapse;
		}
		avg_lapse+=SShT/SShh;
	}
	return avg_lapse/steps;
}

cmf::atmosphere::MeteoStationReference cmf::atmosphere::MeteoStationList::reference_to_nearest( const cmf::geometry::point& p,double z_weight/*=0*/ ) const
{
	if (m_stations.size() == 0) throw std::out_of_range("No stations in list");
	MeteoStation::ptr nearest;
	double min_dist=1e300;
	for(vector::const_iterator it = m_stations.begin(); it != m_stations.end(); ++it)
	{
		MeteoStation::ptr station=*it;
		double dist=p.z_weight_distance(station->get_position(),z_weight);
		if (dist<min_dist)
		{
			min_dist=dist;
			nearest=station;
		}
	}
	if (nearest!=0)
		return MeteoStationReference(nearest,p);
	else
		throw std::runtime_error("No station found");
}

cmf::atmosphere::MeteoStation::ptr cmf::atmosphere::MeteoStationList::add_station( std::string name,cmf::geometry::point location,double latitude/*=51*/,double longitude/*=8*/,double timezone/*=1*/, cmf::math::Time startTime/*=cmf::math::Time(1,1,2001)*/,cmf::math::Time timestep/*=cmf::math::day*/ )
{
	MeteoStation* new_station=new MeteoStation(latitude,longitude,timezone,location.z,startTime,timestep,name);
	new_station->x=location.x;new_station->y=location.y;
	MeteoStation::ptr result(new_station);
	m_stations.push_back(result);
	m_name_map[name]=size()-1;
	return result;
}

cmf::atmosphere::IDW_Meteorology::IDW_Meteorology( const cmf::geometry::point& position,const MeteoStationList& stations,double z_weight, double power )
: m_position(position)
{
	point p=position;
	// Create a vector of distances to the stations
	num_array dist(stations.size());
	for (size_t i = 0; i < stations.size() ; ++i)
		dist[i] = p.distanceTo(point(stations[i]->x,stations[i]->y))+z_weight*abs(p.z-stations[i]->z);
	
	// Calculate the weight of each station using IDW
	num_array weights_ = 1./dist.power(power);
	double weightsum = weights_.sum();
	weights_/=weightsum; // normalize the weights
	// Create a weight map
	for (size_t i = 0; i < stations.size() ; ++i)
		weights[stations[i]] = weights_[i];
}

cmf::atmosphere::Weather cmf::atmosphere::IDW_Meteorology::get_weather( cmf::math::Time t ) const
{
	Weather res = Weather() * 0.0; // Create a NULL weather
	// Add each station record, weighted by IDW
	for(weight_map::const_iterator it = weights.begin(); it != weights.end(); ++it)
	    res += it->first->get_data(t,m_position.z) * it->second;
	return res;
}

real cmf::atmosphere::IDW_Meteorology::get_instrument_height() const
{
	real res=0.0;
	for(weight_map::const_iterator it = weights.begin(); it != weights.end(); ++it)
		res += it->first->InstrumentHeight * it->second;
	return res;

}
