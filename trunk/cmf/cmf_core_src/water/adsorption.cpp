#include "adsorption.h"
#include <cmath>
using namespace cmf::water;
LinearAdsorption::LinearAdsorption( real _K,real _m )
	: K(_K), m(_m)
{

}

real FreundlichAdsorbtion::freesolute( real xt,real V ) const
{
	//
	// the Freundlich isotherm x_ad/m = K*c^n cannot be rearranged for c if n!=1
	// hence we have to iterate the solution using Newton's method

	real 
		// Calculate xf for n=1, as a first guess
		xf = V*xt/(K*m + V);

	// Do Newton iteration for xf
	for(int i=0;i<maxiter;++i) {
		// Get total concentration for actual xf
		real xt_calc = m * K * pow(xf/V,n);
		// Get derivate from xt(xf) = m n K c^n/xf + 1
		real dxf_err = m * K * n * pow(xf/V,n)/xf + 1;
		// if difference is small enough
		if (abs(xt_calc-xt)<epsilon) {
			// return actual xf
			return xf;
		} else {
			// get new xf by newton iteration step
			xf = xf - (xt_calc - xt)/dxf_err;
		}
	}
	if (strict) 
		throw std::runtime_error("Newton iteration for FreundlichAdsorption took too many iterations (default=100)");
	else
		return xf;
}


FreundlichAdsorbtion::FreundlichAdsorbtion( const FreundlichAdsorbtion& other ) 
	: K(other.K), n(other.n), m(other.m), epsilon(other.epsilon), maxiter(other.maxiter), strict(other.strict)
{}

FreundlichAdsorbtion::FreundlichAdsorbtion( real _K,real _n, real _m, real _epsilon/*=1e-12*/, int _maxiter/*=100*/, bool _strict/*=true*/ )
	: K(_K), n(_n), m(_m), epsilon(_epsilon),maxiter(_maxiter),strict(_strict)
{}

FreundlichAdsorbtion* FreundlichAdsorbtion::copy( real m/*=-1*/ ) const
{
	FreundlichAdsorbtion* res = new FreundlichAdsorbtion(*this);
	if (m>=0) res->m = m;
	return res;
}

real LangmuirAdsorption::freesolute( real xt,real V ) const
{
	/*
	This complicated stuff is derived using sympy but not yet tested:
	
	xf = Symbol('xf')
	xt = Symbol('xt')
	m = Symbol('m')
	K = Symbol('K')
	qmax = Symbol('qmax')
	V = Symbol('V')
	# Define sorbent load q
	q = (xt-xf)/m
	# Define concentration
	c = xf/V
	# Langmuir isotherme q = (K*qmax*c)/(1+K*c)
	lm_iso = Eq(q,(K*qmax*c)/(1+K*c))
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
			+ /*-*/ sqrt(   square(K)*square(m)
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
