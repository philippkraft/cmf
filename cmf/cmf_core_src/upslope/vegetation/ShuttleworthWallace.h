

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
#ifndef ShuttleworthWallace_h__
#define ShuttleworthWallace_h__

#include "../cell.h"
#include <cmath>
#include "../../atmosphere/meteorology.h"
#include "ET.h"
#include "../../water/flux_connection.h"
#include "../SoilLayer.h"

namespace cmf {
	namespace upslope {
		namespace ET {
			/// Calculates the sum of soil evaporation and transpiration according to Shuttleworth & Wallace 1985, as implemented in BROOK 90 (Federer 1990)
			///
			/// The difference to BROOK90 is, that the actual transpiration is not calculated by plant resitance and potential gradient between plant and soil,
			/// but by an piecewise linear function of the pF value \f$ pF = \log_{10}\left(-\Psi [hPa]\right) \f$:
			/// \f[ \frac{T_{act}}{T_{pot}} = \begin{cases}1 & pF \le 3.35 \\ 
			/// \frac{pF - 4.2}{3.35 - 4.2} & pF \in [3.35 .. 4.2] \\
			/// 0 & pF \ge 4.2\end{cases} \f]
			///
			/// Calculation procedure, as in BROOK 90:
			///
			/// Evapotranspiration from the canopy: 
			/// \f$\lambda ET_{canopy} = \frac {r_{ac} \Delta\ R_{n,canopy} + c_p\rho D_0}{\Delta \gamma r_{ac} + \gamma r_{sc}} \f$
			///
			/// Evaporation from the ground: 
			/// \f$\lambda E_{ground} = \frac {r_{as} \Delta\ R_{n,ground} + c_p\rho D_0}{\Delta \gamma r_{as} + \gamma r_{ss}} \f$
			///
			/// In case of a complete surface water covered ground, the surface resistance \f$r_{ss}\f$ becomes 0. (GIR)
			///
			/// with
			///  - \f$ \Delta = \frac{de_s}{dT} = 4098\ 0.6108 \exp\left(\frac{17.27 T}{T+237.3}\right)(T+237.3)^{-2} \f$, the slope of the sat. vap. press. T function
			///  - \f$ R_{n,ground} = R_n \exp(-C_R LAI) \f$, the net radiation flux to the ground
			///  - \f$ R_{n,canopy} = R_n - R_{n,ground} \f$, the net radiation flux to the canopy
			///  - \f$ \lambda,c_p\rho,\gamma\f$ latent heat of vaporization, heat capacity of air, psychrometer constant
			///  - \f$ D_0 \f$ vapor pressure deficit at effective source height, see function D0
			///  - \f$ r_{ac}, r_{sc}, r_{as}, r_{ss} \f$ Resistances for the vapor pressure (see below)
			///

			class ShuttleworthWallace
				: 	public cmf::atmosphere::aerodynamic_resistance

