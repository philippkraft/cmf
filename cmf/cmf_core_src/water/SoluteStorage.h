

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
#ifndef SoluteStorage_h__
#define SoluteStorage_h__

#include "Solute.h"
#include "../math/statevariable.h"
#include "../math/real.h"
#include "adsorption.h"

//#include "Reaction.h"
namespace cmf {
	namespace water {
		class WaterStorage;
		/// @brief A class for the storage of any tracer. The state is the amount (mol, kg etc. see cmf::water) 
		/// of the tracer in the storage.
		///
		/// The derivative function is given by:
		/// \f{eqnarray*}
		/// \frac{dX}{dt}&=&\sum_{f=1}^{F}\left( q_f [X]_f\right) + X_{in} - r^-X \left[\frac{mol}{day}\right]\\
		/// F&=& \mbox{Number of fluxes in water storage} \\
		/// q_f&=& \mbox{Water flux in } \frac{m^3}{day}	\\
		/// \left[X\right]_f &=& \mbox{Concentration of solute X in flux }q_f \mbox{ in } \frac{mol}{m^3} \\
		/// X_{in} &=& \mbox{Absolute source or sink term} \frac{mol}{day} \\
		/// r^- &=& \mbox{Decay rate} \frac 1{day} \\
		/// V &=& \mbox{Volume of water in water storage }\left[m^3\right]
		/// \f}
		class SoluteStorage : public cmf::math::StateVariable
		{
			SoluteStorage(WaterStorage* _water,const cmf::water::solute& _Solute, double InitialState=0) 
				: cmf::math::StateVariable(InitialState),m_water(_water), Solute(_Solute),decay(0),source(0),
				  adsorption(new NullAdsorption)
			{}
			
			WaterStorage* m_water;
			std::unique_ptr<Adsorption> adsorption;
			
		public:
			void set_adsorption(const Adsorption& newadsorption,real m=-1) {
				adsorption.reset(newadsorption.copy(m));
			}
			friend class WaterStorage;
			/// @brief Rate of decay of the solute (in 1/day)
			real decay;
			/// @brief A source or sink term of the solute as an absolute matter flux in state unit/day
			real source;
			/// @brief The solute, which is stored in this
			const cmf::water::solute& Solute;
			/// @brief Returns the concentration of the solute
			real conc() const;
			/// @brief set a new concentration of dissolved tracers. 
			///
			/// In case of adsorption functions, the isotherm is used

			void set_conc(real NewConcentration);

			virtual real dxdt(const cmf::math::Time& time);
			virtual std::string to_string() const;
			typedef std::shared_ptr<SoluteStorage> ptr;
		};
	}
}

#endif // SoluteStorage_h__
