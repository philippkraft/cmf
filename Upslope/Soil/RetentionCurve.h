#ifndef RetentionCurve_h__
#define RetentionCurve_h__
#include "../../math/real.h"
#include <vector>
#include <tr1/memory>
namespace cmf {
	namespace upslope {
		const double rho_wg=9810; // Pa/m
		/// Converts a pressure in Pa to a lenght of a water column in m
		double pressure_to_waterhead(double Pressure);
		/// Converts a height of a water column in m to a pressure in Pa
		double waterhead_to_pressure(double waterhead);
		/// Converts a pF value to a height of a water column in m
		double pF_to_waterhead(double pF);
		/// Converts a height of a water column to a pF value
		double waterhead_to_pF(double waterhead);


		/// Abstract base class for different types of retention curves
		///
		/// This class, and its children uses wetness instead of volumetric water content. The wetness of a soil is defined as water content per void volume
		class RetentionCurve {

		public:
			/// Returns the conductivity in m/day at a certain depth and water content
			virtual real K(real wetness,real depth) const = 0;
			/// Returns the effective wetness, using a residual pF value
			/// \f[w_{eff}  = \frac{w_{act}-w\left(pF_r\right)}{1-w\left(pF_r\right)}\f]
			virtual real Wetness_eff(real wetness,real pF_r=4.2) const
			{
				real w_r=Wetness_pF(pF_r);
				return (wetness-w_r)/(1-w_r);
			}
			/// Returns the porosity at a certain depth
			virtual real Porosity(real depth) const = 0;
			/// Returns the void volume of a soil column
			virtual real VoidVolume(real upperDepth,real lowerDepth,real Area) const = 0;
			/// Returns the thickness of a soil column with a certain pore volume
			virtual real FillHeight(real lowerDepth,real Area,real Volume) const =0;
			/// Returns the transmissivity of a part of a soil column
			virtual real Transmissivity(real upperDepth,real lowerDepth,real wetness) const = 0;
			/// returns the wetness (volumetric water content per pore space) at a given suction pressure
			virtual real Wetness(real suction) const = 0;
			/// returns the volumetric water content at a given pF value
			real Wetness_pF(real pF) const {return Wetness(pF_to_waterhead(pF));}
			/// returns the wetness of the soil at given water content
			virtual real MatricPotential(real wetness) const = 0;
			virtual RetentionCurve* copy() const=0;
		};
		/// Provides the use of the Brooks-Corey retention curve
		/// \f{eqnarray*}
		/// W(\theta) &=& \frac{\theta - \theta_r}{\theta_s - \theta_r} \\
		/// K(W) &=& K_{sat} W^{2+3b} \\
		/// \Psi(W) &=& \Psi_X \left(\frac{W}{W_X}\right)^{-b} \\
		/// W(\Psi) &=& {\left( \frac{\Psi_X}{\Psi}\right) }^{\frac{1}{b}}\ W_X
		/// \f}	
		/// where:
		/// - \f$K\f$ is the conductivity in \f$\frac m {day}\f$
		/// - \f$W\f$ is the wetness (Volume of soil water per volume of pores)
		/// - \f$b\f$ is the shape of the retention curve (usually between 4 (sand) and 14 (clay))
		/// - \f$\Psi(W)\f$ is the matric potential in \f$m H_2O\f$ at wetness W
		/// - \f$\Psi_X\f$ is a matric potential at a known wetness	in \f$m H_2O\f$
		/// - \f$W_X\f$ is the wetness with a known matric potential
		///	for dynamic changes with depth, exponential decays of porosity and saturated conductivity are used
		/// The decay function is: \f$ v(d)=v(0) (1+a)^{-d} \f$, where v is the value (\f$ K_{sat},\Phi\f$), d is the depth in m
		/// and a is the fractional decay per m. E.g. 0.1 means the value has in 1 m depth 90% of the value at the surface
		class BrooksCoreyRetentionCurve : public RetentionCurve
		{
		private:
			real m_Ksat;
			real m_KsatDecay;
		public:
			virtual real K(real wetness,real depth) const;
			void SetKsat(real ksat,real ksat_decay)
			{
				m_Ksat=ksat;
				m_KsatDecay=ksat_decay;
			}
		private:
			real m_Porosity;
			real m_PorosityDecay;
			real m_m,m_Wi,m_Psi_i,m_n,m_b;
			void Set_Saturated_pF_curve_tail_parameters();
		public:
			/// real (Porosity)
			real Porosity(real depth) const;
			/// Sets the porosity (Volume of pores per volume of soil) and the exponential porosity decline with depth
			void SetPorosity(real porosity,real porosity_decay=0);
			/// Returns the pore volume in a region of a soil column
			/// 
			/// If there is a porosity decay, the void volume is the integral of porosity over depth times area
			real VoidVolume(real upperDepth,real lowerDepth,real Area) const;
			
