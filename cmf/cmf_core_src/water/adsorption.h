
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
#ifndef adsorption_h__
#define adsorption_h__
#include "../math/real.h"
namespace cmf {
	namespace water {
		/// Abstract class to use adsorption process for tracers on surfaces.
		///
		/// Use the derived classes to use a certain isotherm
		class Adsorption {
		public:
			/// Returns the mass of dissolved tracer as a function of the total tracer mass in the solute storage and the water volume
			///
			/// @param xt \f$x_t\f$ the total tracer mass in the storage
			/// @param V \f$V m^3\f$ the water volume in the storage
			/// @returns \f$x_f\f$ the dissolved mass of the tracer
			virtual real freesolute(real xt,real V) const=0;
			/// returns a copy of the Adsorption object. If the adsorption is depending on the sorbent mass, you can give a positive value for
			/// the sorbent mass m. If the value is not given or negative, m is used from the original object.
			virtual Adsorption* copy(real m=-1) const=0;

		};

		/// A class for tracers without interaction with the storage container. freesolute returns xt
		class NullAdsorption : public Adsorption{
		public:
			virtual real freesolute(real xt,real V) const {
				return xt;
			}
			virtual NullAdsorption* copy(real m=-1) const {
				return new NullAdsorption();
			}
		};
		/// @brief This class calculates the adsorption equilibrium between sorbat and sorbent using the linear (Henry) isotherme.
		///
		/// Henry isotherme:
		///
		/// \f[\frac{x_{ad}}{m} = K c\f]
		/// where
		///  - \f$x_{ad} = x_{tot} - x_{free}\f$ is the adsorbed tracer mass
		///     - \f$x_{tot}\f$ is the total tracer mass
		///     - \f$x_{free}\f$ is the dissolved tracer mass
		///  - \f$m\f$ is the mass of the sorbent in the same unit as the tracer mass
		///  - \f$K\f$ is the Henry sorption coefficient
		///  - \f$c = \frac{x_{free}}{V}\f$ is the concentration of the tracer in tracer mass per m3
		///
		/// CMF stores in a solute storage the total mass of a tracer and needs to calculate the free tracer mass.
		/// Calculating \f$x_{free}\f$ from \f$x_{tot}\f$ gives from the eq. above:
		/// \f[x_{free} = x_{tot} \frac{V}{K m + V}\f]
		class LinearAdsorption : public Adsorption {
		public:
			/// Herny sorption coefficient
			real K;
			/// Mass of sorbent in storage (CEC, clay mass etc.) in unit of tracer
			real m;
			real freesolute(real xt,real V) const {
				return V*xt/(K*m + V);
			}
			LinearAdsorption(real K,real m);
			LinearAdsorption(const LinearAdsorption& other)
				: K(other.K), m(other.m) {}
			LinearAdsorption* copy(real m=-1) const {
				LinearAdsorption* res = new LinearAdsorption(*this);
				if (m>=0) res->m = m;
				return res;
			}
		};
		/// @brief This class calculates the adsorption equilibrium between sorbat and sorbent using the Freundlich isotherme.
		///
		/// Freundlich isotherme:
		///
		/// \f[\frac{x_{ad}}{m} = K c^n\f]
		/// where
		///  - \f$x_{ad} = x_{tot} - x_{free}\f$ is the adsorbed tracer mass
		///     - \f$x_{tot}\f$ is the total tracer mass
		///     - \f$x_{free}\f$ is the dissolved tracer mass
		///  - \f$m\f$ is the mass of the sorbent in the same unit as the tracer mass
		///  - \f$K\f$ is the Henry sorption coefficient
		///  - \f$c = \frac{x_{free}}{V}\f$ is the concentration of the tracer in tracer mass per m3
		///  - \f$n\f$ is the Freundlich exponent
		///
		/// CMF stores in a solute storage the total mass of a tracer and needs to calculate the free tracer mass.
		/// The eq. above can not be rearanged to get \f$x_{free}\f$ from \f$x_{tot}\f$. Instead, the value is iterated
		/// using Newton's method with the solution of the LinearAdsorption as an initial guess. If n is near to 1, using
		/// LinearAdsorption will speed up your calculations. The simplest physically based adsorption model by Langmuir 
		/// (LangmuirAdsorption) has also a anylitical solution and is calculated faster then Freundlich.
		class FreundlichAdsorbtion: public Adsorption {
		public:
			/// Freundlich half saturation
			real K;
			/// Freundlich n
			real n;
			/// Mass of sorbent (CEC, clay mass etc.)
			real m;
			/// Tolerable error for Newton iteration
			real epsilon;
			/// Maximum number of iterations
			int maxiter;
			/// If true, the Newton iteration will throw an error, if the error condition epsilon is not fullfilled after maxiter iterations
			bool strict;
			real freesolute(real xt,real V) const;

			/// @param K,n Freundlich coefficents
			/// @param m Mass of sorbent in units of tracer
			/// @param epsilon Tolerance of New iteration for the calculation of dissolved tracer from total trace, default = 1e-12
			/// @param maxiter Maximum number of iterations, default = 100
			/// @param strict If strict is true, the iteration will throw a runtime error, if the method does not converge
			FreundlichAdsorbtion(real K,real n, real m, real epsilon=1e-12, int maxiter=100, bool strict=true);
			FreundlichAdsorbtion(const FreundlichAdsorbtion& other);
			FreundlichAdsorbtion* copy(real m=-1) const;
		};

		/// Langmuir Adsorption
		///
		/// @TODO: Check correctness of eq.
		/// http://en.wikipedia.org/wiki/Langmuir_equation
		class LangmuirAdsorption : public Adsorption {
		public:
			real freesolute(real xt,real V) const;
			real K;
			real m;
			LangmuirAdsorption(real K,real m);
			LangmuirAdsorption(const LangmuirAdsorption& other);
			LangmuirAdsorption* copy(real m=-1) const;

		};
	}
}
#endif