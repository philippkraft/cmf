#ifndef Meteorology_h__
#define Meteorology_h__


#include "../math/Time.h"
#include "../math/timeseries.h"
#include "../water/Solute.h"
#include "../water/WaterStorage.h"
#include "../Geometry/maps.h"
#include <string>
namespace cmf {
	/// Contains classes to describe interactions with the atmosphere
	namespace atmosphere {
		double vapour_pressure(double T);
		double global_radiation(cmf::math::Time t,double height,double sunshine_fraction,double longitude=8,double latitude=51,int time_zone=1,bool daily=0);
		double Pressure(double height);

		/// A structure to return all parts of the incoming radiation, all values in \f$\frac{MJ}{m^2 day}\f$
		struct Weather
		{
			double
				T,					///< actual Temperature \f$T \left[^\circ C\right]\f$
				Tmax,				///< Maximum daily Temperature \f$T_{max} \left[^\circ C\right]\f$
				Tmin,				///< daily minimum Temperature	\f$T_{min} \left[^\circ C\right]\f$
				Tground,    ///< actual Temperature of the ground
				Prec,				///< Precipitation \f$P \left[\frac{mm}{day}\right] \f$
				Windspeed,	///< Windspeed \f$u \left[\frac{m}{s}\right] \f$
				e_a,				///< Actual vapor pressure \f$e_a\ [kPa]\f$
				e_s,				///< Saturated vapor pressure in \f$e_s [kPa]\f$
				sunshine,		///< Fractional sunshine duration (per potential sunshine duration) \f$\frac n N\ [-]\f$
				Rs;					///< Global Radiation in \f$R_s \left[\frac{MJ}{m^2 day}\right]\f$
			
			/// Calculates the net radiation flux  \f$R_n \left[\frac{MJ}{m^2 day}\right]\f$
			///
			/// \f{eqnarray*}
			/// R_{n} &=& R_{ns} - R_{nl} \\
			/// \mbox{ Net short wave radiation: }R_{ns} &=& (1-\alpha)	R_s \\
			/// \mbox{ Net long wave radiation: }R_{nl} &=& R_{black}\ \beta_{v}\ \beta_{c}	\\
			/// \mbox{Black body radiation: } 
			/// R_{black} &=& \left\{\begin{array}{cl} 
			///                    \sigma T^4 & \mbox{for less than daily time steps} \\
			///                    \sigma \frac {T_{max}^4 + T_{min}^4} 2  & \mbox{for daily time steps}
			///                    \end{array} \right. \\
			/// T &=& \mbox{Temperature }[K] \\
			/// \sigma &=& 4.903\ 10^{-9} \frac{MJ}{K^4 m^2 day} \mbox{ Stefan-Boltzmann constant } \\
			/// \mbox{Long wave reflectance:     } \\
			/// \mbox{by water vapor: }\beta_{v} &=& 0.34 - 0.14 \sqrt{e_a} \\
			/// \mbox{ by clouds: }\beta_{c} &=& 0.1 + 0.9 \frac n N  
			/// \f}
			/// @param albedo the albedo \f$\alpha\f$ of the surface
			/// @param daily If true, the net radiation for daily averages will be calculated
			double Rn(double albedo,bool daily=false) const;
			/// Calculates the mean pressure for a specific height
			Weather()
				:	T(15),Tmax(17),Tmin(10),Tground(16),
				Prec(0.0),e_s(vapour_pressure(15)),e_a(0.8*vapour_pressure(15)),
				Windspeed(2.),sunshine(0.5),Rs(15) {}
			std::string ToString() const
			{
				std::stringstream sstr;
				sstr.precision(3);
				sstr << "T(min/act/max)=" <<Tmin<<"/"<< T<<"/"<<Tmax << "deg C," 
					<< " rH=" << 100*e_a/e_s << " %,"
					<< " Rs=" << Rs << " MJ/day"
					<< " Wind=" << Windspeed << " m/s";
				return sstr.str();
			}
		};

		class Meteorology
		{
		public:
#ifndef SWIG
			cmf::atmosphere::Weather operator() (cmf::math::Time t,double x, double y, double z) 
			{
				return GetData(t,x,y,z);
			}
			cmf::atmosphere::Weather operator() (cmf::math::Time t,cmf::geometry::point p) 
			{
				return GetData(t,p.x,p.y,p.z);
			}
#endif
			virtual cmf::atmosphere::Weather GetData(cmf::math::Time t,double x, double y, double z)=0;

