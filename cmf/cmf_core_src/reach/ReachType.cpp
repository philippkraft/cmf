

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
#include "ReachType.h"
#include <cmath>
#include "../math/real.h"
#include <stdexcept>
#define sqr(a) ((a)*(a))

using namespace cmf::river;
double IChannel::qManning( double A,double slope ) const
{
	double 
		nManning = get_nManning(),
		d=get_depth(A),
		P=get_wetted_perimeter(d); 
	if (A<=0.0 || P<=0.0) {
		return 0.0;
	} else {
		return A*pow(A/P,2./3.)*sqrt(slope)/nManning;
	}
}


/************************************************************************/
/* SWAT reach type                                                                     */
/************************************************************************/

/// \f[ d = \sqrt{\frac{A}{\Delta_{bank}} + \frac{{w_{bottom}}^2}{4 {\Delta_{bank}}^2}} - \frac{w_{bottom}}{2 \Delta_{bank}} \f]
/// If \f$d>d_{IChannel}\f$
/// \f[d=d_{IChannel}+\sqrt{\frac{A-A(d_{IChannel})}{\Delta_{flood\ plain}} + \frac{{w(d_{IChannel})}^2}{4 {\Delta_{flood\ plain}}^2}} - \frac{w(d_{IChannel})}{2 \Delta_{flood\ plain}} \f]
double SWATReachType::get_depth( double get_flux_crossection ) const
{
	//Calculate depth from the void volume of the IChannel, using the SWAT river geometry (trapezoid)
	double d = 
		sqrt(
			get_flux_crossection/BankSlope
			+ (BottomWidth*BottomWidth / (4 * BankSlope*BankSlope))
		)
		- BottomWidth/(2*BankSlope);
	//If the IChannel is full
	if (d>ChannelDepth)
	{
		//Calculate the get_flux_crossection of the full IChannel
		double A_ch=(BottomWidth+BankSlope*ChannelDepth)*ChannelDepth;
		//Calculate the width of the full IChannel
		double w_full=get_channel_width(ChannelDepth);
		//Calculate the sum of IChannel depth and depth of the floodplain filling
		d=ChannelDepth 
			+ sqrt((get_flux_crossection-A_ch)/FloodPlainSlope + w_full*w_full/(4*FloodPlainSlope*FloodPlainSlope))
			- w_full/(2*FloodPlainSlope);
	}
	return d;
}

/// If \f$d\le d_{Channel} \ f$
/// \f[P = w_{Bottom} + 2  \sqrt{1+ \Delta_{bank}^2} d \f]
/// else, if the river floods the flood plain
/// \f[P = P(d_{Channel} + 2 \sqrt{1+ {\Delta_{flood\ plain}}^2} (d-d_{Channel} \f]
double SWATReachType::get_wetted_perimeter( double depth ) const
{
	if (depth<=ChannelDepth) //All the water fits in the IChannel
		return BottomWidth + 2.*depth*sqrt(1+BankSlope*BankSlope);
	else	//The IChannel is full, and water flows on the flood plain
		return get_wetted_perimeter(ChannelDepth) + 2.*(depth-ChannelDepth)*sqrt(1+FloodPlainSlope*FloodPlainSlope);
}
/// If \f$d\le d_{Channel} \ f$
/// \f[w = w_{Bottom} + 2  \Delta_{bank} d \f]
/// else, if the river floods the flood plain
/// \f[w = w(d_{Channel}) + 2 \Delta_{flood\ plain} (d-d_{Channel}) \f]
double SWATReachType::get_channel_width( double depth ) const
{
	if (depth<=ChannelDepth) //All the water fits in the IChannel
		return BottomWidth + 2.*BankSlope*depth;
	else	//The IChannel is full, and water flows on the flood plain
		return get_channel_width(ChannelDepth) + 2.*FloodPlainSlope*(depth-ChannelDepth);
}
/// If \f$d\le d_{Channel} \ f$
/// \f[A = \left(w_{Bottom} + Delta_{bank} d\right) d \f]
/// else, if the river floods the flood plain
/// \f[P = P(d_{Channel}) + \left(w(d_{Channel} + Delta_{flood\ plain} \left(d-d_{Channel}\right)\right) (d-d_{Channel}) \f]
double SWATReachType::get_flux_crossection( double depth ) const
{
	if (depth<=ChannelDepth) //All the water fits in the IChannel
		return (BottomWidth + BankSlope*depth) * depth;
	else	//The IChannel is full, and water flows on the flood plain
		return get_flux_crossection(ChannelDepth) + (get_channel_width(ChannelDepth)+FloodPlainSlope*(depth-ChannelDepth))*(depth-ChannelDepth);
}

