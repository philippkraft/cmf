

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
#ifndef surfacefluxes_h__
#define surfacefluxes_h__

#include "../../water/flux_connection.h"
#include "../cell.h"
#include "../../atmosphere/precipitation.h"
#include "AtmosphericFluxes.h"
#include "../vegetation/ET.h"
namespace cmf {
	namespace upslope {
		namespace connections {
			/// @defgroup surfacefluxes Connections for local aboveground fluxes
			/// @ingroup connections

			/// @ingroup surfacefluxes
			/// @brief Calculates the overflow of a canopy storage using a kinematic wave approach
			///
			/// This model routes only water that exceeds the canopy capacity to the ground with
			/// an ad hoc estimated function:
			/// \f[q_{CO} = \left(\frac{V_{act}-V_{max}}{V_{max}}\right) ^2 \cdot 2400 \frac{A_{cell}}{1000}\f]
			/// With:
			/// - \f$q_{CO}(t)[\frac{m^3}{day}]\f$: The flux from canopy to the ground
			/// - \f$V_{act}[mm]=1000 [mm/m] \frac{V_{canopy}[m^3]}{A_{cell} [m^2]}\f$ The stored water of the canopy in mm
			/// - \f$V_{max}[mm]=c_{LAI}[mm] LAI\f$ The capacity of the canopy in mm, defined by the factor CanopyCapacityPerLAI [mm/LAI], and 
			///   the leaf area index LAI. (see: cmf::upslope::Vegetation)
			class CanopyOverflow : public cmf::water::flux_connection {
			protected:
				cmf::upslope::Cell & m_cell;
				std::tr1::weak_ptr<cmf::water::WaterStorage> m_Canopy;
				virtual real calc_q(cmf::math::Time t) ;
				virtual void NewNodes()
				{
					m_Canopy=cmf::water::WaterStorage::cast(left_node());
				}
			public:
				CanopyOverflow(cmf::water::WaterStorage::ptr Canopy,cmf::water::flux_node::ptr target,cmf::upslope::Cell & cell)
					: cmf::water::flux_connection(Canopy,target,"Canopy overflow"),m_cell(cell) {
						NewNodes();
				}
				static CanopyOverflow* use_for_cell(cmf::upslope::Cell & cell)
				{
					// If the canopy of this cell is not a storage, create a canopy storage
					cmf::water::WaterStorage::ptr canopy=cell.add_storage("Canopy",'C');	
					if (cell.get_rain_source()->connection_to(*cell.get_surfacewater()))
					{
						cell.get_rain_source()->remove_connection(cell.get_surfacewater());
					}
					new Rainfall(canopy,cell,false,true);
					new Rainfall(cell.get_surfacewater(),cell,true,false);
					new ET::CanopyStorageEvaporation(canopy,cell.get_evaporation(),cell);
					return new CanopyOverflow(canopy,cell.get_surfacewater(),cell);
				}
			};

