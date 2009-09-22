#ifndef ET_h__
#define ET_h__
#include "../../water/FluxConnection.h"
#include "../../Atmosphere/Meteorology.h"
#include "../../reach/OpenWaterStorage.h"
#include "../SoilWaterStorage.h"
#include "../Soil/RetentionCurve.h"
#include "ShuttleworthWallace.h"
namespace cmf {
	namespace upslope {
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
			real PenmanMonteith(cmf::atmosphere::Weather A,const cmf::upslope::vegetation::Vegetation & veg,double h);
			real Tact(real Tpot,const cmf::upslope::SoilWaterStorage & sw,const cmf::upslope::vegetation::Vegetation & veg);
			
			class constantETpot : public cmf::water::FluxConnection {
			protected:
				cmf::upslope::SoilWaterStorage* sw;
				
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					sw=dynamic_cast<cmf::upslope::SoilWaterStorage*>( m_left);
				}
			public:
				real ETpot_value;
				real GetETpot(cmf::math::Time t) const {return ETpot_value;}
				constantETpot(cmf::upslope::SoilWaterStorage& source,cmf::water::FluxNode& ET_target,double constantETpot_value) 
					: FluxConnection(source,ET_target,"Constant get_evaporation"),ETpot_value(constantETpot_value)	
				{
					NewNodes();
				}
			};
			/// Calculates the potential evapotranspiration according to FAO(1998)
			/// \f{eqnarray*}
			/// \lambda get_evaporation &=& \frac{\Delta\left(R_n - G\right)+\rho_a c_p \frac{e_s - e_a}{r_a}}{\Delta + \gamma\left(1+\frac{r_s}{r_a}\right)} \mbox{ FAO 1998, Eq. 3} \\
			/// \mbox{With:} \\
			/// \Delta &=& 4098 \frac{0.6108 e^{17.27 T}}{(T+237.3)^2} \frac{kPa}{^\circ C} \mbox{	(FAO 1998, Eq. 13): Slope of vapor pressure }	\\
			/// T &=& \mbox{Actual Temperature in } ^\circ C  \\
			/// R_n &=& \mbox{net Radiation (see Atmosphere) in } \frac{MJ}{m^2day}	\\
			/// G &=& 0 \ \frac{MJ}{m^2day} \mbox{ if daily average (FAO 1998, Eq. 42)} \\
			///     && 0.1 R_n \ \mbox{ if day time (FAO 1998, Eq. 45)} \\
			///     && 0.5 R_n \ \mbox{ if night time (FAO 1998, Eq. 46)} \\
			/// \gamma &=& \frac{c_p P}{\epsilon \lambda} \mbox{ (FAO 1998,Eq. 8): Psychrometric constant } \frac{kPa}{^\circ C} \\
			/// c_p &=& 0.001013 \frac{MJ}{kg\ ^\circ C}\mbox{ specific heat at constant pressure } \\
			/// P &=& 101.3 \left(\frac{293-0.0065z}{293}\right)^{5.26}  \mbox{ (FAO 1998,Eq. 7): Mean pressure kPa as a function of elevation above sea level in m} \\
			/// \epsilon &=& 0.622 \mbox{	ratio molecular weight of water vapour/dry air} \\
			/// \lambda &=& 2.45 \frac{MJ}{kg} \mbox{ (FAO 1998,Eq. 8): latent heat of vaporization} \\
			/// R &=& 0.287 \frac{kJ}{kg\ k}\mbox{ Specific gas constant }		\\
			/// \rho_a &=&	\frac{P}{1.01(T+273)R} \mbox{ (FAO 1998,Box. 6): Mean air density at constant pressure} \\
			/// e_s &=& \mbox{ Saturated vapor pressure (see Atmosphere) in } kPa \\
			/// e_a &=& \mbox{ Actual vapor pressure (see Atmosphere) in } kPa \\
			/// r_a &=& \frac{\ln\left(\frac{2-d}{z_{om}}\right)\ln\left(\frac{2-d}{z_{oh}}\right)}{k^2 u_2} \mbox{ (FAO 1998, Eq. 4/Box 4): Aerodynamic resitance in } \frac s m \\
			/// && d=\frac 2 3 h,z_{om}=0.123 h,z_{oh}=0.1 z_{om}, k=0.41 \\
			/// h &=& \mbox{ Vegetation height in }m \\
			/// u_2 &=& \mbox{ Windspeed in 2m above ground (see Atmosphere) } \frac m s \\
			/// r_s &=& \frac{r_l}{LAI_{Active}} \mbox{ (FAO 1998, Eq. 5/Box 5): bulk surface resistance} \frac s m \\
			/// && r_l=100 \frac s m, LAI_{Active}=0.5 LAI
			/// \f}
			class PenmanMonteithET : public cmf::water::FluxConnection {

