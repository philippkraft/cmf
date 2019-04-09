

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
#ifndef MultiIntegrator_h__
#define MultiIntegrator_h__
#include "integrator.h"
#include <memory>
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
            std::string to_string() const override;

            /// Resets the integrator
			virtual void reset()
			{
				for(integ_vector::iterator it = m_integrators.begin(); it != m_integrators.end(); ++it)
				{
				    (**it).set_t(m_t);
				}
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
