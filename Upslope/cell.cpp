#include "cell.h"
#include "../project.h"
#include "SoilWaterStorage.h"
#include "connections/AtmosphericFluxes.h"
#include "connections/surfacefluxes.h"
#include "connections/infiltration.h"
#include "../Reach/ManningConnection.h"
#include "../Reach/VolumeHeightRelation.h"
#include "../Reach/ReachType.h"
#include "../Reach/Reach.h"
#include "Topology.h"
#include "../Atmosphere/Precipitation.h"
#include "../Atmosphere/Meteorology.h"

int cmf::upslope::Cell::cell_count=0;
cmf::upslope::Cell::~Cell()
{
	remove_layers();
}


cmf::upslope::Cell::Cell( double _x,double _y,double _z,double area,cmf::project& _project/*=0*/ ) 
: x(_x),y(_y),z(_z),m_Area(area),m_project(_project),
m_SurfaceWater(new cmf::water::FluxNode(_project)),	Id(cell_count++),
m_Canopy(0),m_Snow(0),m_SurfaceWaterStorage(0),m_meteo(new cmf::atmosphere::ConstantMeteorology)
{
	m_SurfaceWater->Name="Surfacewater";
	m_SurfaceWater->Location=get_position();
	m_Evaporation.reset(new cmf::water::FluxNode(_project,cmf::geometry::point(x,y,z+20)));
	m_Evaporation->Name="Evaporation";
	m_Transpiration.reset(new cmf::water::FluxNode(_project,cmf::geometry::point(x,y,z+20)));
	m_Transpiration->Name="Transpiration";
	m_rainfall.reset(new cmf::atmosphere::RainCloud(*this));
	m_rainfall->Name="Rainy cloud";
	_project.m_cells.push_back(this);
	new connections::Rainfall(get_surfacewater(),*this);
}

