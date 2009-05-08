#ifndef Precipitation_h__
#define Precipitation_h__
#include "../water/FluxConnection.h"
#include "../math/timeseries.h"
#include "../water/Solute.h"
#include <tr1/memory>

namespace cmf { 
	namespace atmosphere {
		/// Holds the precipitation and concentration timeseries
		class PrecipitationData
		{
		public:
			virtual real q(cmf::math::Time t,double x,double y,double z) =0;
			real q(cmf::math::Time t, cmf::geometry::point p)
			{
				return q(t,p.x,p.y,p.z);
			}
			virtual PrecipitationData* copy() const=0;
			virtual cmf::water::WaterQuality conc(cmf::math::Time t,double x,double y,double z)=0;
			virtual bool RecalcFluxes(cmf::math::Time t) {return true;}
		};
		class RainfallNode : public cmf::water::FluxNode
		{
			std::tr1::shared_ptr<cmf::atmosphere::PrecipitationData> source;
		public:
			void SetSource(const cmf::atmosphere::PrecipitationData& other)
			{
				source.reset(other.copy());
			}
			virtual real q(cmf::math::Time t,double x,double y,double z)
			{
				if (source.get())
					return source->q(t,x,y,z);
				else
					return 0.0;
			}
			virtual cmf::water::WaterQuality conc(cmf::math::Time t,double x,double y,double z)
			{
				if (source.get())
					return source->conc(t,x,y,z);
				else
					return cmf::water::WaterQuality();
			}
			virtual bool RecalcFluxes(cmf::math::Time t) {return true;}
			RainfallNode(const cmf::project& _project) : cmf::water::FluxNode(_project)
			{
				 Name="Precipitation";
			}
			cmf::atmosphere::RainfallNode* copy() const
			{
				RainfallNode* res=new RainfallNode(this->project());
				res->source=source;
				return res;
			}
		};
		class SinglePrecipitationTimeseries : public PrecipitationData
		{
		public:
			/// the precipitation timeseries
			cmf::math::timeseries Data;
			/// the concentration timeseries
			cmf::water::SoluteTimeseries RainFallConcentration;
			/// Returns the concentration of the rainfall at time t
			virtual cmf::water::WaterQuality conc(cmf::math::Time t,double x, double y,double z)
			{
				return RainFallConcentration.conc(t);
			}
			virtual real q(cmf::math::Time t,double x,double y,double z)
			{
				return Data[t];
			}
			/// Conversion constructor, taking a timeseries
			SinglePrecipitationTimeseries(const cmf::math::timeseries& data) 
				: Data(data),RainFallConcentration(data.begin,data.step)			{			}
			SinglePrecipitationTimeseries(const cmf::math::timeseries& data,const cmf::water::SoluteTimeseries& concentration)
				: Data(data),RainFallConcentration(concentration) {}

			/// Conversion constructor, creates a constant precipitation
			SinglePrecipitationTimeseries(double constantPrecipitation) 
				:  Data(),RainFallConcentration()
			{
				Data.Add(constantPrecipitation);
			}
			cmf::atmosphere::SinglePrecipitationTimeseries* copy() const
			{
				return new SinglePrecipitationTimeseries(Data,RainFallConcentration);
			}
		};
	}
}
#endif // Precipitation_h__
