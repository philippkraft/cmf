

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
#ifndef RetentionCurve_h__
#define RetentionCurve_h__
#include "../../math/real.h"
#include <vector>
#include <stdexcept>
#include "../../cmfmemory.h"
#include "../../math/num_array.h"
namespace cmf {
	namespace upslope {
		const double rho_wg=9810; // Pa/m
		/// Converts a pressure in Pa to a length of a water column in m
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
			virtual real K(real wetness) const {
				throw std::runtime_error("This retention curve type can't calculate conductivity (K), choose another type");
			}
			cmf::math::num_array K(const cmf::math::num_array& wetness) const {
				cmf::math::num_array res(wetness.size());
				for (ptrdiff_t i = 0; i < wetness.size() ; ++i)
					res[i] = K(wetness[i]);
				return res;
			}

			/// Returns the effective wetness, using a residual pF value
			/// \f[w_{eff}  = \frac{w_{act}-w\left(pF_r\right)}{1-w\left(pF_r\right)}\f]
			virtual real Wetness_eff(real wetness,real pF_r=4.2) const
			{
				real w_r=Wetness_pF(pF_r);
				return (wetness-w_r)/(1-w_r);
			}
			/// Returns the porosity at a certain depth
			virtual real Porosity(real depth=0.0) const = 0;
			/// Returns the void volume of a soil column
			virtual real VoidVolume(real upperDepth,real lowerDepth,real Area) const {
				throw std::runtime_error("This retention curve type can't calculate the VoidVolume, choose another type");
			};
			/// Returns the thickness of a soil column with a certain pore volume
			virtual real FillHeight(real lowerDepth,real Area,real Volume) const {
				throw std::runtime_error("This retention curve type can't calculate the FillHeight, choose another type");
			};
			/// returns the wetness (volumetric water content per pore space) at a given suction pressure
			virtual real Wetness(real suction) const {
				throw std::runtime_error("This retention curve type can't calculate the Wetness from a suction pressure, choose another type");
			};
			/// returns the water content \f$theta\f$ for a given wetness
			virtual real theta(real wetness) const {
				return Porosity(0.0) * wetness;
			}
			/// Returns the Diffusivity of the soil. 
			///
			/// Not implemented for all retention curves. Diffusivity is used by
			/// MACROlikeMacroMicroExchange
			virtual real Diffusivity(real wetness) const {
				throw std::runtime_error("This retention curve type can't calculate the Diffusivity, choose another type");
			}
			cmf::math::num_array Diffusivity(cmf::math::num_array& wetness) {
				cmf::math::num_array res(wetness.size());
				for (ptrdiff_t i = 0; i < wetness.size() ; ++i)
					res[i] = Diffusivity(wetness[i]);
				return res;
			}
			cmf::math::num_array Wetness(const cmf::math::num_array& suction) const {
				cmf::math::num_array res(suction.size());
				for (ptrdiff_t i = 0; i < suction.size() ; ++i)
					res[i] = Wetness(suction[i]);
				return res;
			}
			cmf::math::num_array theta(const cmf::math::num_array& wetness) const {
				cmf::math::num_array res(wetness.size());
				for (ptrdiff_t i=0;i<wetness.size(); ++i) 
					res[i] = theta(wetness[i]);
				return res;
			}
			virtual real dPsiM_dW(real wetness) const {
				throw std::runtime_error("This retention curve type can't calculate dPsi_M/dW analytically, choose another type");
			}
			cmf::math::num_array dPsiM_dW(const cmf::math::num_array& wetness) const {
				cmf::math::num_array res(wetness.size());
				for (ptrdiff_t i = 0; i < wetness.size() ; ++i)
					res[i] = dPsiM_dW(wetness[i]);
				return res;				
			}
			/// returns the volumetric water content at a given pF value
			real Wetness_pF(real pF) const {return Wetness(pF_to_waterhead(pF));}
			cmf::math::num_array Wetness_pF(const cmf::math::num_array& pF) const {
				cmf::math::num_array res(pF.size());
				for (ptrdiff_t i = 0; i < pF.size() ; ++i)
					res[i] = Wetness_pF(pF[i]);
				return res;
			}
			/// returns the wetness of the soil at given water content
			virtual real MatricPotential(real wetness) const {
				throw std::runtime_error("This retention curve type can't calculate the matrix potential for a wetness, choose another type");
			};
			cmf::math::num_array MatricPotential(const cmf::math::num_array& wetness) const {
				cmf::math::num_array res(wetness.size());
				for (ptrdiff_t i = 0; i < wetness.size() ; ++i)
					res[i] = MatricPotential(wetness[i]);
				return res;
			
			}
			virtual RetentionCurve* copy() const=0;
			virtual ~RetentionCurve()
			{

			}
		};

