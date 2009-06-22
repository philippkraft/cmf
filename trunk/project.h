#ifndef project_h__
#define project_h__
#include <tr1/memory>
#include <vector>
#include <set>
#include "Atmosphere/Meteorology.h"
#include "Atmosphere/Precipitation.h"
#include "Geometry/geometry.h"
#include "Upslope/cell.h"
#include "Upslope/Soil/RetentionCurve.h"
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
		project();
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

	};
	/// A class to iterate through the neighbors of a cell (const). Not needed from the Python side, use the generator cell.neighbors instead.
// 	class bc_iterator
// 	{
// 	private:
// 		typedef project::boundary_map::iterator map_iterator;
// 		map_iterator current,end;
// 
// 	public:
// #ifndef SWIG
// 		/// Returns the current cell (dereference)
// 		cmf::water::FluxNode& operator*() {
// 			return *current->second;}
// 		cmf::water::FluxNode* operator->() {
// 			return current->second.get();}
// 		bc_iterator& operator++(){
// 			this->next();
// 			return *this;
// 		}
// #endif
// 		bool operator==(const bc_iterator& cmp) const {return current==cmp.current;}
// 		bool operator!=(const bc_iterator& cmp) const {return current!=cmp.current;}
// 		bc_iterator(cmf::project& p) : current(p.m_boundaries.begin()),end(p.m_boundaries.end()) {}
// 		
// 
// 		cmf::water::FluxNode& node() {
// 			return *current->second;}
// 		bool valid() const
// 		{
// 			return current!=end;
// 		}
// 		/// Points the iterator to the next neighbor
// 		cmf::water::FluxNode& next(){
// 			if (current==end) throw std::out_of_range("No neighbors left");
// 			cmf::water::FluxNode& res=*current->second;
// 			++current;
// 			return res;
// 		}
// 	};

	
}
#endif // project_h__
