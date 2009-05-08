#include "project.h"
#include "upslope/SoilWaterStorage.h"
#include "Upslope/Topology.h"
#include <set>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <algorithm>
cmf::upslope::Cell* cmf::project::Cell( cmf::geometry::point p,double max_dist/*=1e20*/ )
{
	return find_cell(m_cells,p,max_dist);
}

// void cmf::project::SetVegetation(cmf::maps::IMap<cmf::upslope::vegetation::Vegetation> & vegmap )
// {
// 	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
// 	{
// 		cmf::upslope::Cell & cell=**it;
// 		cell.Vegetation=vegmap(cell.Center());
// 	}
// }
// 
// void cmf::project::AddCellsFromDEM( cmf::geometry::Raster<double> DEM )
// {
// 	using namespace cmf::upslope;
// 	//Number of cells
// 	int 
// 		ncols=DEM.ColumnCount(),
// 		nrows = DEM.RowCount();
// 
// 	//Create a 2Dim array for referencing the new Cells
// 	upslope::Cell** CellArray =new upslope::Cell* [ncols*nrows];
// 	///Calculate area from each cell
// 	double 
// 		area=DEM.XCellsize()*DEM.YCellsize(),
// 		fwstraight=0.5 * 0.5*(DEM.XCellsize()+DEM.YCellsize()),
// 		fwdiagonal=0.354 * 0.5 * (DEM.XCellsize()+DEM.YCellsize());
// 	//Cycle through all the rows of the raster
// 	int countUpcExist=0;							// Number of existing UpslopeCells
// 
// 	for (int row=0;row<nrows;row++)
// 	{
// 		//Calculate real world coordinate from the row number
// 		double y= DEM.Yllcorner()+DEM.Height()-(row+0.5)*DEM.YCellsize();
// 		//Cycle through all columns of the raster
// 		for (int col=0;col<ncols;col++)
// 		{
// 			//Create UpslopeCell only if DEM-Value is not NO DATA
// 			if (DEM.HasData(col,row))
// 			{
// 				//Calculate real world coordinate from the row number
// 				double x=DEM.Xllcorner()+(col+0.5)*DEM.XCellsize();
// 				//Get terrain height from DEM
// 				double z=DEM.IdentifyColRow(col,row);
// 				//New Upslope cell
// 				CellArray[col+row*ncols]=NewCell(x,y,z,area);
// // 				CellArray[col+row*ncols]->geometry=cmf::geometry::BoundingBox(
// // 																			 x-DEM.XCellsize()*0.5,y-DEM.YCellsize()*0.5,
// // 																			 x+DEM.XCellsize()*0.5,y+DEM.YCellsize()*0.5);
// 				CellArray[col+row*ncols]->Id = col+row*ncols+1;
// 
// 			}
// 			//Null-Pointer for cells without data (CellValue=raster.NoData())
// 			else CellArray[col+row*ncols]=0;
// 		} //next col
// 	} // next row
// 
// 	//Add Neighbors to the cells 
// 	int xpos[] = {-1,0,1,-1,1,-1,0,1}; //relative x Positions of the 8 Neighbors
// 	int ypos[] = {-1,-1,-1,0,0,1,1,1}; //relative y Position of the 8 Neighbors
// 
// #pragma omp parallel for
// 	for (int row=0;row<nrows;row++)
// 	{
// 		//Cycle through all columns of the raster
// 		for (int col=0;col<ncols;col++)
// 		{
// 			//The index of the current cell (col,row)
// 			int curCell=col+row*ncols;
// 			//Cycle through the 8 adjectent positions of the current cell (col,row)
// 			for (int pos=0;pos<8;pos++)
// 			{
// 				//Position of the current neighbor in the Cell-Array
// 				int neighCell=col+xpos[pos]+(row+ypos[pos])*ncols;
// 				//If the current cell exists
// 				if (CellArray[curCell] &&
// 					//and also the possible Neighbor is not out of the raster bounds
// 					col+xpos[pos]>=0 && col+xpos[pos]<ncols && row+ypos[pos]>=0 && row+ypos[pos]<nrows &&
// 					//and also the possible neighbor exists
// 					CellArray[neighCell])
// 					//add the neighboring cell to the Neighbor Collection of the current cell
// 					CellArray[curCell]->get_topology().AddNeighbor(*CellArray[neighCell],xpos[pos] && ypos[pos] ? fwdiagonal : fwstraight);
// 			} //Next position
// 		} // Next col
// 	}
// 	delete[] CellArray;
// }
// 
// void cmf::project::AddCellsFromPolygons( const cmf::geometry::Polygons & polygons,const cmf::geometry::Points & centers,double slither_tolerance/*=0.1*/ )
// {
// 	using namespace cmf::upslope;
// 	using namespace cmf::geometry;
// 	for (int i = 0; i < int(polygons.size()) ; ++i)
// 	{
// 		upslope::Cell* cell=NewCell(centers[i],polygons[i].Area());
// 		cell->Id=i+1;
// // 		cell->geometry=polygons[i];
// 	}
// #pragma omp parallel for
// 	for (int i = 0; i < (int)polygons.size() ; ++i)
// 	{
// 		for (int j = 0; j < int(polygons.size()) ; ++j)
// 		{
// 			if (i!=j)
// 			{
// 				Line l=polygons[i][0].SharedBoundary(polygons[j][0], slither_tolerance*0.1).MaxStraight();
// 				if (l.size()>0)
// 				{
// 					double dist=l[0].distanceTo(l[-1]);
// 					if (dist>slither_tolerance)
// 						m_cells[i]->get_topology().AddNeighbor(*m_cells[j],dist);
// 				}
// 			}
// 		}
// 	}
// }
// 
// void cmf::project::AddFlexibleSaturatedZone( cmf::maps::IMap<cmf::upslope::RCurve> & r_curve_map,cmf::maps::IMap<double> & soildepth_map )
// {
// 	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
// 	{
// 		cmf::upslope::Cell & cell=**it;
// 		cmf::upslope::FlexibleSizeSaturatedZone::Create(cell,soildepth_map(cell.Center()),r_curve_map(cell.Center()));
// 	}
// }
// 
// void cmf::project::AddFlexibleSaturatedZone( cmf::maps::IMap<cmf::upslope::RCurve> & r_curve_map,double soildepth/*=1.0*/ )
// {
// 	cmf::maps::SingleValueMap<double> sdmap(soildepth);			
// 	AddFlexibleSaturatedZone(r_curve_map,sdmap);
// }
// 
// void cmf::project::AddLayers( cmf::maps::IMap<cmf::upslope::Profile> & r_curve_map,cmf::maps::IMap<double> & soildepth_map )
// {
// 	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
// 	{
// 		cmf::upslope::Cell & cell=**it;
// 		const cmf::upslope::Profile& prof=r_curve_map(cell.Center());
// 		double max_depth=soildepth_map(cell.Center());
// 		double d=0;
// 		for (int i = 0; i < prof.size() ; ++i)
// 		{
// 			if (prof.upperboundary(i)<max_depth)
// 			{
// 				cmf::upslope::SoilWaterStorage::Create(cell,i<prof.size()-1 ? minimum(prof.lowerboundary(i),max_depth) : max_depth,prof.r_curve(i));
// 			}
// 		}
// 	}
// }
// void cmf::project::AddLayers( cmf::maps::IMap<cmf::upslope::Profile> & r_curve_map )
// {
// 	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
// 	{
// 		cmf::upslope::Cell & cell=**it;
// 		const cmf::upslope::Profile& prof=r_curve_map(cell.Center());
// 		for (int i = 0; i < prof.size() ; ++i)
// 		{
// 			cmf::upslope::SoilWaterStorage::Create(cell,prof.lowerboundary(i),prof.r_curve(i));
// 		}
// 	}
// }
// 
void cmf::project::ClearLayers()
{
	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
	{
		cmf::upslope::Cell & cell=**it;
		cell.RemoveLayers();
	}
}

