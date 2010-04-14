

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
#ifndef Precipitation_h__
#define Precipitation_h__
#include "../water/flux_connection.h"
#include "../water/boundary_condition.h"
#include "../water/Solute.h"
#include "../project.h"
#include "../upslope/cell.h"
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
