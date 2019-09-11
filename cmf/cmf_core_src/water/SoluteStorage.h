

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
#ifndef SoluteStorage_h__
#define SoluteStorage_h__

#include "Solute.h"
#include "../math/statevariable.h"
#include "../math/real.h"
#include "adsorption.h"
#include "reaction.h"
#include "WaterStorage.h"
namespace cmf {
	namespace water {

		class WaterStorage;
		/// @defgroup Solutes Solute transport
		/// @brief The framework to transport and modify solutes in the water framework.

		/// @ingroup Solutes
		/// @brief A class for the storage of any tracer. The state is the amount (mol, kg etc. see cmf::water)
		/// of the tracer in the storage.
		///
		/// The derivative function is given by:
		/// \f[\frac{dX}{dt} = \sum_{f=1}^{F}{q_f \cdot [X]_f} + \sum_{r=1}^{R}{q_r([X], t)} \left[\frac{mol}{day}\right] \f]
		/// - \f$F\f$ =  Number of fluxes in water storage
		/// - \f$q_f\f$ = Water flux in \f$\frac{m^3}{day}\f$
		/// - \f$\left[X\right]_f \f$ = Concentration of solute X in flux \f$q_f\f$ in \f$\frac{mol}{m^3} \f$
		/// - \f$q_r([X], t)\f$ = Production or loss rate from R cmf::water::SoluteReaction objects \f$\frac{mol}{day}\f$
		///
		class SoluteStorage : public cmf::math::StateVariable
		{
			SoluteStorage(WaterStorage* _water,const cmf::water::solute& _Solute, double InitialState=0) 
				: cmf::math::StateVariable(InitialState),m_water(_water), Solute(_Solute),
				  adsorption(new NullAdsorption), reactions()
			{}
			
			WaterStorage* m_water;
			std::unique_ptr<Adsorption> adsorption;
			
		public:
			void set_adsorption(const Adsorption& newadsorption,real m=-1) {
				adsorption.reset(newadsorption.copy(m));
			}
			friend class WaterStorage;

			/// @brief Attached reactions
			cmf::List<cmf::water::SoluteReaction::ptr> reactions;
			/// @brief The solute, which is stored in this storage
			const cmf::water::solute& Solute;
			/// @brief Returns the concentration of the solute
			real get_conc() const;
			/// @brief set a new concentration of dissolved tracers. 
			///
			/// In case of adsorption functions, the isotherm is used
			void set_conc(real NewConcentration);
			/// @brief get the waterstorge of the solute storage
			const WaterStorage& get_water() const {
				return *m_water;
			}
			/// @brief Returns True if this solute storage is effected by another state
			virtual bool is_connected(const cmf::math::StateVariable& other) const;
			virtual void add_connected_states(cmf::math::StateVariable::list& states);
			virtual real dxdt(const cmf::math::Time& time);
			virtual real reactive_flux(const cmf::math::Time& time) const;
			virtual std::string to_string() const;

            real get_abs_errtol(real rel_errtol) const override;

    		typedef std::shared_ptr<SoluteStorage> ptr;

            /// Sets a scaling factor for the absolute error tolerance of this storage in terms
            /// of the absolute error tolerance of the owning waterstorage. The absolute error tolerance
            void set_abs_errtol(real scale) {
                this->m_Scale = scale;
            }
        };

    }
}

#endif // SoluteStorage_h__
