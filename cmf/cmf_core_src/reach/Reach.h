

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
#include "../Upslope/cell.h"
#include "ManningConnection.h"
#include <queue>

namespace cmf {
	class project;

	namespace river {
		class Reach;
		typedef std::tr1::shared_ptr<Reach> Reach_ptr;
		/// @ingroup storages
		/// A reach is a specialization of an open water storage.
		///
		/// The OpenWaterStorage attributes and methods are extended by topological features, for the creation 
		/// of a network of reaches.
		class Reach : public OpenWaterStorage
		{
		public:
#ifndef SWIG
			typedef std::tr1::shared_ptr<Reach> ptr;
			operator ptr() {return std::tr1::static_pointer_cast<Reach>(shared_from_this());}
#endif
		private:

			/// Creates a reach
			Reach(const cmf::project& _project,const IChannel& shape, bool diffusive=false)
				: OpenWaterStorage(_project,shape), m_diffusive(diffusive)
			{

			}

			// Typedefs using weak pointers
			typedef std::tr1::weak_ptr<Reach> weak_reach_ptr;
			typedef std::vector<weak_reach_ptr> reach_vector;

			weak_reach_ptr weak_this;
			// The upstream reaches
			reach_vector m_upstream;

			// The downstream reach
			weak_reach_ptr m_downstream;
			Channel m_shape;

			// A private type to store the attributes of the relationship to cells
			struct cell_relation {
				bool soil_cut; // If true, the soil is cutted by the reach with a depth of cell.z - this->Location.z
				real distance; // Avg. distance from cell to reach
				real width;    // Width of the relation
				cell_relation(bool _soil_cut=false,real _dist=-10.0, real _width=0.0) : soil_cut(_soil_cut),distance(_dist),width(_width) {}
				operator bool() const {return this->distance>0;}
			};
			
			// typedef for relation map
			typedef std::map<cmf::upslope::Cell*, cell_relation> cell_map;
			
			// The cell/reach relation map
			cell_map m_cells;
			// adds an upstream reach (only called by set_downstream)
			bool add_upstream(Reach_ptr r);
			// Removes an upstream reach (only called by set_downstream and the destructor)
			bool remove_upstream(const Reach* r);
			// Returns the type of relation to a soil
			cell_relation intersects(cmf::upslope::Cell* cell) const
			{
				cell_map::const_iterator it=m_cells.find(cell);
				return it!=m_cells.end() ? it->second : cell_relation();
			}
			bool m_diffusive;

		public:
			/// Returns the channel shape
			const IChannel& get_height_function() const;
			/// Sets the channel shape
			void set_height_function(const IChannel& val) {m_shape=val;}
			/// Returns the length of the reach
			real get_length() const
			{
				return get_height_function().length;
			}

			/// Connects the reach to an outlet, e.g. a boundary condition
			void set_outlet(cmf::water::flux_node::ptr outlet);
			void set_dead_end();
			/// Connects the reach to another one downstream
			void set_downstream(Reach_ptr new_downstream);
			/// Returns the reach downstream of this (or null if there is no reach downstream)
			Reach_ptr get_downstream() const;
			/// Returns the number of reaches upstream of this
			int upstream_count() const {return int(m_upstream.size());}
			/// Returns a reach upstream of this
			Reach_ptr get_upstream(int index) const;
			/// Returns the cells connected with this
			cmf::upslope::cell_vector get_cells() const;
			/// Connects the surfacewater of a cell with this reach
			/// @param cell The cell with the surface water to be connected with this reach
			/// @param soil_cut True, if this reach might have inflow through the bank
			/// @param distance The average distance from cell to this reach
			/// @param width The flow width from cell to this reach
			void add_cell(cmf::upslope::Cell* cell, bool soil_cut, real distance,real width);
			/// True if bank flow is possible
			bool cuts_soil_of(cmf::upslope::Cell* cell) const
			{
				return intersects(cell).soil_cut;
			}
			/// The average distance from cell to this reach
			real get_distance_to(cmf::upslope::Cell* cell) const {
				return intersects(cell).distance;
			}
			/// The bank length in the cell
			real get_bank_length_with(cmf::upslope::Cell* cell) const {
				return intersects(cell).width;
			}
			bool get_diffusive() const {return m_diffusive;}
			/// Sets all Manning kind connections to either diffusive or kinematic
			void set_diffusive(bool use_diffusive_wave);
			/// Returns the reach most downstream from this reach			
			Reach_ptr get_root();
			
			virtual ~Reach();
			static ptr create(const cmf::project& project,cmf::river::Channel shape, bool diffusive=false);
			
		};



		/// An iterator over every upstream reach from a start reach. Implements both the Python and the C++ iterator interface
		/// Usage C++:
		/// @code
		/// Reach_ptr r;
		/// for (ReachIterator it(r);it.valid();++it) { ... }
		/// @endcode
		/// Usage Python:
		/// @code
		/// for reach in ReachIterator(r):
		///     pass
		/// @endcode
		class ReachIterator {
		private:
			typedef std::pair<Reach_ptr,double> queue_value;
			std::queue<queue_value> reach_queue;
			queue_value current;
		public:
			/// Returns the next reach in the upstream queue. 
			Reach_ptr next();
			/// Returns true, if reaches are left to iterate over
			bool valid() const;
			/// Creates a ReachIterator from a first reach.
			ReachIterator(Reach_ptr first);
			
#ifndef SWIG
			Reach_ptr operator->() const;
			void operator++();
#endif
			/// Returns the current reach
			Reach_ptr reach() const;
			/// Returns the distance to the root reach
			double position() const;
		};

		/// Ensures that rivers have a monotone downward flow direction. 
		/// Reaches with a bottom higher than any upstream reach are lowered to the minimum height of any (possibly distant) upstream reach.
		double make_river_gap(Reach_ptr root_reach);

	}
}
#endif // reach_h__