			/// Temperature below precipitation is considered as snow
			real SnowThresholdTemperature;
			Meteorology() : SnowThresholdTemperature(0.5),Name("") {}
			virtual Meteorology* copy() const=0;
			std::string 
				Name; ///< Name of the Station
			virtual ~Meteorology()
			{
				std::cout << "Meteo '" << Name << "' deleted" << std::endl;
			}
		};
// 		class meteoPointer
// 		{
// 		private:
// 			cmf::atmosphere::Meteorology * meteo;
// 			int* references;
// 			void releaseRef()
// 			{
// 				if (meteo)
// 				{
// 					--(*references);
// 					if (*references==0)
// 					{
// 						delete meteo;
// 						delete references;
// 						std::cout << "Meteorology deleted" << std::endl;
// 					}
// 				}
// 				meteo=0;
// 				references=0;
// 			}
// 		public:
// 			int ref_count() const {return *references;}
// 			cmf::atmosphere::Meteorology * operator->() const {
// 				return meteo;
// 			}
// 			cmf::atmosphere::Meteorology & operator*() const {
// 				return *meteo;
// 			}
// 			meteoPointer& operator=(const meteoPointer& other)
// 			{
// 				if (meteo!=other.meteo)
// 				{
// 					releaseRef();
// 					meteo=other.meteo;
// 					references=other.references;
// 					if (meteo)	++(*references);
// 				}
// 				return *this;
// 			}
// 			meteoPointer& operator=(Meteorology * other)
// 			{
// 				releaseRef();
// 				meteo=other;
// 				if (meteo)
// 				{
// 					references=new int;
// 					*references=1;
// 				}
// 				return *this;
// 			}
// 			bool operator==(const meteoPointer& other)
// 			{
// 				return meteo==other.meteo;
// 			}
// 			bool operator!=(const meteoPointer& other)
// 			{
// 				return meteo!=other.meteo;
// 			}
// 			bool operator<(const meteoPointer& other)
// 			{
// 				return meteo<other.meteo;
// 			}
// 			meteoPointer(const meteoPointer& other)
// 				: meteo(other.meteo),references(other.references)
// 			{
// 				if (meteo)	++(*references);
// 			}
// 			meteoPointer(cmf::atmosphere::Meteorology* meteorology)
// 			{
// 				meteo=meteorology;
// 				references=new int;
// 				*references=meteo ? 1 :0;
// 			}
// 			meteoPointer()
// 			{
// 				meteo=new Meteorology;
// 				references=new int;
// 				*references=1;
// 			}
// 			~meteoPointer()
// 			{
// 				releaseRef();
// 			}
// 
// 		};
		class ConstantMeteorology : public Meteorology
		{
		public:
			Weather weather;
			virtual cmf::atmosphere::Weather GetData(cmf::math::Time t,double x, double y, double z)
			{
				return weather;
			}
			ConstantMeteorology() {}
			ConstantMeteorology(const cmf::atmosphere::Weather& w) : weather(w) {}
			ConstantMeteorology(const cmf::atmosphere::ConstantMeteorology & other) : weather(other.weather) {}

			ConstantMeteorology* copy() const
			{
				return new ConstantMeteorology(*this);
			}
		};