SWATReachType::SWATReachType(double l) 
: IChannel(), m_l(l), BottomWidth(3.0),ChannelDepth(0.5),BankSlope(2.0),FloodPlainSlope(200.0), m_nManning(0.035) {
	if (m_l<=0.0) throw std::runtime_error("Length of a channel needs to be >0.0");
}

SWATReachType::SWATReachType(double l, double BankWidth,double depth ) 
: IChannel(), m_l(l), BottomWidth(3.0),ChannelDepth(0.5),BankSlope(2.0),FloodPlainSlope(100.0), m_nManning(0.035)
{
	if (m_l<=0.0) throw std::runtime_error("Length of a channel needs to be >0.0");
	ChannelDepth=depth;
	BottomWidth=BankWidth-2*BankSlope*depth;
	///For very narrow reaches (w/d is small) use steeper banks, minimum bottom width=0.5 bank width
	if (BottomWidth<0.5*BankWidth)
	{
		BottomWidth=0.5*BankWidth;
		BankSlope=(BankWidth-BottomWidth)/(2*depth);
	}
}
SWATReachType* cmf::river::SWATReachType::copy() const
{
	SWATReachType* res = new SWATReachType(*this);
	return res;
}

cmf::river::SWATReachType::SWATReachType( const SWATReachType& copy )
	: m_nManning(copy.m_nManning),
	BankSlope(copy.BankSlope),
	BottomWidth(copy.BottomWidth),
	ChannelDepth(copy.ChannelDepth),
	FloodPlainSlope(copy.FloodPlainSlope),
	m_l(copy.m_l)
{}

/************************************************************************/
/* Triangular shape                                                                     */
/************************************************************************/


/// \f{eqnarray*}
/// w &=& 2 \Delta\ d
/// \f}
double TriangularReach::get_channel_width( double depth ) const
{
	return 2*BankSlope * depth;
}
/// \f{eqnarray*}
/// P &=& 2 d \sqrt{1+\Delta^2}
/// \f}
double TriangularReach::get_wetted_perimeter( double depth ) const
{
	return 2*depth*sqrt(1+BankSlope*BankSlope);
}
/// \f{eqnarray*}
/// d &=& \sqrt{\frac{A}{\Delta}}
/// \f}
double TriangularReach::get_depth( double area ) const
{
	return sign(area/BankSlope) * sqrt(fabs(area/BankSlope));
}
/// \f{eqnarray*}
/// A &=& d^2 \Delta
/// \f}
double TriangularReach::get_flux_crossection( double depth ) const
{
	return depth * depth * BankSlope;
}

TriangularReach::TriangularReach(double l, double bankSlope/*=2*/ ) 
: IChannel(),m_l(l),BankSlope(bankSlope), m_nManning(0.035)
{
	if (m_l<=0.0) throw std::runtime_error("Length of a channel needs to be >0.0m");
}

TriangularReach* TriangularReach::copy() const
{
	return new TriangularReach(*this);
}
cmf::river::TriangularReach::TriangularReach( const TriangularReach& copy )
	: m_nManning(copy.m_nManning), BankSlope(copy.BankSlope), m_l(copy.m_l)
{}