			real FillHeight(real lowerDepth,real Area,real Volume) const;
			real Transmissivity(real upperDepth,real lowerDepth,real theta) const;
			/// Wetness at a known point of the retention curve
			real wetness_X;
			/// Matric potential at the wetness X in m
			real Psi_X;
			/// Retention curve shape parameter
			real b() const { return m_b;}
			void Set_b(real new_b) {
				m_b=new_b;
				Set_Saturated_pF_curve_tail_parameters();

			}
			/// Returns the suction pressure in m, use conversion functions waterhead_to_pressure and waterhead_to_pF fro conversions
			real MatricPotential(real wetness) const;
			real Wetness(real suction) const;
			/// Creates a brooks corey retention curve
			/// @param ksat Saturated conductivity \f$\frac{m}{day}\f$
			/// @param porosity \f$\frac {m^3 Pores}{m^3 Soil}\f$
			/// @param _b Shape of the retention curve (if you do not know how to parameterize this, take a look at the other constructor)
			/// @param theta_x \f$\theta_X\f$ Water content at a specific suction pressure
			/// @param psi_x Suction pressure for \f$\theta_X\f$ in m water column, use the conversion functions pF_to_waterhead, pressure_to_waterhead to convert pressure in to waterhead height (default pF=2.5)
			/// @param ksat_decay Relative decay of conductivity with depth, e.g. 0.1 means conductivity gets 10% smaller per meter
			/// @param porosity_decay Relative decay of porosity with depth, e.g. 0.1 means conductivity gets 10% smaller per meter
			BrooksCoreyRetentionCurve(real ksat=15,real porosity=0.5,real _b=5,real theta_x=0.2,real psi_x=pF_to_waterhead(2.5),real ksat_decay=0,real porosity_decay=0);

			/// Creates a soiltype from two known points of the retention curve (e.g. fieldcapacity and wilting point)
			/// @param ksat Saturated conductivity \f$\frac{m}{day}\f$
			/// @param porosity \f$\frac {m^3 Pores}{m^3 Soil}\f$
			/// @param theta1 \f$\theta_1\f$ First water content at a specific suction pressure (e.g. fieldcapacity)
			/// @param theta2 \f$\theta_2\f$ Second water content at a specific suction pressure (e.g. wiltingpoint)
			/// @param psi_1 \f$ \Psi_1\f$ Suction pressure for \f$\theta_1\f$ in m water column, use the conversion functions pF_to_waterhead, pressure_to_waterhead to convert pressure in to waterhead height (default pF=2.5)
			/// @param psi_2 \f$ \Psi_2\f$ Suction pressure for \f$\theta_2\f$ in m water column, use the conversion functions pF_to_waterhead, pressure_to_waterhead to convert pressure in to waterhead height (default pF=4.2)
			///
			/// The parameter b (shape of the retention curve) is calculated by:
			/// \f[ b = \frac{\log\left(\frac{\Psi_1}{\Psi_2}\right)}{\log\left(\frac{\theta_2}{\theta_1}\right)} \f]
			static BrooksCoreyRetentionCurve CreateFrom2Points(real ksat,real porosity,real theta1,real theta2,real psi_1=pF_to_waterhead(2.5),real psi_2=pF_to_waterhead(4.2));
			virtual BrooksCoreyRetentionCurve* copy() const
			{
				return new BrooksCoreyRetentionCurve(m_Ksat,m_Porosity,b(),wetness_X*m_Porosity,Psi_X,m_KsatDecay,m_PorosityDecay);
			}
		};
		
