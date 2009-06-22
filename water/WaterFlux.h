#ifndef WaterFlux_h__
#define WaterFlux_h__

#include "Solute.h"
#include "../math/real.h"


namespace cmf {
	namespace water {

		class WaterStorage;
		/// Water fluxes consist of the flux in \f$\frac{m^3}{day}\f$ and a map of concentrations (ConcentrationMap)
		class WaterFlux
		{
		public:
			WaterFlux& Update(real waterflux,const cmf::water::WaterStorage& origin,bool uptake=false);
			/// The water quality
			cmf::water::WaterQuality c;
			/// The amount of moving water in \f$\frac{m^3}{day}\f$
			real q;
			/// Standard constructor
			WaterFlux(): q(0),c() {}
			/// Copy constructor
			WaterFlux(const WaterFlux& forcopy)
				: q(forcopy.q),c(forcopy.c) {}
			/// Creates a clear water flux
			WaterFlux(real flux)
				: q(flux),c() {}
			WaterFlux(real flux,const cmf::water::WaterStorage& origin,bool uptake=false);
			WaterFlux(real flux,WaterQuality wq) : q(flux),c(wq) {}
#ifndef SWIG
			/// Updates the flux with the concentration of the flux origin
			WaterFlux& operator()(real waterflux,const cmf::water::WaterStorage& origin,bool uptake=false)
			{
				return this->Update(waterflux,origin,uptake);
			};
			/// Updates the flux from an existing WaterQuality
			WaterFlux& operator()(real waterflux,const cmf::water::WaterQuality& concentration)
			{
				this->q=waterflux;
				this->c=concentration;
				return *this;
			}
			WaterFlux& operator=(const WaterFlux& right)
			{
				this->q=right.q;
				this->c=right.c;
				return *this;
			}
#endif
			/// Adds water from the origin to the flux (mixing)
			/// Both fluxes may be negative, but mixed signs will throw an error
			/// \f{eqnarray*} 
			/// q&=& \mbox{Flux in }\frac{m^3}{day} \\
			/// c&=& \mbox{Vector of concentrations in } \frac{mol}{m^3} \\
			/// c_{this}&=&\frac{c_{this}q_{this}+c_{other}q_{other}}{q_{this}+q_{other}}	\\
			/// q_{this}&=&q_{this}+q_{other}	\\
			/// \f}
			WaterFlux& Add(real flux,const cmf::water::WaterStorage& origin);
			/// Adds water of a certain water quality to the flux (mixing)
			/// Both fluxes may be negative, but mixed signs will throw an error
			/// \f{eqnarray*} 
			/// q&=& \mbox{Flux in }\frac{m^3}{day} \\
			/// c&=& \mbox{Vector of concentrations in } \frac{mol}{m^3} \\
			/// c_{this}&=&\frac{c_{this}q_{this}+c_{other}q_{other}}{q_{this}+q_{other}}	\\
			/// q_{this}&=&q_{this}+q_{other}	\\
			/// \f}
			WaterFlux& Add(real flux,const cmf::water::WaterQuality& concentrations);
			/// Changes the sign of the flux to positive
			WaterFlux& Abs()
			{
				q=fabs(q);
				return *this;
			}
			/// Returns the %water flux of the given solute in \f$\frac{mol}{day}\f$, depending on the unit of the concentration (see cmf::water )
			/// \f{eqnarray*}
			/// Q_{Solute} &=& q_{H_2O}\left[\frac{m^3}{day}\right] c_{Solute} \left[\frac{mol}{m^3}\right]
			/// \f}
			real matterflux(const cmf::water::Solute& solute)
			{
				return q*c[solute];
			}
			/// Sets all concentrations and the flux to 0
			void clear()
			{
				c.clear();
				q=0.0;
			}
			/// Adds a flux to this, and mixes the concentrations.
			/// Both fluxes may be negative, but mixed signs will throw an error
			/// \f{eqnarray*} 
			/// q&=& \mbox{Flux in }\frac{m^3}{day} \\
			/// c&=& \mbox{Vector of concentrations in } \frac{mol}{m^3} \\
			/// c_{this}&=&\frac{c_{this}q_{this}+c_{other}q_{other}}{q_{this}+q_{other}}	\\
			/// q_{this}&=&q_{this}+q_{other}	\\
			/// \f}
			WaterFlux& operator+=(const WaterFlux& add);
			/// Multiplies the flux with a factor, but is not changing the concentration
			WaterFlux& operator*=(real factor)
			{
				this->q*=factor;
				return *this;
			}
			/// Divides the flux by a factor, but is not changing the concentration
			WaterFlux& operator/=(real factor)
			{
				this->q/=factor;
				return *this;
			}
			WaterFlux operator -()
			{
				WaterFlux res=*this;
				res.q*=-1;
				return res;
			}
		};
	}
}
#ifdef SWIG
%extend cmf::water::WaterFlux
{
	cmf::water::WaterFlux& __call__(double waterflux,const cmf::water::WaterStorage& origin)
	{
		return (*$self)(waterflux,origin);
	}
}
#endif
#endif // WaterFlux_h__

