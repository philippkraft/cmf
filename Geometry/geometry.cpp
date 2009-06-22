#include "geometry.h"
#include "../math/real.h"

#include <string>
#include <sstream>
namespace cmf {
	namespace geometry	{
		point::point():x(0.0),y(0.0),z(0.0) {}
		point::point(const point &p):x(p.x),y(p.y),z(p.z) {}
		point::point(double x_,double y_,double z_):x(x_),y(y_),z(z_) {}
		
		point point::operator+(const point &p) const 
		{
			point res(x+p.x,y+p.y,z-p.z);
			return res;
		}
		point point::operator-(const point &p) const 
		{
			point res(x-p.x,y-p.y,z-p.z);
			return res;
		}
		point point::operator+=(const point &p) 
		{
			x+=p.x;
			y+=p.y;
			z+=p.z;
			return *this;
		}
		point point::operator-=(const point &p) 
		{
			x-=p.x;
			y-=p.y;
			z-=p.z;
			return *this;
		}
		bool point::operator ==(const point &p) const 
		{
			return this->x==p.x && this->y == p.y && this->z==p.z;
		}

		double point::distance3DTo( point p ) const
		{
			return sqrt(square(x-p.x)+square(y-p.y)+square(z-p.z));
		}

		double point::distance_max( point p ) const
		{
			return maximum(fabs(x-p.x),fabs(y-p.y));
		}

		cmf::geometry::point operator*( double d,const point &p )
		{
			return p*d;
		}

		cmf::geometry::point operator/( double d,const point &p )
		{
			return cmf::geometry::point(d,d,d) / p;
		}
	}
}

