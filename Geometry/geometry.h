#ifndef geometry_h__
#define geometry_h__

#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <tr1/memory>
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
				point operator*(double left) const { return point(x*left,y*left);}
				point operator/(double left) const { return point(x/left,y/left);}
				point operator*(const point&p) const { return point(x*p.x,y*p.y);}
				point operator/(const point&p) const { return point(x/p.x,y/p.y);}
				point operator+=(const point &p);
				point operator-=(const point &p);
				bool operator ==(const point &p) const;
				bool operator !=(const point &p) const {return !(*this == p);}
		};
		class Locatable
		{
		public:
			virtual cmf::geometry::point get_position() const=0;
		};

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

		/// Holds the corner coordinates of a bounding box
	}
}



#endif // geometry_h__