		/// In order to calculate ETpot with cmf a big amount of meteorological data is needed,
		/// more data than usually available. The SingleMeteorology class can estimate missing data
		/// from a minimal set. As more data, as one provides, the better the calculation of
		/// ETpot becomes. The minimal data needed is Tmin and Tmax (daily) and precipitation.
		/// To calculate the global radiation (although measured global radiation could be inserted),
		/// the position of meteorological station in geographic coordinates has to be set.
		/// 
		/// 
		/// There are two modes for the meteorology: daily=true and daily=false. If daily=true,
		/// Radiation is given as a daily mean value. If daily=false, Radiation is given as
		/// an hourly mean value, which shows the dial ETpot variation but results in erronous
		/// results if the timestep is daily.
		/// 
		/// 
		/// In the following, we will assume a meteorological station with given Tmin, Tmax,
		/// precipitation and daily mean relative humidity. To use other meteorological data,
		/// please consult the description of the SingleMeteorology class in the API documentation
		/// <h3>
		/// Creating a meteorological station</h3>
		/// <div class="fragment">
		/// <pre class="fragment">
		/// import CMFlib as cmf
		/// latitude=51.2 # Latitude of station in decimal degrees
		/// longitude=8.1 # Longitude of station in decimal degrees (only needed for daily=false)
		/// timezone=1    # Timezone, pos. values mean east of GMT, negative west (Germany=1, Pacific time=-8, only needed for daily=false)
		/// start=cmf.Time(1,1,2001) # Creates all timeseries with this start time, one can change them later
		/// step=cmf.day             # s. start
		/// name="Giessen"           # A name for the station (optional)
		/// meteo=cmf.SingleMeteorology(latitude,longitude,timezone,start,step,name)</pre>
		/// </div>
		/// 
		/// The daily flag is automatically set to true, since the step width is &ge cmf.day
		/// <h3>
		/// Loading data into the meteorological station</h3>
		/// <div class="fragment">
		/// <pre class="fragment">
		/// # MeteoData.txt is tab seperated file containing 
		/// # Tmin [deg C],Tmax [deg C],rHmean [%] and precipitation [mm/day] values for every day
		/// f=file('MeteoData.txt')
		/// for line in file:
		/// meteo.Tmin.Add(float(line.split('\\t')[0]))
		/// meteo.Tmax.Add(float(line.split('\\t')[1]))
		/// meteo.rHmean.Add(float(line.split('\\t')[2]))
		/// meteo.Prec.Add(float(line.split('\\t')[3]))</pre>
		/// </div>
		/// <h3>
		/// Using a meteorological station</h3>
		/// <div class="fragment">
		/// <pre class="fragment">
		/// weather=meteo.GetData(cmf.Time(3,2,2009,14)) # Weather at Feb. 3rd, 2009, 2pm
		/// print 'Global Radiation: ',weather.Rs	       # Daily mean Rs, since daily=true
		/// print 'Temperature:',weather.T               # Daily mean T, since nothing else in known
		/// </pre>
		/// </div>

		class SingleMeteorology : public Meteorology
		{
		private:
			
		public:
			///@name Location and behaviour properties
			//@{
			double 
				Latitude,			///< Latitude in dec. deg.
				Longitude,			///< Longitude in dec. deg.
				Timezone,	///< Time zone, 1 Germany, 0 UK, -8 Pacific U.S.
				Elevation;   ///< elevation of the station in m a.s.l.

			bool daily;
			//@}
			/// Constructor of the Atmosphere object
			/// @param latitude Latitude of the study area (for solar radiation)
			/// @param longitude Longitude of the study area (for solar time)
			/// @param timezone Time zone of the study area (e.g Germany +1,U.S. Pacific time -8
			/// @param elevation Height of the meterological station above sea level [m]
			/// @param startTime Date of the beginning of the climatic data (may be changed for each time series later)
			/// @param timestep Frequency of climatic data (may be changed for each time series later)
			/// @param name Name of the station
			SingleMeteorology(double latitude=51,double longitude=8,double timezone=1,double elevation=0,
				cmf::math::Time startTime=cmf::math::Time(1,1,2001),cmf::math::Time timestep=cmf::math::day,std::string name="");
			SingleMeteorology(const cmf::atmosphere::SingleMeteorology& other);
			cmf::atmosphere::SingleMeteorology* copy() const {return new SingleMeteorology(*this);}
			///@name Data access methods
			//@{
			/// Returns the current Atmosphere state. Uses default values for missing timeseries
			cmf::atmosphere::Weather GetData(cmf::math::Time t,double height) const;
			cmf::atmosphere::Weather GetData(cmf::math::Time t,double x, double y, double z)
			{
				return GetData(t,z);
			}

			
			/// Returns the global radiation at a given time step \f$ R_s \frac{MJ}{m^2day}\f$, see http://www.fao.org/docrep/X0490E/x0490e07.htm#radiation
			/// \f{eqnarray*}
			/// \phi &=& \frac{(\mbox{geogr. Latitude})^\circ \pi}{180^\circ} \mbox{ Latitude in }rad \\
			/// \delta &=& 0.409 \sin\left(\frac{2\pi}{365}DOY - 1.39\right) \mbox{ Declination, DOY is day of year}\\
			/// \omega_s &=& \arccos(-\tan\phi\tan\delta) \mbox{ Sunset angle} \\
			/// G_{sc} &=& 0.0802 \frac{MJ}{m^2min} \mbox{Solar constant} \\
			/// d_r &=& 1+0.033 \cos\left(\frac{2\pi}{365}DOY\right) \mbox{Inverse relative distance Earth-Sun} \\
			/// b &=& \frac{2\pi(DOY-81)}{364}\\
			/// S_c &=& 0.1645\sin(2b)-0.1255\cos(b)-0.025\sin(b) \mbox{ Seasonal correction for solar time} \\
			/// \omega &=& \frac {\pi} {12}	\left(t_h+\frac{(\mbox{geogr. Longitude})^\circ}{15}-\mbox{Timezone}+S_c-12\right) \mbox{ solar time in }rad \\
			/// \mbox{If daily}  \\
			/// R_a &=& \frac{24\ 60}{\pi}G_{sc}\ d_r \left(\omega_s \sin\phi \sin\delta + \cos\phi \cos\delta \sin\omega_s\right) \mbox{Extraterrestrial radiation } \frac{MJ}{m^2 day} \\
			/// \mbox{If hourly} \\
			/// R_a &=& \frac{12\ 24\ 60}{\pi}G_{sc}\ d_r \left(\left(\omega^+ -\omega^-\right) \sin\phi \sin\delta + \cos\phi \cos\delta \left(\sin\omega^+ - \sin\omega^-\right)\right) \\
			/// && \omega^+,\omega^- = \omega \pm\frac{\pi}{24} \\
			/// \frac n N &=& \mbox{Fractional sunshine duration}		 \\
			/// R_s &=& \left(0.25+\left(0.5+2\ 10^{-5}z\right)\frac{n}{N}\right)R_a \mbox{Global radiation in }\frac{MJ}{m^2 day} \\ 
			/// && z=\mbox{Height a.s.l. in }m \\
			/// \f}
			double GetRs(cmf::math::Time t,double height,double sunshine_fraction) const;
			
