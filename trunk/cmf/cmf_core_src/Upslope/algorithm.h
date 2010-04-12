

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
#ifndef algorithm_h__
#define algorithm_h__
#include "cell.h"
#include "../math/num_array.h"
#include "../Atmosphere/Meteorology.h"
#include "../Geometry/geometry.h"
namespace cmf {
	namespace upslope {
		typedef const cell_vector&  cells_ref;


		cmf::upslope::Cell* find_cell(cells_ref,cmf::geometry::point p,double max_dist=1e20);
		cmf::upslope::cell_vector get_boundary_cells(cmf::upslope::cells_ref cells);
		cmf::water::connection_set get_connections(cells_ref cells);
		/// Connects all adjacent cells with a flux connection, implementing the CellConnector protocol
		void connect_cells_with_flux(cells_ref cells, const cmf::upslope::CellConnector& connect,int start_at_layer=0);
		int fill_sinks(cells_ref cells,double min_difference=0.001);
		double area(cells_ref cells);
		void set_meteo_station(cmf::upslope::cells_ref cells,cmf::atmosphere::MeteoStation::ptr meteo_station);
		void set_precipitation(cmf::upslope::cells_ref cells,cmf::math::timeseries data_in_mm_day);

		cmf::geometry::point_vector cell_positions(cmf::upslope::cells_ref cells);
		cmf::geometry::point_vector cell_flux_directions(cmf::upslope::cells_ref cells,cmf::math::Time);


	}
	
}
#endif // algorithm_h__
