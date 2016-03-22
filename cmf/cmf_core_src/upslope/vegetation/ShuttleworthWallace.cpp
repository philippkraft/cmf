

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
//   along with cmf.  if not, see <http://www.gnu.org/licenses/>.
//   
#include "ShuttleworthWallace.h"
#include "../Soil/RetentionCurve.h"
#include "../connections/AtmosphericFluxes.h"
using namespace cmf::math;
using namespace cmf::upslope;
using namespace cmf::upslope::vegetation;
using namespace cmf::upslope::ET;


#define sqr(a) ((a)*(a))
///DT  time step for DFILE interval,  must be 1 d
static const double DT = 1.;
///WTOMJ  (MJ m-2 d-1)/(watt/m2) = 86400 s/d * .000001 MJ/J
static const double WTOMJ = 0.0864;
///ETOM  (mm water)/(MJ/m2) using Lv 2448 MJ/Mg and density of water 1 Mg/m3
///= 1E3 mm/m / (2448 MJ/Mg * 1 Mg/m3)
static const double ETOM = 0.4085;
///CPRHO - volumetric heat capacity of air, J m-3 K-1)
static const double CPRHO = 1240;
///GAMMA - psychrometer constant, kPa/K
static const double GAMMA = 0.067;
///CVLQ - volumetric heat capacity of water, MJ m-2 mm-1 K-1
static const double CVLQ = 0.00418;
///CVICE - volumetric heat capacity of ice, MJ m-2 mm-1 K-1
static const double CVICE = 0.00192;
///GTRANS - atmospheric transmissivity for global radiation, default SOLRAD/I0HDAY
static const double GTRANS = 0.55;
///LF  heat of fusion of water, MJ m-2 mm-1
static const double LF = 0.335;
///LS  latent heat of sublimation of snow, MJ m-2 mm-1
static const double LS = 2.824;
///RHOWG  density of water times gravity acceleration, MPa/m or kPa/mm
static const double RHOWG = 0.00981;
///SIGMA  Stefan-Boltzmann constant, W m-2 K-4)
static const double SIGMA = 0.0000000567;
///SC  solar constant, value from Lean (1991), W/m2
static const double SC = 1367;
///K  vonKarman constant
static const double K = 0.4;
///PI  pi
static const double PI = 3.1416;

double ShuttleworthWallace::RSSa=500.;
double ShuttleworthWallace::RSSb=1.0;
double ShuttleworthWallace::RSSa_pot=-3.22;

/************************************************************************/
/* PET.BAS                                                               */
/************************************************************************/

/// canopy parameters
/// input
///    DOY    day of year (first day of DFILE and run)"
///    MAXHT   maximum height for the year, m, minimum of 0.01 m
///    RELHT() ten pairs of DOY and relative canopy height
///    MAXLAI  maximum projected leaf area index for the year, m2/m2
///    RELLAI()ten pairs of DOY and relative LAI
///    SNOW    water equivalent of snow on the ground, mm
///    SNODEN  snow density, mm/mm
///    MXRTLN  maximum root length per unit land area, m/m2
///    MXKPL   maximum plant conductivity, (mm/d)/MPa
///    CS      ratio of projected SAI to canopy height, m-1
///    DENSEF  density factor
/// output
///    HEIGHT  canopy height above any snow, m, minimum of 0.01 m
///    LAI     leaf area index, m2/m2, minimum of 0.00001
///    SAI     stem area index, m2/m2
///    RTLEN   root length per unit land area, m/m2
///    RPLANT  plant resistivity to water flow, MPa d/mm
void CANOPY(double vegHEIGHT, double vegLAI,           // in-out parameters
			double SNOW,double SNODEN,					           // input parameters
			double vegRTLEN,double vegKPL,double CS, double DENSEF,   // input parameters
			double& HEIGHT, double& LAI, double& SAI, // output parameters
			double& RTLEN, double& RPLANT) // output parameters
{                           

	double SNODEP  = 0.001 * SNOW/SNODEN; //snow depth
	double HNOSNO  = std::max(0.01, vegHEIGHT); //height of canopy without snow
	double HSNO    = std::max(0.,HNOSNO - SNODEP); //height of canopy above snow
	double RATIO   = HSNO / HNOSNO; //fraction of canopy above snow
	LAI            = RATIO * DENSEF * vegLAI;
	SAI            = DENSEF * CS * vegHEIGHT;
	HEIGHT         = std::max(0.01,HSNO);
	if (LAI < 0.00001 ) LAI = 0.00001;

	RTLEN          = DENSEF * vegRTLEN;
	double KPL     = DENSEF * vegKPL; //plant conductivity, mm d-1 MPa-1
	if (KPL < 0.00000001) KPL = 0.00000001;
	RPLANT = 1/KPL;
}

/// calculates saturated vp and DELTA from temp
/// from Murray J Applied Meteorol 6:203
/// input
///    TA      air temperature, degC
/// output
///    ES      saturated vapor pressure at TA, kPa
///    DELTA   dES/dTA at TA, kPa/K
void ESAT(double TA, double& ES,double& DELTA) 
{
	ES = 0.61078 * exp(17.26939 * TA / (TA + 237.3));
	DELTA = 4098 * ES / sqr(TA + 237.3);
	if (TA < 0) {
		ES = 0.61078 * exp(21.87456 * TA / (TA + 265.5));
		DELTA = 5808 * ES / sqr(TA + 265.5);
	}
}

/// soil surface resistance to evaporation
/// input
///   RSSA      soil evaporation resistance at field capacity, s/m
///   RSSB      exponent in relation of soil evap res to water potential
///   PSIF      water potential at field capacity, kPa
///   PSIM      water potential of evaporating layer. kPa
/// output
///   FRSS      Shuttleworth-Wallace soil surface resistance, s/m
double FRSS(double RSSA,double RSSB, double PSIF, double PSIM) 
{
	if (RSSB <= 0.0001)
		return 10000000000.0;
	else
		return RSSA * pow(PSIM / PSIF,RSSB);
}
/// Penman-Monteith transpiration rate equation
/// input
///    AA      net energy input, Rn - S, W/m2
///    VPD      vapor pressure deficit, kPa
///    DELTA   dEsat/dTair, kPa/K
///    RA      boundary layer resistance, s/m
///    RC      canopy resistance, s/m
/// output
///    PM      Penman-Monteith latent heat flux density, W/m2
/// 
double PM(double AA, double VPD, double DELTA, double RA, double RC)
{
	return (RA * DELTA * AA + CPRHO * VPD) / ((DELTA + GAMMA) * RA + GAMMA * RC);
}

