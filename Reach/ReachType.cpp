#include "ReachType.h"
#include <cmath>
#include "../math/real.h"
#include "VolumeHeightRelation.h"
#include <stdexcept>
#define sqr(a) ((a)*(a))
/// \f[ d = \sqrt{\frac{A}{\Delta_{bank}} + \frac{{w_{bottom}}^2}{4 {\Delta_{bank}}^2}} - \frac{w_{bottom}}{2 \Delta_{bank}} \f]
/// If \f$d>d_{channel}\f$
/// \f[d=d_{channel}+\sqrt{\frac{A-A(d_{channel})}{\Delta_{flood\ plain}} + \frac{{w(d_{channel})}^2}{4 {\Delta_{flood\ plain}}^2}} - \frac{w(d_{channel})}{2 \Delta_{flood\ plain}} \f]
double cmf::river::SWATReachType::Depth( double Area ) const
{
	//Calculate depth from the void volume of the channel, using the SWAT river geometry (trapezoid)
	double d = 
		sqrt(
			Area/BankSlope
			+ (BottomWidth*BottomWidth / (4 * BankSlope*BankSlope))
		)
		- BottomWidth/(2*BankSlope);
	//If the channel is full
	if (d>ChannelDepth)
	{
		//Calculate the Area of the full channel
		double A_ch=(BottomWidth+BankSlope*ChannelDepth)*ChannelDepth;
		//Calculate the width of the full channel
		double w_full=Width(ChannelDepth);
		//Calculate the sum of channel depth and depth of the floodplain filling
		d=ChannelDepth 
			+ sqrt((Area-A_ch)/FloodPlainSlope + w_full*w_full/(4*FloodPlainSlope*FloodPlainSlope))
			- w_full/(2*FloodPlainSlope);
	}
	return d;
}

/// If \f$d\le d_{Channel} \ f$
/// \f[P = w_{Bottom} + 2  \sqrt{1+ \Delta_{bank}^2} d \f]
/// else, if the river floods the flood plain
/// \f[P = P(d_{Channel} + 2 \sqrt{1+ {\Delta_{flood\ plain}}^2} (d-d_{Channel} \f]
double cmf::river::SWATReachType::Perimeter( double depth ) const
{
	if (depth<=ChannelDepth) //All the water fits in the channel
		return BottomWidth + 2.*depth*sqrt(1+BankSlope*BankSlope);
	else	//The channel is full, and water flows on the flood plain
		return Perimeter(ChannelDepth) + 2.*(depth-ChannelDepth)*sqrt(1+FloodPlainSlope*FloodPlainSlope);
}
/// If \f$d\le d_{Channel} \ f$
/// \f[w = w_{Bottom} + 2  \Delta_{bank} d \f]
/// else, if the river floods the flood plain
/// \f[w = w(d_{Channel}) + 2 \Delta_{flood\ plain} (d-d_{Channel}) \f]
double cmf::river::SWATReachType::Width( double depth ) const
{
	if (depth<=ChannelDepth) //All the water fits in the channel
		return BottomWidth + 2.*BankSlope*depth;
	else	//The channel is full, and water flows on the flood plain
		return Width(ChannelDepth) + 2.*FloodPlainSlope*(depth-ChannelDepth);
}
/// If \f$d\le d_{Channel} \ f$
/// \f[A = \left(w_{Bottom} + Delta_{bank} d\right) d \f]
/// else, if the river floods the flood plain
/// \f[P = P(d_{Channel}) + \left(w(d_{Channel} + Delta_{flood\ plain} \left(d-d_{Channel}\right)\right) (d-d_{Channel}) \f]
double cmf::river::SWATReachType::Area( double depth ) const
{
	if (depth<=ChannelDepth) //All the water fits in the channel
		return (BottomWidth + BankSlope*depth) * depth;
	else	//The channel is full, and water flows on the flood plain
		return Area(ChannelDepth) + (Width(ChannelDepth)+FloodPlainSlope*(depth-ChannelDepth))*(depth-ChannelDepth);
}

cmf::river::SWATReachType::SWATReachType() :ReachType(), BottomWidth(3.0),ChannelDepth(0.5),BankSlope(2.0),FloodPlainSlope(200.0)
{

}

