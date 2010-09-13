

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
			void AddStatesFromOwner(cmf::math::StateVariableOwner& stateOwner)
			{
				Integrator::AddStatesFromOwner(stateOwner);
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

			/// Copy constructor
			ExplicitEuler_fixed(const Integrator& copy) : Integrator(copy) {}
			virtual ~ExplicitEuler_fixed() {}
			virtual Integrator * Copy() const
			{
				return new ExplicitEuler_fixed(*this);
			}

			///Integrates the vector of state variables
			/// @param MaxTime (ignored) To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed time step
			int integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
			{
				m_dt=TimeStep;
				if (m_dt>MaxTime-get_t())
					m_dt=MaxTime-get_t();
				// Copy derivates multipied with time step to dxdt
				CopyDerivs(get_t(),dxdt,m_dt.AsDays());
				// Update time step with delta x
				AddValuesToStates(dxdt);
				m_t += m_dt;
				
				return 1;
			}

		};

#ifdef EXPERIMENTAL
		/// A simple predictor - corrector solver. Not tested and very experimentally
		/// \f$ y^{n+1} = y^n + \alpha f(y^n + f(y^n)dt)dt + (1-\alpha)f(y^n)dt \f$
		class PredictCorrectSimple : public Integrator
		{
		private:
			num_array 
				dxdt0, dxdt1, old_states;
		public:
			/// Add state variables from a StateVariableOwner
			void AddStatesFromOwner(cmf::math::StateVariableOwner& stateOwner)
			{
				Integrator::AddStatesFromOwner(stateOwner);
				dxdt0.resize(size());
				dxdt1.resize(size());
				old_states.resize(size());
			}
			/// Alpha Weight factor \f$\alpha\f$ to weight \f$f(y^n)\f$ and \f$f(y^{n+1})\f$
			real alpha;
			/// Constructs a new PredictCorrectSimple from a pointer to a vector of state variables
			/// @note The Integrator becomes the owner of states
			/// @param states Statevariable owner of the system
			/// @param Alpha Weight factor \f$\alpha\f$ to weight \f$f(y^n)\f$ and \f$f(y^{n+1})\f$
			PredictCorrectSimple(StateVariableOwner& states, real Alpha)
				: Integrator(states,0.0),alpha(Alpha)
			{}

			/// Constructs a new PredictCorrectSimple
			/// @param Alpha Weight factor \f$\alpha\f$ to weight \f$f(y^n)\f$ and \f$f(y^{n+1})\f$
			PredictCorrectSimple(real Alpha=0.5)
				: Integrator(),alpha(Alpha)
			{	}

			/// Copy constructor
			PredictCorrectSimple(const Integrator& copy) : Integrator(copy) {}
			virtual Integrator * Copy() const
			{
				return new ExplicitEuler_fixed(*this);
			}

			///Integrates the vector of state variables
			/// @param MaxTime (ignored) To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed time step
			int integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
			{
				m_dt=TimeStep;
				if (m_dt>MaxTime-get_t())
					m_dt=MaxTime-get_t();
				CopyStates(old_states);
				// get f(y^n)dt
				CopyDerivs(get_t(),dxdt0,m_dt.AsDays());
				// Update time step with delta x
				AddValuesToStates(dxdt0);
				if (alpha>0)
				{
					// get f(y^n+1)dt
					CopyDerivs(get_t(),dxdt1,m_dt.AsDays() * alpha);
					// reset states to y^n
					SetStates(old_states);
					// update states to y^n+1
					if (alpha<1) 
					{
						dxdt0 *= (1-alpha);
						AddValuesToStates(dxdt0);
					}
					AddValuesToStates(dxdt1);
				}
				m_t += m_dt;

				return 1;
			}

		};
#endif

	}
}

#endif // ExplicitEuler_fixed_h__
