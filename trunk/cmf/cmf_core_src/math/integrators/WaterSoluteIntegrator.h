

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
#ifndef WaterSoluteIntegrator_h__
#define WaterSoluteIntegrator_h__
#include "integrator.h"
#include "../../water/WaterStorage.h"
#include "../../water/SoluteStorage.h"
namespace cmf {
	namespace math {
		/// A SoluteWaterIntegrator implements the cmf::math::Integrator interface, but consists of 
		/// two independent ODE-solvers. Added statevariables are sorted by waterstorages and solute storages and
		/// assigned to the correct solver.
		class SoluteWaterIntegrator : public cmf::math::Integrator
		{
		protected:
			/// integrator for the water storages
			std::auto_ptr<cmf::math::Integrator> WaterIntegrator;
			/// integrator for the solute storages
			std::auto_ptr<cmf::math::Integrator> SoluteIntegrator;
			/// Function to add the states to the appropriate integrator
			void distribute_states()
			{
				for(state_vector::iterator it = m_States.begin(); it != m_States.end(); ++it)
				{
					cmf::water::WaterStorage::ptr ws = std::tr1::dynamic_pointer_cast<cmf::water::WaterStorage>(*it);
					if(ws)	WaterIntegrator->AddState(ws);
					else SoluteIntegrator->AddState(*it);
				}
			}

		public:
			/// Returns the integrator for WaterStorage state variables
			cmf::math::Integrator* get_water_integrator() const
			{
				return WaterIntegrator.get();
			}
			/// Returns the integrator for SoluteStorage state variables
			cmf::math::Integrator* get_solute_integrator() const
			{
				return SoluteIntegrator.get();
			}

			/// Changes the integrator for WaterStorage state variables
			void set_water_integrator(cmf::math::Integrator* templ)
			{
				WaterIntegrator.reset(templ->Copy());
				SoluteIntegrator.reset(SoluteIntegrator->Copy());
				distribute_states();
			}
			/// Changes the integrator for SoluteStorage state variables
			void set_solute_integrator(cmf::math::Integrator* templ)
			{
				SoluteIntegrator.reset(templ->Copy());
				WaterIntegrator.reset(WaterIntegrator->Copy());
				distribute_states();
			}
			/// Add state variables from a StateVariableOwner
			void AddStatesFromOwner(cmf::math::StateVariableOwner& stateOwner)
			{
				Integrator::AddStatesFromOwner(stateOwner);
				distribute_states();
			}
			int integrate(cmf::math::Time t_max,cmf::math::Time dt)
			{
				WaterIntegrator->integrate_until(t_max,dt);
				SoluteIntegrator->integrate_until(t_max,dt);
				m_t=t_max;
				m_dt=WaterIntegrator->get_dt();
				return 1;
			}
			virtual cmf::math::SoluteWaterIntegrator* Copy() const
			{
				return new SoluteWaterIntegrator(*WaterIntegrator,*SoluteIntegrator);
			}
		
			/// Creates a new SoluteWaterIntegrator
			/// @param water_integrator Template for the integrator of WaterStorage state varaiables
			/// @param solute_integrator Template for the integrator of soluteStorage state varaiables
			SoluteWaterIntegrator(const cmf::math::Integrator& water_integrator, const cmf::math::Integrator& solute_integrator)
				: Integrator(), WaterIntegrator(water_integrator.Copy()), SoluteIntegrator(solute_integrator.Copy())
			{

			}

			/// Creates a new SoluteWaterIntegrator
			/// @param water_integrator Template for the integrator of WaterStorage state varaiables
			/// @param solute_integrator Template for the integrator of soluteStorage state varaiables
			/// @param states States to be added to the integrators
			SoluteWaterIntegrator(const cmf::math::Integrator& water_integrator, const cmf::math::Integrator& solute_integrator, cmf::math::StateVariableOwner& states)
				: Integrator(), WaterIntegrator(water_integrator.Copy()), SoluteIntegrator(solute_integrator.Copy())
			{
				AddStatesFromOwner(states);
			}

			/// Resets the integrators (only needed for multistep methods)
			virtual void Reset()
			{
				WaterIntegrator->set_t(m_t);
				SoluteIntegrator->set_t(m_t);
			}



		};
		
	}
	
}
#endif // WaterSoluteIntegrator_h__
