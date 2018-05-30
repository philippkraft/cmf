#include "adsorption.h"
#include "../math/root_finding.h"
#include <cmath>
#include <stdexcept>
using namespace cmf::water;
LinearAdsorption::LinearAdsorption( real _K,real _m )
	: K(_K), m(_m)
{

}
real cmf::water::FreundlichAdsorbtion::totalsolute( real xf, real V ) const
{
	double C_eq = xf / V;
	double q = K * pow(C_eq, 1 / n);
	return q * m;
}

namespace cmf {
	namespace water {
		class FreundlichAdsorptionCalculator : public cmf::math::BrentsMethod
		{
		public:
			const FreundlichAdsorbtion * owner;
			FreundlichAdsorptionCalculator(const FreundlichAdsorbtion * fa)
				: owner(fa)
			{	}
			virtual double f(double c) const {
				return owner->totalsolute(c, 1.0);
			}
		};
	}
}


real FreundlichAdsorbtion::freesolute(real xt, real V ) const
{
	double q = xt / m;
	double C_eq = pow(q/K, n);
	return C_eq * V;
}


FreundlichAdsorbtion::FreundlichAdsorbtion( const FreundlichAdsorbtion& other ) 
	: K(other.K), n(other.n), m(other.m), epsilon(other.epsilon), maxiter(other.maxiter)
{}

FreundlichAdsorbtion::FreundlichAdsorbtion( real _K,real _n, real _m, real _epsilon/*=1e-12*/, int _maxiter/*=100*/)
	: K(_K), n(_n), m(_m), epsilon(_epsilon),maxiter(_maxiter)
{}

FreundlichAdsorbtion* FreundlichAdsorbtion::copy( real m/*=-1*/ ) const
{
	FreundlichAdsorbtion* res = new FreundlichAdsorbtion(*this);
	if (m>=0) res->m = m;
	return res;
}
real cmf::water::LangmuirAdsorption::totalsolute( real xf, real V ) const
{
	double C_eq = xf / V;
	double q = (K * C_eq)/(1 + K * C_eq);

	return q * m;
}

real LangmuirAdsorption::freesolute( real xt,real V ) const
{
	double q = xt / m;
	double C_eq = -q / (K * (q - 1));
	return C_eq * V;
}

LangmuirAdsorption::LangmuirAdsorption( real _K,real _m )
	: K(_K),m(_m)
{}

LangmuirAdsorption::LangmuirAdsorption( const LangmuirAdsorption& other ) 
	: K(other.K), m(other.m)
{}

LangmuirAdsorption* LangmuirAdsorption::copy( real m/*=-1*/ ) const
{
	LangmuirAdsorption* res = new LangmuirAdsorption(*this);
	if (m>=0) res->m = m;
	return res;
}


real cmf::water::LinearAdsorption::freesolute( real xt,real V ) const
{
	double q = xt / m;
	double C_eq = q / K;
	return C_eq * V;
}

LinearAdsorption* cmf::water::LinearAdsorption::copy( real m/*=-1*/ ) const
{
	LinearAdsorption* res = new LinearAdsorption(*this);
	if (m>=0) res->m = m;
	return res;
}

real cmf::water::LinearAdsorption::totalsolute( real xf,real V ) const
{
	double C_eq = xf / V;
	double q = K * C_eq;
	return q * m;
}