			/// @ingroup surfacefluxes
			/// @brief Interception storage overflow according to the Rutter and Morton (1977) model 
			///
			/// Calculates the interception overflow as a storage depending fraction of incoming rainfall
			/// The Rutter model of interception reads as follows after 
			/// Meuser, A., 1990. Effects of afforestation on run-off characteristics. Agric. For. Meteorol. 50: 125-138.:
			/// \f[\frac{dI_C(t)}{dt}=P(t)(1-p_F-p_S)-P(t)(1-p_F-p_S)\frac{I_C(t)}{I_CMAX}-f_I(E-e)(t)\f]
			/// With \f$I_C\f$ the current canopy storage and \f$P(t)\f$ the current rainfall.
			///
			/// The second term of the equation denotes the flux from the canopy to the ground.
			/// The implemented formula for canopy storage overflow reads then as:
			/// \f[q_{CO}(t) = P_{net}(t)\frac{V_{act}[mm]}{V_{max}[mm]}\f]
			/// With:
			/// - \f$q_{CO}(t)[\frac{m^3}{day}]\f$: The flux from canopy to the ground
			/// - \f$P_{net}(t)[\frac{m^3}{day}]\f$: The flux from the rain to the canopy
			/// - \f$V_{act}[mm]=1000 [mm/m] \frac{V_{canopy}[m^3]}{A_{cell} [m^2]}\f$ The stored water of the canopy in mm
			/// - \f$V_{max}[mm]=c_{LAI}[mm]\cdot LAI\f$ The capacity of the canopy in mm, defined by the factor CanopyCapacityPerLAI [mm/LAI], and 
			///   the leaf area index LAI. (see: cmf::upslope::Vegetation)
			class RutterInterception : public cmf::water::flux_connection {
			protected:
				cmf::upslope::Cell & m_cell;
				std::tr1::weak_ptr<cmf::water::WaterStorage> m_Canopy;
				virtual real calc_q(cmf::math::Time t) ;
				virtual void NewNodes()
				{
					m_Canopy=cmf::water::WaterStorage::cast(left_node());
				}
			public:
				RutterInterception(cmf::water::WaterStorage::ptr Canopy,cmf::water::flux_node::ptr target,cmf::upslope::Cell & cell)
					: cmf::water::flux_connection(Canopy,target,"Rutter interception"),m_cell(cell) {
						NewNodes();
				}
				static RutterInterception* use_for_cell(cmf::upslope::Cell & cell)
				{
					// If the canopy of this cell is not a storage, create a canopy storage
					cmf::water::WaterStorage::ptr canopy=cell.add_storage("Canopy",'C');	
					if (cell.get_rain_source()->connection_to(*cell.get_surfacewater()))
					{
						cell.get_rain_source()->remove_connection(cell.get_surfacewater());
					}
					new Rainfall(canopy,cell,false,true);
					new Rainfall(cell.get_surfacewater(),cell,true,false);
					new ET::CanopyStorageEvaporation(canopy,cell.get_evaporation(),cell);
					return new RutterInterception(canopy,cell.get_surfacewater(),cell);
				}

			};
			/// @ingroup surfacefluxes
			/// Calculates snow melt using a simple degree day method
			class SimpleTindexSnowMelt : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::water::WaterStorage> m_Snow;
				std::tr1::weak_ptr<cmf::water::flux_node> m_Surfacewater;
				cmf::upslope::Cell & m_cell;
				virtual real calc_q(cmf::math::Time t);
				virtual void NewNodes()
				{
					m_Snow=cmf::water::WaterStorage::cast(left_node());
					m_Surfacewater=right_node();
				}
			public:
				real SnowMeltRate;
				SimpleTindexSnowMelt(cmf::water::WaterStorage::ptr snow,cmf::water::flux_node::ptr surface_water,cmf::upslope::Cell& cell)
					: flux_connection(snow,surface_water,"Simple T-Index snow melt"),m_cell(cell),SnowMeltRate(7.0)
				{
					NewNodes();
				}
				static void use_for_cell(cmf::upslope::Cell& cell)
				{
					cmf::water::WaterStorage::ptr snow=cell.add_storage("Snow",'S');
					new cmf::upslope::connections::Snowfall(snow,cell);

					new SimpleTindexSnowMelt(snow,cell.get_surfacewater(),cell);
				}
			};


			/// @ingroup surfacefluxes
			/// Calculates snow melt using the surface energy budget method
			class EnergyBudgetSnowMelt : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::water::WaterStorage> m_Snow;
				std::tr1::weak_ptr<cmf::water::flux_node> m_Surfacewater;
				cmf::upslope::Cell & m_cell;
				virtual real calc_q(cmf::math::Time t);
				virtual void NewNodes()
				{
					m_Snow=cmf::water::WaterStorage::cast(left_node());
					m_Surfacewater=right_node();
				}
			public:
				EnergyBudgetSnowMelt(cmf::water::WaterStorage::ptr snow,cmf::water::flux_node::ptr surface_water,cmf::upslope::Cell& cell)
					: flux_connection(snow,surface_water,"Energy budget snow melt"),m_cell(cell)
				{
					NewNodes();
				}
				static void use_for_cell(cmf::upslope::Cell& cell)
				{
					cmf::water::WaterStorage::ptr snow=cell.add_storage("Snow",'S');
					new cmf::upslope::connections::Snowfall(snow,cell);
					new EnergyBudgetSnowMelt(snow,cell.get_surfacewater(),cell);
				}
			};

		}
	}
}

#endif // surfacefluxes_h__
