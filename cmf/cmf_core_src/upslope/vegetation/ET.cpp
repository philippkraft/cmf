

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
#include "ET.h"

namespace cmf {
	namespace upslope {
		namespace ET {

			real PenmanMonteith(real Rn,real ra,real rs,real T,real vap_press_deficit)
			{
				using namespace cmf::atmosphere;
				ra/=cmf::math::day.AsSeconds(); // Convert resistances from s/m to day/m
				rs/=cmf::math::day.AsSeconds(); 
				real
					c_p_rho  = 1.240e-3, // volumetric heat capacity (MJ/(m3 k))
					lambda  = 2.45,	    // latent heat of vaporization (MJ/kg)
					// Slope of vapor pressure (kPa/degC) (FAO 1998, Eq. 13)
					Delta = 4098*0.6108 * exp(17.27*T/(T+237.3)) / ((T+237.3)*(T+237.3)), 
					gamma         = 0.067,											// psychrometric constant [kPa/k] 
					nominator     = rs>0 ? Delta + gamma * (1+rs/ra) : Delta+gamma,	
					radTerm       = Delta * Rn /(nominator * lambda),
					aeroTerm      = c_p_rho/nominator * vap_press_deficit/ra,
					et_pot=(radTerm+aeroTerm);
				return et_pot<0 ? 0 : et_pot;
			}
			real PenmanMonteith(cmf::atmosphere::Weather A,const cmf::upslope::vegetation::Vegetation & veg,double h)
			{
				if (veg.LAI<=0 && veg.StomatalResistance>0)
					return 0.0;

				using namespace cmf::atmosphere;
				double 
					c_p     = 1.013e-3, // specific heat at constant pressure (MJ/(kg k))
					lambda  = 2.45,	    // latent heat of vaporization (MJ/kg)
					epsilon = 0.622,    // ratio molecular weight of water vapour/dry air
					R       = 0.287,		// Specific gas constant (kJ/(kg*k))
					// Soil heat flux
					Delta = 4098*0.6108
					* exp(17.27*A.T/(A.T+237.3))
					/ ((A.T+237.3)*(A.T+237.3)), // Slope of vapor pressure (kPa/degC) (FAO 1998, Eq. 13)
					Rn = A.Rn(veg.albedo), // Net radiation flux 
					G = 0,
					// 					G = SingleMeteorology->daily ? 0					   // If calculation takes place on daily basis, the soil heat flux is 0 (FAO 1998,Eq. 42)
					// 					: Rn>0 ? 0.1*Rn : 0.5 * Rn,	           // On a shorter basis, it is proportional to Rn, different for day and night (FAO 1998,Eq. 45f)
					P=cmf::atmosphere::Pressure(h),         // Mean Pressure in kPa
					gamma=c_p*P/(epsilon*lambda),          // psychrometric constant [kPa °C-1] 
					rho_a=P/(1.01*(A.T+273)*R),		         // mean air density at constant pressure
					d=0.666667*h,	                         // zero plane displacement height
					z_om=0.123*h,                          // roughness length governing momentum transfer
					z_oh=0.1*z_om,                         // roughness length governing transfer of heat and vapour
					k2=0.1681,                             // von Karman's constant squared (0.41^2=0.1681)
					ra_u =   log((h+2-d)/z_om)
					* log((h+2-d)/z_oh)
					/ k2,                           // Aerodynamic resistence * Windspeed
					ra=ra_u/A.Windspeed,                   // Aerodynamic resistence 
					rs=(veg.StomatalResistance<=0)				 // Surface resistance
					? 0 
					: veg.StomatalResistance * 2 / veg.LAI,
					nominator     = Delta + gamma * (1+rs/ra),
					radTerm       = Delta * (Rn - G)/(nominator * lambda),
					aeroTermCoeff = cmf::math::day.AsSeconds() * epsilon * 3.486/ra_u/1.01,
					aeroTerm      = gamma/nominator * aeroTermCoeff / (A.T+273)*A.Windspeed*(A.e_s-A.e_a),
					et_pot=(radTerm+aeroTerm);
				return et_pot<0 ? 0 : et_pot;
			}

