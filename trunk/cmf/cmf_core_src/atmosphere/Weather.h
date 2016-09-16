#ifndef Weather_h__
#define Weather_h__

namespace cmf {
	namespace atmosphere{
		/// @brief Returns the saturated vapor pressure in Pa for temperature T [degC]
		///
		/// The saturated vapor pressure \f$e_s\f$ is calculated follwing the following formula
		/// \f[e_s = 0.6108 \exp{\frac{17.27 T}{T+237.3}}\f]
		/// The definition is from http://www.fao.org/docrep/X0490E/x0490e07.htm#concepts
		double vapour_pressure(double T);
		/// Returns the vapor pressure deficit in Pa for temperature T [degC] and rel. humidity rH [%]
		///
		/// The vapor pressure deficit \f$e_s - e_a\f$ is calculated from rel. humidity as:
		/// \f[e_s - e_a = (1-rH/100) * e_s(T)\f]
		/// The definition is from http://www.fao.org/docrep/X0490E/x0490e07.htm#concepts
		/// @param T Air temperature in degC
		/// @param rH Rel. humidity in %
		///
		/// \f$e_s(T)\f$ is calculated using cmf::atmosphere::vapor_pressure(double)
		double vpd_from_rH(double T,double rH);
		/// Returns the rel. humidity in % for temperature T [degC] and vapor pressure deficit vpd [Pa]
		///
		/// the rel. humidity is calculated from the vapor pressure deficit \f$vpd = e_s - e_a\f$ as:
		/// \f[rH = 100 * \frac{e_a}{e_s(T)}, e_a = e_s(T) - vpd\f]
		/// The definition is from http://www.fao.org/docrep/X0490E/x0490e07.htm#concepts
		/// @param T Air temperature in degC
		/// @param vpd Vapor pressure deficit in Pa
		///
		/// \f$e_s(T)\f$ is calculated using cmf::atmosphere::vapor_pressure(double)
		double rH_from_vpd(double T, double vpd);
		/// 
		/**  @brief Calculates the global radiation in MJ/(m2 day) from the sun position and the sunshine fraction
		
			@param t actual time step
			@param height Height above sea level
			@param sunshine_fraction Fraction of sunshine hours per potential sunshine duration in h/h
			@param longitude,latitude Geographical position in degree. Latitude is only taken into acount for subdaily calculation
			@param time_zone Offset by timezone from GMT, eg. central Europe=1 US west coast = -8
			@param daily If true, the average radiation for the whole day is given (therefore latitude and time zone ignored), otherwise the average of the
			       current hour is returned
			
			The calculation of the global radiation follows http://www.fao.org/docrep/X0490E/x0490e07.htm#radiation.
			
			The following formula is used:
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
			 **/
		double global_radiation(cmf::math::Time t,double height,double sunshine_fraction,double longitude=8,double latitude=51,double time_zone=1,bool daily=0);
		/// Returns the average air pressure for a height (m a.s.l.)
		double Pressure(double height);

		/// @ingroup meteo
	    /// A structure holding meteorological information, excluding precipitation
		struct Weather
		{
		private:
			/// A model parameter, below which T in degC precipitation falls as snow
			static double snow_threshold;
		public:
			double
				T,					///< actual Temperature \f$T \left[^\circ C\right]\f$
				Tmax,				///< Maximum daily Temperature \f$T_{max} \left[^\circ C\right]\f$
				Tmin,				///< daily minimum Temperature	\f$T_{min} \left[^\circ C\right]\f$
				Tground,    ///< actual Temperature of the ground
				Windspeed,	///< Windspeed \f$u \left[\frac{m}{s}\right] \f$
				e_a,				///< Actual vapor pressure \f$e_a\ [kPa]\f$
				e_s,				///< Saturated vapor pressure in \f$e_s [kPa]\f$
				sunshine,		///< Fractional sunshine duration (per potential sunshine duration) \f$\frac n N\ [-]\f$
				Rs,					///< Global Radiation in \f$R_s \left[\frac{MJ}{m^2 day}\right]\f$
				daylength,    /// length of the day in h
				instrument_height; ///< Height of the measuring instuments above the vegetation

			/** Calculates the net radiation flux  \f$R_n \left[\frac{MJ}{m^2 day}\right]\f$
			
			\f{eqnarray*}
			R_{n} &=& R_{ns} - R_{nl} \\
			\mbox{ Net short wave radiation: }R_{ns} &=& (1-\alpha)	R_s \\
			\mbox{ Net long wave radiation: }R_{nl} &=& R_{black}\ \beta_{v}\ \beta_{c}	\\
			\mbox{Black body radiation: } 
			R_{black} &=& \left\{\begin{array}{cl} 
			                    \sigma T^4 & \mbox{for less than daily time steps} \\
			                    \sigma \frac {T_{max}^4 + T_{min}^4} 2  & \mbox{for daily time steps}
			                    \end{array} \right. \\
			 T &=& \mbox{Temperature }[K] \\
			 \sigma &=& 4.903\ 10^{-9} \frac{MJ}{K^4 m^2 day} \mbox{ Stefan-Boltzmann constant } \\
			 \mbox{Long wave reflectance:     } \\
			 \mbox{by water vapor: }\beta_{v} &=& 0.34 - 0.14 \sqrt{e_a} \\
			 \mbox{ by clouds: }\beta_{c} &=& 0.1 + 0.9 \frac n N  
			 \f}
			 @param albedo the albedo \f$\alpha\f$ of the surface
			 @param daily If true, the net radiation for daily averages will be calculated
			**/
			double Rn(double albedo,bool daily=false) const;
			
			
			/// Creates a "weather" from given data
			/// @param T        actual Temperature in deg C
			/// @param Tmax     daily maximum Temperature in deg C
			/// @param Tmin     daily minimum Temperature in deg C
			/// @param rH       actual relative humidity in % [0..100]
			/// @param wind     actual wind speed in m/s
			/// @param sunshine actual fraction of sunshine duration per potential sunshine duration in h/h
			/// @param Rs       actual incoming shortwave global radiation in MJ/(m2 day)
			/// @param daylength length of the day in h
			Weather(double T=15.0,double Tmax=17.0,double Tmin=13.0, 
					double rH=70.0, double wind=2.0,
					double sunshine=0.5,double  Rs=15, double daylength=12);
			/// Returns a string representation
			std::string to_string() const
			{
				std::stringstream sstr;
				sstr.precision(3);
				sstr << "T(min/act/max)=" <<Tmin<<"/"<< T<<"/"<<Tmax << "deg C," 
					<< " rH=" << 100*e_a/e_s << " %,"
					<< " Rs=" << Rs << " MJ/day"
					<< " Wind=" << Windspeed << " m/s";
				return sstr.str();
			}
			/// Sets the snow threshold globally for cmf
			static void set_snow_threshold(double new_threshold);
			/// Returns the actual snow threshold value in degC
			static double get_snow_threshold() {
				return snow_threshold;
			}

			/// inplace add
			Weather& operator+=(const Weather& w);
			/// inplace multiplication
			Weather& operator*=(double factor);
			/// add operator
			Weather operator+(const Weather& w) {
				Weather res=*this;
				res+=w;
				return res;
			}
			/// Multiply
			Weather operator*(double factor) {
				Weather res=*this;
				res*=factor;
				return res;
			}

		};

		
	}
	
}
#endif // Weather_h__
