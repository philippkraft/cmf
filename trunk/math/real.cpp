#include "real.h"
#include <cmath>
real minimum(real a,real b)
{
	return a<b? a : b;
}
real maximum(real a,real b)
{
	return a>b? a : b;
}
real mean(real a,real b)
{
	return 0.5*(a+b);
}
real geo_mean(real a,real b)
{
	return sqrt(fabs(a*b));
}
real sign(real x)
{
	return x<0 ? -1.0 : x>0? 1.0 : 0;
}
real square(real x)
{
	return x*x;
}
real minmax(real x,real min,real max)
{
	return x<min ? min : x>max ? max : x;
}
real piecewise_linear(real x,real xmin,real xmax,real ymin/*=0*/,real ymax/*=1*/)
{
	if (x<xmin)
		return ymin;
	else if (x>xmax)
		return ymax;
	else
		return (x-xmin)/(xmax-xmin)*(ymax-ymin)+ymin;
}
real boltzmann(real x,real x_half,real tau)
{
	return 1.0/(1.0+exp(-(x-x_half)/tau));
}

real hyperbola::f( real x,real ymax,real K )
{
	return x*ymax/(x+K);
}

real hyperbola::rounded_linear( real x,real ymax/*=1*/,real slope/*=1*/,real roundness/*=1*/,real y_offset/*=0*/ )
{
	real
		K=ymax*roundness/slope,
		y0=ymax-K*slope-y_offset,
		x0=y0/slope;
	if (x<=x0)
		return x*slope+y_offset;
	else
		return y_offset + y0 + f(x-x0,K*slope,K);
}

real hyperbola::through_point( real x,real x0,real y0,real slope0,real x1,real y1 )
{
	real ymax=((y0-slope0*x1+slope0*x0)*y1 + (slope0*x0-slope0*x1)*y0)/(y1-slope0*x1+slope0*x0);
	return f(x,ymax,ymax/slope0);
}

