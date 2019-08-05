

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
#ifndef WaterSoluteIntegrator_h__
#define WaterSoluteIntegrator_h__

#include <memory>
#include "integrator.h"
#include "../../water/Solute.h"
namespace cmf {
	namespace math {
#ifndef SWIG
        typedef std::unique_ptr<Integrator> integrator_ptr;
        typedef std::vector<integrator_ptr> solute_integrator_list;
#endif
		/// A SoluteWaterIntegrator implements the cmf::math::Integrator interface, but consists of
		/// two independent ODE-solvers. Added statevariables are sorted by waterstorages and solute storages and
		/// assigned to the correct solver.
        class SoluteWaterIntegrator : public cmf::math::Integrator
		{
		protected:

#ifndef SWIG
			solute_integrator_list m_soluteintegrators;
			integrator_ptr m_waterintegrator;
			cmf::water::solute_vector solutes;

#endif
			// Create the needed integrators
			void erase_integrators();
			// Function to add the states to the appropriate integrator
			void distribute_states();

		public:

			/// Add state variables from a cmf::math::state_list
			int integrate(cmf::math::Time t_max,cmf::math::Time dt) override;
			cmf::math::SoluteWaterIntegrator* copy() const override;
			std::string to_string() const override;

			/// Creates a new SoluteWaterIntegrator
			/// @param solutes Solutes of the project
			/// @param water_integrator Template for the integrator of WaterStorage state variables
			/// @param solute_integrator Template for the integrator of soluteStorage state variables
			SoluteWaterIntegrator(
			        const cmf::water::solute_vector& solutes,
			        const cmf::math::Integrator& water_integrator,
			        const cmf::math::Integrator& solute_integrator
			        );

			/// Creates a new SoluteWaterIntegrator
			/// @param solutes Solutes of the project
			/// @param water_integrator Template for the integrator of WaterStorage state variables
			/// @param solute_integrator Template for the integrator of soluteStorage state variables
			/// @param states States to be added to the integrators
			SoluteWaterIntegrator(
			        const cmf::water::solute_vector& solutes,
			        const cmf::math::Integrator& water_integrator,
			        const cmf::math::Integrator& solute_integrator,
			        const cmf::math::state_list& states);

			/// Resets the integrators (only needed for multistep methods)
			void reset() override;

		};
	}
	
}
#endif // WaterSoluteIntegrator_h__