			{
			private:
				cmf::upslope::Cell& cell;
				mutable cmf::math::Time refresh_time;
				static double RSSa,RSSb,RSSa_pot;
				vegetation::Vegetation v;
				cmf::atmosphere::Weather w;
				std::vector<cmf::water::flux_connection*> _connections;

			public:
				typedef std::shared_ptr<ShuttleworthWallace> ptr;
				/// Aerodynamic resistance between the effective source height to the free atmosphere
				double RAA;
				/// Aerodynamic resistance inside of the canopy
				double RAC; 
				/// Surface resistance of the canopy (depending on stomatal resistance and LAI)
				double RSC;
				/// Aerodynamic resistance from the ground to the effective source height
				double RAS;
				/// Surface resistance of the soil (depending on the topsoil matrix potential)
				double RSS; 
				int refresh_counter;
				/// Calculates all the values
				void refresh(cmf::math::Time t);
				void refresh() {
					refresh_time-=cmf::math::min;
					refresh(refresh_time+cmf::math::min);
				}
				/// Potential transpiration rate in mm/day
				double PTR;
				/// Potential snow evaporation rate in mm/day
				double PSNVP;
				/// Actual snow evaporation rate in mm/day
				double ASNVP;
				/// Ground evaporation rate in mm/day (either from soil or from surfacewater)
				double GER; 
				/// Potential leaf evaporation rate in mm/day
				double PIR;
				/// Actual leaf evaporation rate in mm/day
				double AIR;
				/// potential surface water evaporation rate in mm/day, actual rate depends on surfacewater coverage
				double ATR_sum;
				/// actual transpiration rate per layer in mm/day
				cmf::math::num_array ATR;
				/// multiplier to reduce snow evaporation, dimensionless. 
				/// KSNVP is an arbitrary and frustrating correction factor 
				/// needed for snow evaporation (SNVP) to substantially reduce its value. 
				/// This is the only arbitrary fudge factor in BROOK90 and is needed 
				/// because the Shuttleworth-Gurney aerodynamic resistances as corrected for SAI 
				/// are much too low to give reasonable snow evaporation from forests
				double KSNVP;

				/// If allow_dew is true (default), negative net radiation can lead to the formation of dew 
				/// and rime. Setting it to false, checks each evapotranspiration term to be negative and turns
				/// it then to zero.
				bool allow_dew;

				// Overwrites for methods
				double transp_from_layer(cmf::upslope::SoilLayer::ptr sl, cmf::math::Time t);
				double evap_from_layer(cmf::upslope::SoilLayer::ptr sl, cmf::math::Time t);
				double evap_from_surfacewater(cmf::river::OpenWaterStorage::ptr ows, cmf::math::Time t);
				double evap_from_canopy(cmf::water::WaterStorage::ptr canopy, cmf::math::Time t);
				double evap_from_snow(cmf::water::WaterStorage::ptr snow, cmf::math::Time t);

				virtual void get_aerodynamic_resistance(double & r_ag,double & r_ac, cmf::math::Time t)  const;

				/// Sets the parameters of the soil surface resistance, a function of the actual water potential
				/// \f[ r_{ss} = RSS_a \left(\frac{\Psi}{\Psi_{RSS_a}}\right)^{RSS_b} \f]
				/// @param _RSSa Resistance in s/m at potential in RSSa_pot
				/// @param _RSSb Exponent for curve shape
				/// @param _RSSa_pot Matrix potential (m), where \f$r_{ss} = RSS_a\f$
				static void set_RSS_parameters(double _RSSa=500., double _RSSb=1.0, double _RSSa_pot=-3.22) {
					RSSa = _RSSa;
					RSSb = _RSSb;
					RSSa_pot = _RSSa_pot;
				}




				/// Calculates the transpiration and the soil evaporation from dry surfaces
				ShuttleworthWallace(cmf::upslope::Cell& cell, bool allow_dew=true);

				/// @brief Uses ShuttleworthWallace for the complete ET system of the cell.
				///
				/// The installation includes the following:
				/// * Create a new ShuttleworthWallace instance (SW) as central calculation unit
				/// * Connect each soil layer with cell.transpiration using the generic "transpiration" connection, controlled by SW
				/// * Connect the top soil layer with cell.evaporation using the generic "soilevaporation" connection, controlled by SW
				/// * If a canopy water storage exists, use SW for Canopy evaporation
				/// * If a snow storage exist, use SW for snow evaporation
				/// * if a surface water storage exist, create a connection between it and evaporation (open water transpiration)
				static ShuttleworthWallace::ptr use_for_cell(cmf::upslope::Cell& cell);
				~ShuttleworthWallace();
			};
			

			/// @brief Connection for Shuttleworth-Wallace transpiration
			class SW_transpiration : public cmf::water::flux_connection {
			private:
				ShuttleworthWallace::ptr _owner;
				std::weak_ptr<cmf::upslope::SoilLayer> _soillayer;
			protected:
				real calc_q(cmf::math::Time t) {
					return _owner->transp_from_layer(_soillayer.lock(), t);
				}
				void NewNodes()
				{
					_soillayer = cmf::upslope::SoilLayer::cast(left_node());
				}
			public:
				SW_transpiration(cmf::upslope::SoilLayer::ptr source, cmf::water::flux_node::ptr ET_target, 
					ShuttleworthWallace::ptr owner)
					: flux_connection(source, ET_target, "Shuttleworth Wallace transpiration"),
					_soillayer(source), _owner(owner)
				{
					NewNodes();
				}
			};

