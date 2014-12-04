

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

#include "algorithm.h"
#include "Topology.h"
#include "SoilLayer.h"
#include "../water/flux_node.h"
#include "../water/flux_connection.h"
#include "../atmosphere/precipitation.h"
#include <queue>
#include <set>

using namespace cmf::upslope;
using namespace cmf::water;
using namespace cmf::geometry;
using namespace cmf::math;

Cell* cmf::upslope::find_cell(cells_ref cells,cmf::geometry::point p,double max_dist )
{
	double min_dist=max_dist;
	Cell* res=0;
	for(cell_vector::iterator it = cells.begin(); it != cells.end(); ++it)
	{
		double dist=p.distanceTo(it->get_position());
		if (dist<min_dist)
		{
			min_dist=dist;
			res=it;
		}
	}
	return res;
}

cell_vector cmf::upslope::get_boundary_cells(cells_ref cells)
{
	using namespace cmf;
	using namespace cmf::upslope;
	cell_vector resVector;
	if (cells.size()==0) return resVector;
	//Build a set of upslope cells from Upslope() for faster random access
	std::set<Cell*> upcSet = cells.as_set();

	//Find a cell to start. We use the most western one, which has to be a boundary cell
	Cell* startCell = &cells[0];
	for (cell_vector::iterator it=cells.begin();it!=cells.end();++it)
		if (it->x < startCell->x)
			startCell=it.ptr();

	//From this start cell we are looking for the next neighbor in clockwise direction
	Cell* nextCell=0;
	Cell* thisCell=0;
	//until we are again at the start
	while (startCell!=nextCell)
	{
		//remember the last cell
		Cell* lastCell=thisCell;
		//if the "next" cell is not definied use the start cell
		thisCell=nextCell ? nextCell : startCell;
		//remember the current cell
		resVector.append(*thisCell);
		//the azimuth to the cell from where we are coming (or 0 if it's the first loop)
		double azimuthToLast=lastCell ? thisCell->get_position().azimuth(lastCell->get_position()) : 270;
		//We are looking for the next cell clockwise
		double minAngleDiff=361;
		nextCell=0;
		//Find the next cell in the neighborhood in clockwise direction
		for (neighbor_iterator neighbor = thisCell; neighbor.valid(); ++neighbor)
		{
			// If the connection to the neighbor is active and element of this
			if ((upcSet.find(neighbor->cell)!=upcSet.end()))
			{																						
				double angleDiff=	//the difference angle (in clockwise direction) at thisCell between the last cell and one of the neighbors
					thisCell->get_position().azimuth(neighbor->get_position()) //Azimuth to one of the neighbors
					- azimuthToLast;																	//Minus the Azimuth to the lastCell (0 at start cell)
				//Since neighbors in counter clockwise direction may be negative, we have to solve negative angles (e.g. -90deg->270deg)
				if (angleDiff<=0) angleDiff+=360;
				if (angleDiff<minAngleDiff)
				{
					minAngleDiff=angleDiff;
					nextCell=&(neighbor.cell());
				}
			}
		}
	}
	return resVector;
}


void cmf::upslope::connect_cells_with_flux(cells_ref cells, const CellConnector& connect,int start_at_layer)
{
	std::set<Cell*> cs=cells.as_set();
	int i=0;
	while (cs.size())
	{
		Cell& cell=**cs.begin();
		cs.erase(&cell);
		for (neighbor_iterator it(cell);it.valid();++it)
			if (cs.find(&it.cell())!=cs.end())
				connect(cell,it.cell(),start_at_layer);
	}
}
void insert_connections_in_set(cmf::water::connection_list& cset,cmf::water::flux_node::ptr node)
{
	cmf::water::connection_list cons=node->get_connections();
	for(cmf::water::connection_list::const_iterator it = cons.begin(); it != cons.end(); ++it)
	{
		cset.append(*it);
	}
}
cmf::water::connection_list cmf::upslope::get_connections(cells_ref cells)
{
	using namespace cmf::water;
	cmf::water::connection_list cset;
	//insert_connections_in_set(cset,&Rainfall());
	for(cell_vector::iterator it = cells.begin(); it != cells.end(); ++it)
	{
		//insert_connections_in_set(cset,&it->Evaporation());
		insert_connections_in_set(cset,it->get_surfacewater());
		for (size_t i = 0; i < it->storage_count(); ++i)
		{
			insert_connections_in_set(cset,it->get_storage(i));
		}
		for (size_t i = 0; i < it->layer_count() ; ++i)
		{
			insert_connections_in_set(cset,it->get_layer(i));
		}		 
	}
	return cset;
}


