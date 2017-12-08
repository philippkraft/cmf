

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
#include "cmfmemory.h"
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
		size_t add_node(cmf::water::flux_node::ptr node);

	public:
        /// @brief Returns all state variables of the project. Mostly for internal use.
		cmf::math::StateVariableList get_states(); 
		/// @brief Removes a node from the repository.
		///
		/// Removes a node (boundary condition or water storage) from the node repository
		/// of the project. NOTE: If you have other references to this node, the node is not
		/// deleted. If you are creating a new solver, the node will not be part of the
		/// solver.
		size_t remove_node(cmf::water::flux_node::ptr node);

		/// The solutes transported by the model
		const cmf::water::solute_vector solutes;
		/// @brief The meteorological stations in the project
		cmf::atmosphere::MeteoStationList meteo_stations;
        /// @brief The rainfall stations of the project
		cmf::atmosphere::RainfallStationList rainfall_stations;
        
        /// @brief Uses IDW interpolation to generate meterological data for each cell of project
        ///
        /// Creates a meteo-data source for each cell, using 
        /// spatial interpolated data from all meteorological
        /// stations of the project using Inverse Distance Weighted (IDW) interpolation. 
        /// The meteorolgical value f is calculated with IDW for position x,y,z and time t as follows:
        /// @f{eqnarray*}
        /// f(x,y,z,t) &=& \sum^N_{i=1}{f_i(t) w_i(x,y,z)} \\
        /// w_i(x,y,z) &=& \frac{d_i(x,y,z)^{-p}}{\sum^N_{j=0}{d_j(x,y,z)^{-p}}}  \\
        /// d_i(x,y,z) &=& w_z \left|z-z_i\right| + \sqrt{\left(x-x_i\right)^2 + \left(y-y_i\right)^2} 
        /// @f}
        /// - @f$N@f$ is the number of stations
        /// - @f$f_i(t)@f$ the meteorological value at time t, eg. Temperature, Humidity
        /// - @f$w_i@f$ is the weight of station i
        /// - @f$d_i@f$ is the distance from x,y,z to station i
        /// - @f$p@f$ the power of the weighting function, usually 2.
        /// - @f$x_i,y_i,z_i@f$ is the position of station i in space
        /// - @f$w_z@f$ is a factor to weight the vertical distance between stations and the cell. 0 results
        /// in a pure horizontal interpolation (normal IDW). If @f$w_z=1@f$, height difference is as important as
        /// horizontal distance, and with @f$w_z>1@f$ the height difference is weighted more important than horizontal distance
        /// @see IDW_Meteorology
        /// @param z_weight @f$w_z@f$ the weight of height difference between cell and station
        /// @param power the power of the distance weight
		void use_IDW_meteo(double z_weight=0,double power=2);
        
        /// @brief Connects all cells of the project with its nearest meteorological station.
        ///
        /// Distance is calculated as follows:
        /// @f[d_i(x,y,z) = w_z \left|z-z_i\right| + \sqrt{\left(x-x_i\right)^2 + \left(y-y_i\right)^2} @f]
        /// - @f$d_i@f$ is the distance from x,y,z to station i
        /// - @f$p@f$ the power of the weighting function, usually 2.
        /// - @f$x_i,y_i,z_i@f$ is the position of station i in space
        /// - @f$w_z@f$ is a factor to weight the vertical distance between stations and the cell. 0 results
        /// in a pure horizontal interpolation (normal IDW). If @f$w_z=1@f$, height difference is as important as
        /// horizontal distance, and with @f$w_z>1@f$ the height difference is weighted more important than horizontal distance
        /// @param z_weight @f$w_z@f$ the weight of height difference between cell and station
 		void use_nearest_meteo(double z_weight=0);
        /// @brief Uses IDW interpolation to generate rainfall data for each cell of project
        ///
        /// Creates a rainfall-data source for each cell, using 
        /// spatial interpolated data from all meteorological
        /// stations of the project using Inverse Distance Weighted (IDW) interpolation. 
        /// The rainfall intensity P is calculated with IDW for position x,y,z and time t as follows:
        /// @f{eqnarray*}
        /// P(x,y,z,t) &=& \sum^N_{i=1}{P_i(t) w_i(x,y,z)} \\
        /// w_i(x,y,z) &=& \frac{d_i(x,y,z)^{-p}}{\sum^N_{j=0}{d_j(x,y,z)^{-p}}}  \\
        /// d_i(x,y,z) &=& w_z \left|z-z_i\right| + \sqrt{\left(x-x_i\right)^2 + \left(y-y_i\right)^2} 
        /// @f}
        /// - @f$N@f$ is the number of stations
        /// - @f$P_i(t)@f$ the meteorological value at time t, eg. Temperature, Humidity
        /// - @f$w_i@f$ is the weight of station i
        /// - @f$d_i@f$ is the distance from x,y,z to station i
        /// - @f$p@f$ the power of the weighting function, usually 2.
        /// - @f$x_i,y_i,z_i@f$ is the position of station i in space
        /// - @f$w_z@f$ is a factor to weight the vertical distance between stations and the cell. 0 results
        /// in a pure horizontal interpolation (normal IDW). If @f$w_z=1@f$, height difference is as important as
        /// horizontal distance, and with @f$w_z>1@f$ the height difference is weighted more important than horizontal distance
        /// @see IDW_Meteorology
        /// @param z_weight @f$w_z@f$ the weight of height difference between cell and station
        /// @param power the power of the distance weight
		void use_IDW_rainfall(double z_weight=0,double power=2);
        /// @brief Connects all cells of the project with its nearest rainfall station.
        ///
        /// Distance is calculated as follows:
        /// @f[d_i(x,y,z) = w_z \left|z-z_i\right| + \sqrt{\left(x-x_i\right)^2 + \left(y-y_i\right)^2} @f]
        /// - @f$d_i@f$ is the distance from x,y,z to station i
        /// - @f$p@f$ the power of the weighting function, usually 2.
        /// - @f$x_i,y_i,z_i@f$ is the position of station i in space
        /// - @f$w_z@f$ is a factor to weight the vertical distance between stations and the cell. 0 results
        /// in a pure horizontal interpolation (normal IDW). If @f$w_z=1@f$, height difference is as important as
        /// horizontal distance, and with @f$w_z>1@f$ the height difference is weighted more important than horizontal distance
        /// @param z_weight @f$w_z@f$ the weight of height difference between cell and station
		void use_nearest_rainfall(double z_weight=0);
		/// Returns the cells in the project
		const upslope::cell_vector& get_cells() const {return m_cells;}
		/// Returns the reference to the cell at index in the project
		upslope::Cell& get_cell(ptrdiff_t index)
		{
			return m_cells[index];
		}
		/// The number of cells in the project
		size_t size() const { return m_cells.size();}

		/// @brief Creates a new project
        ///
        /// @param solute_names A string representing the names of the solutes to be used in the project. Sepereate solute names with space.
		project(std::string solute_names="");
		~project();
		
		/// Creates a new cell
        /// @return A new cell, owned by the project
        /// @param x,y,z Position of the cell center in project coordinates (m)
        /// @param area Area of the cell in m^2
        /// @param with_surfacewater If true, the cell will own a surfacewater storage upon creation
		cmf::upslope::Cell* NewCell(double x,double y,double z, double area,bool with_surfacewater = false);
		
		/// Creates a new Dirichlet boundary condition and adds it to the list of outlets
		/// The potential of the Dirichlet boundary equals z, but can be changed
        /// @param name Name of the boundary condition for output
        /// @param x,y,z Position of the boundary condition in project coordinates
		cmf::water::DirichletBoundary::ptr NewOutlet(std::string name,double x=0, double y=0, double z=0);
		
		/// Creates a new generic water storage at position x,y,z. The storage is added to the project nodes
        /// @return A new water storage, owned by the project
        /// @param name Name of the generic water storage for output
        /// @param x,y,z Position of the generic water storage condition in project coordinates
		cmf::water::WaterStorage::ptr NewStorage(std::string name,double x=0, double y=0, double z=0);
		
		/// Creates a new open water storage with a prism geometry. The open water storage is added to the project nodes
        /// @return A new open water storage, owned by the project
        /// @param name  Name of the open water storage for output
        /// @param x,y,z Position of the open water storage in project coordinates
        /// @param area Surface area of the open water storage
		cmf::river::OpenWaterStorage::ptr NewOpenStorage(std::string name,double x, double y, double z,double area);
		
		/// @brief Creates a new Neumann boundary for a target water storage
		///
		/// The boundary is stored with the project
		/// @return A new Neumann boundary
		/// @param name The name of the boundary condition
		/// @param target The water storage to which the Neumann boundary is bound
		cmf::water::NeumannBoundary::ptr NewNeumannBoundary(std::string name, cmf::water::WaterStorage::ptr target);
		/// Creates a new reach
		/// @return A new reach, owned by the project
		/// @param x,y,z Position of the reach in project coordinates
		/// @param shape Crossectional geometry of the river. Any class inheriting from cmf::water::IChannel
		/// @param diffusive If true, this reach uses by default a diffusive wave connection
		cmf::river::Reach::ptr NewReach(double x,double y, double z, cmf::river::IChannel& shape, bool diffusive=false);

		/// Returns the reach at index
		cmf::river::Reach::ptr get_reach(ptrdiff_t index);
		/// Returns the number of reaches in this project
		size_t reach_count() const {return ptrdiff_t(m_reaches.size());}
		
		/// Returns the node from the project nodes at index
		cmf::water::flux_node::ptr get_node(ptrdiff_t index) {
			return m_nodes[index];
		}
		/// Returns the number of nodes saved with this project
		size_t node_count() const {return m_nodes.size();}
		
		/// Returns a list of all storages of this project
		cmf::water::node_list get_storages();
#ifndef SWIG
		cmf::upslope::Cell& operator[](ptrdiff_t index) {
			return this->get_cell(index);
		}
#endif

	};


	
}
#endif // project_h__
