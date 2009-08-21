#include "RetentionCurve.h"
#include <cmath>
#include "../../math/real.h"

// Class for an parabolic extrapolation of a retention curve
namespace cmf {
	namespace upslope {
		class parabolic_extrapolation
		{
		private:
			real a,b,c,w0,psi0;
		public:
			// Calculates the parabola parameters a,b,c using the starting point of the wetness / potential curve (w0,pot0) and the slope of the curve
			parabolic_extrapolation(real wet0,real pot0,real slope0) : w0(wet0),psi0(pot0)
			{
				real w0_2=w0*w0;	// wetness squared
				a=(-slope0 - pot0 + slope0*w0)/(1 - 2*w0 + w0_2);
				b=(slope0 + 2*w0*pot0 - slope0*w0_2)/(1 - 2*w0 + w0_2);
				c=(pot0 - slope0*w0 - 2*w0*pot0 + slope0*w0_2)/(1 - 2*w0 + w0_2);
			}
			// returns the potential for a wetness. Does not test for wetness>w0
			real get_potential(real wetness)
			{
				return a*wetness*wetness + b*wetness + c;
			}
			// returns the wetness for a potential. The user is responsible to ensure the potential is higher than the starting point
			real get_wetness(real potential)
			{
				return 0.5*sqrt(-4*c/a+4*potential/a+(b*b)/(a*a))-b/(2*a);
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
double cmf::upslope::waterhead_to_pF(double waterhead)        { return log10(fabs(waterhead*rho_wg))-2;}

real cmf::upslope::BrooksCoreyRetentionCurve::K( real wetness,real depth ) const
{
	real Ksat=m_Ksat*pow(1-m_KsatDecay,depth);
	return Ksat*minimum(pow(wetness,2+3*b()),1);
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
	if (m_KsatDecay==0)
		return (lowerDepth-upperDepth)*K(wetness,lowerDepth);
	else
	{
		real C=-log(1-m_KsatDecay);	// integration constant
		return (K(wetness,upperDepth) - K(wetness,lowerDepth))/C;
	}
}

real cmf::upslope::BrooksCoreyRetentionCurve::MatricPotential( real wetness ) const
{
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
		return -m_m*(wetness-m_n)*(1-wetness);
	}
	else
	{
		return Psi_X * pow(wetness/wetness_X,-b());
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


cmf::upslope::BrooksCoreyRetentionCurve::BrooksCoreyRetentionCurve( real ksat/*=15*/,real porosity/*=0.5*/,real _b/*=5*/,real theta_x/*=0.2*/,real psi_x/*=pF_to_waterhead(2.5)*/,real ksat_decay/*=0*/,real porosity_decay/*=0*/ ) : m_Ksat(ksat),m_Porosity(porosity),m_b(_b),wetness_X(theta_x/porosity),Psi_X(psi_x),m_PorosityDecay(porosity_decay),m_KsatDecay(ksat_decay)
{
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
	real m=1-1/n;
	real h=-100 * suction;
	real w0=0.99;
	real p0=MatricPotential(w0);
	if (suction<=p0)
		return pow(1+pow(alpha*h,n),-m);
	else
	{
		real dp0=1e6*(p0-MatricPotential(w0-1e-6));
		cmf::upslope::parabolic_extrapolation p(w0,p0,dp0);
		return p.get_wetness(suction);
	}
}
	

real cmf::upslope::VanGenuchtenMualem::MatricPotential( real w ) const
{
	real w0=0.99; // Wetness at the border between non-linear and linear part = wetness(h0)
	if (w<=w0)
	{
		// inverse m and inverse n
		real mi=1/(1-1/n),ni=1/n;
		return -0.01*pow(1-pow(w,mi),ni)/(alpha*pow(w,mi*ni));
	}
	else
	{
		real p0=MatricPotential(w0);
		real dp0=1e6*(p0-MatricPotential(w0-1e-6));
		cmf::upslope::parabolic_extrapolation p(w0,p0,dp0);
		return p.get_potential(w);
	}
}

real cmf::upslope::VanGenuchtenMualem::K( real wetness,real depth ) const
{
	if (wetness>=1) return Ksat*exp(-(wetness-1)*10);
	real m=1-1/n;
	return Ksat*minimum(sqrt(wetness)*square(1-pow(1-pow(wetness,1/m),m)),1);
}

real cmf::upslope::VanGenuchtenMualem::VoidVolume( real upperDepth,real lowerDepth,real Area ) const
{
	return (lowerDepth-upperDepth)*Area*Porosity(upperDepth);
}

real cmf::upslope::VanGenuchtenMualem::Transmissivity( real upperDepth,real lowerDepth,real wetness ) const
{
	return K(wetness,lowerDepth)*(lowerDepth-upperDepth);
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

