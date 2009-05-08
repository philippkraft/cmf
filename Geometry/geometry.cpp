#include "geometry.h"
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

		std::string point::AsWKT() const
		{
			std::stringstream str;
			str << "POINT(" << x << " " << y << ")";
			return str.str();
		}

		double Ring::SignedArea() const
		{
			int n=int(size());
			const Ring& v=*this;
			if (v[0]==v[n-1]) n--;
			double A=0;
			double xi,yi,xi1,yi1;
			for (int i = 0; i < n ; i++)
			{
				xi=v[i].x;yi=v[i].y;
				xi1=v[(i+1)%n].x;yi1=v[(i+1)%n].y;
				A+=xi*yi1-xi1*yi;
			}
			return A*0.5;

		}

		double Ring::Area() const
		{
			return abs(SignedArea());
		}

		cmf::geometry::point Ring::Center() const
		{
			int n=int(this->size());
			const Ring& v=*this;
			double A=SignedArea();
			point p(0,0);
			double xi,yi,xi1,yi1;
			for (int i = 0; i < n ; i++)
			{
				xi=v[i].x;yi=v[i].y;
				xi1=v[(i+1)%n].x;yi1=v[(i+1)%n].y;
				p.x+=(xi+xi1)*(xi*yi1-xi1*yi)/(6*A);
				p.y+=(yi+yi1)*(xi*yi1-xi1*yi)/(6*A);
				p.z+=v[i].z/n;
			}
			return p;
		}

		bool Ring::Includes( point p ) const
		{
			if ((!bbox.isempty()) && (!bbox.Includes(p)))
			{
				return 0;
			}
			/*
			Copyright (c) 1970-2003, Wm. Randolph Franklin 

			Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: 

			Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimers. 
			Redistributions in binary form must reproduce the above copyright notice in the documentation and/or other materials provided with the distribution. 
			The name of W. Randolph Franklin may not be used to endorse or promote products derived from this Software without specific prior written permission. 
			THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
			*/
			int i,j,c=0;
			int npol=int(size());
			const Ring & v=*this;
			for (i = 0, j = npol-1; i < npol; j = i++) {
				if (
					(
					((v[i].y<=p.y) && (p.y<v[j].y)) 
					||
					((v[j].y<=p.y) && (p.y<v[i].y))
					) 
					&&
					(
					p.x < (v[j].x - v[i].x) * (p.y - v[i].y) / (v[j].y - v[i].y) + v[i].x
					)
					)

					c = !c;
			}
			return c!=0;

		}

		bool Ring::Includes( point p )
		{
			if (bbox.isempty()) bbox=boundingbox();
			return ((const Ring &)*this).Includes(p);
		}

		bool Ring::Includes(const shape& shp ) const
		{
			cmf::geometry::Points v=shp.Vertices();
			for(cmf::geometry::Points::const_iterator it = v.begin(); it != v.end(); ++it)
			{
				if (!Includes(*it)) return false;  
			}				
			return true;
		}
		bool Ring::Includes(const shape& shp )
		{
			cmf::geometry::Points v=shp.Vertices();
			for(cmf::geometry::Points::iterator it = v.begin(); it != v.end(); ++it)
			{
				if (!Includes(*it)) return false;  
			}				
			return true;
		}
		std::string Ring::AsWKT() const
		{
			std::stringstream str;
			str << "POLYGON((";
			for (int i = 0; i < size() ; ++i)
			{
				point p=Vertex(i);
				str << p.x << " " << p.y;
				if (i+1!=size())
					str << ",";
			}
			point p0=Vertex(0);
			point pn=Vertex(-1);
			if (!(p0==pn))
			{
				str << "," << p0.x << " " << p0.y;
			}
			str << "))";
			return str.str();

		}

		cmf::geometry::Line Ring::SharedBoundary( const Ring & with,double tolerance ) const
		{
			// Compares each vertex of this with each vertex of with, if the distance is smaller than tolerance, a shared vertex is found
			
			// Create empty line
			cmf::geometry::Line result;
			// Shortcut reference to (*this)
			const Ring& v=*this;
			// If ring is closed, don't go to last vertex
			int n=(v[0]==v[-1])?size()-1:size();
			// For each vertex in this
			for (int i = 0; i < n ; ++i)
			{
				const cmf::geometry::point& curp=v[i];
				int m=(with[0]==with[-1])?with.size()-1:with.size();
				// For each vertex in with
				for (int j = 0; j < m ; ++j)
				{
					const cmf::geometry::point& np=with[j];
					// If distance is smaller than tolerance, add center of both points to line
					if (curp.distanceTo(np)<tolerance)
						result.AddPoint((curp+np)/2);
				}
			}
			return result;
		}
		std::string Line::AsWKT() const
		{
			std::stringstream str;
			str << "LINESTRING(";
			for (int i = 0; i < size() ; ++i)
			{
				point p=Vertex(i);
				str << p.x << " " << p.y;
				if (i+1!=size())
					str << ",";
			}
			str << ")";
			return str.str();
		}

		cmf::geometry::point Line::Center() const
		{
			return PointOnLine(Length()/2);			
		}

		double Line::Length() const
		{
			double l=0;
			for (int i = 0; i < size()-1 ; ++i)
				l+=point::distance(Vertex(i),Vertex(i+1));
			return l;
		}

		cmf::geometry::point Line::PointOnLine( double d ) const
		{
			double l=0, lastl=0;
			int i=0;
			while (l<d && i<size()-1)
			{
				lastl=l;
				l+=point::distance(Vertex(i),Vertex(i+1));
			}
			return Vertex(i+1)*(d-lastl)+Vertex(i)*(l-d);
		}

		std::string MultiPoint::AsWKT() const
		{
			std::stringstream str;
			str << "MULTIPOINT(";
			for (int i = 0; i < size() ; ++i)
			{
				point p=Vertex(i);
				str << p.x << " " << p.y;
				if (i+1!=size())
					str << ",";
			}
			str << ")";
			return str.str();

		}

		Line MultiPoint::MaxStraight() const
		{
			if (size()<2) return Line();

			point p1,p2;
			double maxdist=0,curdist=0;
			const MultiPoint & mp=*this;
			for (int i = 0; i < size()-1 ; ++i)
			{
				for (int j = i+1; j < size() ; ++j)
				{
					curdist=point::distance(mp[i],mp[j]);
					if (curdist>maxdist)
					{
						maxdist=curdist;
						p1=mp[i];
						p2=mp[j];
					}				
				}
			}
			Line l;
			l.AddPoint(p1);
			l.AddPoint(p2);
			return l;
		}

		std::string PolyLine::AsWKT() const
		{
			if (!size()) return "";
			else if (size()==1) return m_parts[0].AsWKT();
			else
			{
				std::stringstream str;
				str << "MULTILINESTRING(";
				for(Lines::const_iterator it = m_parts.begin(); it != m_parts.end(); ++it)
				{
					str << "(";
					for (int i = 0; i < it->size() ; ++i)
					{
						point p=it->Vertex(i);
						str << p.x << " " << p.y;
						if (i+1!=it->size())
							str << ",";
					}
					str << ")";				    
				}
				str << ")";
				return str.str();
			}

		}

		std::string Polygon::AsWKT() const
		{
			if (!size()) return "";
			else if (size()==1) return m_parts[0].AsWKT();
			else
			{
				std::stringstream str;
				str << "POLYGON(";
				for(Rings::const_iterator it = m_parts.begin(); it != m_parts.end(); ++it)
				{
					str << "(";
						for (int i = 0; i < it->size() ; ++i)
						{
							point p=it->Vertex(i);
							str << p.x << " " << p.y;
							if (i+1!=it->size())
								str << ",";
						}
						str << ")";				    
				}
				str << ")";
				return str.str();
			}

		}

		bool Polygon::Includes( cmf::geometry::point p ) const
		{
			bool res=false;
			for(Rings::const_iterator it = m_parts.begin(); it != m_parts.end(); ++it)
				if (it->Includes(p)) res= !res;
			return res;
		}

		bool Polygon::Includes( cmf::geometry::point p )
		{
			bool res=false;
			for(Rings::iterator it = m_parts.begin(); it != m_parts.end(); ++it)
				if (it->Includes(p)) res= !res;
			return res;
		}

		bool Polygon::Includes( const cmf::geometry::shape& shp ) const
		{
			bool res=false;
			for(Rings::const_iterator it = m_parts.begin(); it != m_parts.end(); ++it)
				if (it->Includes(shp)) res= !res;
			return res;
		}

		bool Polygon::Includes( const cmf::geometry::shape& shp )
		{
			bool res=false;
			for(Rings::iterator it = m_parts.begin(); it != m_parts.end(); ++it)
				if (it->Includes(shp)) res= !res;
			return res;
		}
	}
}

