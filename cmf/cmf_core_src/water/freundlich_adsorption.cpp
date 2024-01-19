#include "freundlich_adsorption.h"
#include "../math/root_finding.h"
#include <cmath>
#include <stdexcept>


real cmf::water::FreundlichAdsorbtion::totalsolute( real xf, real V ) const
{
	double Ceq = xf / V;
	double q = K * pow(Ceq, 1 / n);
	return q * m + xf;
}

namespace cmf {
	namespace water {
		class FreundlichAdsorptionCalculator
			: public cmf::math::root_finding::Bisect
		{
		public:
			const cmf::water::FreundlichAdsorbtion * owner;
			FreundlichAdsorptionCalculator(const cmf::water::FreundlichAdsorbtion * fa)
				: Bisect(owner->epsilon, owner->maxiter), owner(fa)
			{	}
			virtual double f(double c) const {
				return owner->totalsolute(c, 1.0);
			}
		};
	}
}


real cmf::water::FreundlichAdsorbtion::freesolute(real xt, real V ) const
{
	//
	// the Freundlich isotherm x_tot = x_ad + x_free = (m*K*c^n) + (c*V) cannot be rearranged for c if n!=1
	// hence we have to iterate the solution using Brent's method
	cmf::water::FreundlichAdsorptionCalculator fac(this);

	double c_free = fac(0, xt, xt);
	return c_free * V;
}


cmf::water::FreundlichAdsorbtion::FreundlichAdsorbtion( const cmf::water::FreundlichAdsorbtion& other )
	: K(other.K), n(other.n), m(other.m), epsilon(other.epsilon), maxiter(other.maxiter)
{}

cmf::water::FreundlichAdsorbtion::FreundlichAdsorbtion( real _K,real _n, real _m, real _epsilon/*=1e-12*/, int _maxiter/*=100*/)
	: K(_K), n(_n), m(_m), epsilon(_epsilon),maxiter(_maxiter)
{
    // throw std::runtime_error("Freundlich Adsorption is currently broken and can not be used");
}

cmf::water::FreundlichAdsorbtion* cmf::water::FreundlichAdsorbtion::copy( real m/*=-1*/ ) const
{
	cmf::water::FreundlichAdsorbtion* res = new cmf::water::FreundlichAdsorbtion(*this);
	if (m>=0) res->m = m;
	return res;
}
