#ifndef Topology_h__
#define Topology_h__

#include "cell.h"
#include <memory>
#include <map>
#include <vector>
#include <set>
namespace cmf {
	namespace upslope {
		class NeighborIterator;
		/// represents the connectivity of cells to each other
		class Topology : public cmf::geometry::Locatable
		{
		private:
			friend class NeighborIterator;
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
			void AddNeighbor(Cell & target,double flowwidth)
			{
				AddNeighbor(target.get_topology(),flowwidth);
			}
			void AddNeighbor(Topology & target,double flowwidth)
			{
				m_Neighbors[&target]=flowwidth;
			}
			void RemoveNeighbor(Topology & target)
			{
				if (m_Neighbors.find(&target)!=m_Neighbors.end())
				{
					m_Neighbors.erase(&target);
					target.RemoveNeighbor(*this);
				}
			}
			size_t neighbor_count() const { return m_Neighbors.size();}
			Cell * MainOutlet(bool forceRecalc=false);

			double ContributingArea() const;
			static void calculate_contributing_area(const cmf::upslope::cell_vector&);
			bool operator==(const Topology& cmp)
			{
				return &cell==&cmp.cell;
			}

		};
		/// A class to iterate through the neighbors of a cell (const). Not needed from the Python side, use the generator cell.neighbors instead.
		class NeighborIterator
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
			NeighborIterator& operator++(){
				return next();
			}
			NeighborIterator(cmf::upslope::Topology& topo) : current(topo.m_Neighbors.begin()),end(topo.m_Neighbors.end()) {}
			bool operator==(const NeighborIterator& cmp) const {return current==cmp.current;}
			bool operator==(const Cell* cmp) const {return current->first->cell == cmp;}
			bool operator!=(const NeighborIterator& cmp) const {return current!=cmp.current;}
#endif

			NeighborIterator(cmf::upslope::Cell* cell) : current(cell->get_topology().m_Neighbors.begin()),end(cell->get_topology().m_Neighbors.end()) {}

			Cell& cell() {
				return *current->first->cell;}
			double flowwidth() {
				return current->second;}
			bool valid() const
			{
				return current!=end;
			}
			/// Points the iterator to the next neighbor
			NeighborIterator& next(){
				if (current==end) throw std::out_of_range("No neighbors left");
				++current; 
				return *this;
			}
		};

		
	}
	
}
#endif // Topology_h__