			real stressedET::Tact( real Tpot ) const
			{
				return m_stressfunction->Tact(this,Tpot) ;

			}

			stressedET::stressedET( cmf::upslope::SoilLayer::ptr source,cmf::water::flux_node::ptr ET_target,std::string _type ) 
				: flux_connection(source,ET_target,_type),  m_stressfunction(new SuctionStress())
			{
				NewNodes();
			}


			real constantETpot::calc_q( cmf::math::Time t ) 
			{
				cmf::upslope::SoilLayer::ptr layer=sw.lock();
				const cmf::upslope::vegetation::Vegetation& veg=layer->cell.vegetation; 
				return Tact(ETpot_value);
			}
			real timeseriesETpot::calc_q( cmf::math::Time t )
			{
				cmf::upslope::SoilLayer::ptr layer=sw.lock();
				const cmf::upslope::vegetation::Vegetation & veg=layer->cell.vegetation;
				return Tact(ETpot(t));
			}
			

			real PenmanMonteithET::r_s( const cmf::upslope::vegetation::Vegetation & veg ) 
			{
				if (veg.StomatalResistance)
					return veg.StomatalResistance/(0.5*veg.LAI);
				else
					return 0.0;
			}

			real PenmanMonteithET::r_a( cmf::atmosphere::Weather A,real veg_height ) 
			{
				real
					h=veg_height,
					d=0.666667*h,	                         // zero plane displacement height
					z_om=0.123*h,                          // roughness length governing momentum transfer
					z_oh=0.1*z_om,                         // roughness length governing transfer of heat and vapour
					k2=0.1681,                             // von Karman's constant squared (0.41^2=0.1681)
					ra_u =   log((h+A.instrument_height-d)/z_om)
								 * log((h+A.instrument_height-d)/z_oh)
								 / k2;																	// Aerodynamic resistence * Windspeed
				return ra_u/maximum(A.Windspeed,0.5);   // Aerodynamic resistence
			}
			real PenmanMonteithET::ETpot(cmf::math::Time t) const {
				cmf::upslope::SoilLayer::ptr layer = sw.lock();
				cmf::upslope::Cell & cell = layer->cell;
				cmf::atmosphere::Weather A = cell.get_weather(t);
				cmf::upslope::vegetation::Vegetation veg = cell.vegetation;
				real
					Rn = A.Rn(veg.albedo, daily), // Net radiation flux 
					G = daily ? 0									// If calculation takes place on daily basis, the soil heat flux is 0 (FAO 1998,Eq. 42)
					: Rn>0 ? 0.1*Rn : 0.5 * Rn,	           // On a shorter basis, it is proportional to Rn, different for day and night (FAO 1998,Eq. 45f)
					PM = PenmanMonteith(Rn - G, r_a(A, veg.Height), r_s(veg), A.T, A.e_s - A.e_a);
				return PM;
			}
			real PenmanMonteithET::calc_q( cmf::math::Time t )
			{
				real PM = ETpot(t);
				cmf::upslope::SoilLayer::ptr layer = sw.lock();
				cmf::upslope::Cell & cell = layer->cell;
				return Tact(PM) * (1 - cell.leave_wetness());
			}

			void PenmanMonteithET::use_for_cell( cmf::upslope::Cell & cell )
			{
				for (int i = 0; i < cell.layer_count() ; ++i)
				{
					new PenmanMonteithET(cell.get_layer(i),cell.get_transpiration());
				}
			}
			real CanopyStorageEvaporation::calc_q( cmf::math::Time t )
			{
				cmf::water::WaterStorage::ptr canopy=c_stor.lock();
				if (left_node()->is_empty()>=1.0) return 0;
				cmf::atmosphere::Weather w=m_cell.get_weather(t);
				cmf::upslope::vegetation::Vegetation veg=m_cell.vegetation;
				real PM=PenmanMonteith(w,veg,m_cell.z)*0.001*m_cell.get_area();
				return PM * m_cell.leave_wetness();
			}