			/// Calculates a timeseries of the sunshine fraction (to put into Sunshine) from a timeseries of absolute sunshine duration, using the potential sunshine duration in hours,
			/// see http://www.fao.org/docrep/X0490E/x0490e07.htm#radiation
			/// \f{eqnarray*}
			/// \phi &=& \frac{(\mbox{geogr. Latitude})^\circ \pi}{180^\circ} \mbox{ Latitude in }rad \\
			/// \delta &=& 0.409 \sin\left(\frac{2\pi}{365}DOY - 1.39\right) \mbox{ Declination, DOY is day of year}\\
			/// \omega_s &=& \arccos(-\tan\phi\tan\delta) \mbox{ Sunset angle} \\
			/// N &=& \frac{24}{\pi}\omega_s \mbox{ potential duration of sunshine in }h \\
			/// \frac n N && n\mbox{Absolute sunshine duration}
			/// \f}
			void SetSunshineFraction(cmf::math::timeseries sunshine_duration); 

			//@}
			/// @name Additional properties
			//@{
			/// Slope of temperature/height regression. Typical values are \f$ -0.0004 .. -0.001 \frac{^\circ C}{m} \f$, default is \f$ 0\frac{^\circ C}{m} \f$ (no temperature adjusting)
			double temp_height_slope;
			//@}
			/// @name Timeseries of meteorological data
			//@{
			cmf::math::timeseries
				/// Timeseries of daily precipitation in \f$ P \left[\frac{mm}{day}\right] \f$	<b>Requiered</b>
				Prec,
				/// Timeseries of Temperature \f$ T\left[^\circ C\right] \f$ <b>Optional, can be calculated from </b> \f$ T=\frac{T_{max} + T_{min}} 2 \f$
				T,
				/// Timeseries of daily maximum Temperature \f$ T_{max} \left[^\circ C\right] \f$ 
				/// <b>Requiered, but you can generate it from Generate TminTmax if a high resolution timeseries T is available</b>
				Tmax,
				/// Timeseries of daily minimum Temperature \f$ T_{min}\left[^\circ C\right] \f$ 
				/// <b>Requiered, but you can generate it from Generate TminTmax if a high resolution timeseries T is available</b>
				Tmin,
				/// Timeseries of upper soil temperature \f$ T_{ground}\left[^\circ C\right] \f$
				/// <b> Optional</b>, if missing \f$ T_{ground} = T \f$
				Tground,
				/// Timeseries of incoming solar energy in \f$ R_s \left[\frac{MJ}{m^2 day}\right]\f$. <b>Requiered, but can be generated with GenerateRadiation</b>
				Windspeed,
				/// Timeseries of relative mean Humidity  \f$ rH_{mean} \left[\%\right] \f$ <b>Optional, not used if rHmax or Tdew is available</b>
				rHmean,
				/// Timeseries of daily minimum relative Humidity \f$ rH_{min} \left[\%\right] \f$ <b>Optional, only used if rHmax is used </b>
				rHmin,
				/// Timeseries of daily maximum relative Humidity \f$ rH_{max} \left[\%\right] \f$	 <b>Optional, not used if Tdew is available</b>
				rHmax,
				/// Timeseries of dew point temperature \f$ T_{dew}\left[^\circ C\right]\f$					<b>Optional, if neither Tdew, rHmax or rHmean is available then Tdew = Tmin</b>
				Tdew,
				/// Fractional sunshine \f$\frac{n}{N}\f$. Sunshine duration per potential sunshine duration<b>Optional, if not present 0.5</b>
				/// If you have the total duration of sunshine, use CalcSunshineFraction. If you have cloud coverage instead of total sunshine duration you may assume
				/// that the fractional sunshine duration equals 1-cloudfraction
				Sunshine,
				/// Global Radiation in \f$ \frac{MJ}{m^2 day} \f$ <b> Optional, if not available GetRs is used</b>
				Rs;