int cmf::upslope::fill_sinks( cells_ref cells,double min_difference/*=0.001*/ )
{
	using namespace cmf::upslope;
	int NoOfActions=0;
	cell_vector boundary=cmf::upslope::get_boundary_cells(cells);
	/// A set of boundary cells, (set::find is faster than vector::find) needed for the alghorithm
	std::set<Cell*> boundarySet=boundary.as_set();


	//Fill sinks according to 
	//Planchon and Darboux, 2002 O. Planchon and F. Darboux, 
	//A fast, simple and versatile algorithm to fill the depressions of digital elevation models, 
  //Catena 46 (2002), pp. 159?176.

	//We need to remember the old height
	std::map<Cell*,double>& oldHeightMap=*new std::map<Cell*,double>;

	//Step 1: Fill the whole DEM with water (without the boundaries)
	for (cell_vector::iterator it=cells.begin();it!=cells.end();++it)
	{
		if (boundarySet.find(it)==boundarySet.end())
		{
			oldHeightMap[it] = it->z;
			it->z=1e17;
		}
	}
	//Step 2: Drain the water, until the sinks are filled
	const double e=min_difference;
	bool continueIncrementation=true;
	while (continueIncrementation)
	{
		continueIncrementation=false;
		//Cycle through all cells
		for (std::map<Cell*,double>::iterator it=oldHeightMap.begin();it!=oldHeightMap.end();it++)
		{
			//Reference variable, its just an alias for the referenced variables, not a memory position itself
			double& 
				Wc=it->first->z,
				Zc=it->second;

			if (Wc > Zc)
			{
				//Loop through the neighbors end exit if operation 1 was performed
				for (neighbor_iterator n_iter(it->first); n_iter.valid() ; ++n_iter)
				{
					double Wn=n_iter->z;
					//Operation 1 applicable?
					if (Zc>=Wn + e)
					{
						//Do operation 1 (If there is a lower new height than the original height of the cell in the neighborhood, we can use the original height again)
						Wc=Zc;
						continueIncrementation=true;
						//nothing else to be done in the neighbor loop
						break;
					}
					//Operation 2 applicable?
					else if (Wc > Wn + e)
					{
						//Do Operation 2 (If there is a lower new height than the actual cell height in the neighborhood, we use that height)
						Wc = Wn + e;
						continueIncrementation=true;
						++NoOfActions;
					}
				} //for Neighbors
			}
		} // For cells in oldHeightMap
	} //As long there's something to do
	delete &oldHeightMap;
	return NoOfActions;

}

cmf::geometry::point_vector cmf::upslope::cell_positions( cells_ref cells )
{
	cmf::geometry::point_vector res(int(cells.size()));
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < (int)cells.size() ; ++i)
	{
		res.set(i,cells[i].get_position());
	}
	return res;
}

cmf::geometry::point_vector cmf::upslope::cell_flux_directions( cells_ref cells,cmf::math::Time t)
{
	cmf::geometry::point_vector res(int(cells.size()));
#pragma omp parallel for
	for (ptrdiff_t i = 0; i < (int)cells.size() ; ++i)
	{
		cmf::geometry::point p;
		Cell & c = cells[i];
		for (ptrdiff_t j = 0; j < ptrdiff_t(c.storage_count()) ; ++j)
			p+=c.get_storage(j)->get_3d_flux(t);
		for (ptrdiff_t j = 0; j < ptrdiff_t(c.layer_count()) ; ++j)
				p+=c.get_layer(j)->get_3d_flux(t);
		res.set(i,p);
	}
	return res;
}

