

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
#include "Atmosphere/Meteorology.h"
#include "Atmosphere/Precipitation.h"
#include "Geometry/geometry.h"
#include "Upslope/cell.h"
#include "Reach/reach.h"
#include "Upslope/Soil/RetentionCurve.h"
#include "water/collections.h"
/// The main namespace of the model framework. Contains the other namespaces and the project class
namespace cmf {
	class bc_iterator;

  /// @brief The study area, holding all cells, outlets and streams
	class project	: public cmf::math::StateVariableOwner
	{
	private:
		friend class cmf::upslope::Cell;
		upslope::cell_vector m_cells;
		std::vector<cmf::river::Reach_ptr> m_reaches;

	protected:
		virtual void AddStateVariables(cmf::math::StateVariableVector& vector);
	public:
		const cmf::water::solute_vector solutes;
		cmf::atmosphere::MeteoStationList meteo_stations;
		cmf::water::node_list outlets;
		const upslope::cell_vector& get_cells() const {return m_cells;}
		upslope::Cell& get_cell(int index)
		{
			return *m_cells.at(index<0 ? m_cells.size()+index : index);
		}
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
			m_cells.push_back(new_cell);
			return new_cell;
		}
		/// Creates a new cell
		cmf::upslope::Cell* NewCell(cmf::geometry::point p, double Area)
		{
			return NewCell(p.x,p.y,p.z,Area);
		}
		/// Creates a new Drichelet boundary condition and adds it to the list of outlets
		/// The potential of the Drichelet boundary equals p.z
		cmf::water::DricheletBoundary::ptr NewOutlet(std::string name,cmf::geometry::point p);


		cmf::river::Reach_ptr get_reach(int index);
		int reach_count() const {return int(m_reaches.size());}

		cmf::water::node_list get_storages();
		/// Creates a new reach
		cmf::river::Reach_ptr NewReach(cmf::river::Channel shape, bool diffusive=false);

	};


	
}
#endif // project_h__