/// closed canopy parameters
/// input
///    HEIGHT  canopy height, m, minimum of 0.01 m
///    ZMINH   ZA minus HEIGHT, reference height above canopy top, m
///    LAI     leaf area index, m2/m2, minimum of 0.00001
///    SAI     stem area index, m2/m2
///    CZS     ratio of roughness to height for smooth closed canopies
///    CZR     ratio of roughness to height for rough closed canopies
///    HS      height below which CZS applies, m
///    HR      height above which CZR applies, m
///    LPC     minimum LAI defining a closed canopy
///    CS      ratio of projected SAI to canopy height, m-1
/// input and output
///    Z0GS     roughness parameter of soil surface, m
/// output
///    Z0C     roughness length for closed canopy, m
///    DISPC   zero-plane displacement for closed canopy, m
///    Z0      roughness parameter, m
///    DISP    zero-plane displacement, m
///    ZA      reference height for TA, EA, UA, above ground, m
void ROUGH(double HEIGHT, double ZMINH, double LAI, double SAI, double CZS, double CZR, double HS, double HR, double LPC, double CS, 
		   double& Z0GS, double& Z0C, double& DISPC, double& Z0, double& DISP, double& ZA)
{
	if (HEIGHT >= HR) 
		Z0C = CZR * HEIGHT;
	else if (HEIGHT <= HS)
		Z0C = CZS * HEIGHT;
	else
		Z0C = CZS * HS + (CZR * HR - CZS * HS) * (HEIGHT - HS) / (HR - HS);
	DISPC = HEIGHT - Z0C / 0.3;
	if (Z0GS > Z0C) Z0GS = Z0C;
	double RATIO = (LAI + SAI) / (LPC + CS * HEIGHT);
	if (RATIO >= 1.) {
		//  closed canopy
		Z0 = Z0C;
		DISP = DISPC;
	} else {
		//  sparse canopy modified from Shuttleworth and Gurney (1990)
		double XX = RATIO * pow(-1. + exp(0.909 - 3.03 * Z0C / HEIGHT),4);
		DISP = 1.1 * HEIGHT * log(1. + pow(XX ,0.25));
		Z0 = std::min(0.3 * (HEIGHT - DISP), Z0GS + 0.3 * HEIGHT * sqrt(XX));
	}
	ZA = HEIGHT + ZMINH;
}

///canopy surface resistance, RSC, after Shuttleworth and Gurney (1990) and
///   Stewart (1988)
///input
///   RAD     solar radiation on canopy, W/m2
///   TA      mean  temperature for the day at reference height, degC
///   VPD     vapor pressure deficit, kPa
///   LAI     projected leaf area index
///   SAI     projected stem area index
///   GLMIN   minimum leaf conductance, closed stomates, all sides, s/m
///   GLMAX   maximum leaf conductance, open stomates, all sides, s/m
///   R5      solar radiation at which conductance is halved, W/m2
///   CVPD    vpd at which leaf conductance is halved, kPa
///   RM      maximum solar radiation, at which FR = 1, W/m2
///   CR      light extinction coefficient for LAI, projected area
///   TL      temperature below which stomates are closed, degC
///   T1      lowest temp. at which stomates not temp. limited, degC
///   T2      highest temp. at which stomates not temp. limited,degC
///   TH      temperature above which stomates are closed, degC
///output
///   RSC     canopy surface resistance, s/m
void SRSC(double RAD, double TA, double VPD, double LAI, double SAI, 
		  double GLMIN, double GLMAX, double R5, double CVPD, double RM, double CR, 
		  double TL, double T1, double T2, double TH, double & RSC)
{

	// solar radiation limitation integrated down through canopy
	// Stewart (1988) and Saugier and Katerji (1991)
	double FRINT = 0;
	double FS = (2. * LAI + SAI) / (2. * LAI);
	if (RAD <= 0.0000000001)
		FRINT = 0;
	else {
		double R0 = RM * R5 / (RM - 2. * R5);
		FRINT = ((RM + R0) / (RM * CR * FS)) * log((R0 + CR * RAD) / (R0 + CR * RAD * exp(-CR * FS * LAI)));
	}
	// vapor deficit limitation
	// Lohammar et al. (1980) and Stannard (1993)
	double FD = 1. / (1. + VPD / CVPD);
	double FT = 0.;
	// temperature limitation
	if (TA <= TL) 
		FT = 0.;
	else if (TA > TL && TA < T1) 
		FT = 1. - sqr((T1 - TA) / (T1 - TL));
	else if (TA >= T1 && TA <= T2)
		FT = 1.;
	else if (TA > T2 && TA < TH) 
		FT = 1. - sqr((TA - T2) / (TH - T2));
	else
		FT = 0;
	double GSC = FD * FT * FRINT * (GLMAX - GLMIN) + LAI * GLMIN;
	RSC = 1. / GSC;
}

///Shuttleworth and Wallace (1985) ground evaporation when transpiration known
///input
///   AA      net radiation at canopy top minus ground flux, W/m2
///   ASUBS   net radiation minus ground flux at ground, W/m2
///   VPD     vapor pressure deficit, kPa
///   RAA     boundary layer resistance, s/m
///   RAS     ground-air resitance, s/m
///   RSS     ground evaporation resistance, s/m
///   DELTA   dEsat/dTair, kPa/K
///   ARATE   actual transpiration rate, mm/d
///output
///   ERATE   ground evaporation rate, mm/d
void SWGE(double AA, double ASUBS, double VPD, double RAA, double RAS, double RSS, double DELTA, double ARATE, double& ERATE)
{
	double RS = (DELTA + GAMMA) * RAS + GAMMA * RSS; 
	double RA = (DELTA + GAMMA) * RAA;   
	// total latent heat flux density, W/m2
	double LEC = ARATE / (ETOM * WTOMJ); 
	//as in Shuttleworth and Wallace (1985)
	// actual transpiration latent heat flux density, W/m2
	double LE = (RS / (RS + RA)) * LEC + (CPRHO * VPD + DELTA * RAS * ASUBS + DELTA * RAA * AA) / (RS + RA);

	ERATE = ETOM * WTOMJ * (LE - LEC);
}


