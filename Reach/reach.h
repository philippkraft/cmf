#ifndef reach_h__
#define reach_h__
#include "../water/FluxConnection.h"
#include "../water/WaterStorage.h"
#include "VolumeHeightRelation.h"
#include "ReachType.h"
#include "../math/real.h"
#include "../Upslope/cell.h"
#include "ManningConnection.h"
#include <queue>

namespace cmf {
	namespace river {
		class ReachIterator;
		/// A combination of a open water storage with channel base geometry and a cell
		class Reach
		{
		private:
			friend class cmf::upslope::Cell;
			std::auto_ptr<cmf::river::ReachType> channel_shape;
			std::vector<Reach*> upstream;
			bool add_upstream(Reach * r);
			bool remove_upstream(Reach* r);
			cmf::river::OpenWaterStorage * water;
			Reach * downstream;
			/// Creates a new reach object, by adding an OpenWaterStorage to the cell. Connects the surfacewater of the reach automatically with the river
			Reach(cmf::upslope::Cell& Cell,real Length,cmf::river::ReachType* rt,cmf::river::OpenWaterStorage* waterstorage,real depth,real width);
		public:
			void set_downstream(Reach* downstream_reach);
			void set_outlet(cmf::water::FluxNode& outlet);
			void set_dead_end() {set_downstream(0);}
			/// Returns the cell this reach is related to
			/// Gets the water storage of the cell
			cmf::river::OpenWaterStorage& get_water() {return *water;}
			/// Holds the length of the reach
			real length;
			/// Returns the reaches this one is connected to. The vector is sorted by height
			Reach& get_upstream(int index)
			{
				return *upstream.at(index<0 ? upstream.size()+index : index);
			}
			int upstream_count() const {return int(upstream.size());}
			Reach* get_downstream() const { return downstream;}
			Reach* get_root() 
			{
				if (downstream)
					return downstream->get_root();
				else
					return this;
			}
			bool operator<(const cmf::river::Reach& cmp) const;
			bool operator==(const cmf::river::Reach& cmp) const;
			bool operator!=(const cmf::river::Reach& cmp) const;
			operator cmf::river::OpenWaterStorage&();
		};
		/// An iterator over every upstream reach from a start reach. Implements both the Python and the C++ iterator interface
		class ReachIterator {
		private:
			std::queue<Reach*> reach_queue;
		public:
			/// Returns the next reach in the upstream queue. 
			Reach* next()
			{
				if (reach_queue.empty()) throw std::out_of_range("No reaches in queue");
				for (int i = 0; i < reach_queue.front()->upstream_count() ; ++i)
				{
					reach_queue.push(&reach_queue.front()->get_upstream(i));
				}
				Reach* result=reach_queue.front();
				reach_queue.pop();
				return result;
			}
			/// Returns true, if reaches are left to iterate over
			bool valid() const
			{
				return !reach_queue.empty();
			}
			/// Creates a ReachIterator from a first reach.
			ReachIterator(Reach* first) 
			{
				reach_queue.push(first);
			}
			
#ifndef SWIG
			Reach* operator->() const
			{
				return reach_queue.front();
			}
			void operator++()
			{
				next();
			}
#endif
			/// Returns the current reach
			Reach* reach() const
			{
				return reach_queue.front();
			}
		};
		/// Ensures that rivers have a monotone downward flow direction. 
		/// Reaches with a bottom higher than any upstream reach are lowered to the minimum height of any (possibly distant) upstream reach.
		double make_river_gap(Reach * root_reach);

	}
}
#endif // reach_h__
