

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
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
#ifndef ET_h__
#define ET_h__
#include "../../water/flux_connection.h"
#include "../../atmosphere/meteorology.h"
#include "../../reach/OpenWaterStorage.h"
#include "../SoilLayer.h"
#include "../Soil/RetentionCurve.h"
#include "../cell.h"
#include "waterstress.h"
/// @defgroup ET Evapotranspiration
/// @ingroup connections


namespace cmf {
	namespace atmosphere {
		/// @ingroup meteo, ET
		/// A logarithmic wind profile
		/// @todo Cite literature for this windprofile and insert equation
		class log_wind_profile : public aerodynamic_resistance {
		private:
			cmf::upslope::Cell& cell;
		public:
			explicit log_wind_profile(cmf::upslope::Cell& _cell)
				: cell(_cell)
			{}
			void get_aerodynamic_resistance(double & r_ag,double & r_ac, cmf::math::Time t) const final;
			virtual ~log_wind_profile()
			{}
		};
		
	}
	namespace upslope {
		/// @ingroup ET
		/// Contains different flux_connection classes for the description of evaporation and transpiration
		namespace ET {


			/// Returns the potential ET after Penman-Monteith using some simplifications for a given Radiation balance,
			/// aerodynamic and surface resistances, and a vapor pressure deficit
			///
			/// \f[ ET = \frac{\Delta R_n}{\lambda \Delta + \gamma + \gamma \frac{r_s}{r_a}}
			/// + \frac{c_p\rho_a}{\Delta + \gamma + \gamma \frac{r_s}{r_a}} \frac{e_s - e_a}{r_a} \f]
			/// where
			///  - \f$ ET \f$ is the evapotranspiration in \f$\frac{kg}{m^2 day}\approx \frac{mm}{day}\f$
			///  - \f$ \Delta \left[\frac{kPa}{k}\right]= 4098\ 0.618	 \exp\left(\frac{17.27 T}{T+237.3}\right (T+237.3)^{-2} \f$ is the slope of vapor pressure
			///  - \f$ R_n \left[\frac{MJ}{m^2 day}\right]\f$ is the radiation balance
			///  - \f$ r_s \left[\frac s m\right] \f$ is the surface resistance
			///  - \f$ r_a \left[\frac s m\right] \f$ is the aerodynamic resistance
			///  - \f$ \gamma = 0.067 \left[\frac{kPa}{k}\right] \f$ is the psychrometer constant
			///  - \f$ e_s - e_a \left[kPa\right]\f$ is the vapor pressure deficit
			///
			/// @param Rn Radiation balance in \f$ \frac{MJ}{m^2 day} \f$
			/// @param ra Aerodynamic resistance in \f$ \frac s m \f$
			/// @param rs Surface resistance in \f$ \frac s m \f$, is 0 for free water
			/// @param T Actual Temperature in \f$ ^\circ C \f$
			/// @param vap_press_deficit Deficit of vapor pressure \f$ kPa \f$
			real PenmanMonteith(real Rn,real ra,real rs,real T,real vap_press_deficit);


			/// Returns the potential ET after Penman-Monteith using some simplifications for a weather and a vegetation object.
			/// aerodynamic and surface resistances, and a vapor pressure deficit
			///
			/// @param A Current weather
			/// @param veg Vegetation data
			/// @param h Height above sea level in m (for air pressure estimation)
			real PenmanMonteith(cmf::atmosphere::Weather A,const cmf::upslope::vegetation::Vegetation & veg,double h);


			/// @ingroup ET
			/// @brief An abstract base class for ET Methods with a WaterStressFunction
			class stressedET : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::SoilLayer> sw;
				void NewNodes()	{
					sw=std::dynamic_pointer_cast<cmf::upslope::SoilLayer>(left_node());
				}
				stressedET(cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target,std::string _type);
				real Tact(real Tpot) const;
			public:
				SoilLayer::ptr get_layer() const {
					return sw.lock();
				}
				virtual real ETpot(cmf::math::Time t) const  {
					return 0.0;
				}

			};

