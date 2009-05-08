#ifndef AtmosphericFluxes_h__
#define AtmosphericFluxes_h__

#include "../../water/FluxConnection.h"
#include "../cell.h"
#include "../../project.h"
#include "../../Atmosphere/Precipitation.h"
namespace cmf {
	namespace upslope {

		namespace connections {
			class Rainfall : public cmf::water::FluxConnection
			{
			protected:
				cmf::upslope::Cell & m_cell;
				virtual real calc_q(cmf::math::Time t)
				{
					bool snow=m_cell.HasSnowStorage() && m_cell.Weather(t).T<m_cell.project().Meteorology().SnowThresholdTemperature;
					if (snow)
						return 0.0;
					else
					{
						cmf::upslope::vegetation::Vegetation veg=m_cell.GetVegetation();
						real f=0; // Fraction of rainfall to use
						if (Throughfall) f+=1-veg.CanopyClosure;
						if (InterceptedRainfall) f+=veg.CanopyClosure;
						return f*m_cell.Rain(t)*m_cell.Area()*0.001; // Convert mm/day to m3/day
					}
				}
				void NewNodes() {}
			public:
				bool Throughfall;
				bool InterceptedRainfall;
				Rainfall(cmf::water::FluxNode& target,cmf::upslope::Cell & cell,bool getthroughfall=true,bool getintercepted=true) 
					: cmf::water::FluxConnection(cell.project().Rainfall(),target,
					getthroughfall && getintercepted ? "Rainfall" : getthroughfall ? "Throughfall" : getintercepted ? "Intercepted rain" : "No Rain"),
					m_cell(cell),Throughfall(getthroughfall),InterceptedRainfall(getintercepted) {
						NewNodes();
				}
			};
			class Snowfall : public cmf::water::FluxConnection
			{
			protected:
				cmf::upslope::Cell & m_cell;
				virtual real calc_q(cmf::math::Time t)
				{
					bool snow=m_cell.HasSnowStorage() && m_cell.Weather(t).T<m_cell.project().Meteorology().SnowThresholdTemperature;
					if (snow)
						return m_cell.Rain(t)*m_cell.Area()*0.001; // Convert mm/day to m3/day
					else
						return 0.0;
				}
				void NewNodes() {}

			public:
				Snowfall(cmf::water::FluxNode& target,cmf::upslope::Cell & cell) 
					: cmf::water::FluxConnection(cell.project().Rainfall(),target,"Snowfall"),m_cell(cell) {
						NewNodes();
				}


			};
		}
	}
}
#endif // AtmosphericFluxes_h__
