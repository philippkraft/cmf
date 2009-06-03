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
			real Tact(real Tpot,const cmf::upslope::SoilWaterStorage & sw,const cmf::upslope::vegetation::Vegetation & veg)
			{
				real
					rootfraction=veg.RootFraction(sw.get_upper_boundary(),sw.get_lower_boundary()),
					sw_pF=cmf::upslope::waterhead_to_pF(sw.get_matrix_potential());
				return piecewise_linear(sw_pF,3.35,4.2,Tpot*sw.cell.get_area()*0.001*rootfraction,0);
			}

			real constantETpot::calc_q( cmf::math::Time t ) 
			{
				cmf::upslope::vegetation::Vegetation veg=sw->cell.get_vegetation(); 
				return Tact(ETpot_value,*sw,veg);
			}

			real PenmanMonteithET::r_s( const cmf::upslope::vegetation::Vegetation & veg ) const
			{
				if (veg.StomatalResistance)
					return veg.StomatalResistance/(0.5*veg.LAI);
				else
					return 0.0;
			}

			real PenmanMonteithET::r_a( cmf::atmosphere::Weather A,const cmf::upslope::vegetation::Vegetation & veg ) const
			{
				real
					h=veg.Height,
					d=0.666667*h,	                         // zero plane displacement height
					z_om=0.123*h,                          // roughness length governing momentum transfer
					z_oh=0.1*z_om,                         // roughness length governing transfer of heat and vapour
					k2=0.1681,                             // von Karman's constant squared (0.41^2=0.1681)
					ra_u =   log((h+2-d)/z_om)
								 * log((h+2-d)/z_oh)
								 / k2;																	// Aerodynamic resistence * Windspeed
				return ra_u/maximum(A.Windspeed,0.5);   // Aerodynamic resistence
			}

			real PenmanMonteithET::calc_q( cmf::math::Time t )
			{
				cmf::upslope::Cell & cell=sw->cell;
				if (!cell.has_wet_leaves())
				{
					cmf::atmosphere::Weather A=cell.get_weather(t);
					cmf::upslope::vegetation::Vegetation veg=cell.get_vegetation();
					real 
						Rn = A.Rn(veg.albedo,daily), // Net radiation flux 
						G = daily ? 0									// If calculation takes place on daily basis, the soil heat flux is 0 (FAO 1998,Eq. 42)
						: Rn>0 ? 0.1*Rn : 0.5 * Rn,	           // On a shorter basis, it is proportional to Rn, different for day and night (FAO 1998,Eq. 45f)
						PM=PenmanMonteith(Rn-G,r_a(A,veg),r_s(veg),A.T,A.e_s-A.e_a);
					return Tact(PM,*sw,veg);
				}
				else
					return 0.0;
			}

			real ShuttleworthWallaceET::calc_q( cmf::math::Time t )
			{
				cmf::upslope::Cell & cell=m_soilwaterstorage->cell;
				cmf::atmosphere::Weather w=cell.get_weather(t);
				cmf::upslope::vegetation::Vegetation veg=cell.get_vegetation();
				bool IsEvaporation=m_right == &m_cell.get_evaporation();
				using namespace cmf::upslope::vegetation;
				if (IsEvaporation)
				{
					// If source is soil and target is evaporation calc soil evaporation
					if (m_soilwaterstorage && !m_cell.has_surface_water()) 
					{
						ShuttleworthWallace _ET_mm(w,m_soilwaterstorage->get_matrix_potential(),veg,!cell.has_wet_leaves());
						return _ET_mm.E * 1e-3 * cell.get_area() * (1-m_cell.snow_coverage());
					}
					// If source is canopy (which stores water)
					else if (m_waterstorage && m_waterstorage==m_cell.get_canopy() && !m_waterstorage->is_empty())
					{			
						ShuttleworthWallace _ET_mm(w,m_cell.layer_count() ? m_cell.get_layer(0).get_matrix_potential(): -2,veg,true);
						return _ET_mm.T * 1e-3*cell.get_area();
					}
					else if (m_waterstorage && !m_waterstorage->is_empty())
					{
						ShuttleworthWallace _ET_mm(w,0,veg,false);
					}
				}
				else if (m_soilwaterstorage && !m_cell.has_wet_leaves()) // Transpiration=Root uptake from soil (not presented if canopy is wet)
				{
					ShuttleworthWallace _ET_mm(w,m_soilwaterstorage->get_matrix_potential(),veg,false);
					return _ET_mm.T * 1e-3*cell.get_area();
				}
				if (cell.has_wet_leaves() && (cell.has_surface_water() || cell.snow_coverage()>=1))
					return 0;
				else
				{
					cmf::upslope::vegetation::ShuttleworthWallace _ET_mm(w,m_soilwaterstorage->get_potential(),veg,!cell.has_wet_leaves());
					real _ET_m3=0;
					// If the canopy is not empty, no water is taken from the soil by transpiration
					real root_frac=cell.get_vegetation().RootFraction(m_soilwaterstorage->get_upper_boundary(),m_soilwaterstorage->get_lower_boundary());
					if (!cell.has_wet_leaves())
						_ET_m3 += _ET_mm.T * 1e-3 * cell.get_area() * root_frac;			
					// use soil evaporation only if the soil water storage is the first layer and there is no snow and no surface water
					if (m_soilwaterstorage->get_upper_boundary()==0 && (!cell.has_surface_water()))
						_ET_m3 += _ET_mm.E * 1e-3 * cell.get_area() * (1-cell.snow_coverage());
					return _ET_m3;
				}
			}

			real CanopyStorageEvaporation::calc_q( cmf::math::Time t )
			{
				if (m_left->is_empty()) return 0;
				cmf::atmosphere::Weather w=m_cell.get_weather(t);
				cmf::upslope::vegetation::Vegetation veg=m_cell.get_vegetation();
				real PM=PenmanMonteith(w,veg,m_cell.z)*0.001*m_cell.get_area();
				return minimum(c_stor->State()*2/cmf::math::h.AsDays(),PM);
			}

			real HargreaveET::calc_q( cmf::math::Time t )
			{
				double pi=cmf::geometry::PI;
				cmf::atmosphere::Weather A=sw->cell.get_weather(t);
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
					TD = abs(A.Tmax-A.Tmin),  
					// Continentality factor, ranging between 0.19 (coastal regions) and 0.162
					// Calculated according to Z. Samani, see http://cagesun.nmsu.edu/~zsamani/research_material/files/Hargreaves-samani.pdf
					KT = 0.00185*(TD*TD) - 0.0433 * TD + 0.4023,
					//gives reference crop et in m / d
					etrc = 0.0135 * KT * s0 * sqrt(TD) * ( A.T + 17.8 );
				return etrc*sw->cell.get_vegetation().LAI/12;

			}
		}
	}
}