#ifndef geometry_h__
#define geometry_h__

#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#define sqr(a) ((a)*(a))

namespace cmf {
	/// Contains geometric features like point (=location) and raster datasets
	namespace geometry	{

		const double PI=3.141592654;
		class point;
		typedef std::vector<cmf::geometry::point> Points;
		
		class shape
		{
		public:
			virtual Points Vertices() const=0;
			virtual point Center() const=0;
			/// Returns the OGC well known text representation of the shape as defined in
			/// OpenGIS Implementation Specification for Geographic information - Simple feature access - Part 1: Common architecture
			/// http://www.opengeospatial.org/standards/is
			virtual std::string AsWKT() const=0;
		};

		/// 2D-Point Class.
		///
		/// Used as location property anywhere in the text 
		/// - Calculation of distances
		/// - +,-,-=,*= Operators overloaded
		class point : public shape
		{
		private:
			static const double& max(const double& a,const double& b) {return (a>b)?a:b;}
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
				double distance3DTo(point p) const {
					return sqrt(sqr(x-p.x)+sqr(y-p.y)+sqr(z-p.z));
				}
				static double distance(point p1,point p2) {
					return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
				}
				///Returns the distance by the maximum orthogonal offset
				double distance_max(point p) const {
					return max(fabs(x-p.x),fabs(y-p.y));
				}
				///Returns the azimuth angle of the line \f$ \overline{this,p} \f$ to the Azimuth in degrees
				double azimuth(point p) const {
					double _azimuth=atan2(p.x-x,p.y-y)*180/PI;	
					return _azimuth<0 ? 360+_azimuth : _azimuth;						
				}
				///Returns the angle between the line \f$ \overline{this,p} \f$ to the x-Axis in degrees
				double angleToXAxis(point p) const {
					return atan2(p.y-y,p.x-x)*180/PI;						
				}
				std::string AsWKT() const;
				Points Vertices() const	{ 
					Points p;
					p.push_back(*this);
					return p;}
				size_t Vertex() const { return 1;}
#ifndef SWIG
				point& operator=(const point &p) {this->x=p.x;this->y=p.y;this->z=p.z; return *this;}
#endif
				point operator+(const point &p) const; 
				point operator-(const point &p) const;
				point operator*(double left) const { return point(x*left,y*left);}
				point operator/(double left) const { return point(x/left,y/left);}
				point operator+=(const point &p);
				point operator-=(const point &p);
				bool operator ==(const point &p) const;
				bool operator !=(const point &p) const {return !(*this == p);}
		};

