

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   cmf is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
//   
#include "geometry.h"
#include "../math/real.h"

#include <string>
#include <sstream>
#define sqr(x) ((x)*(x))

#include <limits>
inline bool isfinite(real v)
{
	typedef  std::numeric_limits<real> limit;
	return 
		v != limit::infinity() &&
		v != -limit::infinity() &&
		v == v;
}
namespace cmf {
	namespace geometry	{
		point::point():x(0.0),y(0.0),z(0.0) {}
		point::point(const point &p):x(p.x),y(p.y),z(p.z) {}
		point::point(double x_,double y_,double z_):x(x_),y(y_),z(z_) {}
		
		point point::operator+(const point &p) const 
		{
			point res(x+p.x,y+p.y,z+p.z);
			return res;
		}
		point point::operator-(const point &p) const 
		{
			point res(x-p.x,y-p.y,z-p.z);
			return res;
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

		double point::length() const
		{
			return sqrt(sqr(x)+sqr(y)+sqr(z));
		}

		double point::distanceTo( point p ) const
		{
			return sqrt(sqr(x-p.x)+sqr(y-p.y));
		}
		cmf::geometry::point operator*( double d,const point &p )
		{
			return p*d;
		}

		cmf::geometry::point operator/( double d,const point &p )
		{
			return cmf::geometry::point(d,d,d) / p;
		}

		double dot( const point &p1, const point &p2 )
		{
			return p1.x*p2.x + p1.y*p2.y + p1.z * p2.z;
		}

		double distance( const point & p1, const point & p2 )
		{
			return p1.distance3DTo(p2);
		}
	}
}

