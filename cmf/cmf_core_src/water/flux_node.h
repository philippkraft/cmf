

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
#ifndef flux_node_h__
#define flux_node_h__

#include "../math/time.h"
#include "../math/real.h"
#include "../geometry/geometry.h"
#include "Solute.h"
#include <map>
#include <vector>
#include <set>
#include <string>
#include <stdexcept>
#include "../cmfmemory.h"
#include <algorithm>

#include "../math/num_array.h"
#include "../math/statevariable.h"

namespace cmf {
	class project;
	namespace water {

		class flux_connection;
		class connection_list;
		class node_list;

		/// @defgroup nodes	Water nodes
		/// @todo Elaborate on Water nodes

		/// @ingroup nodes
		/// @brief Base class for everything that can be connected by fluxes. 
		///
		/// Flux nodes can be WaterStorages, flux end points, sinks, sources and 
		/// bridges to other model domains (e.g. Ponded water to river system).
		/// The base class can be used where a simple routing, potentially with mixing,
		/// is needed.
		class flux_node 
#ifndef SWIG
			: public std::enable_shared_from_this<flux_node> 
#endif
		{
		public:
			/// Shortcut to the shared pointer
			typedef std::shared_ptr<cmf::water::flux_node> ptr;
		private:
			// Pointer for a connection
			typedef std::shared_ptr<flux_connection> con_ptr;
			// Registers a newly build connection
			void RegisterConnection(flux_connection* newConnection);
			// Deregisters a connection, and kills it
			void DeregisterConnection(flux_connection* target);
			// Allows the flux_connection class to call thereregister / deregister themselfs
			friend class flux_connection;
			// allows the waterbalance_integrator to call the water balance without refreshing the connection
			friend class waterbalance_integrator;

			// The mapping of the connections. The id is holding the node id's
			typedef std::map<int, con_ptr > ConnectionMap;
			ConnectionMap m_Connections;

			// Returns the waterbalance (sum of all current fluxes) without updating the calculated fluxes
			double water_balance_without_refresh() const;
			// The project this node belongs to
			cmf::project& m_project;

		public:
			/// @brief Returns the project, this node is part of
			cmf::project& get_project() const {return m_project;}
			/// @brief The Id of the node
			const int node_id;
			/// @brief true, if this is a waterstorage
			virtual bool is_storage() const {return false;}
			/// @brief The Name of this node
			std::string Name;
			virtual std::string to_string() const {return "{" + Name + "}";}
			cmf::water::connection_list get_connections() const;

			/// The destructor deletes all connections
			virtual ~flux_node();
			bool operator ==(const cmf::water::flux_node& other) const {return this==&other;}

			/// @brief Pure flux_nodes do not influence fluxes, therefore no recalculation of fluxes is required by flux_node.
			/// WaterStorage overrides this, since state changes require an update of the fluxes
			virtual bool RecalcFluxes(cmf::math::Time t) {return false;}
			/// @brief Returns the connection between this and target
			cmf::water::flux_connection* connection_to(const cmf::water::flux_node& target);
			/// @brief Remove the connection
			bool remove_connection(cmf::water::flux_node::ptr To);
			/// @brief Returns the actual flux between this and target	(positive sign means "from this into target")
			real flux_to( const cmf::water::flux_node& target,cmf::math::Time t );
			cmf::geometry::point flux3d_to(const cmf::water::flux_node& target,cmf::math::Time t );
			/// @brief Returns the sum of all flux vectors
			cmf::geometry::point get_3d_flux(cmf::math::Time t);
			/// @brief Returns the sum of all fluxes (positive and negative) at time t. 
            ///
            /// Single fluxes can be excluded from the calculation
			/// @param t Time of the query
			/// @param Without A flux_connection that is excluded from the waterbalance (e.g. to prevent closed circuits)
			real waterbalance(cmf::math::Time t,const flux_connection* Without=0) const;
            /// @brief returns the waterblance
			real operator()(cmf::math::Time t) const {
				return waterbalance(t);
			}
			/// @brief Returns the water quality of the flux_node, if it is not overridden this is the mix of the incoming fluxes
			virtual real conc(cmf::math::Time t, const cmf::water::solute& Solute) const;
			
			/// @brief The spatial position of the node
			cmf::geometry::point position;
			/// @brief Returns the water potential of the node in m waterhead
            ///
			/// The base class water storage always returns the height of the location
			virtual real get_potential() const
			{
				return position.z;
			}
            /// @brief Sets the potential of this flux node
			virtual void set_potential(real new_potential)
			{
				throw std::runtime_error("Potential of " + Name + " is read only");
			}
			/// @brief Returns true if the node has no water.
			virtual double is_empty() const
			{
				return 1;
			}
			
			flux_node(cmf::project& _project,cmf::geometry::point location=cmf::geometry::point());
			
		};


