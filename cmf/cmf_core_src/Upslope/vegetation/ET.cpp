

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

			real Tact(real Tpot,const cmf::upslope::SoilLayer & sw,const cmf::upslope::vegetation::Vegetation & veg)
			{
				real
					rootfraction=veg.RootFraction(sw.get_upper_boundary(),sw.get_lower_boundary()),
					Tpot_vol=Tpot*sw.cell.get_area() * 0.001 * rootfraction;
			
				return piecewise_linear(sw.get_matrix_potential(),-160,-5,0,Tpot_vol);
			}

			real constantETpot::calc_q( cmf::math::Time t ) 
			{
				cmf::upslope::SoilLayer::ptr layer=sw.lock();
				cmf::upslope::vegetation::Vegetation veg=layer->cell.get_vegetation(); 
				return Tact(ETpot_value,*layer,veg);
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
					ra_u =   log((h+A.instument_height-d)/z_om)
								 * log((h+A.instument_height-d)/z_oh)
								 / k2;																	// Aerodynamic resistence * Windspeed
				return ra_u/maximum(A.Windspeed,0.5);   // Aerodynamic resistence
			}

			real PenmanMonteithET::calc_q( cmf::math::Time t )
			{
				cmf::upslope::SoilLayer::ptr layer=sw.lock();
				cmf::upslope::Cell & cell=layer->cell;
				if (!cell.has_wet_leaves())
				{
					cmf::atmosphere::Weather A=cell.get_weather(t);
					cmf::upslope::vegetation::Vegetation veg=cell.get_vegetation();
					real 
						Rn = A.Rn(veg.albedo,daily), // Net radiation flux 
						G = daily ? 0									// If calculation takes place on daily basis, the soil heat flux is 0 (FAO 1998,Eq. 42)
						: Rn>0 ? 0.1*Rn : 0.5 * Rn,	           // On a shorter basis, it is proportional to Rn, different for day and night (FAO 1998,Eq. 45f)
						PM=PenmanMonteith(Rn-G,r_a(A,veg.Height),r_s(veg),A.T,A.e_s-A.e_a);
					return Tact(PM,*layer,veg);
				}
				else
					return 0.0;
			}

			void PenmanMonteithET::use_for_cell( cmf::upslope::Cell & cell )
			{
				for (int i = 0; i < cell.layer_count() ; ++i)
				{
					new PenmanMonteithET(cell.get_layer(i),cell.get_transpiration());
				}
			}
			real ShuttleworthWallaceET::calc_q( cmf::math::Time t )
			{
				cmf::upslope::SoilLayer::ptr layer=m_SoilLayer.lock();
				cmf::water::WaterStorage::ptr canopy=m_waterstorage.lock();
				cmf::upslope::Cell & cell=layer->cell;
				cmf::atmosphere::Weather w=cell.get_weather(t);
				cmf::upslope::vegetation::Vegetation veg=cell.get_vegetation();
				bool IsEvaporation = right_node() == m_cell.get_evaporation();
				using namespace cmf::upslope::vegetation;
				if (IsEvaporation)
				{
					// If source is soil and target is evaporation calc soil evaporation
					if (layer && !m_cell.has_surface_water()) 
					{
						ShuttleworthWallace _ET_mm(w,layer->get_matrix_potential(),veg,!cell.has_wet_leaves());
						return _ET_mm.E * 1e-3 * cell.get_area() * (1-m_cell.snow_coverage());
					}
					// If source is canopy (which stores water)
					else if (canopy && canopy == m_cell.get_canopy() && !canopy->is_empty())
					{			
						ShuttleworthWallace _ET_mm(w,m_cell.layer_count() ? m_cell.get_layer(0)->get_matrix_potential(): -2,veg,true);
						return _ET_mm.T * 1e-3*cell.get_area();
					}
					else if (canopy && !canopy->is_empty())
					{
						ShuttleworthWallace _ET_mm(w,0,veg,false);
					}
				}
				else if (canopy && !m_cell.has_wet_leaves()) // Transpiration=Root uptake from soil (not presented if canopy is wet)
				{
					ShuttleworthWallace _ET_mm(w,layer->get_matrix_potential(),veg,false);
					return _ET_mm.T * 1e-3*cell.get_area();
				}
				if (cell.has_wet_leaves() && (cell.has_surface_water() || cell.snow_coverage()>=1))
					return 0;
				else
				{
					cmf::upslope::vegetation::ShuttleworthWallace _ET_mm(w,layer->get_potential(),veg,!cell.has_wet_leaves());
					real _ET_m3=0;
					// If the canopy is not empty, no water is taken from the soil by transpiration
					real root_frac=cell.get_vegetation().RootFraction(layer->get_upper_boundary(),layer->get_lower_boundary());
					if (!cell.has_wet_leaves())
						_ET_m3 += _ET_mm.T * 1e-3 * cell.get_area() * root_frac;			
					// use soil evaporation only if the soil water storage is the first layer and there is no snow and no surface water
					if (layer->get_upper_boundary()==0 && (!cell.has_surface_water()))
						_ET_m3 += _ET_mm.E * 1e-3 * cell.get_area() * (1-cell.snow_coverage());
					return _ET_m3;
				}
			}

			void ShuttleworthWallaceET::use_for_cell( cmf::upslope::Cell& cell )
			{
				for (int i = 0; i < cell.storage_count() ; ++i)
				{
					new ShuttleworthWallaceET(cell.get_storage(i),cell.get_evaporation(),cell);
				}

				for (int i = 0; i < cell.layer_count() ; ++i)
				{
					if (i == 0) new ShuttleworthWallaceET(cell.get_layer(i),cell.get_evaporation(),cell,"Soil evaporation (SW)");
					new ShuttleworthWallaceET(cell.get_layer(i),cell.get_transpiration(),cell,"Root water uptake (SW)");
				}
			}

			real CanopyStorageEvaporation::calc_q( cmf::math::Time t )
			{
				cmf::water::WaterStorage::ptr canopy=c_stor.lock();
				if (left_node()->is_empty()) return 0;
				cmf::atmosphere::Weather w=m_cell.get_weather(t);
				cmf::upslope::vegetation::Vegetation veg=m_cell.get_vegetation();
				real PM=PenmanMonteith(w,veg,m_cell.z)*0.001*m_cell.get_area();
				return minimum(canopy->get_state()*2/cmf::math::h.AsDays(),PM);
			}

			real HargreaveET::calc_q( cmf::math::Time t )
			{
				cmf::upslope::SoilLayer::ptr layer=sw.lock();
				double pi=cmf::geometry::PI;
				cmf::atmosphere::Weather A=layer->cell.get_weather(t);
				cmf::upslope::vegetation::Vegetation veg=layer->cell.get_vegetation();
				double 
					// Latitude in radians
					phi = pi * 51 / 180.0,
					//solar declination (radians)
					gamma = 0.4039f * sin((2*pi / 365 * t.AsDate().DOY()) - 1.405),
					//sunset hour angle (radians)
					ws = acos (-tan(phi) * tan(gamma)),
					//relative distance between earth and sun
					dr = 1 + 0.033 *  cos((2*pi/365 * t.AsDate().DOY()) ),
					//s0 = extraterrestrial solar radiation  mm/day
					s0 = 15.392 * dr *  ( ws * sin(phi) * sin(gamma) + cos(phi) * cos(gamma) * sin(ws)),
					//difference between mean monthly min and max temperatures - an estimate!
					TD = fabs(A.Tmax-A.Tmin),  
					// Continentality factor, ranging between 0.19 (coastal regions) and 0.162
					// Calculated according to Z. Samani, see http://cagesun.nmsu.edu/~zsamani/research_material/files/Hargreaves-samani.pdf
					KT = 0.00185*(TD*TD) - 0.0433 * TD + 0.4023,
					//gives reference crop et in m / d
					etrc = 0.0135 * KT * s0 * sqrt(TD) * ( A.T + 17.8 );

				

				return Tact(etrc*veg.LAI/2.88,*layer,veg);

			}

			void HargreaveET::use_for_cell( cmf::upslope::Cell & cell )
			{
				for (int i = 0; i < cell.layer_count() ; ++i)
				{
					new HargreaveET(cell.get_layer(i),cell.get_evaporation());
				}
			}
			real PenmanEvaporation::calc_q( cmf::math::Time t )
			{
				cmf::river::OpenWaterStorage::ptr source=m_source.lock();
				if (left_node()->is_empty()) return 0;
				cmf::atmosphere::Weather w=m_meteo->get_weather(t);
				real PM=PenmanMonteith(w.Rn(0.1),PenmanMonteithET::r_a(w,0.1),0.0,w.T,w.e_s-w.e_a) * 0.001 * source->wet_area();
				return minimum(source->get_state()*2/cmf::math::h.AsDays(),PM);
			}

			PenmanEvaporation::PenmanEvaporation( cmf::river::OpenWaterStorage::ptr source,cmf::water::flux_node::ptr Evap_target,const cmf::atmosphere::Meteorology& meteo ) : cmf::water::flux_connection(source,Evap_target,"Penman evaporation from open water"), m_meteo(meteo.copy())
			{
				NewNodes();
			}
		}
	}
}
