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

    /// @ brief The study area, holding all cells and outlets
    /// @ brief The study area, holding all cells and outlets
	class project	: public cmf::math::StateVariableOwner
	{
	private:
		friend class cmf::upslope::Cell;
		upslope::cell_vector m_cells;

	protected:
		virtual void AddStateVariables(cmf::math::StateVariableVector& vector)
		{
			for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
			{
				(**it).AddStateVariables(vector);   
			}
		}
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

		std::vector<cmf::river::Reach_ptr> reaches;
		cmf::river::Reach_ptr NewReach(cmf::river::Channel shape, bool diffusive=false);

	};

	
}
#endif // project_h__