cmf::project::project() :	debug(false)
{
	m_meteo =meteo_pointer(new cmf::atmosphere::ConstantMeteorology);
	m_precipitation=prec_pointer(new cmf::atmosphere::RainfallNode(*this));
	m_precipitation->Name="Precipitation";
}

double cmf::project::Area() const
{
	double _area=0;
#pragma omp parallel for reduction(+ : _area)
	for (int i = 0; i < (int)CellCount() ; i++)
	{
		_area+=Cell(i).Area();
	}
	return _area;
}

cmf::project::~project()
{
	for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		delete *it;
	m_cells.clear();
}
void insert_connections_in_set(std::set<cmf::water::FluxConnection*>& cset,cmf::water::FluxNode* node)
{
	cmf::water::connection_vector cons=node->Connections();
	for(cmf::water::connection_vector::const_iterator it = cons.begin(); it != cons.end(); ++it)
	{
	    cset.insert(*it);
	}
}
cmf::water::connection_set cmf::project::get_connections() const
{
	using namespace cmf::water;
	std::set<FluxConnection*> cset;
	insert_connections_in_set(cset,&Rainfall());
	for(cmf::upslope::cell_vector::const_iterator it = m_cells.begin(); it != m_cells.end(); ++it)
	{
		cmf::upslope::Cell& c=**it;
		insert_connections_in_set(cset,&c.Evaporation());
		insert_connections_in_set(cset,&c.SurfaceWater());
		for (int i = 0; i < c.StorageCount(); ++i)
		{
			insert_connections_in_set(cset,&c.GetStorage(i));
		}
		for (int i = 0; i < c.LayerCount() ; ++i)
		{
			insert_connections_in_set(cset,&c.Layer(i));
		}		 
	}
	return cset;
}

