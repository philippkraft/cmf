

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
#ifndef project_h__
#define project_h__
#include <tr1/memory>
#include <vector>
#include <set>
#include "atmosphere/meteorology.h"
#include "atmosphere/precipitation.h"
#include "geometry/geometry.h"
#include "upslope/cell.h"
#include "reach/Reach.h"
#include "upslope/Soil/RetentionCurve.h"
#include "water/collections.h"
#include "upslope/cell_vector.h"
/// The main namespace of the model framework. Contains the other namespaces and the project class
namespace cmf {
	class bc_iterator;

	/// @brief The study area, holding all cells, outlets and streams
	/// \todo Where the fuck do you describe those stinkin tracers? They should be here!
	class project	: public cmf::math::StateVariableOwner
	{
	private:
		friend class cmf::upslope::Cell;
		cmf::upslope::cell_vector m_cells;
		std::vector<cmf::river::Reach::ptr> m_reaches;

	public:
		cmf::math::state_queue get_states(); 

		/// The solutes transported by the model
		const cmf::water::solute_vector solutes;
		/// The meteorological stations in the project
		cmf::atmosphere::MeteoStationList meteo_stations;
		cmf::atmosphere::RainfallStationList rainfall_stations;

		void use_IDW_meteo(double z_weight=0,double power=2);
		void use_nearest_meteo(double z_weight=0);
		void use_IDW_rainfall(double z_weight=0,double power=2);
		void use_nearest_rainfall(double z_weight=0);
		/// The outlets of the model system
		cmf::water::node_list outlets;
		/// Returns the vector of cells in the project
		const upslope::cell_vector& get_cells() const {return m_cells;}
		/// Returns the reference to the cell at index in the project
		upslope::Cell& get_cell(int index)
		{
			return m_cells[index];
		}
		/// The number of cells in the project
		int size() const { return int(m_cells.size());}
		/// If set to true, creation and deletion of objects is logged
		bool debug;
		/// Creates a new project
		project(std::string solute_names="");
		~project();
		/// Creates a new cell
		cmf::upslope::Cell* NewCell(double x,double y,double z, double Area)
		{
			cmf::upslope::Cell* new_cell=new cmf::upslope::Cell(x,y,z,Area,*this);
			m_cells.append(*new_cell);
			return new_cell;
		}
		/// Creates a new Drichelet boundary condition and adds it to the list of outlets
		/// The potential of the Drichelet boundary equals p.z
		cmf::water::DricheletBoundary::ptr NewOutlet(std::string name,double x, double y, double z);

		/// Returns the reach at index
		cmf::river::Reach::ptr get_reach(int index);
		/// Returns the number of reaches in this project
		int reach_count() const {return int(m_reaches.size());}
		/// Returns any storages of this project
		cmf::water::node_list get_storages();
		/// Creates a new reach
		cmf::river::Reach::ptr NewReach(double x,double y, double z, double length, char Type='T',double width=0.5,double depth=0.1, bool diffusive=false);

	};


	
}
#endif // project_h__