			real HargreaveET::calc_q( cmf::math::Time t )
			{
				double pi=cmf::geometry::PI;
				cmf::upslope::SoilLayer::ptr layer = get_layer();
				cmf::atmosphere::Weather A=layer->cell.get_weather(t);
				cmf::upslope::vegetation::Vegetation veg=layer->cell.vegetation;
				double etrc = ETpot(t);
				
				return Tact(etrc*veg.LAI/2.88) * (1-layer->cell.leave_wetness());

			}

			void HargreaveET::use_for_cell( cmf::upslope::Cell & cell )
			{
				for (int i = 0; i < cell.layer_count() ; ++i)
				{
					new HargreaveET(cell.get_layer(i),cell.get_transpiration());
				}
			}
			real HargreaveET::ETpot(cmf::math::Time t) const
			{
				double pi = cmf::geometry::PI;
				cmf::upslope::SoilLayer::ptr layer = get_layer();
				cmf::atmosphere::Weather A = layer->cell.get_weather(t);
				cmf::upslope::vegetation::Vegetation veg = layer->cell.vegetation;
				double
					// Latitude in radians
					phi = pi * lat / 180.0,
					//solar declination (radians)
					gamma = 0.4039f * sin((2 * pi / 365 * t.AsDate().DOY()) - 1.405),
					//sunset hour angle (radians)
					ws = acos(-tan(phi) * tan(gamma)),
					//relative distance between earth and sun
					dr = 1 + 0.033 *  cos((2 * pi / 365 * t.AsDate().DOY())),
					//s0 = extraterrestrial solar radiation  mm/day
					s0 = 15.392 * dr *  (ws * sin(phi) * sin(gamma) + cos(phi) * cos(gamma) * sin(ws)),
					//difference between mean monthly min and max temperatures - an estimate!
					TD = fabs(A.Tmax - A.Tmin),
					// Continentality factor, ranging between 0.19 (coastal regions) and 0.162
					// Calculated according to Z. Samani, see http://cagesun.nmsu.edu/~zsamani/research_material/files/Hargreaves-samani.pdf
					KT = 0.00185*(TD*TD) - 0.0433 * TD + 0.4023,
					//gives reference crop et in m / d
					etrc = 0.0135 * KT * s0 * sqrt(TD) * (A.T + 17.8);


				return etrc;
			}
			real PenmanEvaporation::calc_q( cmf::math::Time t )
			{
				cmf::river::OpenWaterStorage::ptr source=m_source.lock();
				if (left_node()->is_empty()>=1.0) return 0;
				cmf::atmosphere::Weather w=m_meteo->get_weather(t);
				real PM=PenmanMonteith(w.Rn(0.1),PenmanMonteithET::r_a(w,0.1),0.0,w.T,w.e_s-w.e_a) * 0.001 * source->wet_area();
				return minimum(source->get_state()*2/cmf::math::h.AsDays(),PM);
			}

			PenmanEvaporation::PenmanEvaporation( cmf::river::OpenWaterStorage::ptr source,cmf::water::flux_node::ptr Evap_target,const cmf::atmosphere::Meteorology& meteo ) : cmf::water::flux_connection(source,Evap_target,"Penman evaporation from open water"), m_meteo(meteo.copy())
			{
				NewNodes();
			}



			real TurcET::calc_q( cmf::math::Time t )
			{
				cmf::upslope::SoilLayer::ptr layer = get_layer();
				cmf::upslope::vegetation::Vegetation veg = layer->cell.vegetation;

				real ETp = ETpot(t);
				return Tact(ETp * veg.LAI/2.88) * (1-layer->cell.leave_wetness());
			}

			void TurcET::use_for_cell( cmf::upslope::Cell & cell )
			{
				for (int i = 0; i < cell.layer_count() ; ++i)
				{
					new TurcET(cell.get_layer(i),cell.get_transpiration());
				}

			}