///atmospheric resistances RAA, RAC, and RAS
///from Shuttleworth and Gurney (1990)
///input
///   UA      wind speed at reference height, m/s
///   ZA      reference height, m
///   HEIGHT  canopy height, m
///   Z0      roughness parameter, m
///   DISP    zero-plane displacement, m
///   Z0C     roughness length for closed canopy, m
///   DISPC   zero-plane displacement for closed canopy, m
///   Z0GS    roughness parameter of soil surface, m
///   LWIDTH  characteristic leaf width, m
///   RHOTP   ratio of total leaf area to projected leaf area
///   NN      wind/diffusivity extinction coefficient
///   LAI     projected leaf area index
///   SAI     projected stem area index
///output
///   RAA     boundary layer resistance, s/m
///   RAC     leaf-air resistance, s/m
///   RAS     ground-air resitance, s/m
void SWGRA(double UA, double ZA, double HEIGHT, double Z0, double DISP, double Z0C, double DISPC, double Z0GS, double LWIDTH, double RHOTP, 
		   double NN, double LAI, double SAI, double& RAA, double& RAC, double& RAS)
{
	double USTAR = K * UA / (log((ZA - DISP) / Z0));
	double KH = K * USTAR * (HEIGHT - DISP);
	RAS = (HEIGHT * exp(NN) / (NN * KH)) * (exp(-NN * Z0GS / HEIGHT) - exp(-NN * (Z0C + DISPC) / HEIGHT));
	if (RAS < 1.) RAS = 1.;

	RAA = log((ZA - DISP) / (HEIGHT - DISP)) / (K * USTAR) + (HEIGHT / (NN * KH)) * (-1. + exp(NN * (HEIGHT - DISPC - Z0C) / HEIGHT));

	double UH = (USTAR / K) * log((HEIGHT - DISP) / Z0);
	// the Shuttleworth-Gurney RB equation is strictly for one side of flat leaves
	// when RHOTP > 2, LWIDTH is small (needles) so RAC is small
	// their equation should have NN in numerator, see Choudhury and Monteith(1988)
	double RB = (100. * NN) * sqrt(LWIDTH / UH) / (1. - exp(-NN / 2.));
	RAC = RB / (RHOTP * LAI + PI * SAI);
	//note LAI is prevented from being less than 1E-5
}

///Shuttleworth and Wallace (1985) transpiration and ground evaporation
///input
///   AA      net radiation at canopy top minus ground flux, W/m2
///   ASUBS   net radiation minus ground flux at ground, W/m2
///   VPD     vapor pressure deficit, kPa
///   RAA     boundary layer resistance, s/m
///   RAC     leaf-air resistance, s/m
///   RAS     ground-air resistance, s/m
///   RSC     canopy surface resistance, s/m
///   RSS     ground evaporation resistance, s/m
///   DELTA   dEsat/dTair, kPa/K
///output
///   PRATE   potential transpiration rate, mm/d
///   ERATE   ground evaporation rate, mm/d
void SWPE(double AA, double ASUBS, double VPD, double RAA, 
		  double RAC, double RAS, double RSC, double RSS, 
		  double DELTA, double& PRATE, double& ERATE)
{
	double RS = (DELTA + GAMMA) * RAS + GAMMA * RSS;
	double RC = (DELTA + GAMMA) * RAC + GAMMA * RSC;
	double RA = (DELTA + GAMMA) * RAA;
	double CCS = 1. / (1. + RS * RA / (RC * (RS + RA)));
	double CCC = 1. / (1. + RC * RA / (RS * (RC + RA)));
	double PMS = PM(AA, VPD - DELTA * RAS * (AA - ASUBS) / CPRHO, DELTA, RAA + RAS, RSS);
	double PMC = PM(AA, VPD - DELTA * RAC * ASUBS / CPRHO, DELTA, RAA + RAC, RSC);
	double LE = (CCC * PMC + CCS * PMS);
	double D0 = VPD + RAA * (DELTA * AA - (DELTA + GAMMA) * LE) / CPRHO;
	PRATE = ETOM * WTOMJ * PM(AA - ASUBS, D0, DELTA, RAC, RSC);
	ERATE = ETOM * WTOMJ * PM(ASUBS, D0, DELTA, RAS, RSS);
}

///ratio of wind speed at reference height (above canopy) to
///   wind speed at weather station
///input
///  ZA      reference height, m
///  DISP    height of zero-plane, m
///  Z0      roughness parameter, m
///  FETCH   weather station fetch, m
///  ZW      weather station measurement height for wind,above any zero plane, m
///  Z0W     weather station roughness parameter, m
///output
///  WNDADJ  ratio
double WNDADJ(double ZA, double DISP, double Z0, double FETCH, double ZW, double Z0W)
{
	if (Z0W < 0.000001) {
		return 1.;
	} else {
		///  Brutsaert (1982) equation 7-39
		double HIBL = 0.334 * pow(FETCH, 0.875) * pow(Z0W,0.125);
		///  Brutsaert equations 7-41 and 4-3
		return log(HIBL / Z0W) * log((ZA - DISP) / Z0) / (log(HIBL / Z0) * log(ZW / Z0W));
	}
}


