

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
#ifndef reach_h__
#define reach_h__
#include "../water/flux_connection.h"
#include "../water/WaterStorage.h"
#include "ReachType.h"
#include "../math/real.h"
#include "../upslope/cell.h"
#include "ManningConnection.h"
#include <queue>

namespace cmf {
	class project;

	namespace river {
		class Reach;
		/// @ingroup storages
		/// @brief A reach represents the section of a riover and is a specialization of an open water storage.
		///
		/// The OpenWaterStorage attributes and methods are extended by topological features, for the creation 
		/// of a network of reaches.
		class Reach : public OpenWaterStorage
		{
		public:
			typedef std::shared_ptr<Reach> ptr;
#ifndef SWIG
			operator ptr() {return std::static_pointer_cast<Reach>(shared_from_this());}
#endif
		private:

			/// Creates a reach
			Reach(cmf::project& _project,const IChannel& shape, bool diffusive=false);

			// Typedefs using weak pointers
			typedef std::weak_ptr<Reach> weak_reach_ptr;
			typedef std::vector<weak_reach_ptr> reach_vector;

			weak_reach_ptr weak_this;
			// The upstream reaches
			reach_vector m_upstream;

			// The downstream reach
			weak_reach_ptr m_downstream;
			typedef std::weak_ptr<cmf::water::flux_node> weak_node_ptr;
			weak_node_ptr m_outlet;

			// adds an upstream reach (only called by set_downstream)
			bool add_upstream(ptr r);
			// Removes an upstream reach (only called by set_downstream and the destructor)
			bool remove_upstream(const Reach* r);
			// Returns the type of relation to a soil
			bool m_diffusive;

		public:
			/// Returns the channel shape
			Channel get_reachtype() const;
			/// Sets the channel shape
			void set_height_function(const IChannel& val);
			/// Returns the length of the reach
			real get_length() const;

			/// Returns the average width of the reach
			real get_width() const;
			
			
			/// Connects the reach to an outlet, e.g. a boundary condition
			void set_outlet(cmf::water::flux_node::ptr outlet);
			/// Deletes any downstream connection
            void set_dead_end();
			/// Connects the reach to another one downstream
			void set_downstream(ptr new_downstream,bool use_meanchannel=false);
			/// Returns the reach downstream of this (or null if there is no reach downstream)
			cmf::water::flux_node::ptr get_downstream() const;

			
			/// Returns the number of reaches upstream of this
			int upstream_count() const {return int(m_upstream.size());}
			/// Returns a reach upstream of this
			ptr get_upstream(int index) const;
			/// Connects the surfacewater of a cell with this reach
			/// @param cell The cell with the surface water to be connected with this reach
			/// @param width The flow width from cell to this reach
			/// @param diffusive If diffusive is false, a ManningKinematic connection is used, else a ManningDiffusive connection
			void connect_to_surfacewater( cmf::upslope::Cell* cell, real width,bool diffusive );
			/// Returns the distance (d) for connections between this reach and a cell. 
			/// If the effective inner cell distance (defined as \f$ R_{Cell} = 0.5\frac{\sqrt{A}}{\pi}\f$) is smaller 
			/// than the distance between the center points, the cell radius is returned
			double distance_to_cell(cmf::upslope::Cell* cell) const;
			/// Returns if flow is calculated using a diffusive wave model
			bool get_diffusive() const {return m_diffusive;}
			/// Sets all Manning kind connections to either diffusive or kinematic
			void set_diffusive(bool use_diffusive_wave);
			/// Returns the reach most downstream from this reach			
			ptr get_root();
			
			virtual ~Reach();
			static ptr create(cmf::project& project,const cmf::river::IChannel& shape, bool diffusive=false);
			
		};




		/// An iterator over every upstream reach from a start reach. Implements both the Python and the C++ iterator interface
		/// Usage C++:
		/// @code
		/// ptr r;
		/// for (ReachIterator it(r);it.valid();++it) { ... }
		/// @endcode
		/// Usage Python:
		/// @code
		/// for reach in ReachIterator(r):
		///     pass
		/// @endcode
		class ReachIterator {
		private:
			typedef std::pair<Reach::ptr,double> queue_value;
			std::queue<queue_value> reach_queue;
			queue_value current;
		public:
			/// Returns the next reach in the upstream queue. 
			Reach::ptr next();
			/// Returns true, if reaches are left to iterate over
			bool valid() const;
			/// Creates a ReachIterator from a first reach.
			ReachIterator(Reach::ptr first);
			
#ifndef SWIG
			Reach::ptr operator->() const;
			void operator++();
#endif
			/// Returns the current reach
			Reach::ptr reach() const;
			/// Returns the distance to the root reach
			double position() const;
		};

		/// Ensures that rivers have a monotone downward flow direction. 
		/// Reaches with a bottom higher than any upstream reach are lowered to the minimum height of any (possibly distant) upstream reach.
		double make_river_gap(Reach::ptr root_reach);

	}
}
#endif // reach_h__