/************************************************************************/
/* Rectangular Reach                                                                     */
/************************************************************************/

double RectangularReach::get_channel_width( double depth ) const
{
	return m_width;
}

double RectangularReach::get_wetted_perimeter( double depth ) const
{
	return 2*depth+m_width;
}

double RectangularReach::get_depth( double area ) const
{
	return area/m_width;
}

double RectangularReach::get_flux_crossection( double depth ) const
{
	return depth*m_width;
}

RectangularReach* RectangularReach::copy() const
{
	return new RectangularReach(*this);
}

RectangularReach::RectangularReach( const RectangularReach& copy )
	: m_nManning(copy.m_nManning), m_l(copy.m_l), m_width(copy.m_width) {}

RectangularReach::RectangularReach( double l,double width ) 
: IChannel(),m_l(l), m_width(width), m_nManning(0.035)
{
	if (m_l<=0.0) throw std::runtime_error("Length of a channel needs to be >0.0m");
}
/************************************************************************/
/* Pipe                                                                 */
/************************************************************************/
double PipeReach::get_channel_width( double depth ) const
{
	if (depth<0 || depth>radius*2)
		return 0;
	else
		return 2*sqrt(fabs(sqr(radius)-sqr(radius-depth)));
}

double PipeReach::get_wetted_perimeter( double depth ) const
{
	if (depth<=0)
		return 0;
	else if (depth>=2*radius)
		return 2*Pi*radius;
	else
		return acos((radius-depth)/radius)*radius;
}

double PipeReach::get_depth( double area ) const
{
	if (area>=Pi*sqr(radius)) 
		return 2*radius;
	else if (area<=0)
		return 0;
	else
		return radius*(1-cos(area/sqr(radius)));
}

double PipeReach::get_flux_crossection( double depth ) const
{
	if (depth<=0)
		return 0;
	else if (depth>=2*radius)
		return Pi*sqr(radius);
	else
		return acos((radius-depth)/radius)*sqr(radius);
}
PipeReach* PipeReach::copy() const 
{ 
	return new PipeReach(*this);
}
PipeReach::PipeReach(const PipeReach& copy)
	: m_nManning(copy.m_nManning), radius(copy.radius), m_l(copy.m_l) {}
PipeReach::PipeReach( double l,double diameter ) 
: IChannel(),m_l(l), radius(diameter * 0.5), m_nManning(0.035)
{
	if (m_l<=0.0) throw std::runtime_error("Length of a channel needs to be >0.0m");
}
/************************************************************************/
/* Channel                                                              */
/************************************************************************/

Channel::Channel( char typecode, double length, double width/*=1.*/,double depth/*=0.25*/ )
: IChannel()
{
	IChannel* newChannel=0;
	switch(typecode)
	{
	case 'T': 
		newChannel = new TriangularReach(length); 
		break;
	case 'R':	
		newChannel = new RectangularReach(length,width);	 
		break;
	case 'S':	
		newChannel= new SWATReachType(length,width,depth);
		break;
	case 'P': 
		newChannel = new PipeReach(length,width);
		break;
	default:
		throw std::runtime_error("Not supported reach type shortcut");
		break;
	};
	m_channel.reset(newChannel);

}

Channel::Channel( const Channel& for_copy ) 
: IChannel(), m_channel(for_copy.m_channel->copy())
{}

Channel::Channel( const IChannel& for_wrapping ) 
: IChannel(), m_channel(for_wrapping.copy())
{}

Channel::Channel( const IVolumeHeightFunction& for_casting ) 
: IChannel()
{
	const IChannel *cast = dynamic_cast<const IChannel *>(&for_casting);
	if(cast == 0)
		throw std::runtime_error("Failed interpreting a non channel h(V) function as a channel");
	else
	{
		m_channel.reset(cast->copy());
	}
}

Channel::Channel( double length ) 
: IChannel(), m_channel(new TriangularReach(length))
{
	if (length<=0.0) throw std::runtime_error("Length of a channel needs to be >0.0m");
}

