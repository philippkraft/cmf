

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
#include "Integrator.h"
#include "../../water/WaterStorage.h"
#include "../../water/SoluteStorage.h"
namespace cmf {
	namespace math {
		class SoluteWaterIntegrator : public cmf::math::Integrator
		{
		protected:
			std::auto_ptr<cmf::math::Integrator> WaterIntegrator;
			std::auto_ptr<cmf::math::Integrator> SoluteIntegrator;
			void distribute_states()
			{
				for(cmf::math::StateVariableVector::iterator it = m_States.begin(); it != m_States.end(); ++it)
				{
					cmf::water::WaterStorage *ws = dynamic_cast<cmf::water::WaterStorage *>(*it);
					if(ws)	WaterIntegrator->AddState(*ws);
					else SoluteIntegrator->AddState(**it);
				}
			}

		public:
			cmf::math::Integrator* get_water_integrator() const
			{
				return WaterIntegrator.get();
			}
			cmf::math::Integrator* get_solute_integrator() const
			{
				return SoluteIntegrator.get();
			}
			void set_water_integrator(cmf::math::Integrator* templ)
			{
				WaterIntegrator.reset(templ->Copy());
				SoluteIntegrator.reset(SoluteIntegrator->Copy());
				distribute_states();
			}
			void set_solute_integrator(cmf::math::Integrator* templ)
			{
				SoluteIntegrator.reset(templ->Copy());
				WaterIntegrator.reset(WaterIntegrator->Copy());
				distribute_states();
			}
			void AddStatesFromOwner(cmf::math::StateVariableOwner& stateOwner)
			{
				Integrator::AddStatesFromOwner(stateOwner);
				distribute_states();
			}
			int Integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
			{
				WaterIntegrator->IntegrateUntil(MaxTime,TimeStep);
				SoluteIntegrator->IntegrateUntil(MaxTime,TimeStep);
				m_Time=MaxTime;
				m_TimeStep=WaterIntegrator->TimeStep();
				return 1;
			}
			virtual cmf::math::SoluteWaterIntegrator* Copy() const
			{
				return new SoluteWaterIntegrator(*WaterIntegrator,*SoluteIntegrator);
			}
			SoluteWaterIntegrator(const cmf::math::Integrator& water_integrator, const cmf::math::Integrator& solute_integrator)
				: Integrator(), WaterIntegrator(water_integrator.Copy()), SoluteIntegrator(solute_integrator.Copy())
			{

			}
			virtual void Reset()
			{
				WaterIntegrator->ModelTime(m_Time);
				SoluteIntegrator->ModelTime(m_Time);
			}
			SoluteWaterIntegrator(const cmf::math::Integrator& water_integrator, const cmf::math::Integrator& solute_integrator, cmf::math::StateVariableOwner& states)
				: Integrator(), WaterIntegrator(water_integrator.Copy()), SoluteIntegrator(solute_integrator.Copy())
			{
				 AddStatesFromOwner(states);
			}


		};
		
	}
	
}
#endif // WaterSoluteIntegrator_h__
