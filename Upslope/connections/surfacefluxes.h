#ifndef surfacefluxes_h__
#define surfacefluxes_h__

#include "../../water/flux_connection.h"
#include "../cell.h"
#include "../../Atmosphere/Precipitation.h"
#include "AtmosphericFluxes.h"
#include "../vegetation/ET.h"
namespace cmf {
	namespace upslope {
		namespace connections {
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
					if (cell.get_rainfall()->get_connection(*cell.get_surfacewater()))
					{
						cell.get_rainfall()->remove_connection(cell.get_surfacewater());
					}
					new Rainfall(canopy,cell,false,true);
					new Rainfall(cell.get_surfacewater(),cell,true,false);
					new ET::CanopyStorageEvaporation(canopy,cell.get_evaporation(),cell);
					return new CanopyOverflow(canopy,cell.get_surfacewater(),cell);
				}
			};
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
		}
	}
}

#endif // surfacefluxes_h__
