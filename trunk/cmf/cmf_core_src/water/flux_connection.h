

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
#ifndef flux_connection_h__
#define flux_connection_h__
#include "../math/time.h"
#include "../math/real.h"
#include "../geometry/geometry.h"
#include "Solute.h"
#include <map>
#include <vector>
#include <set>
#include <string>
#include "flux_node.h"
#include <stdexcept>
#include <tr1/memory>
namespace cmf {
	class project;
	namespace water {

		/// @defgroup connections Flux connections
		///
		/// The connections between the nodes (boundary conditions, storages) of the water network
		/// The connections in cmf hold the processes for the calculation of fluxes between water storages and model boundaries
		/// @todo Elaborate on this

		/// @ingroup connections
		/// Represents a connection between flux_nodes, where water fluxes occur. 
		class flux_connection 
		{
		protected:
			typedef std::tr1::weak_ptr<flux_node> weak_flux_node_ptr;
		private:
			friend class flux_node;
			friend class flux_integrator;
			static int nextconnectionid;
			flux_connection(const flux_connection& copy):connection_id(nextconnectionid) {
				throw std::runtime_error("Never copy construct a flux_connection");
			}
			std::tr1::weak_ptr<flux_connection> weak_this;
			weak_flux_node_ptr m_left;
			weak_flux_node_ptr m_right;
			real m_tracer_filter;
		protected:
			virtual void NewNodes()=0;
			bool RecalcAlways;
			/// this function needs to be overridden by realizations of connections
			virtual real calc_q(cmf::math::Time t) = 0;
			// Connection stores q to prevent unnecessary recalculations
			real m_q; // Positive q means flux left to right
			// returns the actual flux, recalculates it, if one of the two flux nodes requires it
			real q(cmf::math::Time t) {
				if (RecalcAlways || m_left.lock()->RecalcFluxes(t) || m_right.lock()->RecalcFluxes(t))
					m_q=calc_q(t);
				return m_q;
			}

			double prevent_negative_volume(double flow) {
				if (flow>0)
					return flow * (1 - left_node()->is_empty());
				else
					return flow * (1 - right_node()->is_empty());
			}


		public:
			typedef std::tr1::shared_ptr<flux_connection> ptr;
#ifndef SWIG
			ptr get_ptr() const {
				return weak_this.lock();
			}
			
#endif
			/// Returns the left node of this connection
			flux_node::ptr left_node() const {return m_left.expired() ? flux_node::ptr() : flux_node::ptr(m_left);}
			/// returns the right node of this connection
			flux_node::ptr right_node() const {return m_right.expired() ? flux_node::ptr() : flux_node::ptr(m_right);}
			
			/// Deregisters this connection from its nodes. Returns true if only one reference is left.
			bool kill_me();


			const int connection_id;
			
			bool operator==(const cmf::water::flux_connection& other) {return connection_id==other.connection_id;}
			
			/// Returns the other end of a connection than the asking end
			flux_node::ptr get_target(const flux_node& inquirer);
			/// With index 0, the left node is returned, with index 1 the right node of the connection
			flux_node::ptr get_target(int index) const;
			
			void exchange_target(flux_node::ptr oldtarget,flux_node::ptr newTarget);
			
			
			/// Returns the current flux through a connection. Negative signs mean out of the inquirer, positive are inflows to the inquirer
			real q(const flux_node& inquirer,cmf::math::Time t) {
				return inquirer==*left_node() ? -q(t) 
					: inquirer==*right_node() ? q(t) : 0;  
			}
			
			/// Returns the concentration of the flux. If not overridden,
			/// it returns the concentration of the source of the flux (direction depending)
			real conc(cmf::math::Time t, const cmf::water::solute& _Solute);
			/// A string describing the type of the connection
			const std::string type;
			/// A value ranging from 0 to 1 
			real get_tracer_filter() { return m_tracer_filter;}
			void set_tracer_filter(real value) { 
				if (value<0 || value>1) throw std::runtime_error("Tracer filter must be between 0 and 1");
				m_tracer_filter = value;
			}
			std::string to_string() const;
			std::string short_string() const;
			/// Creates a flux connection between the flux_node left and right
			/// @param left flux_node on the one side of the connection
			/// @param right flux_node on the other side of the connection
			/// @param _type Type of the flux connection
			flux_connection(flux_node::ptr left,flux_node::ptr right,std::string _type);

			virtual ~flux_connection();
		};

		int replace_node(cmf::water::flux_node::ptr oldnode,cmf::water::flux_node::ptr newnode);
		

		/// The flux_integrator is an integratable for precise output of average fluxes over time. It can be added to
		/// solver (any cmf::math::Integrator), which is than calling the integrate method at each substep.
		class flux_integrator : public cmf::math::integratable {
		private:
			double _sum;
			cmf::math::Time _start_time;
			cmf::math::Time _t;
			std::tr1::weak_ptr<flux_connection> _connection;
			std::string _name;

		public:
			/// Returns the amount of water along this connection in the integration time in m3
			double sum() const {
				return _sum;
			}
			/// Returns the duration of the integration
			cmf::math::Time integration_t() const {
				return _t-_start_time;
			}
			/// Returns the start time of the integration
			cmf::math::Time t0() const { return _start_time; }

			/// Returns the average flux of the integration time in m3/day
			double avg() const;
			/// Initializes the integration
			void reset(cmf::math::Time t);
			/// Returns the flux_connection
			flux_connection::ptr connection() const;
			/// Integrates the flux a timestep further. Note: until is an absolute time. If until is before t0, the integration is initilized again
			void integrate(cmf::math::Time until);
			/// Creates a flux_integrator from an connection
			flux_integrator(cmf::water::flux_connection& connection);
			/// Creates a flux_integrator from the endpoints of a connection. Throws if there is no connection between the endpoints
			flux_integrator(cmf::water::flux_node::ptr left, cmf::water::flux_node::ptr right);
		};


		/// @ingroup connections
		/// Routes the sum of all other fluxes to a target
		class waterbalance_connection : public flux_connection
		{
		protected:
			real calc_q(cmf::math::Time t) 
			{
				real q = left_node()->waterbalance(t,this);
				if (q > 0) return q;
				else return 0.0;
			}
			void NewNodes() {}
		public:
			waterbalance_connection(flux_node::ptr source,flux_node::ptr target) 
				: flux_connection(source,target,"waterbalance connection")
			{
				RecalcAlways=true;
			}
		};

		/// @ingroup connections
		/// Flux from one node to another, controlled by the user or an external program, 
		/// by changing the flux constant
		class external_control_connection : public flux_connection {
		protected:
			real calc_q(cmf::math::Time t)	{
				return flux;
			}
			void NewNodes() {}
		public:
			real flux;
			external_control_connection(flux_node::ptr source,flux_node::ptr target,real flux_value=0) 
				: flux_connection(source,target,"external controlled connection"),flux(flux_value)			
			{
				RecalcAlways=true;
			}
		};
		/// @ingroup connections
		/// Sets a constant flux between two nodes, if an external_control_connection exists. 
		// Otherwise an error is thrown
		void set_flux(flux_node::ptr source,flux_node::ptr target,real flux_value);
		
		/// @ingroup connections
		/// Checks if a constant flux between two nodes can be set. Returns true
		/// if the nodes are connected by an external_control_connection
		bool can_set_flux(flux_node::ptr source,flux_node::ptr target);



	}
}

#endif // flux_connection_h__
