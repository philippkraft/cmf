

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
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
#include "RetentionCurve.h"
#include <cmath>
#include <limits>
#include <sstream>
#include "../../math/real.h"

// Class for an parabolic extrapolation of a retention curve
namespace cmf {
	namespace upslope {
		class parabolic_extrapolation
		{
		public:
			real a,b,c,w0,psi0;
			// Calculates the parabola parameters a,b,c using the starting point of the wetness / potential curve (w0,pot0) and the slope of the curve
			parabolic_extrapolation(real wet0,real pot0,real slope0) 
				: w0(wet0),psi0(pot0)
			{
				real w0_2=w0*w0;	// wetness squared
				a=(-slope0 - pot0 + slope0*w0)/(1 - 2*w0 + w0_2);
				b=(slope0 + 2*w0*pot0 - slope0*w0_2)/(1 - 2*w0 + w0_2);
				c=(pot0 - slope0*w0 - 2*w0*pot0 + slope0*w0_2)/(1 - 2*w0 + w0_2);
			}
			// returns the potential for a wetness. Does not test for wetness>w0
			real get_potential(real wetness) {
				return a*wetness*wetness + b*wetness + c;
			}
			// returns the wetness for a potential. The user is responsible to ensure the potential is higher than the starting point
			real get_wetness(real potential) {
				return (sqrt(4*a*potential-4*a*c+square(b))-b)/(2*a);
			}
		};
	}
}


/// Converts a pressure in Pa to a length of a water column in m
double cmf::upslope::pressure_to_waterhead(double Pressure)		{	return Pressure/rho_wg;		}
/// Converts a height of a water column in m to a pressure in Pa
double cmf::upslope::waterhead_to_pressure(double waterhead)  { return waterhead*rho_wg;  }
/// Converts a pF value to a height of a water column in m
double cmf::upslope::pF_to_waterhead(double pF)               { return -pow(10,pF+2)/rho_wg;}
/// Converts a height of a water column to a pF value
double cmf::upslope::waterhead_to_pF(double waterhead)        {
	if (waterhead>=0) return -std::numeric_limits<double>::infinity();
	else return log10(-waterhead*rho_wg)-2;
}

real cmf::upslope::BrooksCoreyRetentionCurve::K( real wetness) const
{
	wetness=minmax(wetness,0,1);
	return Ksat*minimum(pow(wetness,2*get_b()+3),1);
}

real cmf::upslope::BrooksCoreyRetentionCurve::Porosity( real depth ) const
{
	return m_Porosity*pow(1-m_PorosityDecay,depth);
}

void cmf::upslope::BrooksCoreyRetentionCurve::SetPorosity( real porosity,real porosity_decay/*=0*/ )
{
	m_Porosity = porosity;m_PorosityDecay=porosity_decay;
}

real cmf::upslope::BrooksCoreyRetentionCurve::VoidVolume( real upperDepth,real lowerDepth,real Area ) const
{
	if (m_PorosityDecay==0)
		return (lowerDepth-upperDepth)*Area*Porosity(upperDepth);
	else
	{
		real C=-log(1-m_PorosityDecay); // integration constant
		return (Porosity(upperDepth) - Porosity(lowerDepth)) * Area/C;
	}
}

real cmf::upslope::BrooksCoreyRetentionCurve::FillHeight( real lowerDepth,real Area,real Volume ) const
{
	if (m_PorosityDecay==0) 
		return Volume/(Porosity(lowerDepth)*Area);
	else
	{
		real V_to_lower=VoidVolume(0,lowerDepth,Area);	 // Volume from surface to lower depth
		real V_to_upper=V_to_lower - Volume;						 // Volume from surface to unknown upper depth
		real C=log(1-m_PorosityDecay);									 // integration constant
		// The function here is the VoidVolume function solved for depth
		real d_upper=(log(C*V_to_upper+m_Porosity)-log(m_Porosity))/C;	// upper depth
		return lowerDepth-d_upper; // return fill height
			
	}
}

real cmf::upslope::BrooksCoreyRetentionCurve::Transmissivity( real upperDepth,real lowerDepth,real wetness) const
{
	if (upperDepth>=lowerDepth) return 0.0;
	return (lowerDepth-upperDepth)*K(wetness);
}

