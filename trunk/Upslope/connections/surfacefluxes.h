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
				CanopyOverflow(cmf::water::storage_pointer Canopy,cmf::water::flux_node::ptr target,cmf::upslope::Cell & cell)
					: cmf::water::flux_connection(Canopy,target,"Canopy overflow"),m_cell(cell) {
						NewNodes();
				}
				static CanopyOverflow* use_for_cell(cmf::upslope::Cell & cell)
				{
					// If the canopy of this cell is not a storage, create a canopy storage
					cmf::water::storage_pointer canopy=cell.add_storage("Canopy",'C');	
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
				SimpleTindexSnowMelt(cmf::water::storage_pointer snow,cmf::water::flux_node::ptr surface_water,cmf::upslope::Cell& cell)
					: flux_connection(snow,surface_water,"Simple T-Index snow melt"),m_cell(cell),SnowMeltRate(7.0)
				{
					NewNodes();
				}
				static void use_for_cell(cmf::upslope::Cell& cell)
				{
					cmf::water::storage_pointer snow=cell.add_storage("Snow",'S');
					new cmf::upslope::connections::Snowfall(snow,cell);
					
					new SimpleTindexSnowMelt(snow,cell.get_surfacewater(),cell);
				}
			};
			/// Calculates the flux of melted water from the snow pack pores to the free surface water
			///
			/// \f[q_{SnW,SW} = K_{Sn}\left(1+\exp\left(-\frac 1{0.05}\frac{V_{SnW}}{c_{cap}V_{Sn}}\right)\right)^{-1} \f]
			class SnowWaterOverflow : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::water::WaterStorage > m_SnowWater;
				std::tr1::weak_ptr<cmf::water::flux_node> m_SurfaceWater;
				std::tr1::weak_ptr<cmf::water::WaterStorage> m_Snow;
				cmf::upslope::Cell & m_cell;
				virtual real calc_q(cmf::math::Time t);
				virtual void NewNodes()
				{
					m_SnowWater=cmf::water::WaterStorage::cast(left_node());
					 m_SurfaceWater=right_node();
				}
			public:
				/// Relative water holding capacity of snow \f$ c_{cap}\f$, default=0.1 mm H2O/mm Snow
				real RelCapacity;
				/// Water conductivity of snow \f$K_{Sn}\f$, default=1cm/day
				real SnowConductivity;
				SnowWaterOverflow(cmf::water::storage_pointer snow_water,cmf::water::flux_node::ptr surface_water,cmf::water::storage_pointer snow,cmf::upslope::Cell& cell,real relative_capacity=0.1,real snowConductivity=864.)
					: cmf::water::flux_connection(snow_water,surface_water,"Snow water overflow"),
					m_Snow(snow),	m_cell(cell),
					RelCapacity(relative_capacity),SnowConductivity(snowConductivity) {
						NewNodes();
				}

			};
			/// A relative simple degree day procedure for snow melt
			///
			/// \f[q_{Sn,SnW} = r_{Melt} \left(T-T_{Snow}\right) \f]
			class HBVSnowMelt : public cmf::water::flux_connection {
			protected:
				std::tr1::weak_ptr<cmf::water::WaterStorage > m_Snow, m_SnowWater;
				cmf::upslope::Cell& m_cell;
				virtual real calc_q(cmf::math::Time t);
				virtual void NewNodes()
				{
					m_Snow=cmf::water::WaterStorage::cast(left_node());
					m_SnowWater=cmf::water::WaterStorage::cast(right_node());
				}
			public:
				/// Melting rate of snow in \f$\frac{mm}{^\circ C\ day}\f$, default = \f$7\frac{mm}{^\circ C\ day}\f$
				real SnowMeltRate;
				/// Refreeze rate of snow as fraction of melting rate, default=0.05
				real RefreezeRate;
				HBVSnowMelt(cmf::water::storage_pointer snow,cmf::water::storage_pointer snow_water,cmf::upslope::Cell & cell,real snowmeltrate=7,real refreezeRate=0.05)
					: cmf::water::flux_connection(snow,snow_water,"HBV snow melt"),m_cell(cell),SnowMeltRate(snowmeltrate),RefreezeRate(refreezeRate) {
						NewNodes();
				}
				static void use_for_cell(cmf::upslope::Cell & cell)
				{
					cmf::water::storage_pointer snow=cell.add_storage("Snow",'S');
					cmf::water::storage_pointer snowwater=cell.add_storage("Snow water");
					new cmf::upslope::connections::Snowfall(snow,cell);
					new HBVSnowMelt(snow,snowwater,cell);
					new SnowWaterOverflow(snowwater,cell.get_surfacewater(),snow,cell);
				}
			};
		}
	}
}

#endif // surfacefluxes_h__