		/// Holds the corner coordinates of a bounding box
		class BoundingBox : public shape
		{
		public:
			double
				xmin, ///< x coordinate of lower left corner
				ymin,	///< y coordinate of lower left corner
				xmax, ///< x coordinate of upper right corner
				ymax; ///< y coordinate of upper right corner
			/// returns the center of the bounding box (z=0)
			point Center() const { return point((xmin+xmax)/2,(ymin+ymax)/2); }
			/// Returns true if the point p is inside the bounding box
			bool Includes(point p) const {return (p.x>=xmin && p.y>=ymin && p.x < xmax && p.y < ymax); }
			/// Returns true, if the bounding box is empty
			bool isempty() const {return xmin==0 && ymin==0 && xmax==0 && ymax==0 ;}
			/// Makes the bounding box empty
			void makeempty() {xmin=0;xmax=0;ymin=0;ymax=0;}
			/// Extends the BoundingBox to include the given point
			void extend(point p)
			{
				if (this->isempty())
				{
					xmin=p.x;
					xmax=p.x;
					ymin=p.y;
					ymax=p.y;
				}
				else
				{
					xmin=p.x<xmin?p.x:xmin;
					xmax=p.x>xmax?p.x:xmax;
					ymin=p.y<ymin?p.y:ymin;
					ymax=p.y>ymax?p.y:ymax;
				}
			}
			/// Extends the bounding box to contain the given Boundingbox although
			void extend(const BoundingBox& bbox)
			{
				 if (isempty())
				 {
					 xmin=bbox.xmin;xmax=bbox.xmax;ymin=bbox.ymin;ymax=bbox.ymax;
				 }
				 else
				 {
					 xmin=bbox.xmin<xmin?bbox.xmin:xmin;
					 xmax=bbox.xmax>xmax?bbox.xmax:xmax;
					 ymin=bbox.ymin<ymin?bbox.ymin:ymin;
					 ymax=bbox.ymax>ymax?bbox.ymax:ymax;
				 }
			}
			std::string AsWKT() const {return "";}
			/// Creates an empty bounding box
			BoundingBox() : xmin(0),xmax(0),ymin(0),ymax(0) {}
			/// Creates a bounding box between to coordinates
			BoundingBox(double X1,double Y1,double X2, double Y2) : xmin(X1<X2?X1:X2),ymin(Y1<Y2?Y1:Y2),xmax(X1>X2?X1:X2),ymax(Y1>Y2?Y1:Y2) {}
			/// Creates a bounding box between to coordinates
			BoundingBox(point p1,point p2) : xmin(p1.x<p2.x?p1.x:p2.x),ymin(p1.y<p2.y?p1.y:p2.y),xmax(p1.x>p2.x?p1.x:p2.x),ymax(p1.y>p2.y?p1.y:p2.y) {}
			/// Copies the bounding box
			BoundingBox(const BoundingBox & copy) : xmin(copy.xmin),ymin(copy.ymin),xmax(copy.xmax),ymax(copy.ymax) {}
			BoundingBox(double* bbox) : xmin(bbox[0]),ymin(bbox[1]),xmax(bbox[2]),ymax(bbox[3]) {}
			Points Vertices() const {
				Points p;
				p.push_back(point(xmin,ymin));
				p.push_back(point(xmin,ymax));
				p.push_back(point(xmax,ymax));
				p.push_back(point(xmax,ymin));
				return p;
			}

		};
		class Line;
		class MultiPoint : public shape
		{
		private:
		protected:
			Points m_points;
			BoundingBox bbox;
		public:
			/// Adds a point to the shape
			void AddPoint(point p)
			{
				m_points.push_back(p);
				bbox.makeempty();
			}
			void AddPoint(double x,double y,double z=0)
			{
				AddPoint(point(x,y,z));
			}
			void RemovePoint(int pos)
			{
				m_points.erase(m_points.begin()+pos);
				bbox.makeempty();
			}
			int size() const {return int(m_points.size());}
			point Vertex(int i) const
			{
				int ndx=i<0 ? ndx=int(size())+i : i;
				return m_points.at(ndx);
			}
			Points Vertices() const {return m_points;}
			point Center() const
			{
				point res;
				for(std::vector<point>::const_iterator it = m_points.begin(); it != m_points.end(); ++it)
				    res+=*it / size();
				return res;
			}
			/// Returns the longest straight line between two vertices of the shape
			Line MaxStraight() const;
			std::string AsWKT() const;
			BoundingBox boundingbox()
			{
				if (bbox.isempty() && size())
				{
					bbox=BoundingBox(m_points[0],m_points[0]);
					for(std::vector<point>::const_iterator it = m_points.begin(); it != m_points.end(); ++it)
						bbox.extend(*it);
				}
				return bbox;
			}
#ifndef SWIG
			const point& operator[](int i) const
			{
				int ndx=i<0 ? ndx=int(size())+i : i;
				return m_points[ndx];
			}
			point & operator[](int i)
			{
				int ndx=i<0 ? ndx=int(size())+i : i;
				return m_points[ndx];
			}
#endif
			MultiPoint() {}
			MultiPoint(const MultiPoint & copy) : m_points(copy.m_points) {}
			MultiPoint(const Points & copy) : m_points(copy) {}
 		};
		class Line : public MultiPoint
		{
		public:
			/// returns the point in the middle of the line
			point Center() const; 
			/// returns the point in the distance d from the start point on the line
			point PointOnLine(double d) const;
			double Length() const;
			/// returns the Well-Known-Text of aline with the vertices of this vector
			std::string AsWKT() const;

		};
		class Ring : public MultiPoint
		{
			friend class Polygon;
			double SignedArea() const;
		public:
			/// returns the Well-Known-Text of a polygon with the vertices of this ring
			std::string AsWKT() const;
			/// returns the Area of a polygon defined by the points
			double Area() const;
			/// returns the centroid of a polygon defined by the points
			point Center() const; 
			/// returns true if p lies in the polygon defined by the points
			///
			/// Algorithm by W. Randolph Franklin, http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
			bool Includes(point p) const;

			/// returns true if p lies in the polygon defined by the points, and creates the bounding box if needed
			///
			/// Algorithm by W. Randolph Franklin, http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
			bool Includes(point p);
			/// Returns true if all vertices of mp are lying in the ring
			bool Ring::Includes(const shape& shp ) const;
			bool Ring::Includes(const shape& shp );
			/// Returns the shared boundary of two rings
			Line SharedBoundary(const Ring & with,double tolerance=0.1) const;

			Ring() {}
			Ring(const cmf::geometry::Ring & copy) : MultiPoint(copy) {}
			Ring(const cmf::geometry::BoundingBox & bbox) : MultiPoint(bbox.Vertices()) {}
			Ring(const cmf::geometry::Line & copy) : MultiPoint(copy) {}
		};
		typedef std::vector<cmf::geometry::Ring> Rings;
		typedef std::vector<cmf::geometry::Line> Lines;
		typedef std::vector<cmf::geometry::MultiPoint> MultiPoints;

