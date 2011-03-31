#ifndef Weather_h__
#define Weather_h__

namespace cmf {
	namespace atmosphere{
				/// Returns the saturated vapor pressure  in Pa for temperature T [degC]
		double vapour_pressure(double T);
		/// Returns the vapor pressure deficit in Pa for temperature T [degC] and rel. humidity rH [%]
		double vpd_from_rH(double T,double rH);
		/// Returns the rel. humidity in % for temperature T [degC] and vapor pressure deficit vpd [Pa]
		double rH_from_vpd(double T, double vpd);
		/// Returns the global radiation in MJ/(m2 day)
		double global_radiation(cmf::math::Time t,double height,double sunshine_fraction,double longitude=8,double latitude=51,double time_zone=1,bool daily=0);
		/// Returns the average air pressure for a height (m a.s.l.)
		double Pressure(double height);

		/// @ingroup meteo
	    /// A structure holding meteorological information, excluding precipitation
		struct Weather
		{
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
			/// @param _T        actual Temperature in deg C
			/// @param _Tmax     daily maximum Temperature in deg C
			/// @param _Tmin     daily minimum Temperature in deg C
			/// @param _rH       actual relative humidity in % [0..100]
			/// @param _wind     actual wind speed in m/s
			/// @param _sunshine actual fraction of sunshine duration per potential sunshine duration in h/h
			/// @param _Rs       actual incoming shortwave global radiation in MJ/(m2 day)
			/// @param _daylength length of the day in h
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
			/// A model parameter, below which T in degC precipitation falls as snow
			static double snow_threshold;
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
