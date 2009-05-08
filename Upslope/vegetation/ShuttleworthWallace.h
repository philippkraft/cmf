#ifndef ShuttleworthWallace_h__
#define ShuttleworthWallace_h__

#include "../vegetation/StructVegetation.h"
#include <cmath>
#include "../../Atmosphere/Meteorology.h"

namespace cmf {
	namespace upslope {
		namespace vegetation {
			/// Calculates the sum of soil evaporation and transpiration according to Shuttleworth & Wallace 1985, as implemented in BROOK 90 (Federer 1990)
			///
			/// The difference to BROOK90 is, that the actual transpiration is not calculated by plant resitance and potential gradient between plant and soil,
			/// but by an piecewise linear function of the pF value \f$ pF = \log_{10}\left(-\Psi [hPa]\right) \f$:
			/// \f[ \frac{T_{act}}{T_{pot}} = \left\{\begin{array}{cl}
			/// 1 & \mbox{if $pF \le 3.35$} \\ 
			/// \frac{pF - 4.2}{3.35 - 4.2} & \mbox{if $pF \in [3.35 .. 4.2] $} \\
			/// 0 & \mbox{if $pF \ge 4.2$} \end{array}\right. \f]
			///
			/// Calculation procedure, as in BROOK 90:
			///
			/// Evapotranspiration from the canopy: 
			/// \f$\lambda ET_{canopy} = \frac {r_{ac} \Delta\ R_{n,canopy} + c_p\rho D_0}{\Delta \gamma r_{ac} + \gamma r_{sc}} \f$
			///
			/// Evaporation from the ground: 
			/// \f$\lambda E_{ground} = \frac {r_{as} \Delta\ R_{n,ground} + c_p\rho D_0}{\Delta \gamma r_{as} + \gamma r_{ss}} \f$
			///
			/// with
			///  - \f$ \Delta = \frac{de_s}{dT} = 4098\ 0.6108 \exp\left(\frac{17.27 T}{T+237.3}\right)(T+237.3)^{-2} \f$, the slope of the sat. vap. press. T function
			///  - \f$ R_{n,ground} = R_n \exp(-C_R LAI) \f$, the net radiation flux in the ground
			///  - \f$ R_{n_canopy} = R_n - R_{n,ground} \f$, the net radiation flux in the canopy
			///  - \f$ \lambda,c_p\rho,\gamma,C_R \f$ constants lambda, c_p_rho, gamma, C_R
			///  - \f$ D_0 \f$ vapor pressure deficit at effective source height, see function D0
			///  - \f$ r_{ac}, r_{sc}, r_{as}, r_{ss} \f$ Resistances for the vapor pressure (see below)
			///

