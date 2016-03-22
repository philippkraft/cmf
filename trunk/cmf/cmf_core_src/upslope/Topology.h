

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
#ifndef Topology_h__
#define Topology_h__

#include "cell.h"
#include "algorithm.h"
#include "../cmfmemory.h"
#include <map>
#include <vector>
#include <set>
namespace cmf {
	namespace upslope {
		class neighbor_iterator;
		/// represents the connectivity of cells to each other
		class Topology
		{
		private:
			friend class neighbor_iterator;
			friend class neighbor_const_iterator;
			friend class Cell;
			typedef std::map<Topology*,double> mapNeighbor;
			mapNeighbor m_Neighbors;
			double m_CatchmentSize;
			Cell* m_MainOutlet;
			Topology(Cell * _cell) 
				: cell(_cell),m_CatchmentSize(0),m_MainOutlet(0),x(_cell->x),y(_cell->y),z(_cell->z)
			{			}
			double distance_to(const Topology& cmp) const
			{
				using namespace cmf::geometry;
				return point::distance(point(x,y,z),point(cmp.x,cmp.y,cmp.z));
			}
			double slope_to(const Topology& cmp) const
			{
				return (cmp.z-z)/distance_to(cmp);
			}

		public:
			/// The cell, this topology belongs to
			Cell * const cell;
			/// References cell.x
			double & x;
			/// References cell.y
			double & y;
			/// References cell.z
			double & z;
			/// Returns the center of the cell
			cmf::geometry::point get_position() const {return cmf::geometry::point(x,y,z);}
			/// @brief returns the flow width between this cell and the target cell. Returns 0 if no topology is defined
			double flowwidth(Cell & target)
			{
				return flowwidth(target.get_topology());
			}
			double flowwidth(Topology & target)
			{
				mapNeighbor::const_iterator it_n = m_Neighbors.find(&target);
				if (it_n==m_Neighbors.end())
					return 0;
				else
					return it_n->second;
			}

			/// @brief Adds a neighbor cell to the topology with flowwidth
			void AddNeighbor(Cell & target,double flowwidth)
			{
				AddNeighbor(target.get_topology(),flowwidth);
			}
			void AddNeighbor(Topology & target,double flowwidth);

			/// Removes the topological relation to the given cell
			void RemoveNeighbor(Topology & target);

			/// Returns the number of neighbors
			size_t neighbor_count() const { return m_Neighbors.size();}

			/// Returns the mainoutlet (steepest lower neighbor)
			Cell * MainOutlet(bool forceRecalc=false);

			/// Get the contributing area (steepest path upwards)
			double ContributingArea() const;

			/// Calculates the contributing area for a bunch of cells
			static void calculate_contributing_area(const cmf::upslope::cell_vector&);
			bool operator==(const Topology& cmp)
			{
				return &cell==&cmp.cell;
			}

		};
		/// A class to iterate through the neighbors of a cell (const). Not needed from the Python side, use the generator cell.neighbors instead.
		class neighbor_iterator
		{
		private:
			typedef Topology::mapNeighbor::iterator map_iterator;
			map_iterator current,end;

		public:
#ifndef SWIG
			/// Returns the current cell (dereference)
			Topology& operator*() {
				return *current->first;}
			Topology* operator->() {
				return current->first;}
			neighbor_iterator& operator++(){
				return next();
			}
			neighbor_iterator(cmf::upslope::Topology& topo) : current(topo.m_Neighbors.begin()),end(topo.m_Neighbors.end()) {}
			bool operator==(const neighbor_iterator& cmp) const {return current==cmp.current;}
			bool operator==(const Cell* cmp) const {return current->first->cell == cmp;}
			bool operator!=(const neighbor_iterator& cmp) const {return current!=cmp.current;}
#endif

			neighbor_iterator(cmf::upslope::Cell* cell) : current(cell->get_topology().m_Neighbors.begin()),end(cell->get_topology().m_Neighbors.end()) {}

			Cell& cell() {
				return *current->first->cell;}
			double flowwidth() {
				return current->second;}
			bool valid() const
			{
				return current!=end;
			}
			/// Points the iterator to the next neighbor
			neighbor_iterator& next(){
				if (current==end) throw std::out_of_range("No neighbors left");
				++current; 
				return *this;
			}
		};

		
	}
	
}
#endif // Topology_h__
