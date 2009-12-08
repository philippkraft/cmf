#ifndef Precipitation_h__
#define Precipitation_h__
#include "../water/flux_connection.h"
#include "../water/boundary_condition.h"
#include "../water/Solute.h"
#include "../project.h"
#include "../Upslope/cell.h"
#include <tr1/memory>

namespace cmf { 
	namespace atmosphere {
		/// @ingroup nodes
		/// RainCloud is a specialization of NeumannBoundary . The only difference is a reference to the owning cell and the unit conversion of the flux timeseries from mm/day to m3/day
		class RainCloud : public cmf::water::NeumannBoundary
		{
		public:
			typedef std::tr1::shared_ptr<cmf::atmosphere::RainCloud > ptr;
#ifndef SWIG
			operator ptr() {return std::tr1::static_pointer_cast<RainCloud >(shared_from_this());}
#endif
		private:
			friend class cmf::upslope::Cell;
			RainCloud(const cmf::upslope::Cell& _cell) 
				: cmf::water::NeumannBoundary(_cell.project(),0.0,cmf::water::SoluteTimeseries(),cmf::geometry::point(_cell.x,_cell.y,_cell.z+100)), cell(_cell)
			{	 }
			const cmf::upslope::Cell& cell;
		protected:
			virtual real scale_function(real value) const
			{
				return 1e-3*cell.get_area()*flux_scale(value);
			}
		public:
			const cmf::upslope::Cell& get_cell() const
			{
				return cell;
			}
		};

	}
}
#endif // Precipitation_h__