double cmf::upslope::get_area( cells_ref cells )
{
	real sum=0;
	for(cell_vector::iterator it = cells.begin(); it != cells.end(); ++it)
		sum+=it->get_area();
	return sum;
}

cmf::geometry::point cmf::upslope::get_center( cells_ref cells )
{
	cmf::geometry::point center;
	real sum=0;
	for(cell_vector::iterator it = cells.begin(); it != cells.end(); ++it) {
		sum    += it->get_area();
		center += it->get_position() * it->get_area();
	}
	return center/sum;

}

double cmf::upslope::cell_distance( Cell& c1, Cell& c2)
{
	point p1(c1.x,c1.y,c1.z), 
		  p2(c2.x,c2.y,c2.z);
	return p1.distanceTo(p2);
}

cmf::math::num_array cmf::upslope::cell2cellflux( Cell& source,Cell& target,cmf::math::Time t,bool subsurface_only/*=false*/ )
{
	size_t offset=subsurface_only ? 0 : 1;
	num_array res(source.layer_count() + offset,0.0);
	if (!subsurface_only)
		res[0] = source.get_surfacewater()->flux_to(*target.get_surfacewater(),t);
	for(size_t i=0;i<source.layer_count();++i) {
		SoilLayer::ptr l = source.get_layer(i);
		connection_list connections = l->get_connections();
		for(connection_list::const_iterator it=connections.begin();it!=connections.end();++it) {
			flux_connection& conn = **it;
			SoilLayer::ptr l2 = SoilLayer::cast(conn.get_target(*l));
			if (l2) {
				res[offset+i] += l->flux_to(*l2,t);
			}
		}
	}
	return res;

}


inline bool isstream(Topology& t,double area_threshold) {
	return t.ContributingArea()>area_threshold;
}

cmf::upslope::subcatchment::subcatchment(Cell& pourpoint_cell,double area_threshold/*=1e308*/ )
:	pourpoint(pourpoint_cell)
{
	using namespace cmf::upslope;
	// To do lists
	std::queue<Cell*> to_do;
	// Local queue of stream cells (is empty at end of each loop)
	std::queue<Cell*> to_do_stream;

	// Start with pourpoint
	to_do.push(&pourpoint_cell);
					   
	// While there is something to do
	while (to_do.size())	{
		// Get a cell to work on
		Cell* cell = to_do.front();

		// for each neighbor of cell
		for (neighbor_iterator neighbor(*cell); neighbor.valid(); ++neighbor)
		{
			// if cell is mainoutlet of cell
			if (neighbor->MainOutlet() == cell) {
				// if neighbor is a stream cell
				if (isstream(*neighbor,area_threshold)) {
					// add neighbor to stream to do queue
					to_do_stream.push(&neighbor.cell());
				} else {
					// add neighbor to usual to do
					to_do.push(&neighbor.cell());
				}
			}
		}

		// If only one inflowing cell is a stream cell, do not divide subcatchment, but proceed
		if (to_do_stream.size() == 1) {
			// add stream cell to to do queue
			to_do.push(to_do_stream.front());
			// and empty the stream cell queue
			to_do_stream.pop();

		} else if (to_do_stream.size() > 1) { 
			// If more than one inflow cell is stream cell, start a new subcatchments
			while (to_do_stream.size()) {
				inflowcells.append(*to_do_stream.front());
				to_do_stream.pop();
			}
		}
		// add cell to cells of subcatchment
		cells.append(*cell);
		// Work done on cell
		to_do.pop();
	} // end while(to_do)
}
