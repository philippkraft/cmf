#ifndef real_h__
#define real_h__
#include <cfloat>
#include <string>
#ifndef REAL32
	typedef double real;
#define REAL_MAX DBL_MAX
#else
	typedef float real;	
#define REAL_MAX FLT_MAX
#endif
#ifdef _DEBUG
	const bool Debug=1;
#else
	const bool Debug=0;
#endif
	const std::string VERSION = std::string("cmf compiled ") + std::string(__DATE__) + " - " + std::string(__TIME__);

	// Some helper functions
	/// Returns the minimum of two values
	real minimum(real a,real b);
	real maximum(real a,real b);
	real minmax(real x,real min,real max);
	real mean(real a,real b);
	real geo_mean(real a,real b);
	real harmonic_mean(real a,real b);
	real piecewise_linear(real x,real xmin,real xmax,real ymin=0,real ymax=1);
	real boltzmann(real x,real tau,real x_half);
	real sign(real x);
	real square(real x);


	const real Pi=3.141592654;


#endif // real_h__
