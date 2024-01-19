#include "adsorption.h"
#include <cmath>
#include <stdexcept>
using namespace cmf::water;
LinearAdsorption::LinearAdsorption( real _K,real _m )
	: K(_K), m(_m)
{

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
		print(ccode(lm_xf))
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

