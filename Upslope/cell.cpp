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
cmf::upslope::Cell::~Cell()
{
	RemoveLayers();
}


cmf::upslope::Cell::Cell( double _x,double _y,double _z,double area,cmf::project& _project/*=0*/ ) 
: x(_x),y(_y),z(_z),m_Area(area),m_project(_project),
m_Evaporation(new cmf::water::FluxNode(_project)),m_Transpiration(new cmf::water::FluxNode(_project)),
m_SurfaceWater(new cmf::water::FluxNode(_project)),m_Canopy_pos(-1),m_Snow_pos(-1),m_SurfaceWater_pos(-1)
{
	m_Evaporation->Name="Evaporation";
	m_Evaporation->Location=cmf::geometry::point(x,y,z+GetVegetation().Height+1);
	m_Transpiration->Name="Transpiration";
	m_Transpiration->Location=cmf::geometry::point(x,y,z+GetVegetation().Height+1);
	m_SurfaceWater->Name="SurfaceWater";
	m_SurfaceWater->Location=Center();
	_project.m_cells.push_back(this);
	new connections::Rainfall(SurfaceWater(),*this);
}

real cmf::upslope::Cell::SaturatedDepth()	
{
	if (m_SatDepth!=-10000)
	{
		//m_SatDepth=Layer(0).SaturatedDepth();
		m_SatDepth=Layer(-1).LowerBoundary();
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
	for (int i = 0; i < StorageCount() ; ++i)
		GetStorage(i).AddStateVariables(vector);
	for (int i = 0; i < LayerCount() ; ++i)
		Layer(i).AddStateVariables(vector);
}

void cmf::upslope::Cell::AddLayer( cmf::upslope::SoilWaterStorage* layer )
{
	if (m_Layers.size()==0)	
		new connections::CompleteInfiltration(*layer,SurfaceWater());
	m_Layers.push_back(layer);
}

void cmf::upslope::Cell::AddLayer(real lowerboundary,const cmf::upslope::RCurve& r_curve,real saturateddepth/*=-10*/ )
{
	SoilWaterStorage::Create(*this,lowerboundary,r_curve,saturateddepth);
}
void cmf::upslope::Cell::AddVariableLayerPair(real lowerboundary,const cmf::upslope::RCurve& r_curve )
{
	FlexibleSizeSaturatedZone::Create(*this,lowerboundary,r_curve);
}
cmf::upslope::vegetation::Vegetation cmf::upslope::Cell::GetVegetation() const
{
	vegetation::Vegetation res=Vegetation;
	real snw_cvr=SnowCover();
	res.albedo=snw_cvr * 0.9 + (1-snw_cvr)*res.albedo;
	return res;
}

cmf::atmosphere::Weather cmf::upslope::Cell::Weather( cmf::math::Time t ) const
{
	return project().Meteorology()(t,x,y,z);
}

void cmf::upslope::Cell::RemoveLayers()
{
	std::vector<SoilWaterStorage*> layers(m_Layers);
	m_Layers.clear();
	for(std::vector<SoilWaterStorage*>::iterator it = layers.begin(); it != layers.end(); ++it)
		delete *it;
}

void cmf::upslope::Cell::RemoveLastLayer()
{
	cmf::upslope::SoilWaterStorage* lastlayer=&Layer(-1);
	m_Layers.pop_back();
	FlexibleSizeSaturatedZone *pLayer = dynamic_cast<FlexibleSizeSaturatedZone *>(lastlayer);
	if(pLayer)
	{
		delete pLayer;
		lastlayer=&Layer(-1);
		m_Layers.pop_back();
	}
	delete lastlayer;
}

const cmf::project& cmf::upslope::Cell::project() const
{
return m_project;
}

real cmf::upslope::Cell::Rain( cmf::math::Time t ) const
{
	return project().Rainfall(t,x,y,z);
}

cmf::water::WaterStorage& cmf::upslope::Cell::AddStorage( std::string Name,char storage_role/*='N'*/, bool isopenwater/*=false*/ )
{
	cmf::water::WaterStorage* ws=0;
	if (storage_role=='W')
	{
		if (m_SurfaceWater_pos>=0) return *m_storages[m_SurfaceWater_pos];
		ws=cmf::river::OpenWaterStorage::FromNode(*m_SurfaceWater,Area());
		m_SurfaceWater.reset();
		m_SurfaceWater_pos=int(m_storages.size());
	}
	else
	{
		ws=isopenwater ? new cmf::river::OpenWaterStorage(project(),Area())	: new cmf::water::WaterStorage(project());
		ws->Location=Center();
		ws->Name=Name;
		if (storage_role=='C')
		{
			if (m_Canopy_pos>=0) return *m_storages[m_Canopy_pos];
			m_Canopy_pos=int(m_storages.size());
			ws->Location.z+=GetVegetation().Height;
		}
		else if (storage_role=='S')
		{
			if (m_Snow_pos>=0) return *m_storages[m_Snow_pos];
			m_Snow_pos=int(m_storages.size());
		}
	}
	storage_pointer sp(ws);
	m_storages.push_back(sp);
	if (project().debug) 
		std::cout << " Add storage " << sp->Name << " to " << this->ToString() 
		<< " as " << (storage_role=='W' ? "surface water" : storage_role=='C' ? "canopy" : storage_role=='S' ? "snow" : "anything")
		<< std::endl;
	return *m_storages.back();
	
}

cmf::water::WaterStorage& cmf::upslope::Cell::GetStorage( int index )
{
	return *m_storages.at(index<0 ? m_storages.size()+index : index);
}

const cmf::water::WaterStorage& cmf::upslope::Cell::GetStorage( int index ) const
{
	return *m_storages.at(index<0 ? m_storages.size()+index : index);

}

void cmf::upslope::Cell::SetSaturatedDepth( real depth )
{
	for (int i = 0; i < LayerCount() ; ++i)
	{
		Layer(i).SetPotential(z-depth);
	}
}


cmf::water::WaterStorage* cmf::upslope::Cell::GetSnow() const
{
	return m_Snow_pos<0 ? 0 : m_storages.at(m_Snow_pos).get();
}

cmf::water::WaterStorage* cmf::upslope::Cell::GetCanopy() const
{
	return m_Canopy_pos<0 ? 0 : m_storages.at(m_Canopy_pos).get();
}

cmf::river::Reach& cmf::upslope::Cell::AddReach(double length,char reach_type, double depth/*=0.5*/,double width/*=-1.*/, std::string Name/*="Reach"*/ )
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

void cmf::upslope::Cell::RemoveStorage( cmf::water::WaterStorage& storage )
{
	for (int i = 0; i < this->StorageCount(); ++i)
	{
		if (storage==this->GetStorage(i))
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