		class PolyLine : public shape
		{
		private:
			Lines m_parts;
		public:
			double Length() const
			{
				double l=0;
				for(Lines::const_iterator it = m_parts.begin(); it != m_parts.end(); ++it)
					l+=it->Length();				    
				return l;
			}
			point Center() const
			{
				double l=0,lastl=0,c_l=Length()/2;
				for(Lines::const_iterator it = m_parts.begin(); it != m_parts.end(); ++it)
				{
					lastl=l;
					l+=it->Length();
					if (l>c_l)
						return it->PointOnLine(c_l-lastl);
				}
				return m_parts[m_parts.size()][-1];
			}
			size_t size() const
			{
				return m_parts.size();
			}
			cmf::geometry::Line& AddPart(const cmf::geometry::Line & part)
			{
				m_parts.push_back(part);
				return m_parts.back();
			}
			cmf::geometry::Line& AddPart()
			{
				m_parts.push_back(Line());
				return m_parts.back();
			}
			cmf::geometry::BoundingBox boundingbox()
			{
				BoundingBox res;
				for(Lines::iterator it = m_parts.begin(); it != m_parts.end(); ++it)
					res.extend(it->boundingbox());
				return res;
			}
			cmf::geometry::PolyLine straight_parts()	const
			{
				PolyLine res;
				for(Lines::const_iterator it = m_parts.begin(); it != m_parts.end(); ++it)
				    res.AddPart(it->MaxStraight());
				return res;
			}
			PolyLine()	{}
			PolyLine(const PolyLine & copy)
			{
				for (size_t i = 0; i < copy.size() ; ++i)
				{
					m_parts.push_back(copy[int(i)]);
				}
			}
			PolyLine(const Line & copy)
			{
				m_parts.push_back(copy);
			}
			cmf::geometry::Points Vertices()	const
			{
				Points p;
				for(Lines::const_iterator it = m_parts.begin(); it != m_parts.end(); ++it)
				{
				  Points v=it->Vertices();
					p.insert(p.end(),v.begin(),v.end());  
				}
				return p;
			}
			std::string AsWKT() const;
#ifndef SWIG
			const Line& operator[](int i) const
			{
				int ndx=i<0 ? ndx=int(size())+i : i;
				return m_parts[ndx];
			}
			Line & operator[](int i)
			{
				int ndx=i<0 ? ndx=int(size())+i : i;
				return m_parts[ndx];
			}
#endif
		};
		class Polygon : public shape
		{
		private:
			Rings m_parts;
		public:
			double Area() const
			{
				double a=0;
				for(Rings::const_iterator it = m_parts.begin(); it != m_parts.end(); ++it)
				{
						a+=it->SignedArea();
				}
				return abs(a);
			}
			cmf::geometry::point Center() const
			{
				point res;double a=0;double p_a;
				for(Rings::const_iterator it = m_parts.begin(); it != m_parts.end(); ++it)
				{
					p_a=it->Area();
					a+=p_a;
					res+=it->Center()*p_a;
				}
				return res/a;
			}
			size_t size() const
			{
				return m_parts.size();
			}
			cmf::geometry::Ring& AddPart(const cmf::geometry::Ring & part)
			{
				m_parts.push_back(part);
				return m_parts.back();
			}
			cmf::geometry::Ring& AddPart()
			{
				m_parts.push_back(Ring());
				return m_parts.back();
			}
			cmf::geometry::BoundingBox boundingbox()
			{
				BoundingBox res;
				for(Rings::iterator it = m_parts.begin(); it != m_parts.end(); ++it)
					res.extend(it->boundingbox());
				return res;
			}
			Polygon()	{}
			Polygon(const Polygon & copy)
			{
				for (size_t i = 0; i < copy.size() ; ++i)
				{
					m_parts.push_back(copy[int(i)]);
				}
			}
			Polygon(const cmf::geometry::Ring & copy)
			{
				m_parts.push_back(copy);
			}
			Polygon(const cmf::geometry::BoundingBox & copy)
			{
				m_parts.push_back(copy);
			}
			std::string AsWKT() const;
#ifndef SWIG
			const Ring& operator[](int i) const
			{
				int ndx=i<0 ? ndx=int(size())+i : i;
				return m_parts.at(ndx);
			}
			Ring & operator[](int i)
			{
				int ndx=i<0 ? ndx=int(size())+i : i;
				return m_parts.at(ndx);
			}
#endif
			bool Includes(cmf::geometry::point p) const;

			/// returns true if p lies in the polygon defined by the points, and creates the bounding box if needed
			///
			/// Algorithm by W. Randolph Franklin, http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
			bool Includes(cmf::geometry::point p);
			/// Returns true if all vertices of mp are lying in the ring
			bool Includes(const cmf::geometry::shape& shp) const;
			bool Includes(const cmf::geometry::shape& shp);
			Points Vertices()	const
			{
				Points p;
				for(Rings::const_iterator it = m_parts.begin(); it != m_parts.end(); ++it)
				{
					Points v=it->Vertices();
					p.insert(p.end(),v.begin(),v.end());  
				}
				return p;
			}

		};


		typedef std::vector<cmf::geometry::PolyLine> PolyLines;
		typedef std::vector<cmf::geometry::Polygon> Polygons;
	}
}



#endif // geometry_h__