cmf::upslope::Cell::Cell( const Cell& cpy ) 
: m_project(cpy.project()),
	x(cpy.x),y(cpy.y),z(cpy.z),m_Area(cpy.get_area())
{

}
real cmf::upslope::Cell::get_saturated_depth()	
{
	if (m_SatDepth!=-10000)
	{
		//m_SatDepth=get_layer(0).get_saturated_depth();
		m_SatDepth=get_layer(-1).LowerBoundary();
		for(layer_vector::reverse_iterator it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
		{
			SoilWaterStorage& l=**it;
			if (l.Wetness()>=1)
			{
				m_SatDepth=l.UpperBoundary();
			}
			else
			{
				m_SatDepth=minimum(m_SatDepth,l.UpperBoundary()-l.MatrixPotential());
				//return m_SatDepth;
			}
		}
	}
	return m_SatDepth;
}



void cmf::upslope::Cell::AddStateVariables( cmf::math::StateVariableVector& vector )
{
	for (int i = 0; i < storage_count() ; ++i)
		get_storage(i).AddStateVariables(vector);
	for (int i = 0; i < layer_count() ; ++i)
		get_layer(i).AddStateVariables(vector);
}

void cmf::upslope::Cell::add_layer( cmf::upslope::SoilWaterStorage* layer )
{
	if (m_Layers.size()==0)	
		new connections::CompleteInfiltration(*layer,get_surfacewater());
	m_Layers.push_back(layer);
}

void cmf::upslope::Cell::add_layer(real lowerboundary,const cmf::upslope::RCurve& r_curve,real saturateddepth/*=-10*/ )
{
	SoilWaterStorage::Create(*this,lowerboundary,r_curve,saturateddepth);
}
void cmf::upslope::Cell::add_variable_layer_pair(real lowerboundary,const cmf::upslope::RCurve& r_curve )
{
	FlexibleSizeSaturatedZone::Create(*this,lowerboundary,r_curve);
}
cmf::upslope::vegetation::Vegetation cmf::upslope::Cell::get_vegetation() const
{
	vegetation::Vegetation res=m_vegetation;
	real snw_cvr=snow_coverage();
	res.albedo=snw_cvr * 0.9 + (1-snw_cvr)*res.albedo;
	return res;
}

cmf::atmosphere::Weather cmf::upslope::Cell::get_weather( cmf::math::Time t ) const
{
	return get_meteorology()(t);
}

void cmf::upslope::Cell::remove_layers()
{
	std::vector<SoilWaterStorage*> layers(m_Layers);
	m_Layers.clear();
	for(std::vector<SoilWaterStorage*>::iterator it = layers.begin(); it != layers.end(); ++it)
		delete *it;
}

void cmf::upslope::Cell::remove_last_layer()
{
	cmf::upslope::SoilWaterStorage* lastlayer=&get_layer(-1);
	m_Layers.pop_back();
	FlexibleSizeSaturatedZone *pLayer = dynamic_cast<FlexibleSizeSaturatedZone *>(lastlayer);
	if(pLayer)
	{
		delete pLayer;
		lastlayer=&get_layer(-1);
		m_Layers.pop_back();
	}
	delete lastlayer;
}

const cmf::project& cmf::upslope::Cell::project() const
{
return m_project;
}

real cmf::upslope::Cell::rain( cmf::math::Time t ) const
{
	return get_rainfall()(t);
}

cmf::water::WaterStorage& cmf::upslope::Cell::add_storage( std::string Name,char storage_role/*='N'*/, bool isopenwater/*=false*/ )
{
	if (storage_role=='C' && m_Canopy) return *m_Canopy;
	if (storage_role=='S' && m_Snow)   return *m_Snow;
	cmf::water::WaterStorage* ws=0;
	ws=isopenwater ? new cmf::river::OpenWaterStorage(project(),get_area())	: new cmf::water::WaterStorage(project());
	ws->Location=get_position();
	ws->Name=Name;
	if (storage_role=='C')
	{
		m_Canopy=ws;
		ws->Location.z+=get_vegetation().Height;
	}
	else if (storage_role=='S')
	{
		m_Snow=ws;
	}
	storage_pointer sp(ws);
	m_storages.push_back(sp);
	if (project().debug) 
		std::cout << " Add storage " << sp->Name << " to " << this->ToString() 
		<< " as " << (storage_role=='W' ? "surface water" : storage_role=='C' ? "canopy" : storage_role=='S' ? "snow" : "anything")
		<< std::endl;
	return *m_storages.back();
	
}

cmf::water::WaterStorage& cmf::upslope::Cell::get_storage( int index )
{
	return *m_storages.at(index<0 ? m_storages.size()+index : index);
}

const cmf::water::WaterStorage& cmf::upslope::Cell::get_storage( int index ) const
{
	return *m_storages.at(index<0 ? m_storages.size()+index : index);

}

void cmf::upslope::Cell::set_saturated_depth( real depth )
{
	for (int i = 0; i < layer_count() ; ++i)
	{
		get_layer(i).SetPotential(z-depth);
	}
}


cmf::water::WaterStorage* cmf::upslope::Cell::get_snow() const
{
	return m_Snow;
}

cmf::water::WaterStorage* cmf::upslope::Cell::get_canopy() const
{
	return m_Canopy;
}

cmf::river::Reach& cmf::upslope::Cell::add_reach(double length,char reach_type, double depth/*=0.5*/,double width/*=-1.*/, std::string Name/*="Reach"*/ )
{
	using namespace cmf::river;
	// Create the cross section geometry
	cmf::river::ReachType* rt=create_reachtype(reach_type,width,depth);
	// Create the volume height relation of the water storage, using the cross section geometry
	cmf::river::Channel ch(*rt,length);
	// Create the water storage
	cmf::river::OpenWaterStorage* ws=new OpenWaterStorage(project(),ch);
	ws->Name=Name;
	// Set the location of the water storage
	ws->Location=cmf::geometry::point(x,y,z-depth);
	// Add water storage to the water storages of the cell
	storage_pointer sp(ws);
	m_storages.push_back(sp);
	cmf::river::Reach * r=new cmf::river::Reach(*this,length,rt,ws,depth,width);
	reach_pointer r_shared(r);
	m_reaches.push_back(r_shared);
	return *r_shared;
}

void cmf::upslope::Cell::remove_storage( cmf::water::WaterStorage& storage )
{
	if (&storage==m_Canopy) m_Canopy=0;
	if (&storage==m_Snow) m_Snow=0;
	if (&storage==m_SurfaceWaterStorage) {
		m_SurfaceWater.reset(new cmf::water::FluxNode(project()));
		m_SurfaceWater->Name=storage.Name;
		m_SurfaceWater->Location=storage.Location;
		cmf::water::replace_node(*m_SurfaceWaterStorage,*m_SurfaceWater);
		m_SurfaceWaterStorage=0;
	}
	for (int i = 0; i < this->storage_count(); ++i)
	{
		if (storage==this->get_storage(i))
		{
			m_storages.erase(m_storages.begin()+i);
			return;
		}
	}
}

cmf::upslope::Topology& cmf::upslope::Cell::get_topology()
{
	if (!m_topo.get()) 
	{
		Topology* new_topo=new Topology(this);
		m_topo.reset(new_topo);
	}
	return *m_topo;
}

cmf::water::FluxNode& cmf::upslope::Cell::get_evaporation()
{
	return *m_Evaporation;
}

cmf::water::FluxNode& cmf::upslope::Cell::get_transpiration()
{
	return *m_Transpiration;
}

void cmf::upslope::Cell::surfacewater_as_storage()
{
	if (m_SurfaceWaterStorage==0) 
	{
		m_SurfaceWaterStorage=cmf::river::OpenWaterStorage::FromNode(*m_SurfaceWater,get_area());
		storage_pointer ws(m_SurfaceWaterStorage);
		m_storages.push_back(ws);
		m_SurfaceWater.reset();
	}
}