			/// @ingroup ET
			/// @brief A constant evapotranspiration
			///
			/// Uses a constant measured or elsewhere modelled ETpot. Actual Evapotranspiration is calculated
			/// from rootdepth and actual matrix potential in the layers using Tact. The value of ETpot can be
			/// changed during runtime
			class constantETpot : public stressedET {
			protected:
				virtual real calc_q(cmf::math::Time t);
			public:
				real ETpot_value;
				real GetETpot(cmf::math::Time t) const {return ETpot_value;}
				constantETpot(cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target,double constantETpot_value)
					: stressedET(source,ET_target,"Constant get_evaporation"),ETpot_value(constantETpot_value)
				{
				}
				virtual real ETpot(cmf::math::Time t) const {
					return ETpot_value;
				}
			};
			/// @ingroup ET
			/// @brief A timeseries driven evapotranspiration
			///
			/// Uses a timeseries of measured or elsewhere modelled ETpot. Actual Evapotranspiration is calculated
			/// from rootdepth and actual matrix potential in the layers using Tact. The value of ETpot can be
			/// changed during runtime
			class timeseriesETpot: public stressedET {
			protected:
				virtual real calc_q(cmf::math::Time t);
			public:
				cmf::math::timeseries ETpot_data;
				virtual real ETpot(cmf::math::Time t) const {return ETpot_data.get_t(t);}
				timeseriesETpot(cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target,cmf::math::timeseries ETpot_values)
					: stressedET (source,ET_target,"Timeseries based ET connection"), ETpot_data(ETpot_values)
				{
				}

			};
			/// @ingroup ET
			/// Calculates the potential evapotranspiration according to FAO(1998)
			///
			/// Governing equations:
			/// \f[
			/// \lambda ET &=& \frac{\Delta\left(R_n - G\right)+\rho_a c_p \frac{e_s - e_a}{r_a}}{\Delta + \gamma\left(1+\frac{r_s}{r_a}\right)} \mbox{ FAO 1998, Eq. 3} \\
			/// \mbox{With:} \\
			/// \Delta &=& 4098 \frac{0.6108 e^{17.27 T}}{(T+237.3)^2} \frac{kPa}{^\circ C} \mbox{	(FAO 1998, Eq. 13)}	\\
			/// T &=& \mbox{Actual Temperature in } ^\circ C  \\
			/// R_n &=& \mbox{net Radiation (see Atmosphere) in } \frac{MJ}{m^2day}	\\
			/// G &=& 0 \ \frac{MJ}{m^2day} \mbox{ if daily average (FAO 1998, Eq. 42)} \\
			///     && 0.1 R_n \ \mbox{ if day time (FAO 1998, Eq. 45)} \\
			///     && 0.5 R_n \ \mbox{ if night time (FAO 1998, Eq. 46)} \\
			/// \gamma &=& \frac{c_p P}{\epsilon \lambda} \mbox{ (FAO 1998,Eq. 8): Psychrometric constant } \frac{kPa}{^\circ C} \\
			/// c_p &=& 0.001013 \frac{MJ}{kg\ ^\circ C}\mbox{ specific heat at constant pressure } \\
			/// P &=& 101.3 \left(\frac{293-0.0065z}{293}\right)^{5.26}  \mbox{ (FAO 1998,Eq. 7)} \\
			/// \epsilon &=& 0.622 \frac{mol/g \mbox{ vapor}}{mol/g \mbox{ liquid water}} \\
			/// \lambda &=& 2.45 \frac{MJ}{kg} \mbox{ (FAO 1998,Eq. 8)} \\
			/// R &=& 0.287 \frac{kJ}{kg\ k}\mbox{ Spec. gas const.}		\\
			/// \rho_a &=&	\frac{P}{1.01(T+273)R} \mbox{ (FAO 1998,Box. 6)} \\
			/// e_s &=& \mbox{ Sat. vapor press. } [kPa] \\
			/// e_a &=& \mbox{ Act. vapor press. } [kPa] \\
			/// r_a &=& \frac{\ln\left(\frac{2-d}{z_{om}}\right)\ln\left(\frac{2-d}{z_{oh}}\right)}{k^2 u_2} \mbox{ (FAO 1998, Eq. 4/Box 4} \frac s m \\
			/// && d=\frac 2 3 h,z_{om}=0.123 h,z_{oh}=0.1 z_{om}, k=0.41 \\
			/// h &=& \mbox{ Vegetation height in }m \\
			/// u_2 &=& \mbox{ Windspeed in 2m above canopy } \frac m s \\
			/// r_s &=& \frac{r_l}{LAI_{Active}} \mbox{ (FAO 1998, Eq. 5/Box 5)} \frac s m \\
			/// && r_l=100 \frac s m, LAI_{Active}=0.5 LAI
			/// \f]
			class PenmanMonteithET : public stressedET {

