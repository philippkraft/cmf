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
	const std::string CompileDate=__DATE__;

	// Some helper functions
	/// Returns the minimum of two values
	real minimum(real a,real b);
	real maximum(real a,real b);
	real minmax(real x,real min,real max);
	real mean(real a,real b);
	real geo_mean(real a,real b);
	real piecewise_linear(real x,real xmin,real xmax,real ymin=0,real ymax=1);
	real boltzmann(real x,real tau,real x_half);
	real sign(real x);
	real square(real x);
	//real sqr(real x);

	const real Pi=3.141592654;

	/// Provides some hyperbola tools
	class hyperbola
	{
	public:
		/// Returns a hyperbola \f$ y(x)=\frac{x y_{max}}{x+K}\f$ where \f$ y<y_{max}\f$ and \f$ y(K) = \frac{y_{max}}2\f$
		static real f(real x,real ymax,real K);
		/// Returns a rounded cutted linear function
		/// \f{eqnarray*}
		/// y &=& y_{offset} + \left{y_0 + \frac{(x-x_0) y_{max}}{(x-x_0) + K} \mbox{ if } x>x_0 \\ x s \mbox{ if } x\le x_0 \right. \\
		/// K &=& y_{max} \frac r s \\
		/// y_0 &=& y_{max} - K s - y_{offset} \\
		/// x_0 &=& \frac{y_0}{s}
		/// \f}
		/// @param x the value the function is evaluated for
		/// @param ymax the upper limit for the return value
		/// @param slope the slope of the linear part
		/// @param roundness The fraction of the rounded part. 1 is the full function rounded, 0 means a pure piecewise linear function
		/// @param y_offset A y axis shift
		static real rounded_linear(real x,real ymax=1,real slope=1,real roundness=1,real y_offset=0);
		/// Returns the value at x of a hyperbola starting and point (x0,y0) with slope0, which crosses the point (x1,y1)
		static real through_point(real x,real x0,real y0,real slope0,real x1,real y1);
	};


#endif // real_h__
