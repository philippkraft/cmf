

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

#ifndef geometry_h__
#define geometry_h__

#include <cmath>
#include <iostream>
#include <string>
#include <stdexcept>
#include "../math/num_array.h"
#include "../math/real.h"
namespace cmf {
	/// Contains geometric features like point (=location) and raster datasets
	namespace geometry	{
		// \f$\pi = 3.141592654\f$
		const double PI=3.141592654;
		class point;


	
		/// 2D-Point Class.
		///
		/// Used as location property anywhere in the text 
		/// - Calculation of distances
		/// - +,-,-=,*= Operators overloaded
		class point 
		{
		private:
			public:
				double x; ///< East-value of the location in m.
				double y; ///< North-value of the location in m.
				double z; ///< Height of the location in m.a.s.l.
				///Empty Constructor. Creates an (0 0) location
				point(); 
				///Copy Constructor
				point(const point& p);
				///Creates a point from two doubles
				point(double x_,double y_,double z_=0.0);
				///Returns the horizontal euclidian distance to another point p.
				/// \f$\sqrt{(this.x-p.x)^2+(this.y-p.y)^2}\f$
				double distanceTo(point p) const;
				/// Returns the horizontal euclidian distance plus the absolute of the height difference times a factor.
				/// \f$\sqrt{(this.x-p.x)^2+(this.y-p.y)^2}\ +\ w_{z}|this.z-p.z|\f$
				double z_weight_distance(point p,double z_weight) const {
					return distanceTo(p) + z_weight * abs(z - p.z);
				}
				/// Returns the euclidian distance in space to another point p.
				double distance3DTo(point p) const;
				/// Returns the horizontal euclidian distance between p1 and p2.
				static double distance(point p1,point p2) {
					return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
				}
				///Returns the distance by the maximum orthogonal offset.
				double distance_max(point p) const;
				///Returns the azimuth angle of the line \f$ \overline{this,p} \f$ to the Azimuth in degrees
				double azimuth(point p) const {
					double _azimuth=atan2(p.x-x,p.y-y)*180/PI;	
					return _azimuth<0 ? 360+_azimuth : _azimuth;						
				}
				///Returns the angle between the line \f$ \overline{this,p} \f$ to the x-Axis in degrees
				double angleToXAxis(point p) const {
					return atan2(p.y-y,p.x-x)*180/PI;						
				}
				/// Returns x+y+z
				double sum() const {
					return x+y+z;
				}
				double length() const;
#ifndef SWIG
				
				point& operator=(const point& p) {this->x=p.x;this->y=p.y;this->z=p.z; return *this;}
#endif
				point operator+(const point& p) const;
				point& operator+=(const point& left) { x+=left.x;y+=left.y;z+=left.z; return *this;} 

				point operator-(const point& p) const;
				point& operator-=(const point& left) { x-=left.x;y-=left.y;z-=left.z; return *this;} 

				point operator*(double left) const { return point(x*left,y*left,z*left);}
				point& operator*=(double left) { x*=left;y*=left;z*=left; return *this;} 
				point operator/(double left) const { return point(x/left,y/left,z/left);}
				point& operator/=(double left) { x/=left;y/=left;z/=left; return *this;} 

				point operator*(const point& p) const { return point(x*p.x,y*p.y,z*p.z);} 
				point& operator*=(const point& left) { x*=left.x;y*=left.y;z*=left.z; return *this;}

				point operator/(const point& p) const { return point(x/p.x,y/p.y,z/p.z);} 
				point& operator/=(const point& left) { x/=left.x;y/=left.y;z/=left.z; return *this;}

				bool operator ==(const point &p) const; 
				bool operator !=(const point &p) const {return !(*this == p);} 
		};
		double dot(const point &p1, const point &p2);
		double distance(const point & p1, const point & p2);
#ifndef SWIG
		point operator*(double d,const point &p);	
		point operator/(double d,const point &p);
#endif
		/// Holds three arrays x,y and z for fast access of point coordinates
		class point_vector
		{
		public:
#ifndef SWIG
			/// Return point at index
			point operator[](ptrdiff_t index) const
			{
				return cmf::geometry::point(X[index],Y[index],Z[index]);
			}
#endif
			cmf::math::num_array 
				X, ///< x coordinates
				Y, ///< y coordinates
				Z; ///< z coordinates
			
			/// Create a point vector of a specific size
			point_vector(ptrdiff_t size) : X(size),Y(size),Z(size)
			{

			}
			/// Return a point at index
			point get(ptrdiff_t index) const { return cmf::geometry::point(X[index],Y[index],Z[index]);}
			/// Change the point at index
			void set(ptrdiff_t index,cmf::geometry::point p) {X[index]=p.x;Y[index]=p.y;Z[index]=p.z;}
			/// Return the number of points in the point_vector
			size_t size() const
			{
				return X.size();
			}
		};


	}
}



#endif /* geometry_h__*/