			protected:
				real calc_q(cmf::math::Time t);
			public:

				bool daily;
				PenmanMonteithET(cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target)
					: stressedET(source,ET_target,"Penman Monteith transpiration") {
				}
				static real r_s(const cmf::upslope::vegetation::Vegetation & veg) ;
				static real r_a(cmf::atmosphere::Weather A,real  veg_height) ;
				/// @brief Connects all soil layers with the transpiration node of the cell
				static void use_for_cell(cmf::upslope::Cell & cell);
				virtual real ETpot(cmf::math::Time t) const;
			};

			///@ingroup ET
			/// Calculates the Evapotranspiration using Priestley-Taylor equation
			///
			/// \f[lambda ET = \alpha \frac{\Delta}{\Delta + \gamma} \left(R_n - G\right)\f]
			/// where:
			///  - \f$\Delta = 4098 \frac{0.6108 e^{17.27 T}}{(T+237.3)^2} \frac{kPa}{^\circ C}\f$, the slope of the vapor pressure/ temperature curve
			///  - \f$\gamma = \frac{c_p P}{\epsilon \lambda} \frac{kPa}{^\circ C}\f$  Psychrometric constant
			///  - \f$\lambda = 2.45 \frac{MJ}{kg}\f$ the latent heat of vaporization
			///  - \f$R_n \frac{MJ}{m^2day}\f$ net Radiation (see Atmosphere)
			///  - \f$G\f$ Ground heat flux
			///  - \f$\alpha\f$ the Priestley-Taylor constant (default 1.26 for humid climates)
			class PriestleyTaylorET : public stressedET {
			protected:
				real calc_q(cmf::math::Time t);
			public:

				bool daily;
				real alpha;
				PriestleyTaylorET(cmf::upslope::SoilLayer::ptr source, cmf::water::flux_node::ptr ET_target,real alpha=1.26);
				/// @brief Connects all soil layers with the transpiration node of the cell
				static void use_for_cell(cmf::upslope::Cell & cell);
				real ETpot(cmf::math::Time t) const;

			};

			/// @ingroup ET
			/// Calculates the Evapotranspiration using Hargreave's equation
			///
			/// \f[ET_{rc} = 0.0135 K_T\ s_0 \sqrt{\Delta T} (T + 17.8)\f]
			/// where:
			/// - \f$ ET_{rc} \f$ the reference crop evapotranspiration in mm/day
			/// - \f$ K_T = 0.00185 {\Delta T}^2 - 0.0433 \Delta T + 0.4023\f$ Continentality factor as given in the reference
			/// - \f$ \Delta T = |T_{max} - T_{min}|[K]\f$ Daily temperature range
			/// - \f$ T [^\circ C]\f$ daily mean temperature
			/// - \f$ s_0 = 15.392  d_r  \left(\omega_s\sin(\Phi)  \sin{\gamma} + \cos{\Phi}\cos{\gamma} * \sin(\omega_s)\right)\f$ the extraterrestrial solar radiation in mm/day
			/// - \f$ d_r = 1 + 0.0033 \cos(DOY\frac{2 \pi}{365})\f$ relative distance between earth and sun
			/// - \f$ \omega_s = \arccos(-\tan{\Phi} \tan{\gamma}) \f$ sunset hour angle (radians)
			/// - \f$ \gamma = 0.4039 \sin(DOY\frac{2 \pi}{365}  - 1.405) \f$ solar declination (radians)
			/// - \f$ \Phi\f$ geographic latitude (radians)
			///
			/// @see SAMANI, Zohrab. [Estimating solar radiation and evapotranspiration using minimum climatological data.][1]
			/// _Journal of Irrigation and Drainage Engineering,_ 2000, 126. Jg., Nr. 4, S. 265-267.
			///
			/// Crop specific potential evapotranspiration is scaled by LAI: \f$ ET_{pot} = ET_{rc} \frac{LAI}{2.88}\f$.
			///
			/// [1]: http://cagesun.nmsu.edu/~zsamani/research_material/files/Hargreaves-samani.pdf
			class HargreaveET : public stressedET {
			protected:
				real calc_q(cmf::math::Time t);
			public:
				HargreaveET(cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target)
					: stressedET(source,ET_target,"HargreaveET")
				{}
				/// @brief Connects all soil layers with the transpiration node of the cell
				static void use_for_cell(cmf::upslope::Cell & cell);
				real ETpot(cmf::math::Time t) const;

			};
			/// @ingroup ET
			/// Calculates ETpot after Oudin et al 2005
			///
			/// https://doi.org/10.1016/j.jhydrol.2004.08.026
			/// This ETpot formula is a generalization of two older approaches by introducing
			/// parameters to shape the dependency of ETpot from temperature and extraterrestrial radiation
			///
			/// \f$ \lambda ET_{pot} = R_{a} \cdot \frac{T_{a} + K_2}{K_1}\f$
			///
			/// Oudin et al (2005) found an optimum for \f$K_1=100, K_2=5\f$.
			/// The origin of this formula lays in Jensen & Haise (1963) with \f$K_1=40, K_2=0\f$
			/// and McGuiness-Bordne (1972) with \f$K_1=68, K_2=5\f$.
			class OudinET : public stressedET {
			protected:
				real calc_q(cmf::math::Time t);
			public:
				OudinET(cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target, double K1=100, double K2=5);
				static OudinET* JensenHaise1963(cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target) {
					return new OudinET(source, ET_target, 40, 0);
				}
				static OudinET* McGuinessBordne1972(cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target) {
					return new OudinET(source, ET_target, 68, 5);
				}
				double K1, K2;
				/// @brief Connects all soil layers with the transpiration node of the cell
				static void use_for_cell(cmf::upslope::Cell & cell);
				real ETpot(cmf::math::Time t) const;

			};

