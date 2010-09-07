

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
#ifndef boundary_condition_h__
#define boundary_condition_h__
#include "flux_connection.h"
#include "WaterStorage.h"
#include "../math/timeseries.h"
namespace cmf {
	namespace water {

		/// A linear scaling functor, with slope and displacement
		class linear_scale
		{
		public:
			/// Displacement of the scale ($b$ in scale function)
			real displacement;
			/// Slope of the scale ($a$ in scale function)
			real slope;
			/// Returns the scaled value $y_{scale}=a\ y+b$
			real operator()(real value)	const
			{
				return value*slope+displacement;
			}
			/// Creates a linear scale (by default it is a unity scale, $a=1; b=0$)
			linear_scale(real _slope=1,real _displacement=0)
				: slope(_slope),displacement(_displacement)
			{

			}
		};
		
		/// @defgroup boundary Boundary conditions
		/// @ingroup nodes

		/// @ingroup boundary
		/// Drichelet (constant head) boundary condition
		///
		/// This boundary condition can be used either as a pure sink boundary condition or as a conditional source / sink boundary condition.
		/// The constant head of the boundary condition is interpreted and handled by the connections of the boundary condition. 
		/// Not head aware connections, should not be used, since they are ignoring the constant head.

		class DricheletBoundary : public cmf::water::flux_node
		{
		protected:
			real m_Potential;
			typedef std::map<solute,real> conc_map;
			conc_map m_concentration;
		public:
			typedef std::tr1::shared_ptr<cmf::water::DricheletBoundary> ptr;
#ifndef SWIG
			operator ptr() {return std::tr1::static_pointer_cast<DricheletBoundary>(shared_from_this());}
#endif
			real get_potential() const
			{
				return m_Potential;
			}
			void set_potential(real new_potential)
			{
				m_Potential=new_potential;
			}
			virtual void set_conc(const cmf::water::solute& _Solute, double value);
			virtual real conc(cmf::math::Time t, const cmf::water::solute& _Solute) const;
			bool is_source;
			double is_empty() const
			{
				return is_source ? 0 : 1;
			}
			bool RecalcFluxes(cmf::math::Time t) const
			{
				return 1;
			}
			DricheletBoundary(const cmf::project& _p,real potential,cmf::geometry::point Location=cmf::geometry::point());
			
		};
		/// @ingroup boundary
		/// A Neumann boundary condition (constant flux boundary condition)
		///
		/// The flux is a timeseries, but can be used as a scalar.
		/// To scale the timeseries to the specific conditions of this boundary condition the linear_scale flux_scale can be used.
		class NeumannBoundary : public cmf::water::flux_node
		{
		public:
			typedef std::tr1::shared_ptr<cmf::water::NeumannBoundary> ptr;
#ifndef SWIG
			operator ptr() {return std::tr1::static_pointer_cast<NeumannBoundary>(shared_from_this());}
		#endif
		protected:

			virtual real scale_function(real value)	const
			{
				return flux_scale(value);
			}
		public:
			/// The timeseries of the boundary flux
			cmf::math::timeseries flux;
			/// The scaling of the flux timeseries
			linear_scale flux_scale;
			/// The concentration timeseries of the flux
			cmf::water::SoluteTimeseries concentration;
			/// Returns the solute concentrations of the flux at a given time
			real conc(cmf::math::Time t, const cmf::water::solute& _Solute) const
			{
				return concentration.conc(t,_Solute);
			}
			/// Returns the flux at a given time
			real operator()(cmf::math::Time t) const;
			double is_empty() const
			{
				return flux.is_empty() ? 1 : 0;
			}
			bool RecalcFluxes(cmf::math::Time t) 
			{
				return true;
			}
			void connect_to(cmf::water::flux_node::ptr target);
			/// Ctor of the Neumann boundary
			/// @param _project The project this boundary condition belongs to
			/// @param _flux The flux timeseries (a scalar is converted to a timeseries automatically)
			/// @param _concentration The concentration timeseries
			/// @param loc The location of the boundary condition
			NeumannBoundary(const cmf::project& _project, 
				cmf::math::timeseries _flux,
				cmf::water::SoluteTimeseries _concentration=cmf::water::SoluteTimeseries(),
				cmf::geometry::point loc=cmf::geometry::point());
			NeumannBoundary(const cmf::project& _project,cmf::geometry::point loc=cmf::geometry::point());

			/// Creates a Neumann Boundary condition connected with target
			static NeumannBoundary::ptr create(cmf::water::flux_node::ptr target);
		};

		/// @ingroup connections
		/// This flux_connection is created, when connecting a Neumann boundary condition with a state variable using Neumann::connect_to
		class NeumannFlux : public cmf::water::flux_connection
		{
		protected:
			std::tr1::weak_ptr<NeumannBoundary> m_bc;
			void NewNodes()
			{
				m_bc=std::tr1::static_pointer_cast<NeumannBoundary> (left_node());
			}
			real calc_q(cmf::math::Time t);
		public:
			NeumannFlux(std::tr1::shared_ptr<NeumannBoundary> left,cmf::water::flux_node::ptr right)
				: flux_connection(left,right,"Neumann boundary flux")
			{
				NewNodes();
			}
			
		};
		/// @ingroup connections
		/// Produces a constant but changeable flux from a source to a target, if enough water is present in the source
		///
		/// \f$ q=\left\{0 \mbox{ if }V_{source}\le V_{min}\\ \frac{V_{source} - V_{min}}{t_{decr} q_{0} - V_{min}}\mbox{ if } V_{source} t_{decr} q_{0}\\ q_{0} \mbox{ else}\le \right. \f$
		class TechnicalFlux : public cmf::water::flux_connection
		{
		protected:
			std::tr1::weak_ptr<cmf::water::WaterStorage> source;
			virtual real calc_q(cmf::math::Time t)
			{
				return piecewise_linear(source.lock()->get_state(),MinState,MinState+FluxDecreaseTime.AsDays()*MaxFlux,0,MaxFlux);
			}
			void NewNodes()
			{
				source = cmf::water::WaterStorage::cast(left_node());
			}

		public:
			/// The requested flux \f$q_{0}\left[frac{m^3}{day}\right]\f$
			real MaxFlux;
			/// The minimal volume of the state  \f$V_{min}\left[m^3\right]\f$
			real MinState;
			/// The flux is linearly decreased, if it takes less than FluxDecreaseTime \f$t_{decr}\f$ to reach MinState with MaxFlux
			cmf::math::Time FluxDecreaseTime;

			/// Produces a constant but changeable flux from a source to a target, if enough water is present in the source
			/// @param source The source of the water
			/// @param target The target of the water
			/// @param maximum_flux The requested flux \f$q_{0}\f$
			/// @param minimal_state Minimal volume of stored water in source
			/// @param flux_decrease_time (cmf::math::Time)
			TechnicalFlux(std::tr1::shared_ptr<cmf::water::WaterStorage> & source,std::tr1::shared_ptr<cmf::water::flux_node> target,real maximum_flux,real minimal_state=0,cmf::math::Time flux_decrease_time=cmf::math::h)
				: flux_connection(source,target,"Technical flux"),MaxFlux(maximum_flux),MinState(minimal_state),FluxDecreaseTime(flux_decrease_time) {}
		};




		
	}
	
}
#endif // boundary_condition_h__