			protected:
				cmf::upslope::SoilWaterStorage* sw;
				real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					sw=cmf::upslope::AsSoilWater(m_left);
				}
			public:

				bool daily;
				PenmanMonteithET(cmf::upslope::SoilWaterStorage& source,cmf::water::FluxNode& ET_target) 
					: FluxConnection(source,ET_target,"Penman Monteith transpiration"),sw(&source) {
						NewNodes();
				}
				static real r_s(const cmf::upslope::vegetation::Vegetation & veg) ;
				static real r_a(cmf::atmosphere::Weather A,real  veg_height) ;
				static void use_for_cell(cmf::upslope::Cell & cell)
				{
					for (int i = 0; i < cell.layer_count() ; ++i)
					{
						new PenmanMonteithET(cell.get_layer(i),cell.get_transpiration());
					}
				}
			};
			/// Calculates the actual transpiration and the soil evaporation from a soil layer
			class ShuttleworthWallaceET : public cmf::water::FluxConnection {
			protected:
				cmf::upslope::SoilWaterStorage* m_soilwaterstorage;
				cmf::water::WaterStorage* m_waterstorage;
				cmf::upslope::Cell& m_cell;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_soilwaterstorage=cmf::upslope::AsSoilWater(m_left);
					m_waterstorage= m_soilwaterstorage ? 0 : cmf::water::AsWaterStorage(m_left);
				}
			public:
				ShuttleworthWallaceET(cmf::water::WaterStorage& source,cmf::water::FluxNode& ET_target,cmf::upslope::Cell& cell,std::string Type="Shuttleworth Wallace get_evaporation") 
					: cmf::water::FluxConnection(source,ET_target,Type),m_cell(cell) {
						NewNodes();
				}
				static void use_for_cell(cmf::upslope::Cell& cell)
				{
					for (int i = 0; i < cell.storage_count() ; ++i)
					{
						new ShuttleworthWallaceET(cell.get_storage(i),cell.get_evaporation(),cell);
					}
						
					for (int i = 0; i < cell.layer_count() ; ++i)
					{
						if (!i) new ShuttleworthWallaceET(cell.get_layer(i),cell.get_evaporation(),cell,"Soil evaporation (SW)");
						new ShuttleworthWallaceET(cell.get_layer(i),cell.get_transpiration(),cell,"Root water uptake (SW)");
					}
				}

			};
			class HargreaveET : public cmf::water::FluxConnection {
			protected:
				cmf::upslope::SoilWaterStorage* sw;
				real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					sw=cmf::upslope::AsSoilWater(m_left);
				}
			public:
				HargreaveET(cmf::upslope::SoilWaterStorage& source,cmf::water::FluxNode& ET_target) 
					: FluxConnection(source,ET_target,"Hargreave get_evaporation"),sw(&source) {
						NewNodes();
				}
				static void use_for_cell(cmf::upslope::Cell & cell)
				{
					for (int i = 0; i < cell.layer_count() ; ++i)
					{
						new HargreaveET(cell.get_layer(i),cell.get_evaporation());
					}
				}

			};
			/// Calculates the evaporation from a canopy storage
			class CanopyStorageEvaporation : public cmf::water::FluxConnection {
			protected:
				const cmf::upslope::Cell & m_cell;
				cmf::water::WaterStorage* c_stor;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					c_stor=cmf::water::AsWaterStorage(m_left);
				}
			public:
				CanopyStorageEvaporation(cmf::water::WaterStorage& CanopyStorage,cmf::water::FluxNode& ET_target,cmf::upslope::Cell & cell)
					: cmf::water::FluxConnection(CanopyStorage,ET_target,"Penman Monteith (canopy) get_evaporation"),m_cell(cell) {
						NewNodes();
				}
			};
			class PenmanEvaporation : public cmf::water::FluxConnection
			{
			protected:
				cmf::river::OpenWaterStorage* m_source;
				std::auto_ptr<cmf::atmosphere::Meteorology> m_meteo;
				virtual real calc_q(cmf::math::Time t);
				void NewNodes()
				{
					m_source=cmf::river::AsOpenWater(m_left);
				}
			public:
				PenmanEvaporation(cmf::river::OpenWaterStorage& source,cmf::water::FluxNode& Evap_target,const cmf::atmosphere::Meteorology& meteo)
					: cmf::water::FluxConnection(source,Evap_target,"Penman evaporation from open water"), m_meteo(meteo.copy())
				{
					NewNodes();
				}
			};
		}
	}
}

#endif // ET_h__