///input
///   TMAX       maximum temperature for the day, øC
///   TMIN       minimum temperature for the day, øC
///   DAYLEN     daylength in fraction of day
///   I0HDAY     potential insolation on horizontal, MJ m-2 d-1
///   EA         vapor pressure for the day, kPa
///   UW         average wind speed for day at weather station, m/s
///   ZA         reference height for TA, EA, UALOCAL, above ground, m
///   DISP       zero-plane displacement, m
///   Z0         roughness parameter, m
///   WNDRAT     ratio of nighttime to daytime wind speed
///   FETCH      weather station fetch, m
///   Z0W        weather station roughness parameter, m
///   ZW         weather station measurement height for wind, m
///   SOLRAD     solar radiation for the day, horizontal surface, MJ/m2
///output
///   SOLRADC    corrected solar radiation for the day, horizontal surface, MJ/m2
///   TA         mean temperature for the day, øC
///   TADTM      average daytime temperature, øC
///   TANTM      average nighttime temperature, øC
///   UA         average wind speed for the day at reference height, m/s
///   UADTM      average wind speed for daytime at ZA, m/s
///   UANTM      average wind speed for nighttime at ZA, m/s
void WEATHER(double TMAX, double TMIN, double DAYLEN, double I0HDAY, double EA, double UW, double ZA, double DISP, double Z0, 
			double WNDRAT, double FETCH, double Z0W, double ZW, double SOLRAD, 
			double& SOLRADC, double& TA, double& TADTM, double& TANTM, double& UA, double& UADTM, double& UANTM)
{
	//estimate SOLRAD if missing or limit if too high
	if (SOLRAD < 0.001) {
		SOLRADC = GTRANS * I0HDAY;
	} else if (SOLRAD > I0HDAY) {
		SOLRADC = 0.99 * I0HDAY;
	} else {
		SOLRADC = SOLRAD;
	}
	///average temperature for day
	TA = (TMAX + TMIN) / 2.;
	///daytime and nighttime average air temperature
	TADTM = TA + ((TMAX - TMIN) / (2. * PI * DAYLEN)) * sin(PI * DAYLEN);
	TANTM = TA - ((TMAX - TMIN) / (2. * PI * (1. - DAYLEN))) * sin(PI * DAYLEN);
	///if no vapor pressure data, use saturated vapor pressure at minimum temp.
	double dummy=0;
	if (EA == 0.) ESAT(TMIN, EA, dummy);
	///if no wind data, use 3 m/s
	if (UW == 0.) UW = 3.;
	///if wind < 0.2 m/s, set to 0.2 to prevent zero divide
	if (UW < 0.2) UW = 0.2;
	///adjust wind speed from weather station to ZA
	UA = UW * WNDADJ(ZA, DISP, Z0, FETCH, ZW, Z0W);
	///daytime and nighttime average wind speed
	UADTM = UA / (DAYLEN + (1. - DAYLEN) * WNDRAT);
	UANTM = WNDRAT * UADTM;
}

/************************************************************************/
/* EVP.BAS                                                               */
/************************************************************************/

///rain interception, used when NPINT > 1
///same routine is used for snow interception, with different calling variables
///input
///   RFAL      rainfall rate, mm/d
///   PINT      potential interception rate, mm/d
///   LAI       projected leaf area index, m2/m2
///   SAI       projected stem area index, m2/m2
///   FRINTL    intercepted fraction of RFAL per unit LAI
///   FRINTS    intercepted fraction of RFAL per unit SAI
///   CINTRL    maximum interception storage of rain per unit LAI, mm
///   CINTRS    maximum interception storage of rain per unit SAI, mm
///   DTP       precipitation interval time step, d
///   INTR      intercepted rain, mm
///output
///   RINT      rain catch rate, mm/d
///   IRVP      evaporation rate of intercepted rain, mm/d
void INTER(double RFAL, double PINT, double LAI, double SAI, double FRINTL, double FRINTS, double CINTRL, double CINTRS, double DTP, double INTR, double& RINT, double& IRVP)
{
	double CATCH = (FRINTL * LAI + FRINTS * SAI) * RFAL;
	double INTRMX = CINTRL * LAI + CINTRS * SAI;
	double NEWINT = INTR + (CATCH - PINT) * DTP;
	if (NEWINT > 0.) {
		///  canopy is wet throughout DTP
		IRVP = PINT;
		if (NEWINT > INTRMX) {
			///     canopy capacity is reached
			RINT = PINT + (INTRMX - INTR) / DTP;
			///     RINT can be negative if INTR exists and LAI or SAI is decreasing over time
		} else {
			///     canopy capacity is not reached
			RINT = CATCH;
		}
	} else {
		///  canopy dries during interval or stays dry
		RINT = CATCH;
		IRVP = (INTR / DTP) + CATCH;
		///  IRVP is < PINT
	}
}

///rain interception with duration in hours, used when NPINT = 1
///same routine is used for snow interception, with different calling variables
///input
///   RFAL      24-hour average rainfall rate, mm/d
///   PINT      potential interception rate, mm/d
///   LAI       projected leaf area index, m2/m2
///   SAI       projected stem area index, m2/m2
///   FRINTL    intercepted fraction of RFAL per unit LAI
///   FRINTS    intercepted fraction of RFAL per unit SAI
///   CINTRL    maximum interception storage of rain per unit LAI, mm
///   CINTRS    maximum interception storage of rain per unit SAI, mm
///   DURATN    average storm duration, hr
///   INTR      intercepted rain storage, mm,
///output
///   RINT      rain catch rate, mm/d
///   IRVP      evaporation rate of intercepted rain, mm/d
void INTER24(double RFAL, double PINT, double LAI, double SAI, double FRINTL, double FRINTS, double CINTRL, double CINTRS, double DURATN, double INTR, double & RINT, double & IRVP)
{
	int IHD = int((DURATN + 0.1) / 2);
	double INTRMX = CINTRL * LAI + CINTRS * SAI; //maximum canopy storage for rain, mm
	double INTRNU = INTR;                        //canopy storage at end of hour, mm
	double SMINT = 0.; //daily accumulated actual catch, mm
	double SMVP = 0.;  //daily accumulated actual evaporation, mm
	double DTH = 1.;  // time step, = 1 hr
	double CATCH = 0.; // maximum RINTHR, mm/hr
	for (int I = 0; I < 24 ; ++I)	{
		if (I < (12 - IHD) || I >= (12 + IHD)) {
			///     before or after rain
			CATCH = 0.;
		} else {
			///     during rain, mm/hr is rate in mm/d divided by hr of rain/d
			CATCH = (FRINTL * LAI + FRINTS * SAI) * RFAL / (2. * IHD);
		}
		double NEWINT = INTRNU + (CATCH - PINT / 24.) * DTH; // first approximation to INTRNU, mm
		double RINTHR = 0; // rain catch rate for hour, mm/hr
		double IRVPHR = 0; // evaporation rate for hour, mm/hr
		if (NEWINT > 0.0001) {
			///     canopy is wet throughout hour, evap rate is PINT
			IRVPHR = PINT / 24.; 
			if (NEWINT > INTRMX) {
				///        canopy capacity is reached
				RINTHR = IRVPHR + (INTRMX - INTRNU) / DTH;
				///        INTRMX - INTRNU can be negative if LAI or SAI is decreasing over time
			} else {
				///        canopy capacity is not reached
				RINTHR = CATCH;
			}
		} else {
			///     canopy dries during hour or stays dry
			RINTHR = CATCH;
			IRVPHR = INTRNU / DTH + CATCH;
			///     IRVPHR for hour is < PI/24
		}
		INTRNU = INTRNU + (RINTHR - IRVPHR) * DTH;
		SMVP = SMVP + IRVPHR * DTH;
		SMINT = SMINT + RINTHR * DTH;
	}
	IRVP = SMVP;
	///           / 1 d
	RINT = SMINT;
	///            / 1 d
}

