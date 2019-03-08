#include <algorithm>
#include "cross_section_reach.h"
#include "../math/root_finding.h"
#include <iostream>
class DepthCalculator : public cmf::math::root_finding::BrentsMethod {
private:
	const cmf::river::CrossSectionReach * cr;
public:
	DepthCalculator(const cmf::river::CrossSectionReach * owner)
		: cr(owner) {}
	virtual double f(double d) const {
		return cr->get_flux_crossection(d);
	}

};

class TrapezoidIterator {
	/*
		Iterates over the x and depth sequence, returning(x, y1, y2) tuple
	where:
		-x is the horizontal wetted width of a section
		- y1 is the left depth of the section below h
		- y2 is the right depth of the section below h

		x = y1 = y2 = 0 for dry sections
	*/

private:
	const cmf::river::CrossSectionReach * cr;
public:
	ptrdiff_t position, length;
	double x, y1, y2;
	double h;
	double dmax;

	TrapezoidIterator(const cmf::river::CrossSectionReach * owner, double _h)
		: cr(owner), position(0), 
		  x(0), y1(0), y2(0), 
		  dmax(0), h(_h)
	{
		dmax = cr->depth.max();
		length = std::min(cr->x.size(), cr->depth.size());
	}
	bool valid() const {
		return position < length;
	}
	TrapezoidIterator& next() {
		if (position >= length) {
			return *this;
		}
		position++;
		double 
			xl=cr->x[position-1], 
			xr=cr->x[position], 
			yl= dmax - cr->depth[position-1], 
			yr= dmax - cr->depth[position];

		if (yl >= h && h > yr) {
			// only right side of segment submerged
			x = (xr - xl) * (h - yr) / (yl - yr);
			y1 = 0;
			y2 = h - yr;
		}
		else if (yr >= h && h > yl) {
			// only left side of segment submerged
			x = (xr - xl) * (h - yl) / (yr - yl);
			y2 = 0;
			y1 = h - yl;
		}
		else if (h > yr && h > yl) {
			// complete segment submerged
			x = xr - xl;
			y1 = h - yl;
			y2 = h - yr;
		}
		else if (yl >= h && yr >= h) {
			// segment not submerged
			x = y1 = y2 = 0.0;
		}
		return *this;
	}

};

double cmf::river::CrossSectionReach::get_channel_width(double depth) const
{
	double width = 0.0;
	for (TrapezoidIterator it(this, depth); it.valid(); it.next()) {
		width += it.x;
	}
	return width;
}

double cmf::river::CrossSectionReach::get_wetted_perimeter(double depth) const
{
	double P = 0.0;
	double dy = 0.0;
	for (TrapezoidIterator it(this, depth); it.valid(); it.next()) {
		dy = it.y2 - it.y1;
		P += sqrt(it.x * it.x + dy * dy);
	}
	return P;
}

double cmf::river::CrossSectionReach::get_depth(double area) const
{
	if (area <= 0.0) {
		return 0.0;
	}
	else {
		DepthCalculator dc(this);
		return dc(0, std::max(area * 10, 1.0), area);
	}
}

double cmf::river::CrossSectionReach::get_flux_crossection(double depth) const
{
	double A = 0.0;
	for (TrapezoidIterator it(this, depth); it.valid(); it.next()) {
		A += it.x * it.y1 + // Area of rectangle part of trapezoid
			 0.5 * it.x * (it.y2 - it.y1); // Area of triangle part of trapezoid
	}
	return A;
}

cmf::river::CrossSectionReach::CrossSectionReach(double _l, cmf::math::num_array _x, cmf::math::num_array _depth)
	: m_l(_l), m_nManning(0.035), x(_x), depth(_depth)
{
}

cmf::river::CrossSectionReach::CrossSectionReach(const cmf::river::CrossSectionReach & other)
	: m_l(other.get_length()), m_nManning(other.get_nManning()), x(other.x), depth(other.depth)
{

}

cmf::river::CrossSectionReach * cmf::river::CrossSectionReach::copy() const
{
	return new CrossSectionReach(*this);
}

void cmf::river::CrossSectionReach::check_iterator(double h) const
{
	for (TrapezoidIterator it(this, h); it.valid(); it.next()) {
		std::cout << "X:" << it.x << " Y1:" << it.y1 << " Y2:" << it.y2 << std::endl;
	}

}
