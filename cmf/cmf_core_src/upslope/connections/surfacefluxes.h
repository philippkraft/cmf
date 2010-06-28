

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
			/// Calculates the overflow of a canopy storage
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
					if (cell.get_rain_source()->get_connection(*cell.get_surfacewater()))
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
				virtual real calc_q(cmf::math::Time t) {
					cmf::water::WaterStorage::ptr snow(m_Snow);
					if (snow->get_volume()<=0) return 0.0;
					cmf::atmosphere::Weather w=m_cell.get_weather(t);
					double c_snow = 2.1 ; // J/(K g) -> MJ/(K m3)
					double lambda_f = 333.5; // MJ/Mg ~=~ MJ/m3 -> Latent energy of ice melt
					// MJ
					double avail_energy = m_cell.heat_flux(t) * m_cell.get_area() / 24
										+ w.Tground * snow->get_volume() * c_snow ;
					// MJ
					double total_melt_energy = lambda_f * snow->get_volume();
					if (avail_energy>=total_melt_energy) // Energy sufficient to melt all the snow 
						return snow->get_volume() * 24;
					else if (avail_energy>=0)
						return avail_energy / lambda_f * 24;
					else
						return 0.0;
				}
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