///allocates total plant resistance to xylem and root layers
///input
///DIM NLAYER AS INTEGER /// number of soil layers (max 50)
///   THICK() layer thicknesses, mm
///   STONEF()stone volume fraction, unitless
///   RTLEN   root length per unit land area, m/m2, MXRTLN * RELHT * DENSEF
///   RELDEN()relative values of root length per unit volume
///   RTRAD   average root radius, mm
///   RPLANT  plant resistance to water flow, MPa d/mm, 1/(KPLANT*RELHT*DENSEF)
///   FXYLEM  fraction of plant resistance in xylem
///output
///   RXYLEM  xylem resistance, MPa d/mm, 1E20 if no roots
///   RROOTI()root resistance for layer, MPa d/mm, 1E20 if no roots
///   ALPHA() modified Cowan alpha, MPa
///local
void PLNTRES(ptrdiff_t NLAYER, const num_array& THICK, const num_array& STONEF, double RTLEN, const num_array& RELDEN, double RTRAD, double RPLANT, 
			 double FXYLEM, double & RXYLEM, num_array& RROOTI, num_array& ALPHA)
{
	double SUM;     //total relative length, mm
	double RTFRAC;  //fraction of total root length in layer
	double RTDENI;  //root density for layer, mm/mm3
	double DELT;    //root cross-sectional area * LI, dimensionless

	///xylem resistance
	RXYLEM = FXYLEM * RPLANT;
	SUM = 0.;
	for (ptrdiff_t I = 0; I < NLAYER ; ++I)
	{
		SUM = SUM + RELDEN[I] * THICK[I] * (1. - STONEF[I]);
	}
	for (ptrdiff_t I = 0; I < NLAYER ; ++I)
	{
		if (RELDEN[I] < 0.00001 || RTLEN < 0.1) {
			///     no roots in layer
			RROOTI[I] = 1E+20;
			ALPHA[I] = 1E+20;
		} else {
			RTFRAC = RELDEN[I] * THICK[I] * (1. - STONEF[I]) / SUM;
			///     root resistance for layer
			RROOTI[I] = (RPLANT - RXYLEM) / RTFRAC;
			///     rhizosphere resistance for layer
			RTDENI = RTFRAC * 0.001 * RTLEN / (THICK[I] * (1. - STONEF[I]));
			///                       .001 is (mm/mm2)/(m/m2) conversion
			DELT = PI * sqr(RTRAD) * RTDENI;
			ALPHA[I] = (1. / (8. * PI * RTDENI)) * (DELT - 3. - 2. * (log(DELT)) / (1. - DELT));
			ALPHA[I] = ALPHA[I] * 0.001 * RHOWG / (THICK[I] * (1. - STONEF[I]));
			///                           .001 is MPa/kPa conversion
		}
	}
}

///  actual transpiration rate by layers
///  watch MPa - kPa conversions carefully
///input
///   J             1 for daytime, 2 for nighttime
///   PTR            average potential transpiration rate over time period, mm/d
///   DISPC          zero-plane displacement for closed canopy, m
///   ALPHA()        modified Cowan alpha, MPa
///   KK()           hydraulic conductivity, mm/d
///   RROOTI()       root resistance for layer, MPa d/mm
///   RXYLEM         xylem resistance, MPa d/mm
///   PSITI()        total soil water potential, kPa
///   NLAYER        number of soil layers (max 20)
///   PSICR          critical potential for plant, MPa
///   NOOUTF        1 if no outflow allowed from roots, otherwise 0
///output
///   ATR            actual transpiration rate over time period, mm/d
///   ATRANI()       actual transpiration rate from layer over time period, mm/d