real cmf::upslope::BrooksCoreyRetentionCurve::MatricPotential( real wetness ) const
{
	if (wetness<=1e-12)
		return -1e6;
	real w0=0.99;
	if (wetness>0.99)
	{
		real p0=MatricPotential(w0);
		real dp0=1e6*(p0-MatricPotential(w0-1e-6));
		cmf::upslope::parabolic_extrapolation p(w0,p0,dp0);
		return p.get_potential(wetness);
	}
	else if (wetness>m_Wi)
	{
		return  -minimum(m_m*(wetness-m_n)*(1-wetness), 1e6);
	}
	else
	{
		return minimum(Psi_X * pow(wetness/wetness_X,-get_b()),1e6);
	}

}
real cmf::upslope::BrooksCoreyRetentionCurve::Wetness( real suction ) const
{
	real w0=0.99;
	real p0=MatricPotential(w0);
	if (suction>p0)
	{
		real dp0=1e6*(p0-MatricPotential(w0-1e-6));
		cmf::upslope::parabolic_extrapolation p(w0,p0,dp0);
		return p.get_wetness(suction);
	}
	if (suction>m_Psi_i)
	{
		real& Psi=suction,
		m=m_m,
		n=m_n,
		m2=m_m*m_m,
		n2=m_n*m_n;
		return (sqrt(4*m*Psi+m2*n2-2*m2*n+m2)+m*n+m)/(2*m);
	}
	else
	{
		return pow(Psi_X/suction,1/m_b)*wetness_X;
	}

}


cmf::upslope::BrooksCoreyRetentionCurve::BrooksCoreyRetentionCurve( real ksat/*=15*/,real porosity/*=0.5*/,real _b/*=5*/,real theta_x/*=0.2*/,real psi_x/*=pF_to_waterhead(2.5)*/,real porosity_decay/*=0*/ )
: Ksat(ksat),m_Porosity(porosity),m_b(_b),wetness_X(theta_x/porosity),Psi_X(psi_x),m_PorosityDecay(porosity_decay)
{
	std::stringstream msg;
	msg << "Can't create Brooks-Corey-Retention curve with ";
	bool error=false;
	if (_b<=1.0 || _b>20.0) {
		msg <<"b="<<_b << " outside [1.0..20-0]";error=true;
	}
	if (porosity>1.0 || porosity<=0.0) {
		msg << (error ? " and " : "") << "porostiy=" << porosity << " not in [0.0..1.0]";error=true;
	}
	if (theta_x<=0.0 || theta_x>1.0) {
		msg << (error ? " and " : "") << "theta_x=" <<theta_x << " not in [0.0..1.0]";error=true;
	}
	if (theta_x>=porosity){
		msg << (error ? " and " : "") << "theta_x > porosity";error=true;
	}
	if (ksat<0.0) {
		msg << (error ? " and " : "") << "with negative Ksat="<< ksat;error=true;
	}
	if (error) throw std::runtime_error(msg.str());

	Set_Saturated_pF_curve_tail_parameters();
}

cmf::upslope::BrooksCoreyRetentionCurve cmf::upslope::BrooksCoreyRetentionCurve::CreateFrom2Points( real ksat,real porosity,real theta1,real theta2,real psi_1/*=pF_to_waterhead(2.5)*/,real psi_2/*=pF_to_waterhead(4.2)*/ )
{
	return BrooksCoreyRetentionCurve(ksat,porosity,log(psi_1/psi_2)/log(theta2/theta1),theta1,psi_1);
}

void cmf::upslope::BrooksCoreyRetentionCurve::Set_Saturated_pF_curve_tail_parameters()
{
	m_Wi=0.9+0.005*m_b;
	m_Psi_i=Psi_X*pow(m_Wi/wetness_X,-m_b);
	m_m=-m_Psi_i/((1-m_Wi)*(1-m_Wi))-m_b*(-m_Psi_i)/(m_Wi*(1-m_Wi));
	m_n=2*m_Wi-(m_b*(-m_Psi_i))/(m_m*m_Wi)-1;
}



