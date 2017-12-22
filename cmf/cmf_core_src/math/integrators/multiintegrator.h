

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
#ifndef MultiIntegrator_h__
#define MultiIntegrator_h__
#include "integrator.h"
#include "../../cmfmemory.h"
namespace cmf {
	namespace math {
		/// The MultiIntegrator is a wrapper for a bunch integrators. The states of the integrators should not have direct connections over integrator boundaries
		class MultiIntegrator : public cmf::math::Integrator
		{
			typedef std::shared_ptr<cmf::math::Integrator> integ_ptr;
			typedef std::vector<integ_ptr> integ_vector;
			integ_vector m_integrators;
			std::unique_ptr<cmf::math::Integrator> m_template;
		public:
			int integrate(cmf::math::Time t_max,cmf::math::Time dt);
			virtual cmf::math::MultiIntegrator* copy() const
			{
				return new MultiIntegrator(*m_template,int(m_integrators.size()));
			}
			/// Resets the integrator
			virtual void reset()
			{
				for(integ_vector::iterator it = m_integrators.begin(); it != m_integrators.end(); ++it)
				{
				    (**it).set_t(m_t);
				}
			}
			/// Only there to override Integrator::AddStatesFromOwner. Throws an exception. Use add_states_to_integrator instead
			void add_states(cmf::math::StateVariableOwner& stateOwner)
			{
				throw std::runtime_error("States can only be added to one integrator of the multi integrator. Use add_state_to_integrator");
			}
			/// Add state variables from a StateVariableOwner
			void add_states_to_integrator(cmf::math::StateVariableOwner& stateOwner, int integrator_position)
			{
				integ_ptr& integ=m_integrators.at(integrator_position<0 ? m_integrators.size()-integrator_position : integrator_position);
				integ->add_states(stateOwner);
			}
			/// Creates a new MultiIntegrator
			/// @param template_integrator Template for the integrators
			/// @param count Number of integrators
			MultiIntegrator(const cmf::math::Integrator& template_integrator,int count)
				: Integrator(), m_template(template_integrator.copy())
			{
				for (int i = 0; i < count ; ++i)
				{
					cmf::math::Integrator* integ=m_template->copy();
					integ_ptr Integ(integ);
					m_integrators.push_back(Integ);
				}
			}
		};

	}

}

#endif // MultiIntegrator_h__
