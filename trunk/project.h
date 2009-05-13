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

namespace cmf {
	class bc_iterator;

	/// The study area, holding all cells and outlets
	class project	: protected cmf::math::StateVariableOwner
	{
	private:
		friend class bc_iterator;
		typedef std::tr1::shared_ptr<cmf::atmosphere::Meteorology> meteo_pointer;
		typedef std::tr1::shared_ptr<cmf::atmosphere::RainfallNode> prec_pointer;
		typedef std::tr1::shared_ptr<cmf::water::FluxNode> node_pointer;
		typedef std::map<std::string,node_pointer> boundary_map;
		prec_pointer m_precipitation;
		meteo_pointer m_meteo;
		upslope::cell_vector m_cells;
		boundary_map m_boundaries;
		friend class cmf::upslope::Cell;
	protected:
		virtual void AddStateVariables(cmf::math::StateVariableVector& vector)
		{
			for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
			{
				(**it).AddStateVariables(vector);   
			}
		}
	public:
		/// If set to true, creation and deletion of objects is logged
		bool debug;
		/// Creates a new project
		project();
		~project();
		/// Returns the actual meteorological data
		cmf::atmosphere::Meteorology & Meteorology() const
		{
			return *m_meteo;
		}
		/// Load a single meteo station for the whole project
		void LoadMeteorology(std::string filename);
		/// Returns the meteorological conditions at a certain point in space and time
		cmf::atmosphere::Weather Weather(cmf::math::Time t, double x,double y,double z) const
		{
			return m_meteo->GetData(t,x,y,z);
		}
		/// Set a new meteorological data object
		void SetMeteorology(const cmf::atmosphere::Meteorology& meteo)
		{
			m_meteo.reset(meteo.copy());
		}
		cmf::atmosphere::RainfallNode & Rainfall() const
		{
			return *m_precipitation;
		}
		/// returns the rainfall intensity at time t and location (x,y,z)
		real Rainfall(cmf::math::Time t, double x,double y,double z) const
		{
			return m_precipitation->q(t,x,y,z);
		}
		/// Set the precipitation data
		void SetPrecipitation(const cmf::atmosphere::PrecipitationData& rainfall)
		{
			m_precipitation->SetSource(rainfall);
		}
		/// Returns the number of cells
		int CellCount() const
		{
			return int(m_cells.size());
		}
		/// Returns a cell 
		cmf::upslope::Cell & Cell(int ndx) const
		{
			if (ndx<0) ndx=int(m_cells.size())+ndx;
			return *m_cells.at(ndx);
		}
		/// Returns the area of the study area
		double cmf::project::Area() const;
		/// Finds a cell at a location (nearest cell center)
		cmf::upslope::Cell* Cell(cmf::geometry::point p,double max_dist=1e20);
		/// Creates a new cell
		cmf::upslope::Cell* NewCell(double x,double y,double z, double Area)
		{
			return new cmf::upslope::Cell(x,y,z,Area,*this);
		}
		/// Creates a new cell
		cmf::upslope::Cell* NewCell(cmf::geometry::point p, double Area)
		{
			return NewCell(p.x,p.y,p.z,Area);
		}
		/// Adds a boundary condition to the project
		void add_boundary_condition(std::string name,cmf::geometry::point Location=cmf::geometry::point());
#ifndef SWIG
		void add_boundary_condition(cmf::water::FluxNode* new_boundary_condition);
#endif
		/// Gets a boundary condition
		cmf::water::FluxNode& get_boundary_condition(std::string name) const;
		int count_boundary_condition() const
		{
			return int(m_boundaries.size());
		}
		operator cmf::upslope::cell_vector&() 
		{
			return m_cells;
		}
		operator const cmf::upslope::cell_vector&() const
		{
			return m_cells;
		}
		/// Deletes all layers in all cells
		void ClearLayers();

	};
	/// A class to iterate through the neighbors of a cell (const). Not needed from the Python side, use the generator cell.neighbors instead.
	class bc_iterator
	{
	private:
		typedef project::boundary_map::iterator map_iterator;
		map_iterator current,end;

	public:
#ifndef SWIG
		/// Returns the current cell (dereference)
		cmf::water::FluxNode& operator*() {
			return *current->second;}
		cmf::water::FluxNode* operator->() {
			return current->second.get();}
		bc_iterator& operator++(){
			this->next();
			return *this;
		}
#endif
		bool operator==(const bc_iterator& cmp) const {return current==cmp.current;}
		bool operator!=(const bc_iterator& cmp) const {return current!=cmp.current;}
		bc_iterator(cmf::project& p) : current(p.m_boundaries.begin()),end(p.m_boundaries.end()) {}
		

		cmf::water::FluxNode& node() {
			return *current->second;}
		bool valid() const
		{
			return current!=end;
		}
		/// Points the iterator to the next neighbor
		cmf::water::FluxNode& next(){
			if (current==end) throw std::out_of_range("No neighbors left");
			cmf::water::FluxNode& res=*current->second;
			++current;
			return res;
		}
	};

	
}
#endif // project_h__