		/// Provides the use of the Brooks-Corey retention curve
		///
		/// \f{eqnarray*}
		/// W(\theta) &=& \frac{\theta - \theta_r}{\theta_s - \theta_r} \\
		/// K(W) &=& K_{sat} W^{2b+3} \\
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
			real m_Porosity;
			real m_PorosityDecay;
			real m_m,m_Wi,m_Psi_i,m_n,m_b;
			void Set_Saturated_pF_curve_tail_parameters();
		public:
			real Ksat;
			virtual real K(real wetness) const;
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
			real residual_theta;
			/// Retention curve shape parameter
			real get_b() const { return m_b;}
			void set_b(real new_b) {
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
			/// @param porosity_decay Relative decay of porosity with depth, e.g. 0.1 means conductivity gets 10% smaller per meter
			BrooksCoreyRetentionCurve(real ksat=15,real porosity=0.5,real _b=5,real theta_x=0.2,real psi_x=pF_to_waterhead(2.5),real porosity_decay=0);

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
				return new BrooksCoreyRetentionCurve(Ksat,m_Porosity,get_b(),wetness_X*m_Porosity,Psi_X,m_PorosityDecay);
			}
			virtual ~BrooksCoreyRetentionCurve()
			{

			}
		};
		
		/// Provides the use of the Van Genuchten - Mualem retention curve (Van Genuchten 1980)
		///
		/// Head - moisture relationship:
		/// \f{eqnarray*}
		/// W(\theta) &=& \frac{\theta - \theta_r}{\theta_s - \theta_r} \\
		/// K(W) &=& K_{sat} \sqrt{W} \left(1-\left(1-W^{1/m}\right)^m\right)^2 \\
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
				alpha, ///< Inverse of water entry potential in 1/cm
				n, ///< Pore size distribution parameter [-]
				Ksat, ///< Saturated conductivity in m/day
				Phi, ///< Porosity in m3/m3
				m, ///< VanGenuchten m (if negative, 1-1/n is used)
				l, ///< VanGenuchten - Mualem tortoisivity, defined in Van Genuchten 1980 as 0.5 (default here)
				theta_r, ///< residual water content in m3 Water/ m3 soil (default=0.0)
				w0; ///< Breakpoint wetness. If W>w0, a parabolic extrapolation is used

			///@brief returns the saturation at a given suction (matrix potential).
			///
			///\f[ W(\Psi) = \left(1+\left(\alpha\,100\frac{cm}{m}\Psi\right)^n\right)^{-m} \f]
			virtual real Wetness(real suction) const;
			virtual real theta(real wetness) const;
			/// @brief returns the matrix potential at a given saturation
			///
			/// \f[\Psi(W) = 0.01 \frac{m}{cm} \frac{{\left(1-{W}^{\frac{1}{m}}\right) }^{\frac{1}{n}}}{\alpha\,{W}^{\frac{1}{m\,n}}}  \f]
			virtual real MatricPotential(real wetness) const;
			
			/// returns \f$\tfrac{d\Psi_M}{dW}\f$
			///
			/// \f[\frac{0.01 w^{\frac{1}{m}} w^{- \frac{1}{m n}} \left(- w^{\frac{1}{m}} + 1\right)^{\frac{1}{n}}}{\alpha m n w \left(- w^{\frac{1}{m}} + 1\right)} + \frac{0.01 w^{- \frac{1}{m n}} \left(- w^{\frac{1}{m}} + 1\right)^{\frac{1}{n}}}{\alpha m n w}\f]
			virtual real dPsiM_dW(real wetness) const;
			/// @brief returns the conductivity of the soil at a given saturation
			///
			/// \f[K(W) = K_{sat} \sqrt{W} \left(1-\left(1-W^{1/m}\right)^m\right)^2 \f]
			virtual real K(real wetness) const;
			virtual real VoidVolume(real upperDepth,real lowerDepth,real Area) const;
			virtual real Transmissivity(real upperDepth,real lowerDepth,real wetness) const;
			virtual real Porosity(real depth) const;
			/// Returns the diffusivity of the soil according to its wetness as given by VanGenuchten 1980
			///
			/// @deprecated The current implementation goes to infinity at saturation, as noted by VanGenuchten. 
			/// Diffusivity is therefore currently not usable in any model.
			///
			/// \f[D(W) = K(W)\left|\frac{d\Psi}{d\theta}\right|\ eq. 10\f]
			/// where:
			/// - \f$D(W)\f$ Diffusivity in \f$m^2/day\f$
			/// - \f$K(W)\f$ Conductivity as a function of saturation W in m/day
			/// - \f$\Psi\f$ Pressure head
			/// - \f$\theta\f$ water content of the soil
			///
			/// Applying Van Genuchten theory (Van Genuchten 1980) yields to:
			/// \f[D(W) = \frac{(1-m)K_{sat}}{\alpha m \Phi} W^{l-1/m}\left(\left(1-W^{1/m}\right)^{-m} + \left(1-W^{1/m}\right)^{m} -2\right)\f]
			/// where:
			///  - \f$m = 1 - \frac 1 n\f$ acc. Mualem theory
			///  - \f$K_{sat}\f$ saturated conductivity in m/day
			///  - \f$\alpha\f$ inverse water entry potential in 1/m. **Note** \f$\alpha\f$ is given in cmf in 1/cm
			///  - \f$\Phi\f$ porosity
			///  - \f$W = \frac{\theta - \theta_r}{\Phi - \theta_r}\f$ saturation of the soil
			virtual real Diffusivity(real wetness) const;
			/// Fits the break point wetness w0, to ensure a specific oversaturation
			/// at a given hydrostatic potential
			/// @param w1 The oversaturation wetness to archieve (>1), default = 1.01
			/// @param Psi_p the hydrostatic potential for w1, default = +1.0 m
			/// @param tolerance
			real fit_w0(real w1=1.01,real Psi_p=1.0,real tolerance=0.05);

			virtual real FillHeight(real lowerDepth,real Area,real Volume) const;
			VanGenuchtenMualem* copy() const;
			/// Creates a van Genuchten-Mualem retention curve
			/// @param Ksat Saturated conductivity in \f$\frac m{day}\f$
			/// @param phi Porosity in \f$\frac{m^3 Pores}{m^3 Soil}\f$
			/// @param alpha Van Genuchten \f$\alpha\f$ in \f$\frac 1{cm}\f$
			/// @param n Van Genuchten n
			/// @param m Van Genuchten m parameter, if negative m is calculated as \f$ 1-\frac 1 n\f$
			VanGenuchtenMualem(real Ksat=15, real phi=0.5,real alpha=0.2178, real n=1.211, real m=-1);
			virtual ~VanGenuchtenMualem()
			{

			}
		};
		/// The linear retention curve provides a simple linear relationship between storage and head
		/// 
		/// Head function (head in m, calculated from upper side control volume)
		/// \f[ h(\theta) = -\Delta z \left( 1 - \frac{\theta - \theta_r}{\theta_s - \theta_r} \right) \f]
		/// Conductivity function
		/// \f[ K(\theta) = K_{sat}  \left(\frac{\theta - \theta_r}{\theta_s - \theta_r}\right)^\beta	\f]
		class LinearRetention : public RetentionCurve
		{
		public:
			real 
				Ksat, ///< Saturated Conductivity \f$\frac m{day} \f$
				porosity,  ///< Pore space \f$\Phi \left[ \frac{m^3}{m^3 \mbox{soil}}\right]\f$
				thickness, ///< Thickness of the layer in m
				residual_wetness; ///< Residual wetness \f$ \frac{\theta_r}{\Phi}, where K(\theta_r) = 0 \f$
			real
				porosity_decay,		///< Depth dependent decay of porosity in fraction/m. 0.1 means a decay of 10% per meter depth
				beta;				///< Power Law Constant. 
			/// Returns the potential below upper side of the control volume in m
			virtual real MatricPotential(real wetness) const;
			/// Returns the wetness (water content per pore volume) at a given head
			virtual real Wetness(real suction) const;
			
			virtual real K(real wetness) const;
			virtual real VoidVolume(real upperDepth,real lowerDepth,real Area) const;
			virtual real Transmissivity(real upperDepth,real lowerDepth,real wetness) const;
			virtual real Porosity(real depth) const;

			virtual real FillHeight(real lowerDepth,real Area,real Volume) const;

			LinearRetention* copy() const
			{
				 return new LinearRetention(*this);
			}
			LinearRetention(real ksat,real phi, real thickness,real residual_wetness=0.1);
			virtual ~LinearRetention()
			{

			}
		};