			real TurcET::ETpot(cmf::math::Time t) const
			{
				cmf::upslope::SoilLayer::ptr layer = get_layer();
				cmf::atmosphere::Weather A = layer->cell.get_weather(t);
				cmf::upslope::vegetation::Vegetation veg = layer->cell.vegetation;
				real
					T = A.T,
					RG = A.Rs * 100,
					rH = 100 * A.e_a / A.e_s,
					C = rH<50 ? 1 + ((50 - rH) / 70) : 1.0,
					ETp = 0.0031 * C * (RG + 209) * (T / (T + 15));
				return ETp;
			}


			PriestleyTaylorET::PriestleyTaylorET( cmf::upslope::SoilLayer::ptr source, cmf::water::flux_node::ptr ET_target,real _alpha/*=1.26*/ )
				: stressedET(source,ET_target,"Priestley-Taylor ET"), alpha(_alpha) {
			}

			void PriestleyTaylorET::use_for_cell( cmf::upslope::Cell & cell )
			{
				for (int i = 0; i < cell.layer_count() ; ++i)
				{
					new PriestleyTaylorET(cell.get_layer(i),cell.get_transpiration());
				}

			}

			real PriestleyTaylorET::ETpot(cmf::math::Time t) const
			{
				cmf::upslope::SoilLayer::ptr layer = sw.lock();
				cmf::upslope::Cell & cell = layer->cell;
				cmf::atmosphere::Weather A = cell.get_weather(t);
				cmf::upslope::vegetation::Vegetation veg = cell.vegetation;
				real
					Rn = A.Rn(veg.albedo, daily), // Net radiation flux 
					G = daily ? 0									// If calculation takes place on daily basis, the soil heat flux is 0 (FAO 1998,Eq. 42)
					: Rn>0 ? 0.1*Rn : 0.5 * Rn,	           // On a shorter basis, it is proportional to Rn, different for day and night (FAO 1998,Eq. 45f)
					c_p_rho = 1.240e-3, // volumetric heat capacity (MJ/(m3 k))
					lambda = 2.45,	    // latent heat of vaporization (MJ/kg)
										// Slope of vapor pressure (kPa/degC) (FAO 1998, Eq. 13)
					Delta = 4098 * 0.6108 * exp(17.27 * A.T / (A.T + 237.3)) / ((A.T + 237.3)*(A.T + 237.3)),
					gamma = 0.067,	// psychrometric constant [kPa/k] 
					radTerm = alpha * Delta / (Delta + gamma) * (Rn - G),

					PT = radTerm / lambda;
				return PT;
			}

			real PriestleyTaylorET::calc_q( cmf::math::Time t )
			{
				cmf::upslope::SoilLayer::ptr layer=sw.lock();
				cmf::upslope::Cell & cell=layer->cell;
				cmf::upslope::vegetation::Vegetation veg=cell.vegetation;
				real PT = ETpot(t);
				return Tact(PT * veg.LAI/2.88) * (1-cell.leave_wetness());

	
			}

		
		}
	}

	void atmosphere::log_wind_profile::get_aerodynamic_resistance( double & r_ag,double & r_ac, cmf::math::Time t ) const
	{
		cmf::atmosphere::Weather w = cell.get_weather(t);
		cmf::upslope::vegetation::Vegetation& v = cell.vegetation;
		double
			h=v.Height,
			d=0.666667*h,	                         // zero plane displacement height
			z_om=0.123*h,                          // roughness length governing momentum transfer
			z_oh=0.1*z_om,                         // roughness length governing transfer of heat and vapour
			k2=0.1681;                             // von Karman's constant squared (0.41^2=0.1681)
		r_ac = (log((h+w.instrument_height-d)/z_om)
			* log((h+w.instrument_height-d)/z_oh))
			/ (k2 * w.Windspeed);                           // Aerodynamic resistence
		double z0_g = 0.001 * cell.snow_coverage() + 0.05 * (1-cell.snow_coverage());
		r_ag = (log((h+w.instrument_height)/z0_g) * log((h+w.instrument_height)/(0.1*z0_g))) / (k2 * w.Windspeed);
	}
}
