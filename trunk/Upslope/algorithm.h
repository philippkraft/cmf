#ifndef algorithm_h__
#define algorithm_h__
#include "cell.h"

namespace cmf {
	namespace upslope {
		typedef const cell_vector&  cells_ref;
		cmf::upslope::Cell* find_cell(cells_ref,cmf::geometry::point p,double max_dist=1e20);
		cmf::upslope::cell_vector boundary_cells(cells_ref cells);
		cmf::water::connection_set get_connections(cells_ref cells);
		/// Connects all adjacent cells with a flux connection, implementing the CellConnector protocol
		void connect_cells_with_flux(cells_ref cells, const cmf::upslope::CellConnector& connect,int start_at_layer=0);
		int fill_sinks(cells_ref cells,double min_difference=0.001);
		double area(cells_ref cells);
		
	}
	
}
#endif // algorithm_h__