			/// @ingroup ET
			/// Calculates ETpot after Turc (DVWK). \f$ET_{act}\f$ is calculated using a WaterStressFunction
			///
			/// \f[
			/// ET_{pot,Turc} = 0.0031 C(rH) (R_G + 209) \frac{T}{T + 15}
			/// \f]
			///
			/// where:
			/// - \f$T\f$ is the mean daily temperature
			/// - \f$C(rH) = \begin{cases} 1 + \frac{50\% - rH}{70\%}, & rH < 50\% \\ 1 & rH > 50\% \end{cases} \f$,
			///   a modification parameter for low humidity
			/// - \f$rH\f$ relative Humidity in %
			/// - \f$R_G\f$ global radiation in \f$J/cm^2\f$
			class TurcET : public stressedET {
			protected:
				real calc_q(cmf::math::Time t);
			public:
				TurcET(cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target)
					: stressedET(source,ET_target,"TurcET") {
				}
				/// @brief Connects all soil layers with the transpiration node of the cell
				static void use_for_cell(cmf::upslope::Cell & cell);
				real ETpot(cmf::math::Time t) const;

			};
			/// @ingroup ET
			/// Calculates the evaporation from a canopy storage
			class CanopyStorageEvaporation : public cmf::water::flux_connection {
			protected:
				cmf::upslope::Cell & m_cell;
				std::weak_ptr<cmf::water::WaterStorage> c_stor;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					c_stor=cmf::water::WaterStorage::cast(left_node());
				}
			public:
				CanopyStorageEvaporation(cmf::water::WaterStorage::ptr CanopyStorage,cmf::water::flux_node::ptr ET_target,cmf::upslope::Cell & cell)
					: cmf::water::flux_connection(CanopyStorage,ET_target,"Penman Monteith (canopy) get_evaporation"),m_cell(cell) {
						NewNodes();
				}

			};
			/// @ingroup ET
			/// Calculates evaporation from an open water body
			class PenmanEvaporation : public cmf::water::flux_connection
			{
			protected:
				std::weak_ptr<cmf::river::OpenWaterStorage> m_source;
				std::unique_ptr<cmf::atmosphere::Meteorology> m_meteo;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_source=cmf::river::OpenWaterStorage::cast(left_node());
				}
			public:
				PenmanEvaporation(cmf::river::OpenWaterStorage::ptr source,cmf::water::flux_node::ptr Evap_target,const cmf::atmosphere::Meteorology& meteo);
			};
		}
	}
}

#endif // ET_h__
