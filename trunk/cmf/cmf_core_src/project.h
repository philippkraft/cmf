

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
	/// \todo Describe tracers
	class project	: public cmf::math::StateVariableOwner
	{
	private:
		friend class cmf::upslope::Cell;
		cmf::upslope::cell_vector m_cells;
		std::vector<cmf::river::Reach::ptr> m_reaches;
		cmf::water::node_list m_nodes;
		int add_node(cmf::water::flux_node::ptr node);

	public:
		cmf::math::StateVariableList get_states(); 
		/// @brief Removes a node from the repository.
		///
		/// Removes a node (boundary condition or water storage) from the node repository
		/// of the project. NOTE: If you have other references to this node, the node is not
		/// deleted. If you are creating a new solver, the node will not be part of the
		/// solver.
		int remove_node(cmf::water::flux_node::ptr node);

		/// The solutes transported by the model
		const cmf::water::solute_vector solutes;
		/// The meteorological stations in the project
		cmf::atmosphere::MeteoStationList meteo_stations;
		cmf::atmosphere::RainfallStationList rainfall_stations;

		void use_IDW_meteo(double z_weight=0,double power=2);
		void use_nearest_meteo(double z_weight=0);
		void use_IDW_rainfall(double z_weight=0,double power=2);
		void use_nearest_rainfall(double z_weight=0);
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
		cmf::upslope::Cell* NewCell(double x,double y,double z, double area,bool with_surfacewater = false);
		/// Creates a new Dirichlet boundary condition and adds it to the list of outlets
		/// The potential of the Dirichlet boundary equals p.z, but can be changed
		cmf::water::DirichletBoundary::ptr NewOutlet(std::string name,double x, double y, double z);
		/// Creates a new generic water storage at position x,y,z. The storage is added to the project nodes
		cmf::water::WaterStorage::ptr NewStorage(std::string name,double x, double y, double z);
		/// Creates a new open water storage with a prism geometry. The open water storage is added to the project nodes
		cmf::river::OpenWaterStorage::ptr NewOpenStorage(std::string name,double x, double y, double z,double area);
		/// Creates a new reach
		cmf::river::Reach::ptr NewReach(double x,double y, double z, double length, char Type='T',double width=0.5,double depth=0.1, bool diffusive=false);

		/// Returns the reach at index
		cmf::river::Reach::ptr get_reach(int index);
		/// Returns the number of reaches in this project
		int reach_count() const {return int(m_reaches.size());}
		
		/// Returns the node from the project nodes at index
		cmf::water::flux_node::ptr get_node(int index) {
			return m_nodes[index];
		}
		/// Returns the number of nodes saved with this project
		int node_count() const {return int(m_nodes.size());}
		
		/// Returns all storages of this project
		cmf::water::node_list get_storages();
#ifndef SWIG
		cmf::upslope::Cell& operator[](int index) {
			return this->get_cell(index);
		}
#endif

	};


	
}
#endif // project_h__
