#include "ShuttleworthWallace.h"
#include "../Soil/RetentionCurve.h"

const real cmf::upslope::vegetation::ShuttleworthWallace::k=0.4;
const real cmf::upslope::vegetation::ShuttleworthWallace::NN=2.5;
const real cmf::upslope::vegetation::ShuttleworthWallace::C_R=0.6;
const real cmf::upslope::vegetation::ShuttleworthWallace::MJ_to_W=1e6/86400.0;
const real cmf::upslope::vegetation::ShuttleworthWallace::c_p_rho=1240;
const real cmf::upslope::vegetation::ShuttleworthWallace::lambda=2.45;
const real cmf::upslope::vegetation::ShuttleworthWallace::gamma=0.067;



cmf::upslope::vegetation::ShuttleworthWallace::Resistances 
	cmf::upslope::vegetation::ShuttleworthWallace::resitances(const cmf::upslope::vegetation::Vegetation & vegParameters, real u,real Rs,real T,real vpd,
																														real soil_matric_potential,real meas_height_above_canopy,bool CanopyStoresWater,bool ReturnPET)
{
	Resistances r;
	real h=vegParameters.Height; // Height of the canopy
	/************************************************************************/
	/* Aerodynamic resistances                                                                     */
	/************************************************************************/
	real z_a=h+meas_height_above_canopy; // Height of the wind measurement
	// Roughness length of the canopy
	real z0_c=piecewise_linear(vegParameters.Height,1,10,0.13,0.05)*vegParameters.Height; 
	// zero plane displacement of the canopy
	real disp_c=h-z0_c/0.3;
	// zero plane displacement
	real disp = disp_c;
	// roughness length
	real z0=z0_c;
	// roughness length of the ground
	real z0gs=minimum(0.01,z0_c);
	// If canopy is not closed, calculate z0 and disp for scarce canopy
	if (vegParameters.CanopyClosure<1)
	{
		real XX=vegParameters.CanopyClosure*pow(-1 + exp(0.909-3.03 * z0_c / h),4);
		disp=1.1*h*log(1+pow(XX,0.25));
		z0=minimum(0.3*(h-disp),z0gs+0.3*h*sqrt(XX));
	}
	// Calculate windspeed at disp
	real u_ = k*u/(log((z_a-disp)/z0));
	// K_h is the KH in BROOK90
	real K_h=k*u_*(h+disp);
	r.aa = log((z_a-disp)/(h-disp))/(k*u_) + (h/(NN*K_h)) * (-1 + exp(NN*(h-disp_c-z0_c)/h));
	r.as = (h*exp(NN)/(NN*K_h)) * (exp(-NN*z0gs/h) - exp(-NN*(z0_c+disp_c)/h));
	real uh=(u_/k)*log((h-disp)/z0);
	real rb=100*NN*sqrt(vegParameters.LeafWidth/uh)/(1-exp(-NN/2));
	r.ac =	rb/(2*maximum(vegParameters.LAI,1e-5));
	/************************************************************************/
	/* Soil surface resitance                                               */
	/************************************************************************/
	real r_ss_fc=500;	//  resistance at field capacity in s/m
	real fc=cmf::upslope::pF_to_waterhead(2.5);
	r.ss = r_ss_fc * (soil_matric_potential/fc);

	/************************************************************************/
	/* Calculate the canopy surface resistance                              */
	/************************************************************************/
	if (CanopyStoresWater)
		r.sc=0;
	else
	{
		Rs*=1e-6*cmf::math::day.AsSeconds(); // Convert MJ/(m2 day) to W/m2
		real
			// Stomatal conductance range
			c_stom_min=0.0003, // minimum stomatal conductivity at night m/s
			c_stom_max=1/vegParameters.StomatalResistance, // maximum stomatal conductivity m/s
			// Parameters for radiation limitiation
			R_5=100,	// Radiation where stomatal conduction is halved in W/m2
			R_max=1000, // Radiation where stomatal conduction is maximal
			R_0=R_max*R_5/(R_max-2*R_5),
			// Radiation limitation
			f_r_int=Rs<=1e-10 ? 0
			: ((R_max + R_0) / (R_max * C_R )) * log((R_0 + C_R * Rs) / (R_0 + C_R * Rs * exp(-C_R * vegParameters.LAI))),
			// Vapor pressure stomatal closure
			// Lohammar et al. (1980) and Stannard (1993)
			c_vpd=2,						 // kPa, vapor press deficit where stomatal conductance is halved
			f_D = 1/(1+vpd/c_vpd),
			// temperature limitation, 
			// zero for freezing periods (TL), parabolic rise between 0 and 10°C (T1), 1 for 10..30(T2) °C, parabolic decrease for 30..40 (TH)°C, 0 for above 40°C
			TL=0,T1=10,T2=30,TH=40,
			f_T = (T > TL && T < T1)   ? 1 - ((T1 - T) / (T1 - TL)) * ((T1 - T) / (T1 - TL))	// parabolic rise
			: (T >= T1 && T <= T2) ? 1																										// no limitation by Temp
			: (T > T2 && T < TH)   ? 1 - ((T - T2) / (TH - T2)) * ((T - T2) / (TH - T2))	// parabolic decrease (to warm)
			: 0,																																					// extreme Temps, (freezing or above 40°C)
			// soil wetness limitation
			pF = cmf::upslope::waterhead_to_pF(soil_matric_potential),
			f_W = ReturnPET ? 1 : piecewise_linear(pF,3.35,4.2,1,0),
			// actual stomatal conductivity	= Product of limiting factors times stomatal conductance times leaf area index
			c_stom = f_D * f_T * f_r_int * f_W * (c_stom_max - c_stom_min) + vegParameters.LAI * c_stom_min;
		r.sc = 1/maximum(c_stom,1e-10);
	}

	return r;
}

