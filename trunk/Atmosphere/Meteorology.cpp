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

double cmf::atmosphere::SingleMeteorology::GetRs(cmf::math::Time t,double height,double sunshine_fraction ) const
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
cmf::atmosphere::Weather cmf::atmosphere::SingleMeteorology::GetData( cmf::math::Time t,double height ) const
{
	Weather A;
	if (Tmax.isempty())
		throw std::runtime_error("Minimal dataset is to have values for Tmax, however Tmin should be provided too");
	double height_correction=temp_height_slope*(height-Elevation);
	
	A.Prec=Prec.isempty() ? 0.0 : Prec[t];
	A.Tmax=Tmax[t]+height_correction;
	A.Tmin=Tmin.isempty() ? A.Tmax-2 : Tmin[t]+height_correction;
	if (T.isempty())
	{
		A.T=0.5*(A.Tmax+A.Tmin);
		A.e_s=0.5*(vapour_pressure(A.Tmax)+vapour_pressure(A.Tmin));
	}
	else
	{
		A.T=T[t]+height_correction;
		A.e_s=vapour_pressure(A.T);
	}
	if (Tground.isempty()) A.Tground=A.T;
	else A.Tground=Tground[t];

	A.Windspeed=Windspeed.isempty() ? 
		2. 	// If windspeed is missing, use 2m/s as default
		: max(Windspeed[t],0.5); 	// Windspeed shoud not be smaller than 0.5m/s

	if (!Tdew.isempty()) // If dew point is available, use dew point
		A.e_a=vapour_pressure(Tdew[t]);
	else if (!rHmax.isempty()) // If dew point is not available but rHmax use rHmax (if possible in combination with rHmin)
		if (!rHmin.isempty())
			A.e_a=0.5*(vapour_pressure(A.Tmin)*rHmax[t]/100.+vapour_pressure(A.Tmax)*rHmin[t]/100.);
		else
			A.e_a=vapour_pressure(A.Tmin)*rHmax[t]/100.;
	else if (!rHmean.isempty()) // If only rHmean is available, use this
		A.e_a=rHmean[t]/100.*A.e_s;
	else // no humidity data available, assume Tmin=Tdew
		A.e_a=vapour_pressure(A.Tmin);
	A.sunshine=Sunshine.isempty() ? 0.5 : Sunshine[t];
	if (Rs.isempty())
		A.Rs=GetRs(t,height,A.sunshine);
	else
		A.Rs=Rs[t];
	return A;
}



void cmf::atmosphere::SingleMeteorology::SetSunshineFraction(cmf::math::timeseries sunshine_duration ) 
{
	Sunshine.clear();
	Sunshine.begin=sunshine_duration.begin;
	Sunshine.step=sunshine_duration.step;
	Sunshine.interpolationpower=sunshine_duration.interpolationpower;
	cmf::math::Time t=sunshine_duration.begin;
	for (int i = 0; i < sunshine_duration.size() ; i++)
	{
		double
			DOY=t.AsDate().DOY()+t.AsDays()-int(t.AsDays()),
			phi=Latitude*PI/180,	 // Latitude [rad]
			decl=0.409*sin(2*PI/365*DOY-1.39), // Declination [rad]
			sunset_angle=acos(-tan(phi)*tan(decl)),	// Sunset hour angle [rad]
			N=24/PI*sunset_angle;
		Sunshine.Add(sunshine_duration[i]/N),
		t+=sunshine_duration.step;
	}

}

void cmf::atmosphere::SingleMeteorology::Save( const std::string& filename )
{
	std::ofstream file;
	try	
	{
		file.open(filename.c_str());
	}
	catch (...)
	{ 
		throw std::runtime_error("Could not open " + filename);
	}
	file << Name << std::endl;
	file << Latitude << " " << Longitude << " " << Timezone << " " << Elevation << std::endl;
	file << daily << std::endl;
	Tmin.Save(file);
	Tmax.Save(file);
	Tdew.Save(file);
	T.Save(file);
	Prec.Save(file);
	rHmean.Save(file);
	rHmax.Save(file);
	rHmin.Save(file);
	Sunshine.Save(file);
	Windspeed.Save(file);
	Rs.Save(file);
	file.close();
}

cmf::atmosphere::SingleMeteorology::SingleMeteorology( const std::string& filename )
{
	std::ifstream file;
	try
	{
		file.open(filename.c_str());
	}
	catch (...) 
	{
		throw std::runtime_error("Could not create " + filename);
	}
	char name[255];
	file.getline(name,255);
	Name=name;
	file >> Latitude >> Longitude >> Timezone >> Elevation >> daily;
	Tmin=cmf::math::timeseries(file);
	Tmax=cmf::math::timeseries(file);
	Tdew=cmf::math::timeseries(file);
	T=cmf::math::timeseries(file);
	Prec=cmf::math::timeseries(file);
	rHmean=cmf::math::timeseries(file);
	rHmax=cmf::math::timeseries(file);
	rHmin=cmf::math::timeseries(file);
	Sunshine=cmf::math::timeseries(file);
	Windspeed=cmf::math::timeseries(file);
	file.close();
}

cmf::atmosphere::SingleMeteorology::SingleMeteorology( double latitude/*=51*/,double longitude/*=8*/,double timezone/*=1*/,double elevation/*=0*/, cmf::math::Time startTime/*=cmf::math::Time(1,1,2001)*/,cmf::math::Time timestep/*=cmf::math::day*/,std::string name/*=""*/ ) 
: Latitude(latitude),Longitude(longitude),Timezone(timezone),Elevation(elevation),daily(timestep>=cmf::math::day),
	Tmax(startTime,timestep,1),Tmin(startTime,timestep,1), T(startTime,timestep,1),
	Prec(startTime,timestep,0),Windspeed(startTime,timestep,1),
	rHmean(startTime,timestep,1),rHmax(startTime,timestep,1),rHmin(startTime,timestep,1),
	Tdew(startTime,timestep,1),Sunshine(startTime,timestep,1),Tground(startTime,timestep,1),
	Rs(startTime,timestep,1),Deposition(),temp_height_slope(0)
{
	Name=name;
	for (cmf::water::SoluteTimeseries::iterator it=Deposition.begin();it!=Deposition.end();++it)
	{
		it->begin=startTime;
		it->step=timestep;
	}
}

cmf::atmosphere::SingleMeteorology::SingleMeteorology( const cmf::atmosphere::SingleMeteorology& other )
:	Latitude(other.Latitude),Longitude(other.Longitude),Timezone(other.Timezone),Elevation(other.Elevation),daily(other.daily),
	Tmax(other.Tmax),Tmin(other.Tmin),T(other.T),
	Prec(other.Prec),Windspeed(other.Windspeed),
	rHmean(other.rHmean),rHmax(other.rHmax),rHmin(other.rHmin),
	Tdew(other.Tdew),Sunshine(other.Sunshine),Rs(other.Rs),Tground(other.Tground),
	Deposition(other.Deposition),temp_height_slope(other.temp_height_slope)
{
	Name=other.Name;
}
cmf::water::WaterFlux cmf::atmosphere::SingleMeteorology::GetDeposition( cmf::math::Time t )
{
	cmf::water::WaterFlux depo;
	size_t i=0;
	for(cmf::water::SoluteTimeseries::iterator it = Deposition.begin(); it != Deposition.end(); ++it)
	{
		if (it->isempty()) 
			depo.c[i]=0;
		else
			depo.c[i]=(*it)[t];
		++i;
	}
	depo.q=Prec[t];
	return depo;
}