void TBYLAYER(ptrdiff_t J,double PTR, double DISPC, 
			  const num_array& ALPHA, const num_array& KK, 
			  const num_array& RROOTI, double RXYLEM, 
			  const num_array& PSITI,ptrdiff_t NLAYER, double PSICR, 
			  ptrdiff_t NOOUTF, 
			  double& ATR,num_array& ATRANI)
{
	if (ATRANI.size() != NLAYER) throw std::runtime_error("Shuttleworth Wallace: Not correctly initialized ATR array");
	num_array RI(RROOTI.size());   ///root plus rhizosphere resistance, MPa d/mm
	double RT;             ///combined root resistance from unflagged layers, MPa d/mm
	double SUM;            ///sum of layer conductances, (mm/d)/MPa
	double TRMIN;          ///largest negative transpiration loss, mm/d
	double PSIT;           ///weighted average total soil water potential for unflagged layers, kPa
	double R;              ///(2/pi)(SUPPLY/PTR)
	double SUPPLY;         ///soil water supply rate, mm/d
	ptrdiff_t IDEL;          ///subscript of flagged layer
	std::vector<bool>FLAG(RROOTI.size()); ///1 if layer has no transpiration uptake, otherwise 0
	ptrdiff_t NEGFLAG;       ///1 if second iteration is needed
	
	///flag layers with no roots, indicated by RROOTI = 1E20
	///if outflow from roots is prevented, flag layers with PSITI <= PSICR
	for (ptrdiff_t I = 0; I < NLAYER ; ++I)
	{
		if (RROOTI[I] > 1E+15) {
			FLAG[I] = 1;
		} else if (NOOUTF == 1 && PSITI[I] / 1000. <= PSICR) {
			FLAG[I] = 1;
		} else {
			FLAG[I] = 0;
		}
	}

	///top of loop for recalculation of transpiration if more layers get flagged
	do {
		NEGFLAG = 0;
		SUM = 0.;
		for (ptrdiff_t I = 0; I < NLAYER ; ++I) {
			if (FLAG[I] == 0) {
				RI[I] = RROOTI[I] + ALPHA[I] / KK[I];
				SUM = SUM + 1. / RI[I];
			} else {
				///        flagged
				ATRANI[I] = 0.;
			}
		}
		if (SUM < 1E-20) {
			///     all layers flagged, no transpiration
			ATR = 0.;
			PSIT = -10000000000.; 
			break;
		} else {
			RT = 1. / SUM;
		}
		///  weighted mean soil water potential
		PSIT = 0.;
		for (ptrdiff_t I = 0; I < NLAYER ; ++I) {
			if (FLAG[I] == 0) {
				PSIT = PSIT + RT * PSITI[I] / RI[I];
			}
		}
		///  soil water supply rate, assumed constant over day
		SUPPLY = (PSIT / 1000. - PSICR - RHOWG * DISPC) / (RT + RXYLEM);
		///  transpiration rate limited by either PTR or SUPPLY
		if (J == 1) {
			///     daytime, PTR is average of a half sine over daytime
			R = (2. / PI) * (SUPPLY / PTR);
			if (R <= 0.) {
				ATR = 0.;
			} else if (R < 1.) {
				ATR = PTR * (1. + R * acos(R) - sin(acos(R)));
			} else {
				ATR = PTR;
			}
		} else {
			///     nighttime, PTR assumed constant over nighttime
			if (SUPPLY <= 0. || PTR <= 0.) {
				ATR = 0.;
			} else {
				ATR = std::min(SUPPLY, PTR);
			}
		}
		///  distribute total transpiration rate to layers
		for (ptrdiff_t I = 0; I < NLAYER ; ++I)	{
			if (FLAG[I] ==1) {
				ATRANI[I] = 0.;
			} else {
				ATRANI[I] = ((PSITI[I] - PSIT) / 1000. + RT * ATR) / RI[I];
				///        check for any negative transpiration losses
				if (ATRANI[I] < 0)  NEGFLAG = 1;
			}
		}
		if (NOOUTF == 1 && NEGFLAG == 1) {
			///     find layer with most negative transpiration and omit it
			IDEL = 0;
			TRMIN = 0.;
			for (ptrdiff_t I = 0; I < NLAYER ; ++I)
			{
				if (ATRANI[I] < TRMIN) {
					TRMIN = ATRANI[I];
					IDEL = I;
				}
			}
			FLAG[IDEL] = 1;
			///     repeat main loop with flagged layers excluded
		} else {
			///     done
			break;
		}
	} while(true);
}
///snow evaporation and condensation
///input
///   DISP              zero-plane displacement, m
///   DISPC             zero-plane displacement for closed canopy of HEIGHT, m
///   EA                vapor pressure for the day, kPa
///   HEIGHT            canopy height, m
///   KSNVP             multiplier to fix snow evaporation problem
///   LAI               leaf area index, m2/m2
///   LWIDTH            leaf width, m
///   NN                wind/diffusivity extinction coefficient
///   RHOTP             ratio of total leaf area to projected area
///   SAI               stem area index, m2/m2
///   TA                mean  temperature for the day at reference height, øC
///   TSNOW             snowpack temperature (isothermal assumed), øC
///   UA                average wind speed for the day at reference height, m/s
///   Z0                roughness parameter, m
///   Z0C               roughness parameter for closed canopy of HEIGHT, m
///   Z0GS              snow surface roughness, m
///   ZA                reference height for TA, EA, UA, above ground, m
///output
///   PSNVP             potential snow evaporation, mm/d
void SNOVAP (double TSNOW, double TA, double EA, double UA, double ZA, double HEIGHT, double Z0, double DISP, 
			 double Z0C, double DISPC, double Z0GS, double LWIDTH, double RHOTP, 
			 double NN, double LAI, double SAI, double KSNVP, double & PSNVP)
{
	double ESNOW; ///  vapor pressure at snow surface, kPa
	double RAA;   ///  Shuttleworth-Wallace atmosphere aerodynamic resistance,s/m
	double RAS;   ///  Shuttleworth-Wallace ground aerodynamic resistance, s/m
	double RAC;   ///  Shuttleworth-Wallace canopy aerodynamic resistance, s/m
	double DELTA;
	///  ignores effect of interception on PSNVP or of PSNVP on PTRAN
	if (TSNOW > -.1) 
		ESNOW = .61;
	else
		///     snow surface vapor pressure saturated at lower of TA and TSNOW
		ESAT(std::min(TA, TSNOW), ESNOW, DELTA);

	SWGRA(UA, ZA, HEIGHT, Z0, DISP, Z0C, DISPC, Z0GS, LWIDTH, RHOTP, NN, LAI, SAI, RAA, RAC, RAS);
	PSNVP = (WTOMJ / LS) * (CPRHO / GAMMA) * (ESNOW - EA) / (RAA + RAS);
	///  fix for PSNVP overestimate
	PSNVP = KSNVP * PSNVP;
}
cmf::upslope::ET::ShuttleworthWallace::ShuttleworthWallace( cmf::upslope::Cell& _cell, bool _allow_dew) 
:	PTR(0.0), ATR_sum(0.0), ATR(), GER(0.0),PIR(0.0),GIR(0.0), PSNVP(0.0), ASNVP(0.0), cell(_cell),KSNVP(1.0),AIR(0.0),
	RAA(0.0),RAC(0.0),RAS(0.0),RSS(0.0),RSC(0.0), refresh_counter(0), allow_dew(_allow_dew)
{
	KSNVP = piecewise_linear(cell.vegetation.Height,1,5,1.0,0.3);
}