#ifdef EXPERIMENTAL
		/// The VGM_BC_RetentionCurve_Windhorst retention curve. Mixture between the van Genuchten-Mualem retention curve for the actual retention curve and the conductivity at a certain wetness is computed like in BrooksCorey
		class VGM_BC_RetentionCurve_Windhorst : public VanGenuchtenMualem
		{
		public:
			real
				eta; ///< Shape parameter of the conductivity curve

			virtual real K(real wetness) const;

			VGM_BC_RetentionCurve_Windhorst* copy() const
			{
					return new VGM_BC_RetentionCurve_Windhorst(*this);
			}
			/// @param Ksat Saturated conductivity in \f$\frac m{day}\f$
			/// @param phi Porosity in \f$\frac{m^3 Pores}{m^3 Soil}\f$
			/// @param alpha Van Genuchten \f$\alpha\f$ in \f$\frac 1{cm}\f$
			/// @param n Van Genuchten n
			/// @param m Van Genuchten m parameter, if negative m is calculated as \f$ 1-\frac 1 n\f$
			/// @param eta Shape parameter of the conductivity curve
			VGM_BC_RetentionCurve_Windhorst(real Ksat=15, real phi=0.5,real alpha=0.2178, real n=1.211, real m=-1,real eta = 9.14);
			virtual ~VGM_BC_RetentionCurve_Windhorst()
			{	}
		};
#endif

	}
}

#endif // RetentionCurve_h__