		int count_node_references(flux_node::ptr node);

		flux_node::ptr get_higher_node(flux_node::ptr node1,flux_node::ptr node2);
		flux_node::ptr get_lower_node(flux_node::ptr node1,flux_node::ptr node2);

		/// @brief The waterbalance_integrator is an integratable for precise output of the average water balance 
		/// of a flux_node over time. It can be added to a solver (any cmf::math::Integrator), 
		/// which is than calling the integrate method at each substep.
		class waterbalance_integrator : public cmf::math::integratable {
		private:
			double _sum;
			cmf::math::Time _start_time;
			cmf::math::Time _t;
			std::weak_ptr<flux_node> _node;
			std::string _name;

		public:
			/// Returns the total absolute waterbalance of the node in integration time [m3]
			double sum() const {
				return _sum;
			}
			/// Returns the duration of the integration
			cmf::math::Time integration_t() const {
				return _t-_start_time;
			}
			/// Returns the start time of the integration
			cmf::math::Time t0() const { return _start_time; }

			/// Returns the average flux over the integration time in m3/day
			double avg() const;
			/// Initializes the integration
			void reset(cmf::math::Time t) {
				_start_time = t;
				_t=_start_time;
				_sum=0.0;
			}
			/// Returns the node of this integrator
			flux_node::ptr get_node() const {
				return _node.lock();
			}
			void set_node(cmf::water::flux_node::ptr node) {
				_sum=0;
				_start_time=cmf::math::Time();
				_t=_start_time;
				_node =  node;
			}
			/// Integrates the flux a timestep further. Note: until is an absolut time. If until is before t0, the integration is initilized again
			void integrate(cmf::math::Time until);
			waterbalance_integrator(cmf::water::flux_node::ptr node) 
				:	_node(node), _sum(0.0), _t(cmf::math::year*5000), _name(node->to_string()+ " (Integrator)") {}
		};
/*
		class ChildNodes;
		class node_ref{
			friend class ChildNodes;
		private:
			struct node_ref_inner {
				flux_node* fn;
				node_ref_inner(flux_node* node) : fn(node) {}
			};
			typedef std::shared_ptr<node_ref_inner> inner_ptr;
			inner_ptr m_ref;
			node_ref(flux_node* fn) : m_ref(inner_ptr(new node_ref_inner(fn))) {}
			void kill_node() {
				delete m_ref->fn;
				m_ref->fn=0;
			}
		public:
			node_ref(const node_ref& from) : m_ref(from.m_ref) {}
			node_ref& operator=(const node_ref& from) {
				m_ref = from.m_ref;
			}
			flux_node& operator*() const{
				if (m_ref->fn) {
					return *(m_ref->fn);
				} else {
					throw std::runtime_error("Node does not exist anymore");
				}
			}
			flux_node* operator->() const {
				if (m_ref->fn) {
					return m_ref->fn;
				} else {
					throw std::runtime_error("Node does not exist anymore");
				}
			}
			operator bool() const {
				return m_ref->fn!=0;
			}
			bool operator==(const node_ref& cmp) const {
				return m_ref->fn == cmp.m_ref->fn;
			}
			bool operator!=(const node_ref& cmp) const {
				return m_ref->fn != cmp.m_ref->fn;
			}
		};
		class ChildNodes {
		private:
			typedef std::vector<node_ref> ChildNodeVector;
			ChildNodeVector m_v;
			friend class flux_node;
			int add_node(flux_node* fn) {
				node_ref nr = fn;
				return add_ref(nr);
			}
			int add_ref(const node_ref& nr) {
				m_v.push_back(nr);
				return m_v.size();
			}
			ChildNodeVector::iterator find(node_ref fn) {
				ChildNodeVector::iterator it = m_v.begin();
				for(; it != m_v.end(); ++it)
					if (*it == fn)
						break;
				return it;
			}
		public:
			int kill_child(node_ref fn) {
				ChildNodeVector::iterator it = find(fn);
				if (it==m_v.end()) {
					throw std::invalid_argument(fn->to_string() + " is not part of this owner");
				} else {
					m_v.erase(it);
					fn.kill_node();
				}
				return m_v.size();
			}
			bool is_child(flux_node* fn) {
				ChildNodeVector::iterator it = find(fn);
				return it!=m_v.end();
			}
			int transfer_node(node_ref fn,ChildNodes& target) {
				ChildNodeVector::iterator it = find(fn);
				if (it==m_v.end()) {
					throw std::invalid_argument(fn->to_string() + " is not part of this owner");
				} else {
					m_v.erase(it);
					target.add_ref(fn);
				}
				return m_v.size();
			}
			~ChildNodes() {
				for(ChildNodeVector::iterator it = m_v.begin(); it != m_v.end(); ++it)
				{
					it->kill_node();
				}
			}
		};
*/


	}

}
#endif // flux_node_h__
