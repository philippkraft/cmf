

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
#ifndef ExplicitEuler_fixed_h__
#define ExplicitEuler_fixed_h__

#include "integrator.h"
namespace cmf {
	namespace math {
		/// An explicit Euler integrator, with a fixed time step
		class ExplicitEuler_fixed : public Integrator
		{
		private:
			num_array 
				dxdt;
		public:
			/// Add state variables from a StateVariableOwner
			void add_states(cmf::math::StateVariableOwner& stateOwner)
			{
				Integrator::add_states(stateOwner);
				dxdt.resize(size());
			}

			/// Constructs a new ExplicitEuler_fixed from a pointer to a vector of state variables
			/// @note The Integrator becomes the owner of states
			/// @param states Statevariables of the system
			ExplicitEuler_fixed(StateVariableOwner& states)
				: Integrator(states,0.0),
				dxdt(m_States.size())
			{}

			/// Constructs a new ExplicitEuler_fixed
			ExplicitEuler_fixed()
				: Integrator(1e-9)
			{	}

			/// copy constructor
			ExplicitEuler_fixed(const Integrator& copy) : Integrator(copy) {}
			virtual ~ExplicitEuler_fixed() {}
			virtual Integrator * copy() const
			{
				return new ExplicitEuler_fixed(*this);
			}

			///Integrates the vector of state variables
			/// @param MaxTime (ignored) To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed time step
			int integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
			{
				if (m_States.size()==0)
					throw std::out_of_range("No states to integrate!");
				m_dt=TimeStep;
				if (m_dt>MaxTime-get_t())
					m_dt=MaxTime-get_t();
				// copy derivates multipied with time step to dxdt
				copy_dxdt(get_t(),dxdt,m_dt.AsDays());
				// Update time step with delta x
				add_values_to_states(dxdt);
				m_t += m_dt;
				
				return 1;
			}

		};

		/// A simple predictor - corrector solver. Not tested and very experimentally
		/// \f$ y^{n+1} = y^n + \alpha f(y^n + f(y^n)dt)dt + (1-\alpha)f(y^n)dt \f$
		class HeunIntegrator : public Integrator
		{
		private:
			num_array 
				dxdt0, dxdt1, old_states;
		public:
			/// Add state variables from a StateVariableOwner
			void add_states(cmf::math::StateVariableOwner& stateOwner)
			{
				Integrator::add_states(stateOwner);
				dxdt0=num_array(size());
				dxdt1=num_array(size());
				old_states=num_array(size());
			}
			/// Alpha Weight factor \f$\alpha\f$ to weight \f$f(y^n)\f$ and \f$f(y^{n+1})\f$
			real alpha;
			/// Constructs a new PredictCorrectSimple from a pointer to a vector of state variables
			/// @note The Integrator becomes the owner of states
			/// @param states Statevariable owner of the system
			/// @param Alpha Weight factor \f$\alpha\f$ to weight \f$f(y^n)\f$ and \f$f(y^{n+1})\f$
			HeunIntegrator(StateVariableOwner& states, real Alpha=0.5)
				: Integrator(states,0.0),alpha(Alpha)
			{}

			/// Constructs a new PredictCorrectSimple
			/// @param Alpha Weight factor \f$\alpha\f$ to weight \f$f(y^n)\f$ and \f$f(y^{n+1})\f$
			HeunIntegrator(real Alpha=0.5)
				: Integrator(),alpha(Alpha)
			{	}

			/// copy constructor
			HeunIntegrator(const Integrator& copy) : Integrator(copy) {}
			virtual ~HeunIntegrator() {}
			virtual Integrator * copy() const
			{
				return new HeunIntegrator(*this);
			}

			///Integrates the vector of state variables
			/// @param MaxTime (ignored) To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed time step
			int integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
			{
				m_dt=TimeStep;
				if (m_dt>MaxTime-get_t())
					m_dt=MaxTime-get_t();
				copy_states(old_states);
				// get f(y_i)dt
				copy_dxdt(get_t(),dxdt0,m_dt.AsDays());
				// Update time step with delta x
				add_values_to_states(dxdt0);
				if (alpha>0)
				{
					// get f(y_i+1)dt
					copy_dxdt(get_t(),dxdt1,m_dt.AsDays() * alpha);
					// reset states to y_i
					set_states(old_states);
					// update states to y_i+1
					if (alpha<1) 
					{
						dxdt0 *= (1-alpha);
						add_values_to_states(dxdt0);
					}
					add_values_to_states(dxdt1);
				}
				m_t += m_dt;

				return 1;
			}

		};


	}
}

#endif // ExplicitEuler_fixed_h__