void cmf::project::AddOutlet( std::string Name,cmf::geometry::point Location/*=cmf::geometry::point()*/ )
{
	cmf::water::FluxNode* node=new cmf::water::FluxNode(*this,Location);
	m_boundaries.push_back(node);
}

cmf::water::FluxNode& cmf::project::GetOutlet( int index )
{
	return *m_boundaries.at(index>0 ? index : m_boundaries.size()+index);
}

void cmf::project::LoadMeteorology( std::string filename )
{
	cmf::atmosphere::SingleMeteorology* newmeteo=new cmf::atmosphere::SingleMeteorology(filename);
	m_meteo.reset(newmeteo);
}
cmf::upslope::Cell* cmf::find_cell( cmf::upslope::cell_vector cells,geometry::point p,double max_dist )
{
	double min_dist=max_dist;
	cmf::upslope::Cell* res;
	for(cmf::upslope::cell_vector::const_iterator it = cells.begin(); it != cells.end(); ++it)
	{
		double dist=p.distanceTo((*it)->Center());
		if (dist<min_dist)
		{
			min_dist=dist;
			res=*it;
		}
	}
	return res;
}
cmf::upslope::cell_vector cmf::boundary_cells(const cmf::upslope::cell_vector& cells)
{
	using namespace cmf;
	using namespace cmf::upslope;
	cell_vector resVector;
	if (cells.size()==0) return resVector;
	//Build a set of upslope cells from Upslope() for faster random access
	cell_set upcSet(cells.begin(),cells.end());

	//Find a cell to start. We use the most western one, which has to be a boundary cell
	Cell* startCell=cells[0];
	for (cell_vector::const_iterator it=cells.begin();it!=cells.end();it++)
		if ((*it)->Center().x < startCell->Center().x)
			startCell=*it;

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
		resVector.push_back(thisCell);
		//the azimuth to the cell from where we are coming (or 0 if it's the first loop)
		double azimuthToLast=lastCell ? thisCell->Center().azimuth(lastCell->Center()) : 270;
		//We are looking for the next cell clockwise
		double minAngleDiff=361;
		nextCell=0;
		//Find the next cell in the neighborhood in clockwise direction
		for (NeighborIterator neighbor = thisCell; neighbor.valid(); ++neighbor)
		{
			// If the connection to the neighbor is active and element of this
			if ((upcSet.find(neighbor->cell)!=upcSet.end()))
			{																						
				double angleDiff=	//the difference angle (in clockwise direction) at thisCell between the last cell and one of the neighbors
					thisCell->Center().azimuth(neighbor->Center()) //Azimuth to one of the neighbors
					- azimuthToLast;																	//Minus the Azimuth to the lastCell (0 at start cell)
				//Since neighbors in counter clockwise direction may be negative, we have to solve negative angles (e.g. -90°->270°)
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


void cmf::connect_cells_with_flux(cell_vector cells, const cmf::upslope::CellConnector& connect,int start_at_layer)
{
	cmf::upslope::cell_set cs(cells.begin(),cells.end());
	while (cs.size())
	{
		cmf::upslope::Cell& cell=**cs.begin();
		cs.erase(&cell);
		for (cmf::upslope::NeighborIterator it=cell;it.valid();++it)
			if (cs.find(&it.cell())!=cs.end())
				connect(cell,it.cell(),start_at_layer);
	}
}