void cmf::upslope::ET::ShuttleworthWallace::refresh( cmf::math::Time t )
{
	if (t == refresh_time) return;
	refresh_time=t;
	++refresh_counter;
	v = cell.vegetation;
	w = cell.get_weather(t);


	const ptrdiff_t lc = cell.layer_count(); 
	const double GLMIN = 1e-6; // min stomatal conductivity (night times)
	const double GLMAX = 1. / v.StomatalResistance; // max stomatal conductivity m/s
	
	// wind/diffusivity extinction coefficient, dimensionless. NN is the canopy extinction coefficient for wind and eddy diffusivity. 
	// NN is fixed at 2.5 following Shuttleworth and Gurney (1990). Federer and others (1995) show that PE is insensitive to the 
	// value of n, but they did not test sparse canopies in a wet climate.
	const double NN = 2.5; 
	const double RHOTP = 2.; // Ratio of total leaf area and projected leaf area
	///   R5      solar radiation at which conductance is halved, W/m2
	///   CVPD    vpd at which leaf conductance is halved, kPa
	///   RM      maximum solar radiation, at which FR = 1, W/m2
	///   CR      light extinction coefficient for LAI, projected area
	///   TL      temperature below which stomates are closed, degC
	///   T1      lowest temp. at which stomates not temp. limited, degC
	///   T2      highest temp. at which stomates not temp. limited,degC
	///   TH      temperature above which stomates are closed, degC

	const double R5 = 100.; // W/m2, radiation where half of the transpiration occurs
	const double RM = 1000.; // W/m2, radiation where stomata are open for transpiration
	const double CVPD = 2.; //  vpd at which leaf conductance is halved, kPa
	const double T_f[] = {0,10,30,40}; // Fuzzy number describing temperature influence on leaf conductance


	
	// get snow package in mm
	double snow = cell.get_snow() ? cell.get_snow()->get_volume() * 1e3 / cell.get_area() : 0.0;

	// Just some temporary variables to be set by CANOPY
	double h,LAI,SAI,RTLEN,RPLANT;
	// Get Canopy parameters
	CANOPY(v.Height,v.LAI,snow,0.3 /* SNODEN*/,v.RootLength(),8./*vegKPL*/,0.035 /*CS*/,v.CanopyClosure,
	       h,LAI,SAI,RTLEN,RPLANT);
	
	// Roughness parameters
	double Z0GS = 0.001 * cell.snow_coverage() + 0.05 * (1-cell.snow_coverage()),
		Z0C,DISPC,Z0,DISP,ZA;
	ROUGH(h,w.instrument_height,LAI,SAI,0.13,0.05,1,10,4.0,0.035,Z0GS,Z0C,DISPC,Z0,DISP,ZA);
	
	// Get plant resistance
	double RXYLEM;
	num_array RROOTI(lc), ALPHA(lc);
	num_array THICK(cell.get_layers().get_thickness());
	num_array ROOTF(cell.get_layers().get_rootfraction());
	num_array STONEF(lc);
	num_array RELDEN(lc);
	num_array KK = cell.get_layers().get_K();
	KK *= 1000.; // convert m/day -> mm/day
	num_array PSITI = cell.get_layers().get_potential();
	PSITI -= cell.z; 
	PSITI *= RHOWG * 1e3; // convert m->kPa

	ptrdiff_t i=0;
	PLNTRES(lc,THICK,STONEF,v.RootLength(),ROOTF,3.5,RPLANT,piecewise_linear(v.Height,0,25,0,0.5),RXYLEM,RROOTI,ALPHA);

	// if wind < 0.2 m/s, set to 0.2 to prevent zero divide (taken from WEATHER in BROOK90)
	double UW = std::max(w.Windspeed,0.2);
	
	double UA = UW * WNDADJ(ZA,DISP,Z0,5000.,10.,0.005);

	if (snow>0.01) {
		SNOVAP(w.Tground,w.T,w.e_a,UA,ZA,h,Z0,DISP,Z0C,DISPC,Z0GS,v.LeafWidth,RHOTP,NN,LAI,SAI,KSNVP,PSNVP);
		if (!allow_dew) {
			PSNVP = maximum(PSNVP,0.0);
		}
	}
	ASNVP = PSNVP * piecewise_linear(snow,0,1);

	double albedo = cell.snow_coverage() * 0.9 + (1-cell.snow_coverage()) * v.albedo;
	double RN = w.Rn(albedo) / WTOMJ;
	double RNground = RN * exp(-v.CanopyPARExtinction * (LAI+SAI));
	RSS = FRSS(RSSa ,RSSb, RSSa_pot, cell.get_layer(0)->get_matrix_potential());
	SWGRA(UA,ZA,h,Z0,DISP,Z0C,DISPC,Z0GS,v.LeafWidth,RHOTP,NN,LAI,SAI,RAA,RAC,RAS);
	if (w.Rs>0) {
		SRSC(w.Rs / WTOMJ, w.T, w.e_s-w.e_a, LAI, SAI, GLMIN, GLMAX,
			R5,CVPD,RM,v.CanopyPARExtinction,
			T_f[0],T_f[1],T_f[2],T_f[3],
			RSC);
	} else {
		RSC = 1 / (GLMIN * LAI);
	}
	double DELTA,ES;
	ESAT(w.T,ES,DELTA);

	SWPE(RN,RNground,w.e_s-w.e_a,RAA,RAC,RAS,RSC,RSS,DELTA,PTR,GER);
	// Shuttleworth-Wallace potential interception and ground evap. rates
	// RSC = 0, RSS not changed
	SWPE(RN,RNground,w.e_s-w.e_a,RAA,RAC,RAS,0,RSS,DELTA,PIR,GIR);

	double VOL_IN_CANOPY = cell.get_canopy() ? cell.m3_to_mm(cell.get_canopy()->get_volume()): 0.0;
	double MAX_VOL_IN_CANOPY = LAI * cell.vegetation.CanopyCapacityPerLAI;
	AIR =  piecewise_linear(VOL_IN_CANOPY,0,MAX_VOL_IN_CANOPY,0,PIR);
	PTR -= AIR;
	if (PTR>0.001)	{
		TBYLAYER(1,PTR,DISPC,ALPHA,KK,RROOTI,RXYLEM,PSITI,lc, RHOWG * pF_to_waterhead(4.2),1,ATR_sum,ATR);
		if (ATR_sum < PTR)
			SWGE(RN,RNground,w.e_s-w.e_a,RAA,RAS,RSS,DELTA,ATR_sum + AIR,GER);

	} else {
		PTR = 0.0;
		ATR = 0.0;
		ATR_sum=0.0;
		ATR = 0.0;	
		SWGE(RN,RNground,w.e_s-w.e_a,RAA,RAS,RSS,DELTA,ATR_sum + AIR,GER);
	}
	if (!allow_dew) {
		PTR = maximum(PTR,0.0);
		GER = maximum(GER,0.0);
		PIR = maximum(PIR,0.0);
		GIR = maximum(GIR,0.0);
	}

}

