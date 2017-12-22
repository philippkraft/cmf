

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
			log_wind_profile(cmf::upslope::Cell& _cell) 
				: cell(_cell)
			{}
			virtual void get_aerodynamic_resistance(double & r_ag,double & r_ac, cmf::math::Time t) const;
			virtual ~log_wind_profile()
			{}
		};
		
	}
	namespace upslope {
		/// @ingroup ET
		/// Contains different flux_connection classes for the description of evaporation and transpiration
		namespace ET {
			/// Abstract class. Child classes are defining a method for transpiration calculation
			class transpiration_method {
			public:
				/// returns the transpiration rate from one layer in m3/day
				virtual double transp_from_layer(cmf::upslope::SoilLayer::ptr,cmf::math::Time t) =0;
				virtual ~transpiration_method() {}
			};

			/// Abstract class. Child classes are defining a method for soil evaporation calculation
			class soil_evaporation_method {
			public:
				/// returns the soil evaporation rate from one layer in m3/day
				virtual double evap_from_layer(cmf::upslope::SoilLayer::ptr,cmf::math::Time t) =0;
				virtual ~soil_evaporation_method() {}
			};
			/// Abstract class. Child classes are defining a method for surface water evaporation calculation
			class surface_water_evaporation_method {
			public:
				virtual double evap_from_openwater(cmf::river::OpenWaterStorage::ptr,cmf::math::Time) =0;
				virtual ~surface_water_evaporation_method()	{}
			};
			/// Abstract class. Child classes are defining a method for intercepted canopy water evaporation calculation
			class canopy_evaporation_method {
			public:
				virtual double evap_from_canopy(cmf::water::WaterStorage::ptr,cmf::math::Time) =0;
				virtual ~canopy_evaporation_method(){}
			};
			/// Abstract class. Child classes are defining a method for snow evaporation calculation
			class snow_evaporation_method {
			public:
				virtual double evap_from_snow(cmf::water::WaterStorage::ptr snow, cmf::math::Time t)=0;
				virtual ~snow_evaporation_method() {}
			};



			/// Flux connection using a transpiration_method
			class transpiration : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::SoilLayer> sw;
				std::shared_ptr<transpiration_method> Method;
				real calc_q(cmf::math::Time t) {
					return Method->transp_from_layer(sw.lock(),t);
				}
				void NewNodes()
				{
					sw=cmf::upslope::SoilLayer::cast(left_node());
				}
			public:
				transpiration(cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target, std::shared_ptr<transpiration_method> _method, std::string method_name) 
					: flux_connection(source,ET_target,"Transpiration using " + method_name),sw(source), Method(_method) {
						NewNodes();
				}
			};
			/// Flux_connection using a soil_evaporation_method
			class soil_evaporation : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::upslope::SoilLayer> sw;
				std::shared_ptr<soil_evaporation_method> Method;
				real calc_q(cmf::math::Time t) {
					return Method->evap_from_layer(sw.lock(),t);
				}
				void NewNodes()
				{
					sw=cmf::upslope::SoilLayer::cast(left_node());
				}
			public:
				soil_evaporation(cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target, std::shared_ptr<soil_evaporation_method> _method, std::string method_name) 
					: flux_connection(source,ET_target,"Soil evaporation using " + method_name),sw(source), Method(_method) {
						NewNodes();
				}
			};
			/// Flux connection using a canopy_evaporation_method
			class canopy_evaporation : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::water::WaterStorage> canopy;
				std::shared_ptr<canopy_evaporation_method> Method;
				real calc_q(cmf::math::Time t) {
					return Method->evap_from_canopy(canopy.lock(),t);
				}
				void NewNodes()
				{
					canopy=cmf::water::WaterStorage::cast(left_node());
				}
			public:
				canopy_evaporation(cmf::water::WaterStorage::ptr source,cmf::water::flux_node::ptr ET_target, std::shared_ptr<canopy_evaporation_method> _method, std::string method_name) 
					: flux_connection(source,ET_target,"Canopy evaporation using " + method_name),canopy(source), Method(_method) {
						NewNodes();
				}
			};

			/// Flux connection using a snow_evaporation_method
			class snow_evaporation : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::water::WaterStorage> snow;
				std::shared_ptr<snow_evaporation_method> Method;
				real calc_q(cmf::math::Time t) {
					return Method->evap_from_snow(snow.lock(),t);
				}
				void NewNodes()
				{
					snow=cmf::water::WaterStorage::cast(left_node());
				}
			public:
				snow_evaporation(cmf::water::WaterStorage::ptr source,cmf::water::flux_node::ptr ET_target, std::shared_ptr<snow_evaporation_method> _method, std::string method_name) 
					: flux_connection(source,ET_target,"Snow evaporation using " + method_name),snow(source), Method(_method) {
						NewNodes();
				}
			};
			/// Flux connection using an surface_water_evaporation_method
			class surface_water_evaporation : public cmf::water::flux_connection {
			protected:
				std::weak_ptr<cmf::river::OpenWaterStorage> surface_water;
				std::shared_ptr<surface_water_evaporation_method> Method;
				real calc_q(cmf::math::Time t) {
					return Method->evap_from_openwater(surface_water.lock(),t);
				}
				void NewNodes()
				{
					surface_water=cmf::river::OpenWaterStorage::cast(left_node());
				}
			public:
				surface_water_evaporation(cmf::river::OpenWaterStorage::ptr source,cmf::water::flux_node::ptr ET_target, std::shared_ptr<surface_water_evaporation_method> _method, std::string method_name) 
					: flux_connection(source,ET_target,"surface_water evaporation using " + method_name),surface_water(source), Method(_method) {
						NewNodes();
				}
			};


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
				std::unique_ptr<cmf::upslope::ET::RootUptakeStessFunction> m_stressfunction;
				void NewNodes()	{
					sw=std::dynamic_pointer_cast<cmf::upslope::SoilLayer>(left_node());
				}
				stressedET(cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target,std::string _type);
				real Tact(real Tpot) const;
			public:
				/// Sets the stress function to limit water uptake
				void set_stressfunction(const RootUptakeStessFunction& stressfunction) {
					m_stressfunction.reset(stressfunction.copy());
				}
				SoilLayer::ptr get_layer() const {
					return sw.lock();
				}
				std::string to_string() const {
					return cmf::water::flux_connection::to_string() + " - " + m_stressfunction->to_string();
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
			/// \f{eqnarray*}
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
			/// \f}
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
			/// \f[lambda ET &=& \alpha \frac{\Delta}{\Delta + \gamma} \left(R_n - G\right)\f]
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
				HargreaveET(cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target, real latitude=51.0) 
					: stressedET(source,ET_target,"HargreaveET") , lat(latitude)
				{}
				real lat;
				/// @brief Connects all soil layers with the transpiration node of the cell
				static void use_for_cell(cmf::upslope::Cell & cell);
				real ETpot(cmf::math::Time t) const;

			};
			/// @ingroup ET
			/// Calculates ETpot after Turc (DVWK). ETact is calculated using a WaterStressFunction
			///
			/// \f$ET_{pot,Turc} = 0.0031 C (R_G + 209) \frac{T}{T + 15}\f$
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