real cmf::upslope::VanGenuchtenMualem::Wetness( real suction) const
{
	real _m = m<0 ? 1-1/n : m;
	real h=-100 * suction;
	//real w0=0.99;
	real p0=MatricPotential(w0);
	if (suction<=p0)
		return pow(1+pow(alpha*h,n),-_m);
	else
	{
		real dp0= dPsiM_dW(w0);
		cmf::upslope::parabolic_extrapolation p(w0,p0,dp0);
		return p.get_wetness(suction);
	}
}
real cmf::upslope::VanGenuchtenMualem::dPsiM_dW(real w) const{
	real _m = m<0 ? 1-1/n : m;
	real mi = 1.0/_m, ni = 1.0/n;
	if (w<=w0) {
		return 
			  pow(w, mi)*pow(w, -(mi*ni))*pow(-pow(w, mi) + 1, ni)
			  /
			 (100*alpha*_m*n*w*(-pow(w, mi) + 1)) 
		   + pow(w, -(mi*ni))*pow(-pow(w, mi) + 1, ni)
			 /
			 (100 * alpha*_m*n*w);
	} else {
		real p0 = MatricPotential(w0);
		real dp0 = dPsiM_dW(w0);
		parabolic_extrapolation p(w0,p0,dp0);
		return 2 * p.a * w + p.b;
	}
			
}	

real cmf::upslope::VanGenuchtenMualem::MatricPotential( real w ) const
{
	real _m = m<0 ? 1-1/n : m;
	//real w0=0.99; // Wetness at the border between non-linear and linear part = wetness(h0)
	if (w<=w0)
	{
		// inverse m and inverse n
		real mi=1/_m,ni=1/n;
		return -0.01*pow(1-pow(w,mi),ni)/(alpha*pow(w,mi*ni));
	}
	else
	{
		real p0=MatricPotential(w0);
		real dp0 = dPsiM_dW(w0);
		cmf::upslope::parabolic_extrapolation p(w0,p0,dp0);
		return p.get_potential(w);
	}
}

real cmf::upslope::VanGenuchtenMualem::K( real wetness) const
{
	real _m = m<0 ? 1-1/n : m;
	if (wetness>=1) return Ksat;
	return Ksat*minimum(pow(wetness,l)*square(1-pow(1-pow(wetness,1/_m),_m)),1);
}

real cmf::upslope::VanGenuchtenMualem::VoidVolume( real upperDepth,real lowerDepth,real Area ) const
{
	return (lowerDepth-upperDepth)*Area*Porosity(upperDepth);
}

real cmf::upslope::VanGenuchtenMualem::Transmissivity( real upperDepth,real lowerDepth,real wetness ) const
{
	return K(wetness)*(lowerDepth-upperDepth);
}

real cmf::upslope::VanGenuchtenMualem::Porosity( real depth ) const
{
	return Phi;
}

real cmf::upslope::VanGenuchtenMualem::FillHeight( real lowerDepth,real Area,real Volume ) const
{
	return Volume/(Area*Phi);
}

cmf::upslope::VanGenuchtenMualem* cmf::upslope::VanGenuchtenMualem::copy() const
{
	return new VanGenuchtenMualem(*this);
}

cmf::upslope::VanGenuchtenMualem::VanGenuchtenMualem( real _Ksat, real _phi,real _alpha, real _n, real _m/*=-1*/ ) 
: n(_n),alpha(_alpha),Phi(_phi),Ksat(_Ksat), m(_m), w0(.99),l(0.5),theta_r(0.0)
{
	std::stringstream msg;
	msg << "Can't create VanGenuchten-Mualem-Retention curve with ";
	bool error=false;
	if (n<=1.0 || n>4.0) {
		msg <<"n="<<_n << " outside [1.0..4.0]";error=true;
	}
	if (_phi>1.0 || _phi<=0.0) {
		msg << (error ? " and " : "") << "phi=" << _phi << " not in [0.0..1.0]";error=true;
	}
	if (_alpha<=0.0 || _alpha>=2.0) {
		msg << (error ? " and " : "") << "alpha=" <<_alpha << " not in [0.0..2.0]";error=true;
	}
	if (_Ksat<0.0) {
		msg << (error ? " and " : "") << "with negative Ksat="<<_Ksat;error=true;
	}
	if (error) throw std::runtime_error(msg.str());
}

real cmf::upslope::VanGenuchtenMualem::fit_w0( real w1/*=1.01*/,real Psi_p/*=1.0*/,real tolerance/*=0.1*/ )
{
	real w1_,error;
	// Do not more then 100 iterations
	for (int i = 0; i < 100 ; ++i)
	{
		// get wetness at Psi_p
		w1_ = Wetness(Psi_p);
		// Compare wetness
		error = (w1_ - w1)/(w1-1);
		// if w1_ is too big, raise w0
		if (error>tolerance)
			w0+=.5*(1-w0);
		// if w1_ is too small, lower w0
		else if (error<-tolerance)
			w0-=.5*(1-w0);
		else
			break;
	}
	return w0;
}

