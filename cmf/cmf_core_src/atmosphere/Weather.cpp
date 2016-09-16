#include "meteorology.h"
#include <cmath>


using namespace cmf::atmosphere;
using namespace cmf::math;
using namespace cmf::geometry;

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
		:	4.903e-9*pow(Tground+273.16,4),

		// Long wave radiation is reflected by vapor
		emissivity= 1.24 *pow(10.0 * e_a / (T+273.16),1./7.),
		// Long wave radiation is reflected by clouds
		clear_sky=0.2+0.8*sunshine,
		Rnl=sigmaT * (emissivity-1) * clear_sky;
	return Rns+Rnl;
}


void cmf::atmosphere::Weather::set_snow_threshold(double new_threshold)
{
	cmf::atmosphere::Weather::snow_threshold = new_threshold;
}

cmf::atmosphere::Weather& cmf::atmosphere::Weather::operator+=( const Weather& w )
{
	T+=w.T;
	Tmax+=w.Tmax;
	Tmin+=w.Tmin;
	Tground+=w.Tground;
	Windspeed+=w.Windspeed;
	e_a+=w.e_a;
	e_s+=w.e_s;
	sunshine+=w.sunshine;
	Rs+=w.Rs;
	daylength+=w.daylength;
	instrument_height+=w.instrument_height;
	return *this;
}

cmf::atmosphere::Weather& cmf::atmosphere::Weather::operator*=( double factor )
{
	T*=factor;
	Tmax*=factor;
	Tmin*=factor;
	Tground*=factor;
	Windspeed*=factor;
	e_a*=factor;
	e_s*=factor;
	sunshine*=factor;
	Rs*=factor;
	daylength*=factor;
	instrument_height*=factor;
	return *this;
}

cmf::atmosphere::Weather::Weather( double _T,double _Tmax,double _Tmin, 
								   double _rH, double _wind,
								   double _sunshine,double _Rs, double _daylength) 
: T(_T), Tmax(_Tmax), Tmin(_Tmin), Tground(_T),e_s(vapour_pressure(_T)),e_a(_rH/100. * vapour_pressure(_T)),
  Windspeed(_wind),sunshine(_sunshine),Rs(_Rs),daylength(_daylength), instrument_height(2)
{

}
double cmf::atmosphere::vapour_pressure( double T )	
{
	return 0.6108*exp(17.27*T/(T+237.3));
}

double cmf::atmosphere::global_radiation( cmf::math::Time t,double height,double sunshine_fraction,double longitude,double latitude,double time_zone , bool daily)
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
	Ra=std::max(Ra,0.0);
	double a_s=0.25,b_s=0.5+2e-5*height;
	return (a_s+b_s*sunshine_fraction)*Ra;

}

double cmf::atmosphere::vpd_from_rH( double T,double rH )
{
	return (1-rH*0.01) * vapour_pressure(T);
}

double cmf::atmosphere::rH_from_vpd( double T, double vpd )
{
	double e_s = vapour_pressure(T), e_a = e_s - vpd;
	return 100 * e_a/e_s;
}
double cmf::atmosphere::Weather::snow_threshold=0.5;
