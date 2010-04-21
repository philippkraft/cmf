

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
#include <tr1/memory>
#include "../math/num_array.h"
#include "../math/statevariable.h"

namespace cmf {
	class project;
	namespace water {

		class flux_connection;
		class node_list;
		typedef std::vector<cmf::water::flux_connection* > connection_vector;
		typedef std::set<cmf::water::flux_connection* > connection_set;

		/// @defgroup nodes	Water nodes
		/// @todo Elaborate on Water nodes
		
		/// Base class for everything that can be connected by fluxes. 
		/// @ingroup nodes
		///
		/// Flux nodes can be WaterStorages, flux end points, sinks, sources and 
		/// bridges to other model domains (e.g. Ponded water to river system).
		/// The base class can be used where a simple routing, potentially with mixing,
		/// is needed.
		class flux_node 
#ifndef SWIG
			: public std::tr1::enable_shared_from_this<flux_node> 
#endif
		{
		public:
			typedef std::tr1::shared_ptr<cmf::water::flux_node> ptr;
#ifndef SWIG
			operator ptr()		{	return shared_from_this();	}
#endif
		private:
			void RegisterConnection(flux_connection* newConnection);
			void DeregisterConnection(flux_connection* target);
			friend class flux_connection;
			typedef std::map<int, std::tr1::shared_ptr<flux_connection> > ConnectionMap;
			ConnectionMap m_Connections;
			const cmf::project& m_project;
		protected:
			cmf::math::Time m_LastQuery;

		public:

			/// Returns the project, this node is part of
			const cmf::project& project() const {return m_project;}
			/// The Id of the node
			const int node_id;
			/// true, if this is a waterstorage
			virtual bool is_storage() const {return false;}
			/// The Name of this node
			std::string Name;
			virtual std::string to_string() const {return "{" + Name + "}";}
			cmf::water::connection_vector get_connections() const;

			//cmf::geometry::point get_direction_to(const flux_node& cmp);

			/// The destructor deletes all connections
			virtual ~flux_node();
			bool operator ==(const cmf::water::flux_node& other) const {return this==&other;}

			/// Pure flux_nodes do not influence fluxes, therefore no recalculation of fluxes is required by flux_node.
			/// WaterStorage overrides this, since state changes require an update of the fluxes
			virtual bool RecalcFluxes(cmf::math::Time t) {return false;}
			/// Returns the connection between this and target
			cmf::water::flux_connection* get_connection(const cmf::water::flux_node& target);
			/// Remove the connection
			bool remove_connection(cmf::water::flux_node::ptr To);
			/// Returns the actual flux between this and target	(positive sign means "from target into this")
			real flux_to( const cmf::water::flux_node& target,cmf::math::Time t );
			cmf::geometry::point flux3d_to(const cmf::water::flux_node& target,cmf::math::Time t );
			cmf::geometry::point get_3d_flux(cmf::math::Time t);
			/// Returns the sum of all fluxes (positive and negative) at time t. Single fluxes can be excluded from the calculation
			/// @param t Time of the query
			/// @param Without A flux_connection that is excluded from the water_balance (e.g. to prevent closed circuits)
			real water_balance(cmf::math::Time t,const flux_connection* Without=0) const;
			/// Returns the water quality of the flux_node, if it is not overridden this is the mix of the incoming fluxes
			virtual real conc(cmf::math::Time t, const cmf::water::solute& Solute) const;
			cmf::geometry::point Location;
			/// Returns the water potential of the node in m waterhead
			/// The base class water storage always returns the height of the location
			virtual real get_potential() const
			{
				return Location.z;
			}
			virtual void set_potential(real new_potential)
			{
				throw std::runtime_error("Potential of " + Name + " is read only");
			}
			virtual bool is_empty() const
			{
				return true;
			}
			
			flux_node(const cmf::project& _project,cmf::geometry::point location=cmf::geometry::point());
			
		};


		int count_node_references(flux_node::ptr node);

		flux_node::ptr get_higher_node(flux_node::ptr node1,flux_node::ptr node2);
		flux_node::ptr get_lower_node(flux_node::ptr node1,flux_node::ptr node2);


	}

}
#endif // flux_node_h__
