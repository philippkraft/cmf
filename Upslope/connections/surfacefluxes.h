#ifndef surfacefluxes_h__
#define surfacefluxes_h__

#include "../../water/FluxConnection.h"
#include "../cell.h"
#include "../../Atmosphere/Precipitation.h"
#include "AtmosphericFluxes.h"
#include "../vegetation/ET.h"
namespace cmf {
	namespace upslope {
		namespace connections {
			/// Calculates the overflow of a canopy storage
			class CanopyOverflow : public cmf::water::FluxConnection {
			protected:
				cmf::upslope::Cell & m_cell;
				cmf::water::WaterStorage * m_Canopy;
				virtual real calc_q(cmf::math::Time t) ;
				virtual void NewNodes()
				{
					m_Canopy=cmf::water::AsWaterStorage(m_left);
				}
			public:
				CanopyOverflow(cmf::water::WaterStorage & Canopy,cmf::water::FluxNode & target,cmf::upslope::Cell & cell)
					: cmf::water::FluxConnection(Canopy,target,"Canopy overflow"),m_cell(cell),m_Canopy(0) {
						NewNodes();
				}
				static CanopyOverflow* use_for_cell(cmf::upslope::Cell & cell)
				{
					// If the canopy of this cell is not a storage, create a canopy storage
					cmf::water::WaterStorage& canopy=cell.add_storage("Canopy",'C');	
					if (cell.get_rainfall().Connection(cell.get_surfacewater()))
					{
						cell.get_rainfall().remove_connection(cell.get_surfacewater());
					}
					new Rainfall(canopy,cell,false,true);
					new Rainfall(cell.get_surfacewater(),cell,true,false);
					new ET::CanopyStorageEvaporation(canopy,cell.get_evaporation(),cell);
					return new CanopyOverflow(canopy,cell.get_surfacewater(),cell);
				}
			};
			class SimpleTindexSnowMelt : public cmf::water::FluxConnection {
			protected:
				cmf::water::WaterStorage * m_Snow;
				cmf::water::FluxNode * m_Surfacewater;
				cmf::upslope::Cell & m_cell;
				virtual real calc_q(cmf::math::Time t)
				{
					if (m_Snow->is_empty())
						return 0.0;
					else 
					{
						real T=m_cell.get_weather(t).T;
						real ThresholdTemp=cmf::atmosphere::Weather::snow_threshold;
						if (T>ThresholdTemp)
						{
							real f=piecewise_linear(m_Snow->get_state()/m_cell.get_area(),0,0.001);
							return f*SnowMeltRate*(T-ThresholdTemp)*m_cell.get_area()*0.001;
						}
						else
							return 0.0;
					}
				}
				virtual void NewNodes()
				{
					m_Snow=cmf::water::AsWaterStorage(m_left);
					m_Surfacewater=m_right;
				}
			public:
				real SnowMeltRate;
				SimpleTindexSnowMelt(cmf::water::WaterStorage& snow,cmf::water::FluxNode& surface_water,cmf::upslope::Cell& cell)
					: FluxConnection(snow,surface_water,"Simple T-Index snow melt"),m_cell(cell),SnowMeltRate(7.0)
				{
					NewNodes();
				}
				static void use_for_cell(cmf::upslope::Cell& cell)
				{
					cmf::water::WaterStorage& snow=cell.add_storage("Snow",'S');
					new cmf::upslope::connections::Snowfall(snow,cell);
					SimpleTindexSnowMelt * res=new SimpleTindexSnowMelt(snow,cell.get_surfacewater(),cell);
				}
			};
			/// Calculates the flux of melted water from the snow pack pores to the free surface water
			///
			/// \f[q_{SnW,SW} = K_{Sn}\left(1+\exp\left(-\frac 1{0.05}\frac{V_{SnW}}{c_{cap}V_{Sn}}\right)\right)^{-1} \f]
			class SnowWaterOverflow : public cmf::water::FluxConnection {
			protected:
				cmf::water::WaterStorage * m_SnowWater;
				cmf::water::FluxNode * m_SurfaceWater;
				cmf::water::WaterStorage* m_Snow;
				cmf::upslope::Cell & m_cell;
				virtual real calc_q(cmf::math::Time t);
				virtual void NewNodes()
				{
					 m_SnowWater=AsWaterStorage(m_left);
					 m_SurfaceWater=m_right;
				}
			public:
				/// Relative water holding capacity of snow \f$ c_{cap}\f$, default=0.1 mm H2O/mm Snow
				real RelCapacity;
				/// Water conductivity of snow \f$K_{Sn}\f$, default=1cm/day
				real SnowConductivity;
				SnowWaterOverflow(cmf::water::WaterStorage& snow_water,cmf::water::FluxNode& surface_water,cmf::water::WaterStorage& snow,cmf::upslope::Cell& cell,real relative_capacity=0.1,real snowConductivity=864.)
					: cmf::water::FluxConnection(snow_water,surface_water,"Snow water overflow"),
					m_Snow(&snow),	m_cell(cell),
					RelCapacity(relative_capacity),SnowConductivity(snowConductivity) {
						NewNodes();
				}

			};
			/// A relative simple degree day procedure for snow melt
			///
			/// \f[q_{Sn,SnW} = r_{Melt} \left(T-T_{Snow}\right) \f]
			class HBVSnowMelt : public cmf::water::FluxConnection {
			protected:
				cmf::water::WaterStorage * m_Snow;
				cmf::water::WaterStorage * m_SnowWater;
				cmf::upslope::Cell& m_cell;
				virtual real calc_q(cmf::math::Time t);
				virtual void NewNodes()
				{
					m_Snow=cmf::water::AsWaterStorage(m_left);
					m_SnowWater=cmf::water::AsWaterStorage(m_right);
				}
			public:
				/// Melting rate of snow in \f$\frac{mm}{^\circ C\ day}\f$, default = \f$7\frac{mm}{^\circ C\ day}\f$
				real SnowMeltRate;
				/// Refreeze rate of snow as fraction of melting rate, default=0.05
				real RefreezeRate;
				HBVSnowMelt(cmf::water::WaterStorage& snow,cmf::water::WaterStorage& snow_water,cmf::upslope::Cell & cell,real snowmeltrate=7,real refreezeRate=0.05)
					: cmf::water::FluxConnection(snow,snow_water,"HBV snow melt"),m_cell(cell),SnowMeltRate(snowmeltrate),RefreezeRate(refreezeRate) {
						NewNodes();
				}
				static HBVSnowMelt* use_for_cell(cmf::upslope::Cell & cell)
				{
					cmf::water::WaterStorage& snow=cell.add_storage("Snow",'S');
					cmf::water::WaterStorage& snowwater=cell.add_storage("Snow water");
					new cmf::upslope::connections::Snowfall(snow,cell);
					HBVSnowMelt * res=new HBVSnowMelt(snow,snowwater,cell);
					new SnowWaterOverflow(snowwater,cell.get_surfacewater(),snow,cell);
					return res;
				}
			};
		}
	}
}

#endif // surfacefluxes_h__
