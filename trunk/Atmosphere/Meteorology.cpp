#include "Meteorology.h"
#include <cmath>
#include "../math/timeseries.h"
#include <fstream>
#define min(a,b) ((a)<(b)) ? (a) : (b)
#define max(a,b) ((a)>(b)) ? (a) : (b)
#define clip(v,mn,mx) (max(min(v,mx),mn))

static const double PI=3.141592654;
double cmf::atmosphere::Pressure( double height )	
{
	return 101.3*pow((293-0.0065*height)/293,5.26);
}
double cmf::atmosphere::Weather::Rn( double albedo,bool daily/*=false*/ ) const
{
	double 
		// Net shortwave radiation
		Rns=(1-albedo)*Rs,
		// Blackbody radiation
		sigmaT= daily ?	
		4.903e-9*(pow(Tmax+273.16,4)+pow(Tmin+273.16,4))/2
		:	4.903e-9*pow(T+273.16,4),

		// Long wave radiation is reflected by vapor
		emissivity=0.34-0.14*sqrt(e_a),      
		// Long wave radiation is reflected by clouds
		clear_sky=0.1+0.9*sunshine, 
		Rnl=sigmaT * emissivity * clear_sky;
	return Rns-Rnl;
}

double cmf::atmosphere::MeteoStation::get_global_radiation(cmf::math::Time t,double height,double sunshine_fraction ) const
{
	return global_radiation(t,height,sunshine_fraction,Longitude,Latitude,Timezone,daily);
}


double cmf::atmosphere::vapour_pressure( double T )	
{
	return 0.6108*exp(17.27*T/(T+237.3));
}

double cmf::atmosphere::global_radiation( cmf::math::Time t,double height,double sunshine_fraction,double longitude,double latitude,int time_zone , bool daily)
{
	double
		DOY=t.AsDate().DOY()+t.AsDays()-int(t.AsDays()),
		phi=latitude*PI/180,	 // Latitude [rad]
		decl=0.409*sin(2*PI/365*DOY-1.39), // Declination [rad]
		sunset_angle=acos(-tan(phi)*tan(decl)),	// Sunset hour angle [rad]
		G_sc=0.0820,
		d_r=1+0.033*cos(2*PI/365*DOY), // Inverse relative distance Earth-Sun
		xx=sin(phi)*sin(decl), // X part of sun path
		yy=cos(phi)*cos(decl), // y part of sun path
		Ra=0; // extra terrestrial radiation
	if (daily)
	{
		Ra=24*60/PI*G_sc*d_r*(sunset_angle*xx+yy*sin(sunset_angle));
	}
	else
	{
		double
			b=2*PI*(DOY-81)/364.,
			S_c=0.1645*sin(2*b)-0.1255*cos(b)-0.025*sin(b), // Seasonal correction for solar time
			hour=24*(t.AsDays() - int(t.AsDays())), // decimal hours of day (local time)
			solar_time=PI/12*(hour-longitude/15+time_zone+S_c-12), // solar time [rad]
			st1=solar_time-PI/24,	// solar time at the beginning of the period
			st2=solar_time+PI/24;	// solar time at th end of the period
		Ra=24*12*60/PI*G_sc*d_r*((st2-st1)*xx+yy*(sin(st2)-sin(st1)));
	}
	Ra=max(Ra,0);
	double a_s=0.25,b_s=0.5+2e-5*height;
	return (a_s+b_s*sunshine_fraction)*Ra;

}

double cmf::atmosphere::Weather::snow_threshold=0.5;

cmf::atmosphere::Weather cmf::atmosphere::MeteoStation::get_data( cmf::math::Time t,double height ) const
{
	Weather A;
	if (Tmax.is_empty())
		throw std::runtime_error("Minimal dataset is to have values for Tmax, however Tmin should be provided too");
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
		A.Rs=get_global_radiation(t,height,A.sunshine);
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
	for (int i = 0; i < sunshine_duration.size() ; i++)
	{
		double
			DOY=t.AsDate().DOY()+t.AsDays()-int(t.AsDays()),
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
	Rs(startTime,timestep,1),T_lapse(startTime,timestep,1)
{
	Name=name;
}

cmf::atmosphere::MeteoStation::MeteoStation( const cmf::atmosphere::MeteoStation& other )
:	Latitude(other.Latitude),Longitude(other.Longitude),Timezone(other.Timezone),z(other.z),daily(other.daily),
	Tmax(other.Tmax),Tmin(other.Tmin),T(other.T),
	Windspeed(other.Windspeed),
	rHmean(other.rHmean),rHmax(other.rHmax),rHmin(other.rHmin),
	Tdew(other.Tdew),Sunshine(other.Sunshine),Rs(other.Rs),Tground(other.Tground),
	T_lapse(other.T_lapse)
{
	Name=other.Name;
}


double cmf::atmosphere::MeteoStationList::calculate_Temp_lapse( cmf::math::Time begin, cmf::math::Time step,cmf::math::Time end )
{
	using namespace cmf::math;
	int n = size();
	int steps=0;
	timeseries temp_lapse(begin,step);
	double avg_lapse=0;
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
		SShT=1/(n-1)*(SShT-sum_h*sum_T);
		SShh=1/(n-1)*(SShh-sum_h*sum_T);
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

cmf::atmosphere::MeteoStationReference cmf::atmosphere::MeteoStationList::reference_to_nearest( const cmf::geometry::Locatable& position,double z_weight/*=0*/ ) const
{
	if (!size()) throw std::out_of_range("No stations in list");
	meteo_station_pointer nearest;
	double min_dist=1e300;
	cmf::geometry::point p=position.get_position();
	for(vector::const_iterator it = m_stations.begin(); it != m_stations.end(); ++it)
	{
		const meteo_station_pointer& station=*it;
		double dist=p.distanceTo(station->get_position())+z_weight*abs(p.z-station->z);
		if (dist<min_dist)
		{
			min_dist=dist;
			nearest=station;
		}
	}
	if (nearest)
		return MeteoStationReference(nearest,position);
	else
		throw std::runtime_error("No station found");
}

cmf::atmosphere::meteo_station_pointer cmf::atmosphere::MeteoStationList::add_station( std::string name,cmf::geometry::point location,double latitude/*=51*/,double longitude/*=8*/,double timezone/*=1*/, cmf::math::Time startTime/*=cmf::math::Time(1,1,2001)*/,cmf::math::Time timestep/*=cmf::math::day*/ )
{
	MeteoStation* new_station=new MeteoStation(latitude,longitude,timezone,location.z,startTime,timestep,name);
	new_station->x=location.x;new_station->y=location.y;
	meteo_station_pointer result(new_station);
	m_stations.push_back(result);
	m_name_map[name]=size()-1;
	return result;
}