		/// Provides the use of the Van Genuchten - Mualem retention curve (Van Genuchten 1980)
		/// \f{eqnarray*}
		/// W(\theta) &=& \frac{\theta - \theta_r}{\theta_s - \theta_r} \\
		/// K(W) &=& K_{sat} \sqrt{W} \left(\left(1-w^{1/m}\right)^m\right)^2 \\
		/// m &=& 1-\frac 1 n \\
		/// \Psi(W) &=& 0.01 \frac{m}{cm} \frac{{\left(1-{W}^{\frac{1}{m}}\right) }^{\frac{1}{n}}}{\alpha\,{W}^{\frac{1}{m\,n}}} \\
		/// W(\Psi) &=& \left(1+\left(\alpha\,100\frac{cm}{m}\Psi\right)^n\right)^{-m}
		/// \f}
		/// where:
		/// - \f$K\f$ is the conductivity in \f$\frac m {day}\f$
		/// - \f$W\f$ is the wetness (Volume of soil water per volume of pores)
		/// - \f$n\f$ is a shape parameter of the retention curve
		/// - \f$\alpha\f$ is inverse of the air entry potential in \f$cm^{-1}\f$
		/// - \f$\Psi(W)\f$ is the matric potential in \f$m H_2O\f$ at wetness W
		class VanGenuchtenMualem : public RetentionCurve
		{
		public:
			real
				alpha,n,Ksat,Phi,Psi_full,w_max;
			///\f[ W(\Psi) &=& \left(1+\left(\alpha\,100\frac{cm}{m}\Psi\right)^n\right)^{-m} \f]
			virtual real Wetness(real suction) const;
			/// \f[\Psi(W) &=& 0.01 \frac{m}{cm} \frac{{\left(1-{W}^{\frac{1}{m}}\right) }^{\frac{1}{n}}}{\alpha\,{W}^{\frac{1}{m\,n}}}  \f]
			virtual real MatricPotential(real wetness) const;
			/// \f[K(W) &=& K_{sat} \sqrt{W} \left(\left(1-w^{1/m}\right)^m\right)^2 \f]
			virtual real K(real wetness,real depth) const;
			/// \f[V_{void}=A\,\left(d_{lower}-d_{upper}\right)\f]
			virtual real VoidVolume(real upperDepth,real lowerDepth,real Area) const;
			/// \f[T=K(W)\,\left(d_{lower}-d_{upper}\right)\f]
			virtual real Transmissivity(real upperDepth,real lowerDepth,real wetness) const;
			/// \f[\Phi(d)=const\f]
			virtual real Porosity(real depth) const;
			virtual real FillHeight(real lowerDepth,real Area,real Volume) const;
			VanGenuchtenMualem* copy() const;
			VanGenuchtenMualem()
				: n(3.5),alpha(0.08),Phi(0.5),Ksat(15),Psi_full(-0.1),w_max(1.1) {}
			/// Creates a van Genuchten-Mualem retention curve
			/// @param _n Van Genuchten n
			/// @param _alpha Van Genuchten \f$\alpha\f$ in \f$\frac 1{cm}\f$
			/// @param _phi Porosity in \f$\frac{m^3 Pores}{m^3 Soil}\f$
			/// @param _Ksat Saturated conductivity in \f$\frac m{day}\f$
			VanGenuchtenMualem(real _n,real _alpha,real _phi,real _Ksat)
				: n(_n),alpha(_alpha),Phi(_phi),Ksat(_Ksat),Psi_full(-0.1),w_max(1.1) {}

		};
		class LinearRetention : public RetentionCurve
		{
		public:
			real Ksat,Phi,thickness;
			virtual real MatricPotential(real wetness) const
			{
				return -thickness*(1-wetness);
			}
			virtual real Wetness(real suction) const
			{
				if (suction<-thickness) 
					return 0.0;
				else 
					return 1+suction/thickness;
			}
			virtual real K(real wetness,real depth) const
			{
				return Ksat*wetness;
			}
			virtual real VoidVolume(real upperDepth,real lowerDepth,real Area) const
			{
				return (lowerDepth-upperDepth)*Area*Phi;
			}
			virtual real Transmissivity(real upperDepth,real lowerDepth,real wetness) const
			{
				return (lowerDepth-upperDepth)*K(wetness,0.5*(lowerDepth+upperDepth));
			}
			virtual real Porosity(real depth) const
			{
				return Phi;
			}

			virtual real FillHeight(real lowerDepth,real Area,real Volume) const
			{
				return Volume/(Area*Phi);
			}
			LinearRetention* copy() const
			{
				 return new LinearRetention(Ksat,Phi,thickness);
			}
			LinearRetention(real _Ksat,real _Phi, real _thickness)
				: Ksat(_Ksat),Phi(_Phi),thickness(_thickness)
			{			}
		};
	}
}

#endif // RetentionCurve_h__

