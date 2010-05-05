

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
#include <stdlib.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <tr1/memory>
#include "../math/num_array.h"
namespace cmf {
	/// Contains geometric features like point (=location) and raster datasets
	namespace geometry	{

		const double PI=3.141592654;
		class point;


		typedef std::vector<cmf::geometry::point> Points;
		
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
				point Center() const { return *this;}
				///Empty Constructor. Creates an (0 0) location
				point(); 
				///Copy Constructor
				point(const point& p);
				///Creates a point from two doubles
				point(double x_,double y_,double z_=0.0);
				///Returns the euclidian distance to another point.
				/// \f$\sqrt{(this.x-p.x)^2+(this.y-p.y)^2}\f$
				double distanceTo(point p) const { 
					return sqrt((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));
				}
				double distance3DTo(point p) const;
				static double distance(point p1,point p2) {
					return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
				}
				///Returns the distance by the maximum orthogonal offset
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
#ifndef SWIG
				point& operator=(const point &p) {this->x=p.x;this->y=p.y;this->z=p.z; return *this;}
#endif
				point operator+(const point &p) const; 
				point operator-(const point &p) const;
				point operator*(double left) const { return point(x*left,y*left,z*left);}
				point operator/(double left) const { return point(x/left,y/left,z/left);}
				point operator*(const point&p) const { return point(x*p.x,y*p.y,z*p.z);}
				point operator/(const point&p) const { return point(x/p.x,y/p.y,z/p.z);}
				point operator+=(const point &p);
				point operator-=(const point &p);
				bool operator ==(const point &p) const;
				bool operator !=(const point &p) const {return !(*this == p);}
		};
#ifndef SWIG
		point operator*(double d,const point &p);
		point operator/(double d,const point &p);
#endif


		/// An interface for objects having a location in space
		class Locatable
		{
		public:
			/// Pure virtual function. Should return the position of the locatable
			virtual cmf::geometry::point get_position() const=0;
			/// Sets the location. If not implemented by the child class, an exception is thrown
			virtual void set_position(cmf::geometry::point p)
			{
				throw std::runtime_error("The position is readonly");
			}
			/// Returns the distance between two locatable objects
			double get_distance_to(const Locatable& cmp)
			{
				return get_position().distance3DTo(cmp.get_position());
			}
			/// Returns a vector with length=1 pointing in the direction of another Locatable
			cmf::geometry::point get_direction_to(const Locatable& cmp)
			{
				cmf::geometry::point p1=get_position(),p2=cmp.get_position();
				double d=p1.distance3DTo(p2);
				if (d<=0) 
					return cmf::geometry::point();
				else
					return (p2-p1)/d;
			}
		};


		/// A minimal implementation of Locatable
		class Location : public Locatable
		{
		private:
			typedef std::tr1::shared_ptr<const point> place_pointer;
			place_pointer place;
		public:
			cmf::geometry::point get_position() const {return *place;}
			Location(cmf::geometry::point position) 
				: place(new point(position))
			{			}
			Location(double x, double y, double z) 
				: place(new point(x,y,z)) {}
		};
		/// Holds three arrays x,y and z for fast access of point coordinates
		class point_vector
		{
		public:
#ifndef SWIG
			point operator[](int index) const
			{
				return cmf::geometry::point(X[index],Y[index],Z[index]);
			}

#endif
			cmf::math::num_array X,Y,Z;
			point_vector(int size) : X(size),Y(size),Z(size)
			{

			}
			point get(int index) const { return cmf::geometry::point(X[index],Y[index],Z[index]);}
			void set(int index,cmf::geometry::point p) {X[index]=p.x;Y[index]=p.y;Z[index]=p.z;}
			int size() const
			{
				return X.size();
			}
		};


	}
}



#endif // geometry_h__
