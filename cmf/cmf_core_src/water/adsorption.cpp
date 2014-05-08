#include "adsorption.h"
#include <cmath>
#include <stdexcept>
using namespace cmf::water;
LinearAdsorption::LinearAdsorption( real _K,real _m )
	: K(_K), m(_m)
{

}
real cmf::water::FreundlichAdsorbtion::totalsolute( real xf, real V ) const
{
	return K*pow(xf/V,n);
}

real FreundlichAdsorbtion::freesolute( real xt,real V ) const
{
	//
	// the Freundlich isotherm x_ad/m = K*c^n cannot be rearranged for c if n!=1
	// hence we have to iterate the solution using Regula falsi
	
	real
		a = 0.0, // left side
		fa = totalsolute(a,V) - xt,
		b = 1.0, // right side
		fb = totalsolute(b,V) - xt,
		c = 0.,
		fc = 0.0;
	int side=0;
	// Do Regula falsi (Illinois variant) for xf
	for(int i=0;i<maxiter;++i) {
		c = (fa*b - fb*a) / (fa - fb);
		fc = totalsolute(c,V) - xt;
		if (fabs(fc)<epsilon) return c;
		if (fc * fb > 0.0) {
			b = c; fb = fc;
			if (side==-1) fa/=2;
			side = -1;
		} else if (fc * fa > 0.0) {
			a = c; fa = fc;
			if (side == +1) fb/=2;
			side = +1;
		} else {
			return c;
		}
	}
	throw std::runtime_error("Too many iterations for xf in FreundlichAdsorption");
	return 0.0;
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
real cmf::water::LangmuirAdsorption::totalsolute( real xf,real V ) const
{
	return K*xf/V/(1+K*xf/V) * m + xf;
}

real LangmuirAdsorption::freesolute( real xt,real V ) const
{
	/*
	This complicated stuff is derived using sympy but not yet tested:
	
	xf = Symbol('xf')
	xt = Symbol('xt')
	m = Symbol('m')
	K = Symbol('K')
	#qmax = Symbol('qmax')
	V = Symbol('V')
	# Define sorbent load q
	q = (xt-xf)/m
	# Define concentration
	c = xf/V
	# Langmuir isotherme q = (K*qmax*c)/(1+K*c)
	lm_iso = Eq(q,(K*c)/(1+K*c))
	# solve for xf
	lm_xf = solve(lm_iso,xf)
	# Get the ccode
	for solution in lm_xf:
		print ccode(lm_xf)
	*/
	return  
		(
			- K*m
			+ K*xt 
			- V 
			+ sqrt(   square(K)*square(m)
					- 2*square(K)*m*xt 
					+ pow(K, 2)*pow(xt, 2) 
					+ 2*K*V*m 
					+ 2*K*V*xt 
					+ square(V)
					)
		)/(2.*K);
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
	return V*xt/(K*m + V);
}

LinearAdsorption* cmf::water::LinearAdsorption::copy( real m/*=-1*/ ) const
{
	LinearAdsorption* res = new LinearAdsorption(*this);
	if (m>=0) res->m = m;
	return res;
}

real cmf::water::LinearAdsorption::totalsolute( real xf,real V ) const
{
	return K*xf/V * m + xf;
}