cmf::upslope::ET::ShuttleworthWallace* cmf::upslope::ET::ShuttleworthWallace::use_for_cell( cmf::upslope::Cell& cell )
{
	// Create a shared ptr of a Shuttlewroth Wallace semantic
	std::shared_ptr<ShuttleworthWallace> sw(new ShuttleworthWallace(cell));
	// Create a connection between cell.transpiration and each soil layer
	for(layer_list::const_iterator it = cell.get_layers().begin(); it != cell.get_layers().end(); ++it)	{
		new transpiration(*it,cell.get_transpiration(),sw,"Shuttleworth - Wallace");
	}
	// Soil evaporation from first layer
	new soil_evaporation(cell.get_layer(0),cell.get_evaporation(),sw,"Shuttleworth - Wallace");
	// If a canopy storage exists, create a connection between canopy an evaporation (interception)
	if (cell.get_canopy())
		new canopy_evaporation(cell.get_canopy(),cell.get_evaporation(),sw,"Shuttleworth - Wallace");
	// If a snow storage exists create a connection between snow and evaporation (snow sublimation)
	if (cell.get_snow())
		new snow_evaporation(cell.get_snow(),cell.get_evaporation(),sw,"Shuttleworth - Wallace");
	// if a surface water storage exist, create a connection between it and evaporation (open water transpiration)
	if (cell.get_surfacewater()->is_storage()) {
		new surface_water_evaporation(cmf::river::OpenWaterStorage::cast(cell.get_surfacewater()),cell.get_evaporation(),sw,"Shuttleworth - Wallace");
	}
	sw->ATR.resize(cell.layer_count());
	sw->ATR = 0.0;
	cell.set_aerodynamic_resistance(sw);
	return sw.get();
}

double cmf::upslope::ET::ShuttleworthWallace::evap_from_openwater( cmf::river::OpenWaterStorage::ptr ows,cmf::math::Time t )
{
	// If open water is empty return zero
	if (ows->RecalcFluxes(t)) refresh(t);
	// Account for dew (but not white frost)
	real res = 0.0;
	if (GER<0.0) {
		res += (1-cmf::upslope::connections::snowfraction(w.T)) * GER * 1e-3 * cell.get_area();
	} 
	// Get evaporation from open water, if open water is not empty
	res += GIR * ows->get_height_function().A(ows->get_volume()) * 1e-3 * (1-ows->is_empty());
	return res;
}

double cmf::upslope::ET::ShuttleworthWallace::evap_from_snow( cmf::water::WaterStorage::ptr snow,cmf::math::Time t )
{
	if (snow->RecalcFluxes(t)) refresh(t);
	real res = 0.0;
	// If GER<0 and it is cold, route rime to snow
	if (GER<0) {
		res += cmf::upslope::connections::snowfraction(w.T) * GER * 1e-3 * cell.get_area();
	}
	// If there is snow, evaporate it with ASNVP
	res += ASNVP * 1e-3 * cell.get_area() * (1-snow->is_empty());
	return res;
}

double cmf::upslope::ET::ShuttleworthWallace::evap_from_canopy( cmf::water::WaterStorage::ptr canopy,cmf::math::Time t )
{
	if (canopy->RecalcFluxes(t)) refresh(t);
	return AIR * 1e-3 * cell.get_area() * (1-canopy->is_empty());
}

double cmf::upslope::ET::ShuttleworthWallace::evap_from_layer( cmf::upslope::SoilLayer::ptr sl,cmf::math::Time t )
{
	// Evaporation only from the first layer
	if (sl->Position != 0) return 0.0;
	// Recalculate fluxes if needed	
	if (sl->RecalcFluxes(t)) refresh(t);
	// Return soil evap. for snow free area
	// neg. GER is rerouted to surfacewater (dew) or snow (rime)
	return std::max(GER,0.0) * std::max(0.0,1 - cell.snow_coverage() - cell.surface_water_coverage())   * 1e-3 * cell.get_area();
}

double cmf::upslope::ET::ShuttleworthWallace::transp_from_layer( cmf::upslope::SoilLayer::ptr sl,cmf::math::Time t )
{
	if (sl->RecalcFluxes(t)) refresh(t);
	if (sl->Position>int(ATR.size())) return 0.0;
	return ATR[sl->Position] * 1e-3 * cell.get_area();
}

void cmf::upslope::ET::ShuttleworthWallace::get_aerodynamic_resistance( double & r_ag,double & r_ac, cmf::math::Time t ) const
{
	r_ag = RAA + RAS;
	r_ac = RAA + RAC;
}













