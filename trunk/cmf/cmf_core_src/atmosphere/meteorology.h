

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
#ifndef Meteorology_h__
#define Meteorology_h__


#include "../math/time.h"
#include "../math/timeseries.h"
#include "../water/Solute.h"
#include "../water/WaterStorage.h"
#include "Weather.h"
#include <string>

namespace cmf {
	namespace upslope {
		class Cell;
	}
	/// @defgroup meteo Meteorological data
	/// Meteorological data contains objects to store an regionalize rainfall measurements on the one hand, 
	/// and other meteorological data, used for ET calculations on the other side. In the following the term 
	/// meteorology is not used for rainfall measurements.

	/// @ingroup meteo
	/// Contains classes to describe interactions with the atmosphere
	namespace atmosphere {
		/// @ingroup meteo
		/// @brief An abstract class, for objects generating Weather records at a specific time.
		class Meteorology
		{
		public:
            /// Returns the Weather at time t
			cmf::atmosphere::Weather operator() (cmf::math::Time t) 
			{
				return get_weather(t);
			}
            /// Returns the Weather at time t. Pure virtual function. Must get implemented by child functions
			virtual cmf::atmosphere::Weather get_weather(cmf::math::Time t) const=0;

			/// Returns a copy of the meteorology object. Pure virtual function, needs to be implemented
			virtual Meteorology* copy() const=0;
			/// Returns the height of the instruments above canopy
			virtual real get_instrument_height() const=0;
		};
		/// @ingroup meteo
		/// A primitive implementation of the Meteorology interface. 
		/// Holds a Weather record and returns it for any date
		class ConstantMeteorology : public Meteorology
		{
		public:
            /// The constant, but changeable Weather record.
			Weather weather;
            /// Returns weather, regardless of t
			virtual cmf::atmosphere::Weather get_weather(cmf::math::Time t) const
			{
				return weather;
			}
			virtual real get_instrument_height() const {return 2;}
            /// Creates a ConstantMeteorology with a standard weather
			ConstantMeteorology() {}
            /// Creates a ConstantMeteorology with weather w
			ConstantMeteorology(const cmf::atmosphere::Weather& w) : weather(w) {}
			/// Copy constructor
			ConstantMeteorology(const cmf::atmosphere::ConstantMeteorology & other) : weather(other.weather) {}
            /// Creates a new instannce of the ConstantMeteorology with the same weather
			ConstantMeteorology* copy() const
			{
				return new ConstantMeteorology(*this);
			}
		};

		class MeteoStationList;
        
		/// @ingroup meteo
		/// @brief A meteorological station holding timeseries to create Weather records
	    ///
		/// In order to calculate ETpot with cmf a big amount of meteorological data is needed,
		/// more data than usually available. The MeteoStation class can estimate missing data
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
		/// please consult the description of the MeteoStation class in the API documentation
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
		/// meteo=cmf.MeteoStation(latitude,longitude,timezone,start,step,name)</pre>
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
		/// weather=meteo.get_data(cmf.Time(3,2,2009,14)) # Weather at Feb. 3rd, 2009, 2pm
		/// print 'Global Radiation: ',weather.Rs	       # Daily mean Rs, since daily=true
		/// print 'Temperature:',weather.T               # Daily mean T, since nothing else in known
		/// </pre>
		/// </div>
		class MeteoStation
		{
		private:
			friend class MeteoStationList;
			MeteoStation(double latitude=51,double longitude=8,double timezone=1,double elevation=0,
				cmf::math::Time startTime=cmf::math::Time(1,1,2001),cmf::math::Time timestep=cmf::math::day,std::string name="");

