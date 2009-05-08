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
#include "Geometry/maps.h"
#include "Geometry/Raster.h"
namespace cmf {
	cmf::upslope::Cell* find_cell(cmf::upslope::cell_vector cells,cmf::geometry::point p,double max_dist=1e20);
	cmf::upslope::cell_vector boundary_cells(const cmf::upslope::cell_vector& cells);
	/// Connects all adjacent cells with a flux connection, implementing the CellConnector protocol
	void connect_cells_with_flux(cell_vector cells, const cmf::upslope::CellConnector& connect,int start_at_layer=0);

	/// The study area, holding all cells and outlets
	class project	: public cmf::math::StateVariableOwner
	{
	private:
		typedef std::tr1::shared_ptr<cmf::atmosphere::Meteorology> meteo_pointer;
		meteo_pointer m_meteo;
		typedef std::tr1::shared_ptr<cmf::atmosphere::RainfallNode> prec_pointer;
		prec_pointer m_precipitation;
		upslope::cell_vector m_cells;
		water::node_vector m_boundaries;
		friend class cmf::upslope::Cell;
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
		void AddOutlet(std::string Name,cmf::geometry::point Location=cmf::geometry::point());
		/// Gets a boundary condition
		cmf::water::FluxNode& GetOutlet(int index);
		int OutletCount() const
		{
			return int(m_boundaries.size());
		}
		/// Sets new vegetation data to the cells
		void SetVegetation(cmf::maps::IMap<cmf::upslope::vegetation::Vegetation> & vegmap);
// 		/// Creates a cell mesh from a squared dem
// 		void AddCellsFromDEM(cmf::geometry::Raster<double> dem);
// 		/// Creates a cell mesh from a squared dem, loaded from an ASCII file in ESRI format
// 		void AddCellsFromDEM(std::string ASCFileName)
// 		{
// 			cmf::geometry::Raster<double> dem(ASCFileName);
// 			AddCellsFromDEM(dem);
// 		}
// 		/// Creates a cell mesh from polygons (depreciated)
// 		void AddCellsFromPolygons(const cmf::geometry::Polygons & polygons,const cmf::geometry::Points & centers,double slither_tolerance=0.1);
// 		/// Adds a flexible saturated zone to all cells using the soil depth map
// 		void AddFlexibleSaturatedZone(cmf::maps::IMap<cmf::upslope::RCurve> & r_curve_map,cmf::maps::IMap<double> & soildepth_map);
// 		/// Adds a flexible saturated zone to all cells using a constant soil depth
// 		void AddFlexibleSaturatedZone(cmf::maps::IMap<cmf::upslope::RCurve> & r_curve_map,double soildepth=1.0);
// 		/// Adds layers from soil profile map and an additional soil depth map
// 		void AddLayers(cmf::maps::IMap<cmf::upslope::Profile> & r_curve_map,cmf::maps::IMap<double> & soildepth_map);
// 		/// Adds layers from soil profile map (soil depth derived from the profile depth)
// 		void AddLayers(cmf::maps::IMap<cmf::upslope::Profile> & r_curve_map);
// 		/// Returns all connections in this project
// 		cmf::water::connection_set get_connections() const;
		/// Deletes all layers in all cells
		void ClearLayers();
#ifndef SWIG
		virtual void AddStateVariables(cmf::math::StateVariableVector& vector)
		{
			for(cmf::upslope::cell_vector::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
			{
				(**it).AddStateVariables(vector);   
			}
		}

#endif
	};
	
}
#endif // project_h__
