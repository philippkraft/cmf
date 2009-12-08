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
real harmonic_mean(real a,real b)
{
	return 2/(1/a + 1/b);
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