		public:
			/// shared pointer
			typedef std::tr1::shared_ptr<MeteoStation> ptr;
			///@name Location and behaviour properties
			//@{
			double 
				Latitude,			///< Latitude in dec. deg.
				Longitude,			///< Longitude in dec. deg.
				Timezone,	///< Time zone, 1 Germany, 0 UK, -8 Pacific U.S.
				x,y,z;   ///< elevation of the station in m a.s.l.
			std::string 
				Name; ///< Name of the Station
			cmf::geometry::point get_position() const
			{
				return cmf::geometry::point(x,y,z);
			}
			bool daily;
			real InstrumentHeight;
			//@}
			/// Copy c'tor
			MeteoStation(const cmf::atmosphere::MeteoStation& other);
			///@name Data access methods
			//@{
			/// Returns the current Atmosphere state. Uses default values for missing timeseries
			cmf::atmosphere::Weather get_data(cmf::math::Time t,double height) const;
			void use_for_cell(cmf::upslope::Cell& c);

			
			/** Returns the global radiation at a given time step \f$ R_s \frac{MJ}{m^2day}\f$, see http://www.fao.org/docrep/X0490E/x0490e07.htm#radiation
			 \f{eqnarray*}
			 \phi &=& \frac{(\mbox{geogr. Latitude})^\circ \pi}{180^\circ} \mbox{ Latitude in }rad \\
			 \delta &=& 0.409 \sin\left(\frac{2\pi}{365}DOY - 1.39\right) \mbox{ Declination, DOY is day of year}\\
			 \omega_s &=& \arccos(-\tan\phi\tan\delta) \mbox{ Sunset angle} \\
			 G_{sc} &=& 0.0802 \frac{MJ}{m^2min} \mbox{Solar constant} \\
			 d_r &=& 1+0.033 \cos\left(\frac{2\pi}{365}DOY\right) \mbox{Inverse relative distance Earth-Sun} \\
			 b &=& \frac{2\pi(DOY-81)}{364}\\
			 S_c &=& 0.1645\sin(2b)-0.1255\cos(b)-0.025\sin(b) \mbox{ Seasonal correction for solar time} \\
			 \omega &=& \frac {\pi} {12}	\left(t_h+\frac{(\mbox{geogr. Longitude})^\circ}{15}-\mbox{Timezone}+S_c-12\right) \mbox{ solar time in }rad \\
			 \mbox{If daily}  \\
			 R_a &=& \frac{24\ 60}{\pi}G_{sc}\ d_r \left(\omega_s \sin\phi \sin\delta + \cos\phi \cos\delta \sin\omega_s\right) \mbox{Extraterrestrial radiation } \frac{MJ}{m^2 day} \\
			 \mbox{If hourly} \\
			 R_a &=& \frac{12\ 24\ 60}{\pi}G_{sc}\ d_r \left(\left(\omega^+ -\omega^-\right) \sin\phi \sin\delta + \cos\phi \cos\delta \left(\sin\omega^+ - \sin\omega^-\right)\right) \\
			 && \omega^+,\omega^- = \omega \pm\frac{\pi}{24} \\
			 \frac n N &=& \mbox{Fractional sunshine duration}		 \\
			 R_s &=& \left(0.25+\left(0.5+2\ 10^{-5}z\right)\frac{n}{N}\right)R_a \mbox{Global radiation in }\frac{MJ}{m^2 day} \\ 
			 && z=\mbox{Height a.s.l. in }m \\
			 \f}
			double get_global_radiation(cmf::math::Time t,double height,double sunshine_fraction) const;
			
			 Calculates a timeseries of the sunshine fraction (to put into Sunshine) from a timeseries of absolute sunshine duration, using the potential sunshine duration in hours,
			 see http://www.fao.org/docrep/X0490E/x0490e07.htm#radiation
			 \f{eqnarray*}
			 \phi &=& \frac{(\mbox{geogr. Latitude})^\circ \pi}{180^\circ} \mbox{ Latitude in }rad \\
			 \delta &=& 0.409 \sin\left(\frac{2\pi}{365}DOY - 1.39\right) \mbox{ Declination, DOY is day of year}\\
			 \omega_s &=& \arccos(-\tan\phi\tan\delta) \mbox{ Sunset angle} \\
			 N &=& \frac{24}{\pi}\omega_s \mbox{ potential duration of sunshine in }h \\
			 \frac n N && n\mbox{Absolute sunshine duration}
			 \f} 
			**/
			void SetSunshineFraction(cmf::math::timeseries sunshine_duration); 

			//@}
			/// @name Timeseries of meteorological data
			//@{
			cmf::math::timeseries
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
				/// Global Radiation in \f$ \frac{MJ}{m^2 day} \f$ <b> Optional, if not available get_global_radiation is used</b>
				Rs,
				/// Temperature lapse, the slope of the temperature / height regression Typical values are \f$ -0.0004 .. -0.001 \frac{^\circ C}{m} \f$, default is \f$ 0\frac{^\circ C}{m} \f$ (no temperature adjusting)
				T_lapse;


			//@}

		};


		/// @ingroup meteo
		/// @brief A reference to a meteorological station. Returns the weather at a given time for its place using MeteoStation::T_lapse
		class MeteoStationReference 
		: public Meteorology
		{
		private:
			MeteoStation::ptr m_station;
			cmf::geometry::point m_location;
		public:
			/// Returns the station referenced
			MeteoStation::ptr get_station() const {return m_station;}
			/// Returns the position of the reference
			cmf::geometry::point get_position() const {return m_location;}
			/// Returns the weather at the time t
			cmf::atmosphere::Weather get_weather(cmf::math::Time t) const
			{
				return m_station->get_data(t,get_position().z);
			}
			/// Creates a reference for a MeteoStation at a location
			real get_instrument_height() const
			{
				return m_station->InstrumentHeight;
			}
			
			/// Create a located reference to a meteo station
			/// @param station MeteoStation
			/// @param location Location of the reference
			MeteoStationReference(MeteoStation::ptr station,cmf::geometry::point location)
				: m_station(station),m_location(location) {}
			/// Copy c'tor
			MeteoStationReference(const MeteoStationReference& copy)
				: m_station(copy.m_station),m_location(copy.m_location) {}
			/// Creates a new copy of the reference
			MeteoStationReference* copy() const
			{
				return new MeteoStationReference(*this);
			}

		};