cmf::river::SWATReachType::SWATReachType( double BankWidth,double Depth ) : BottomWidth(3.0),ChannelDepth(0.5),BankSlope(2.0),FloodPlainSlope(100.0)
{
	ChannelDepth=Depth;
	BottomWidth=BankWidth-2*BankSlope*Depth;
	///For very narrow reaches (w/d is small) use steeper banks, minimum bottom width=0.5 bank width
	if (BottomWidth<0.5*BankWidth)
	{
		BottomWidth=0.5*BankWidth;
		BankSlope=(BankWidth-BottomWidth)/(2*Depth);
	}
}
/// \f{eqnarray*}
/// w &=& 2 \Delta\ d
/// \f}
double cmf::river::TriangularReach::Width( double depth ) const
{
	return 2*BankSlope * depth;
}
/// \f{eqnarray*}
/// P &=& 2 d \sqrt{1+\Delta^2}
/// \f}
double cmf::river::TriangularReach::Perimeter( double depth ) const
{
	return 2*depth*sqrt(1+BankSlope*BankSlope);
}
/// \f{eqnarray*}
/// d &=& \sqrt{\frac{A}{\Delta}}
/// \f}
double cmf::river::TriangularReach::Depth( double area ) const
{
	return sqrt(area/BankSlope);
}
/// \f{eqnarray*}
/// A &=& d^2 \Delta
/// \f}
double cmf::river::TriangularReach::Area( double depth ) const
{
	return depth * depth * BankSlope;
}

cmf::river::TriangularReach::TriangularReach( double bankSlope/*=2*/ ) : ReachType(),BankSlope(bankSlope)
{

}

double cmf::river::ReachType::qManning( double A,double slope ) const
{
	double 
		d=Depth(A),
		P=Perimeter(d)+0.001, // a mm extra to prevent divide by zero
		R=A/P+0.001;          // a mm extra to prevent pow failure
	return A*pow(R,2./3.)*sqrt(slope)/nManning;
}

cmf::river::Channel* cmf::river::ReachType::AsChannel( real length )
{
	return new Channel(*this,length);
}
double cmf::river::RectangularReach::Width( double depth ) const
{
	return m_width;
}

double cmf::river::RectangularReach::Perimeter( double depth ) const
{
	return 2*depth+m_width;
}

double cmf::river::RectangularReach::Depth( double area ) const
{
	return area/m_width;
}

double cmf::river::RectangularReach::Area( double depth ) const
{
	return depth*m_width;
}

cmf::river::RectangularReach* cmf::river::RectangularReach::copy() const
{
	return new RectangularReach(m_width);
}

double cmf::river::PipeReach::Width( double depth ) const
{
	if (depth<0 || depth>m_Radius*2)
		return 0;
	else
		return 2*sqrt(fabs(sqr(m_Radius)-sqr(m_Radius-depth)));
}

double cmf::river::PipeReach::Perimeter( double depth ) const
{
	if (depth<=0)
		return 0;
	else if (depth>=2*m_Radius)
		return 2*Pi*m_Radius;
	else
		return acos((m_Radius-depth)/m_Radius)*m_Radius;
}

double cmf::river::PipeReach::Depth( double area ) const
{
	if (area>=Pi*sqr(m_Radius)) 
		return 2*m_Radius;
	else if (area<=0)
		return 0;
	else
		return m_Radius*(1-cos(area/sqr(m_Radius)));
}

double cmf::river::PipeReach::Area( double depth ) const
{
	if (depth<=0)
		return 0;
	else if (depth>=2*m_Radius)
		return Pi*sqr(m_Radius);
	else
		return acos((m_Radius-depth)/m_Radius)*sqr(m_Radius);
}
cmf::river::PipeReach* cmf::river::PipeReach::copy() const 
{ 
	return new PipeReach(2*m_Radius);
}

cmf::river::ReachType* cmf::river::create_reachtype(char shortcut,double width,double depth)
{
	switch(shortcut)
	{
	case 'T': return new cmf::river::TriangularReach;
		break;
	case 'R':	return new cmf::river::RectangularReach(width);
	case 'S':	return new cmf::river::SWATReachType(width,depth);
	case 'P': return new cmf::river::PipeReach(width);
	default:
		throw std::runtime_error("Not supported reach type shortcut");
	}

}