real cmf::upslope::vegetation::ShuttleworthWallace::D0( Resistances r,real Rn_canopy,real Rn_ground, real vpd, real Delta )
{
	real
		RS = (Delta+gamma) * r.as + gamma * r.ss,
		RC = (Delta+gamma) * r.ac + gamma * r.sc,
		RA = (Delta+gamma) * r.aa,
		cc = 1/(1 + RS * RA / (RC * (RS + RA))),
		cs = 1/(1 + RC * RA / (RS * (RC + RA))),
		Rn=Rn_canopy+Rn_ground,
		PMS = PM(Rn,vpd-Delta*r.as*Rn_canopy/c_p_rho,Delta,r.aa + r.as,r.ss),
		PMC = PM(Rn,vpd-Delta*r.ac*Rn_ground/c_p_rho,Delta,r.aa + r.ac,r.sc),
		LE = (cc * PMC + cs * PMS);
	return vpd + r.aa * (Delta * Rn - (Delta + gamma) * LE)/c_p_rho;
}

cmf::upslope::vegetation::ShuttleworthWallace::ShuttleworthWallace( cmf::atmosphere::Weather w,real soilwater_matrixpotential,const cmf::upslope::vegetation::Vegetation & veg,bool CanopyStoresWater/*=false*/,real measurement_height_above_canopy/*=2*/ )
{
	real vpd = w.e_s-w.e_a;
	Resistances r = resitances(veg,w.Windspeed,w.Rs,w.T,vpd,soilwater_matrixpotential,measurement_height_above_canopy,CanopyStoresWater);
	real 
		Rn = w.Rn(veg.albedo) * MJ_to_W, // Net radiation flux in W/m2
		Rn_oncanopy=veg.CanopyClosure*Rn, // Radiation on canopy
		Rn_ground = Rn_oncanopy*exp(-C_R*veg.LAI) // Radiation through canopy
								+(Rn-Rn_oncanopy),						// direct Radiation to soil
		Rn_canopy = Rn - Rn_ground,
		Delta = 4098*0.6108 * exp(17.27*w.T/(w.T+237.3)) / ((w.T+237.3)*(w.T+237.3)), 
		_D0=D0(r,Rn_canopy,Rn_ground,vpd,Delta);
	this->T=PM(Rn_canopy,_D0,Delta,r.ac,r.sc)/(lambda*MJ_to_W);
	this->E=PM(Rn_ground,_D0,Delta,r.as,r.ss)/(lambda*MJ_to_W);
}

real cmf::upslope::vegetation::ShuttleworthWallace::PM( real Rn,real vpd,real Delta,real r_a,real r_s )
{
	return (r_a * Delta * Rn + c_p_rho * vpd) / ((Delta + gamma) * r_a + gamma * r_s);
}