		/// @ingroup meteo
		/// @brief A list of meteorological stations
        ///
        /// Can find the nearest station for a position and calculate the temperature lapse
		class MeteoStationList
		{
		private:
			typedef std::vector<MeteoStation::ptr> vector;
			vector m_stations;
			typedef std::map<std::string,int> name_map;
			name_map m_name_map;
		public:
			/// Returns the number of stations
			int size() const {return int(m_stations.size());}
			/// Gets the station at index
			MeteoStation::ptr operator[](int index) const
			{
				return m_stations.at(index<0 ? size()+index : index);
			}
			/// gets a station by name
			MeteoStation::ptr operator[](const std::string& Name) const
			{
				name_map::const_iterator pos=m_name_map.find(Name);
				if (pos != m_name_map.end())
					return m_stations.at(pos->second);
				else
					return MeteoStation::ptr();
			}
	
		
			/// Calculates the temperature lapse from all stations in the list and sets the T_lapse attribute of each station.
			///
			/// Returns the average lapse over the whole period.
			double calculate_Temp_lapse(cmf::math::Time begin, cmf::math::Time step,cmf::math::Time end);
			
			/// Creates a meteorological station at a certain position and adds it to the list
			/// @param name Name of the station
			/// @param position The location of the station in map coordinates
			/// @param latitude Latitude of the study area (for solar radiation)
			/// @param longitude Longitude of the study area (for solar time)
			/// @param timezone Time zone of the study area (e.g Germany +1,U.S. Pacific time -8
			/// @param startTime Date of the beginning of the climatic data (may be changed for each time series later)
			/// @param timestep Frequency of climatic data (may be changed for each time series later)
			MeteoStation::ptr add_station(std::string name,cmf::geometry::point position,double latitude=51,double longitude=8,double timezone=1,
				cmf::math::Time startTime=cmf::math::Time(1,1,2001),cmf::math::Time timestep=cmf::math::day);

			/// Removes a station and returns the number of remaining references to the removed station. If the station is deleted, 0 is returned
			int remove_station(int index)
			{
				
				const MeteoStation::ptr& ms=(*this)[index];
				int res=ms.use_count()-1;
				name_map::iterator name_it=m_name_map.find(ms->Name);
				if (name_it!=m_name_map.end()) m_name_map.erase(name_it);
				m_stations.erase(m_stations.begin()+ (index<0 ? size()+index : index));
				return res;
			}
			/// Create empty list
			MeteoStationList() {}
			/// Copy c'tor
			MeteoStationList(const MeteoStationList& copy)
				: m_stations(copy.m_stations),m_name_map(copy.m_name_map)
			{

			}


			/// Creates a MeteoStationReference from the nearest station to position at position
			///
			/// The distance is calculated as \f$ d=\sqrt{(x_{s} - x_{l})^2 + (y_{s} - y_{l})^2} + \lambda_z\|z_{s} - z_{l}\| \f$
			///	Where \f$s\f$ is the station and \f$l\f$ is the locatable
			/// @returns A Meteorology using the data of the nearest station to position 
			/// @param position The position (any locatable, like e.g. Cell possible) to look for the station. The reference should be owned by the locatable
			/// @param z_weight The weight of the height difference \f$\lambda_z\f$
			MeteoStationReference reference_to_nearest(const cmf::geometry::point& position,double z_weight=0) const;
		};


		
		/// @ingroup meteo
		/// Regionalizes meteorological measurements using a simple inverse distance weighted (IDW) method
		/// @sa IDW
		class IDW_Meteorology : public Meteorology
		{
		private:		
			typedef std::map<MeteoStation::ptr,double> weight_map;
			weight_map weights;
			const cmf::geometry::point m_position;
		public:

			/// Creates an reference to a list of stations and interpolates the weather using IDW.
			/// @param position Position of reference
			/// @param stations Meteo stations
			/// @param z_weight Weight of height in IDW procedure
			/// @param power Power of IDW procedure
			IDW_Meteorology(const cmf::geometry::point& position,const MeteoStationList& stations,double z_weight, double power);
			/// Copy c'tor
			IDW_Meteorology(const IDW_Meteorology& copy)
				: m_position(copy.m_position), weights(copy.weights) 
			{}
			virtual Weather get_weather(cmf::math::Time t) const;
			virtual IDW_Meteorology* copy() const {
				return new IDW_Meteorology(*this);
			}
			virtual real get_instrument_height() const;
		};
		
		/// Abstract class. Child classes can be used to calculate aerodynamic resistances against turbulent heat fluxes
		class aerodynamic_resistance {
		public:
			/// aerodynamic resistance from ground to atmosphere (r_ag) and from canopy to atmosphere (r_ac)
			virtual void get_aerodynamic_resistance(double & r_ag,double & r_ac, cmf::math::Time t) const=0;
			/// shared pointer
			typedef std::tr1::shared_ptr<aerodynamic_resistance> ptr;
		};


	}	
}

#endif // Atmosphere_h__