			struct ShuttleworthWallace
			{
				/// Actual soil evaporation in mm/day
				real E;
				/// Actual canopy transpiration in mm/day
				real T;
				/// Actual evapotranspiration in mm/day
				real ET() {return E+T;}
			private:
				struct Resistances	{
					real aa,ac,as,ss,sc;
				};
				/// @name Constants
				//@{
				static const double 
					/// von Karmann constant (0.4)
					k,
					/// wind/diffusivity extinction coefficient, dimensionless (2.5)
					/// NN is the canopy extinction coefficient for wind and eddy diffusivity. 
					/// NN is fixed at 2.5 following Shuttleworth and Gurney (1990). 
					/// Federer and others (1995) show that PE is insensitive to the value of n, 
					/// but they did not test sparse canopies in a wet climate. 
					NN,
					/// Light extinction ratio \f$C_R=0.6\f$, 
					/// should become a variable 0.5 for coniferous, 0.6 for deciduous trees and 0.7 for ground vegetation
					C_R,
					/// Conversion factor \f$ \frac{MJ}{day} \to W = \frac{10^6 \frac J{MJ}}{86400 \frac{s}{day}}\f$
					MJ_to_W,
					///Volumetric heat capacity \f$ c_p\rho = 1240 \frac{J}{m^3 K}\f$
					c_p_rho, 
					///Latent heat of vaporization \f$ \lambda = 2.45 \frac{MJ}{kg}\f$
					lambda,	    
					///Psychrometric constant \f$ \gamma = 0.067 \frac{kPa}{K}\f$
					gamma;		
				//@}
				/// @name Private functions
				//@{
				/// Calculates the resistances of the ground and the canopy
				///
				/// The private type Resistances has the members:
				/// - aa for the aerodynamic resistance of the atmosphere
				/// - ac for the aerodynamic resistance of the canopy
				/// - as for the aerodynamic resistance of the ground
				/// - sc for the surface resistance of canopy
				/// - ss for the surface resistance of the ground
				///
				/// Aerodynamic resistances \f$r_{ac},r_{as},r_{aa}\f$
				/// - \f$ r_{ac} = 100 n \frac{\sqrt{\frac {w_{leaf}}{u_h}}}{1-\exp(-\frac n 2)} \f$
				///
				/// @param vegParameters Vegetation parameters
				/// @param u Windspeed in m/s
				/// @param Rs Global radiation in MJ/(m2 day)
				/// @param T Temperature in °C
				/// @param vpd Vapor pressure deficit
				/// @param soil_matric_potential Matrix potential of soil (in m)
				/// @param meas_height_above_canopy Height of the wind measurement above the canopy
				/// @param CanopyStoresWater Flag indicating wet leaves. If true, a canopy surface resistance of 0 is assumed
				/// @param ReturnPET 
				Resistances resitances(const cmf::upslope::vegetation::Vegetation & vegParameters, real u,real Rs,real T,real vpd,
					real soil_matric_potential,real meas_height_above_canopy=2,bool CanopyStoresWater=false,bool ReturnPET=false);
				/// Returns the latent heat flux after Penman-Monteith in W/m2
				///
				/// \f$ ET = \frac {r_{a} \Delta\ R_n + c_p\rho (e_s-e_a)}{\Delta \gamma r_{a} + \gamma r_{s}} \f$
				/// @param Rn Net radiation in W/m2
				/// @param vpd Vapor pressure deficit in kPa
				/// @param Delta dEsat/dTair, kPa/K 
				/// @param r_a Aerodynamic resistance in s/m
				/// @param r_s surface resistance in s/m
				real PM(real Rn,real vpd,real Delta,real r_a,real r_s);
				/// Calculates the vapor pressure deficit at the effective source height
				///
				/// Calculation of D0 (from BROOK90 (Federer 1990) citing Shuttleworth & Wallace 1985)
				///
				/// \f$ D_0 = e_s - e_a + r_{aa} \frac{\Delta\ R_n - LE (\Delta + \gamma)}{c_p\rho}\f$
				///
				/// with
				/// - \f$e_s-e_a\f$ Saturation deficit
				/// - \f$ r_{aa} \f$ Aerodynamic resistance
				/// - \f$ LE = c_C LE_C + c_S LE_S \f$, latent heat flux
				/// - \f$ LE_C = \frac {(r_{ac} + r_{aa}) \Delta\ R_{n} + c_p\rho D_C}{\Delta \gamma (r_{ac} + r_{aa}) + \gamma r_{sc}} \f$
				/// - \f$ D_C = e_s - e_a - \frac{\Delta}{c_p\rho}R_{n,canopy}\f$
				/// - \f$ LE_S = \frac {(r_{as} + r_{aa}) \Delta\ R_{n} + c_p\rho D_S}{\Delta \gamma (r_{as} + r_{aa}) + \gamma r_{ss}} \f$
				/// - \f$ D_S = e_s - e_a - \frac{\Delta}{c_p\rho}R_{n,ground}\f$
				/// - \f$ \frac 1 {c_C} = \frac{1 + R_S R_A}{R_C (R_S + R_A)} \f$
				/// - \f$ \frac 1 {c_S} = \frac{1 + R_C R_A}{R_S (R_C + R_A)} \f$
				/// - \f$ R_S=(\Delta + \gamma) r_{as} + \gamma r_{ss} \f$
				/// - \f$ R_C=(\Delta + \gamma) r_{ac} + \gamma r_{sc} \f$
				/// - \f$ R_A=(\Delta + \gamma) r_{aa} \f$

				/// @returns   vapor pressure deficit at the effective source height
				/// @param _r Resistances against vapor transport of the surface and the canopy
				/// @param Rn_canopy Net radiation flux to the canopy in W/m2
				/// @param Rn_ground Net radiation flux to the ground
				/// @param vpd Vapor pressure deficit in kPa
				/// @param Delta Slope of the vapor pressure Temperature relationship
				real D0(Resistances _r,real Rn_canopy,real Rn_ground, real vpd, real Delta );
				//@}
			public:
				/// Calculates the transpiration and the soil evaporation from dry surfaces
				/// @param w A meteorological data record
				/// @param soilwater_matrixpotential The suction of the soil water in m water column
				/// @param veg The parameters of the vegetation
				/// @param CanopyStoresWater Flag indicating wet leaves. If true, a canopy surface resistance of 0 is assumed
				/// @param measurement_height_above_canopy The height of the wind speed above the canopy in m
				ShuttleworthWallace(cmf::atmosphere::Weather w,real soilwater_matrixpotential,const cmf::upslope::vegetation::Vegetation & veg,bool CanopyStoresWater=false,real measurement_height_above_canopy=2);
			};
		}		
	}
	
}
#endif // ShuttleworthWallace_h__