Channel& Channel::operator=(const Channel& for_assignment)
{
	m_channel.reset(for_assignment.copy());
	return *this;
}

Channel& Channel::operator=( const IChannel& for_assignment )
{
	m_channel.reset(for_assignment.copy());
	return *this;
}

Channel* Channel::copy() const
{
	return new Channel(*this);
}
MeanChannel::MeanChannel( const IChannel& channel1,const IChannel& channel2 ) 
: IChannel(), m_channel1(channel1),m_channel2(channel2)
{}

MeanChannel::MeanChannel( const MeanChannel& meanChannel ) 
: IChannel(), m_channel1(meanChannel.m_channel1), m_channel2(meanChannel.m_channel2)
{		 }

char MeanChannel::typecode() const {	return 'M';}

double MeanChannel::get_channel_width( double depth ) const {
	return mean(m_channel1.get_channel_width(depth),m_channel2.get_channel_width(depth));
}

double MeanChannel::get_depth( double area ) const					{
	return mean(m_channel1.get_depth(area),m_channel2.get_depth(area));
}

double MeanChannel::get_flux_crossection( double depth ) const {
	return mean(m_channel1.get_flux_crossection(depth),m_channel2.get_flux_crossection(depth));
}

double MeanChannel::get_wetted_perimeter( double depth ) const {
	return mean(m_channel1.get_wetted_perimeter(depth),m_channel2.get_wetted_perimeter(depth));
}

MeanChannel* MeanChannel::copy() const
{
	return new MeanChannel(*this);
}

double MeanChannel::get_length() const
{
	return mean(m_channel1.get_length(),m_channel2.get_length());
}

cmf::river::Channel::Channel()
{
	m_channel.reset();
	double a=0;
}


double cmf::river::Prism::A( double V ) const
{
	return Area * piecewise_linear(V/Area,0,RoughThickness);
}

cmf::river::Prism::Prism( double base_area, double thickness_of_rough_ground/*=0.01*/ ) : Area(base_area),RoughThickness(thickness_of_rough_ground)
{
	if (Area<=0) throw std::runtime_error("Cannot construct prism volume to height relation with a base area of zero");
}



// cmf::river::FlowSurface::FlowSurface( double length, double width,
// 	double _d_puddle/*=0.0*/,double _d_rill/*=0.0*/, 
// 	double nManning/*=0.035*/, double _e_m/*=0.6666667*/ )
// 	: m_nManning(nManning), m_length(length), m_width(width), d_puddle(_d_puddle),d_rill(_d_rill), e_m(_e_m)
// {}
// 
// cmf::river::FlowSurface::FlowSurface( const FlowSurface& other )
//  	: m_nManning(other.m_nManning), m_length(other.m_length), m_width(other.m_width), 
// 	d_puddle(other.d_puddle),d_rill(other.d_rill), e_m(other.e_m)
// 
// {}
// 
// double cmf::river::FlowSurface::get_depth( double area ) const
// {
// 	double dmin = area/m_width;
// 	if (dmin>(d_puddle + d_rill)) {
// 		return dmin;
// 	} else {
// 		return sqrt((d_rill+d_puddle)*area/m_width);
// 	}
// }
// 
// double cmf::river::FlowSurface::get_channel_width( double depth ) const
// {
// 	if (depth>d_puddle + d_rill) {
// 		return m_width;
// 	} else {
// 		return depth/(d_puddle+d_rill) * m_width;
// 	}
// }
// 
// double cmf::river::FlowSurface::qManning( double A,double slope ) const
// {
// 	double h = A/m_width - d_puddle;
// 	if (h<=0) return 0.0;
// 	return sqrt(slope)/get_nManning()* pow(h,e_m) * A;
// }
// 
void cmf::river::MeanChannel::set_nManning( double nManning )
{
 	throw std::runtime_error("Cannot set Manning n for a mean channel. Set value for the parts of the channel");
}