real cmf::upslope::VanGenuchtenMualem::Diffusivity( real wetness ) const
{
	// Get VanGenuchten m (either Mualem Theory or user set)
	real _m = m<0 ? 1-1/n : m;
	if (wetness<=w0) {
		// eq. 41 from MACRO_5 user manual
		// eq 41. first brackets
		real term1 = ((1-_m)*Ksat)/(alpha*100*_m*Phi);
		// eq 41, 2nd part
		real term2 = pow(wetness,l-1/_m);
		real term3 = pow(1-pow(wetness,1/_m),-_m) + pow(1-pow(wetness,1/_m),_m)-2;
		return term1*term2*term3;
	} else {
		real p0=MatricPotential(w0);
		real dp0=1e6*(p0-MatricPotential(w0-1e-6));
		cmf::upslope::parabolic_extrapolation p(w0,p0,dp0);
		return K(wetness) *  2*p.a*wetness + p.b;
	}
}

real cmf::upslope::VanGenuchtenMualem::theta( real wetness ) const
{
	return wetness * (Phi-theta_r) + theta_r;
}

/************************************************************************/
/* Linear Retention curve                                               */
/************************************************************************/

real cmf::upslope::LinearRetention::K( real wetness) const
{
	wetness=minmax(wetness,residual_wetness,1);
	return Ksat * pow((wetness - residual_wetness) / (1 - residual_wetness),beta);
}

real cmf::upslope::LinearRetention::Porosity( real depth ) const
{
	return porosity*pow(1-porosity_decay,depth);
}


real cmf::upslope::LinearRetention::VoidVolume( real upperDepth,real lowerDepth,real Area ) const
{
	if (porosity_decay==0)
		return (lowerDepth-upperDepth)*Area*Porosity(upperDepth);
	else
	{
		real C=-log(1-porosity_decay); // integration constant
		return (Porosity(upperDepth) - Porosity(lowerDepth)) * Area/C;
	}
}

real cmf::upslope::LinearRetention::FillHeight( real lowerDepth,real Area,real Volume ) const
{
	if (porosity_decay==0) 
		return Volume/(Porosity(lowerDepth)*Area);
	else
	{
		real V_to_lower=VoidVolume(0,lowerDepth,Area);	 // Volume from surface to lower depth
		real V_to_upper=V_to_lower - Volume;						 // Volume from surface to unknown upper depth
		real C=log(1-porosity_decay);									 // integration constant
		// The function here is the VoidVolume function solved for depth
		real d_upper=(log(C*V_to_upper+porosity)-log(porosity))/C;	// upper depth
		return lowerDepth-d_upper; // return fill height

	}
}

real cmf::upslope::LinearRetention::Transmissivity( real upperDepth,real lowerDepth,real wetness) const
{
	if (upperDepth>=lowerDepth) return 0.0;
	return (lowerDepth-upperDepth)*K(wetness);
}

real cmf::upslope::LinearRetention::Wetness( real suction ) const
{
	if (suction>-thickness)
		return 1 + (1 - residual_wetness) * suction/thickness;
	else
		return residual_wetness * exp(suction+thickness);
}
real cmf::upslope::LinearRetention::MatricPotential( real wetness ) const
{
	if (wetness>residual_wetness)
		return -thickness * (1-(wetness - residual_wetness)/(1 - residual_wetness));
	else if (wetness>0)
		return log(wetness/residual_wetness)-thickness;
	else
		return -std::numeric_limits<real>::infinity();
}

cmf::upslope::LinearRetention::LinearRetention( real _Ksat,real _Phi, real _thickness,real _residual_wetness/*=0.1*/ ) 
: Ksat(_Ksat),porosity(_Phi),thickness(_thickness),residual_wetness(_residual_wetness) ,beta(1.0), porosity_decay(0.0)
{

}
#ifdef EXPERIMENTAL
real cmf::upslope::VGM_BC_RetentionCurve_Windhorst::K(real wetness) const {
	return Ksat * pow(wetness,eta);
}

cmf::upslope::VGM_BC_RetentionCurve_Windhorst::VGM_BC_RetentionCurve_Windhorst(real _Ksat, real _phi,real _alpha, real _n, real _m, real _eta)
: VanGenuchtenMualem(_Ksat,_phi,_alpha,_n,_m), eta(_eta)
{}
#endif