			/// @brief Connection for Shuttleworth-Wallace ground evaporation
			class SW_evap_from_layer : public cmf::water::flux_connection {
			private:
				ShuttleworthWallace::ptr _owner;
				std::weak_ptr<cmf::upslope::SoilLayer> _soillayer;
			protected:
				real calc_q(cmf::math::Time t) {
					return _owner->evap_from_layer(_soillayer.lock(), t);
				}
				void NewNodes()
				{
					_soillayer = cmf::upslope::SoilLayer::cast(left_node());
				}
			public:
				SW_evap_from_layer(cmf::upslope::SoilLayer::ptr source, cmf::water::flux_node::ptr ET_target, ShuttleworthWallace::ptr owner)
					: flux_connection(source, ET_target, "Shuttleworth Wallace ground evaporation"),
					_soillayer(source), _owner(owner)
				{
					NewNodes();
				}
			};

			/// @brief Connection for Shuttleworth-Wallace canopy interception evaporation
			class SW_evap_from_canopy : public cmf::water::flux_connection {
			private:
				ShuttleworthWallace::ptr _owner;
				std::weak_ptr<cmf::water::WaterStorage> _surfacewater;
			protected:
				real calc_q(cmf::math::Time t) {
					return _owner->evap_from_canopy(_surfacewater.lock(), t);
				}
				void NewNodes()
				{
					_surfacewater = cmf::water::WaterStorage::cast(left_node());
				}
			public:
				SW_evap_from_canopy(cmf::water::WaterStorage::ptr source, cmf::water::flux_node::ptr ET_target, ShuttleworthWallace::ptr owner)
					: flux_connection(source, ET_target, "Shuttleworth Wallace canopy interception evaporation"),
					_surfacewater(source), _owner(owner)
				{
					NewNodes();
				}
			};

			/// @brief Connection for Shuttleworth-Wallace canopy interception evaporation
			class SW_evap_from_snow : public cmf::water::flux_connection {
			private:
				ShuttleworthWallace::ptr _owner;
				std::weak_ptr<cmf::water::WaterStorage> _surfacewater;
			protected:
				real calc_q(cmf::math::Time t) {
					return _owner->evap_from_snow(_surfacewater.lock(), t);
				}
				void NewNodes()
				{
					_surfacewater = cmf::water::WaterStorage::cast(left_node());
				}
			public:
				SW_evap_from_snow(cmf::water::WaterStorage::ptr source, cmf::water::flux_node::ptr ET_target, ShuttleworthWallace::ptr owner)
					: flux_connection(source, ET_target, "Shuttleworth Wallace snow sublimation"),
					_surfacewater(source), _owner(owner)
				{
					NewNodes();
				}
			};


			/// @brief Connection for Shuttleworth-Wallace canopy interception evaporation
			class SW_evap_from_surfacewater : public cmf::water::flux_connection {
			private:
				ShuttleworthWallace::ptr _owner;
				std::weak_ptr<cmf::river::OpenWaterStorage> _surfacewater;
			protected:
				real calc_q(cmf::math::Time t) {
					return _owner->evap_from_surfacewater(_surfacewater.lock(), t);
				}
				void NewNodes()
				{
					_surfacewater = cmf::river::OpenWaterStorage::cast(left_node());
				}
			public:
				SW_evap_from_surfacewater(cmf::river::OpenWaterStorage::ptr source, cmf::water::flux_node::ptr ET_target, ShuttleworthWallace::ptr owner)
					: flux_connection(source, ET_target, "Shuttleworth Wallace surface water evaporation"),
					_surfacewater(source), _owner(owner)
				{
					NewNodes();
				}
			};

		}
	}
	
}
#endif // ShuttleworthWallace_h__