			/// Map of time series of rainfall concentrations. 
			///
			/// Usage: 
			/// @code
			/// meteo.Deposition[Solute].Add(3.0);
			/// double d=meteo.Deposition[Solute][Time];
			/// @endcode
			cmf::water::SoluteTimeseries Deposition;
			/// Returns the rainfall with its current concentration. The concentration is derived from Deposition
			cmf::water::WaterFlux GetDeposition(cmf::math::Time t);
			//@}
			/// @name I/O
			//@{
			/// Save the SingleMeteorology data to an ASCII File with fixed format(	 uses cmf::math::timeseries::Save )
			void Save(const std::string& filename);
			/// Creates a Meterology from a File in fixed format (uses cmf::math::timeseries::timeseries(std::istream&) )
			explicit SingleMeteorology(const std::string& filename);

			//@}

		};
		class nnMapMeteorology : public cmf::atmosphere::Meteorology
		{
			cmf::math::timeseries temp_lapse;
			cmf::maps::NearestNeighborMap<SingleMeteorology*> map;
		public:
			double precalc_temp_lapse(cmf::math::Time begin, cmf::math::Time step,cmf::math::Time end)
			{
				using namespace cmf::math;
				int n = int(map.size());
				int steps=0;
				double avg_lapse=0;
				for (Time t=begin;t<=end;t+=step)
				{
					real SShT=0,SShh=0,sum_T=0,sum_h=0;
					
					for (int i = 0; i < n ; ++i)
					{
						real 
							h=map.Value(i)->Elevation,
							T=map.Value(i)->GetData(t,h).T;
						SShT+=h*T;
						SShh+=h*h;
						sum_h+=h;
						sum_T+=T;
					}
					SShT=1/(n-1)*(SShT-sum_h*sum_T);
					SShh=1/(n-1)*(SShh-sum_h*sum_T);
					temp_lapse.Add(SShT/SShh);
					++steps;
					avg_lapse+=SShT/SShh;
				}
				return avg_lapse/steps;
			}
			virtual cmf::atmosphere::Weather GetData(cmf::math::Time t,double x, double y, double z) 
			{
				cmf::geometry::point p(x,y,z);
				SingleMeteorology& met=*map(p);
				if (!temp_lapse.isempty())
					met.temp_height_slope=temp_lapse[t];
				return met(t,p);			  
			}
			virtual cmf::atmosphere::nnMapMeteorology* copy() const
			{
				nnMapMeteorology* res=new nnMapMeteorology;
				res->temp_lapse=temp_lapse;
				return res;
			}

		};
	}	
}

#ifdef SWIG
%extend cmf::atmosphere::SingleMeteorology {
	%pythoncode
{
    def TimeseriesDictionary(self):
        return {"Tmin":self.Tmin,
                "Tmax":self.Tmax,
                "Tdew":self.Tdew,
                "T":self.T,
                "Prec":self.Prec,
                "rHmean":self.rHmean,
                "rHmax":self.rHmax,
                "rHmin":self.rHmin,
                "Sunshine":self.Sunshine,
                "Windspeed":self.Windspeed,
								"Rs" : self.Rs}
}
}
#endif
#endif // Atmosphere_h__
