#include "spline.h"
#include <limits>
using namespace cmf::math;
cubicspline::cubicspline( const num_array& _x,const num_array& _y ) :
	h(num_array(_x.size())),
	s(num_array(_x.size())),
	f(_y),
	x(_x),
	F(num_array(_x.size())),
	_valid(false)
{

}
void cubicspline::generate()
{
	size_t n = size();
	num_array m(n*n);
	//*********** formation of h, s , f matrix **************//
	for (size_t i = 0; i < size() ; ++i)
	{
		F[i]=(f[i]-f[i-1])/(x[i]-x[i-1]);
		h[i-1]=x[i]-x[i-1];
	}
	for(size_t i=1;i<n-1;i++)
	{
		m[i*n+i]=2*(h[i-1]+h[i]);
		if(i!=1)
		{
			m[i*n+i-1]=h[i-1];
			m[(i-1)*n+i]=h[i-1];
		}
		m[i*n+n-1]=6*(F[i+1]-F[i]);
	}
	//***********  forward elimination **************//

	for(size_t i=1;i<n-2;i++)
	{
		double temp=(m[(i+1)*n+i]/m[i*n+i]);
		for(size_t j=1;j<=n-1;j++)
			m[(i+1)*n+j]-=temp*m[i*n+j];
	}

	//*********** back ward substitution *********//
	for(size_t i=n-2;i>0;i--)
	{
		double sum=0;
		for(size_t j=i;j<=n-2;j++)
			sum+=m[i*n+j]*s[j];
		s[i]=(m[i*n+(n-1)]-sum)/m[i*n+i];
	}
	_valid = true;

}

real cmf::math::cubicspline::operator()( real p )
{
	if (!_valid) generate();
	size_t n = size();
	for(size_t i=0;i<n-1;i++)
		if(x[i]<=p && p<=x[i+1])
		{
			real dx = p - x[i];
			real a=(s[i+1]-s[i])/(6*h[i]);
			real b=s[i]/2;
			real c=(f[i+1]-f[i])/h[i]-(2*h[i]*s[i]+s[i+1]*h[i])/6;
			real d=f[i];
			real sum=a*pow(dx,3)+b*pow(dx,2)+c*dx+d;
			return sum;
		}
		return std::numeric_limits<real>::quiet_NaN();
}