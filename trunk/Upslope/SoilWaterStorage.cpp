#include "SoilWaterStorage.h"
#include "connections/subsurfacefluxes.h"
#include "Topology.h"
/************************************************************************/
/* SoilWaterStorage                                                     */
/************************************************************************/
// Actual volumetric water content of the soil (m3 H2O/m3 Soil)
// Actual conductivity
// total Potential
real cmf::upslope::SoilWaterStorage::Potential() const
{
	real
		mp=MatrixPotential(),
		gp=GravitationalPotential();
	return mp+gp;
}


void cmf::upslope::SoilWaterStorage::SetPotential( real waterhead )
{
	real m_pot=waterhead - GravitationalPotential();
	if (m_pot>=0)
		State(Soil().VoidVolume(UpperBoundary(),LowerBoundary(),cell.Area()));
	else
	{
		real w=Soil().Wetness(m_pot);
		real vv=Soil().VoidVolume(UpperBoundary(),LowerBoundary(),cell.Area());
		State(vv*w);
	}
}

real cmf::upslope::SoilWaterStorage::GravitationalPotential() const
{
	return cell.z-UpperBoundary();
}

// public ctor
cmf::upslope::SoilWaterStorage::SoilWaterStorage( cmf::upslope::Cell & _cell,real lowerboundary,const RCurve& r_curve,real saturateddepth/*=-10*/ ) 
: cmf::water::WaterStorage(_cell.project(), 0),cell(_cell),m_retentioncurve(r_curve),
	m_lowerboundary(lowerboundary),Position(_cell.LayerCount())
{
	Location=cmf::geometry::point(_cell.Center().x,_cell.Center().y,_cell.Center().z - lowerboundary);
	m_upperboundary = cell.LayerCount()>0 ? cell.Layer(-1).LowerBoundary() : 0;
	if (m_lowerboundary-m_upperboundary<=0)
		throw std::runtime_error("0 m thickness of layer");
	SetPotential(cell.Center().z - saturateddepth);
	std::stringstream sstr;
	sstr << "Layer " << UpperBoundary() << "-" << LowerBoundary() << "m @(" << cell.Center().x << "," << cell.Center().y << "," << cell.Center().z << ")";
	Name=sstr.str();

	cell.AddLayer(this);
}
// protected constructor
cmf::upslope::SoilWaterStorage::SoilWaterStorage( cmf::upslope::Cell & _cell,real upperBoundary,real lowerboundary,const RCurve& r_curve,int _Position ) 
: cmf::water::WaterStorage(_cell.project()),cell(_cell),m_retentioncurve(r_curve),
m_lowerboundary(lowerboundary),m_upperboundary(upperBoundary),Position(_Position)
{
	Location=cmf::geometry::point(_cell.Center().x,_cell.Center().y,_cell.Center().z - lowerboundary);
	if (m_lowerboundary-m_upperboundary<=0)
		throw std::runtime_error("0 m thickness of layer");

}

real cmf::upslope::SoilWaterStorage::GetFlowCrosssection( const cmf::upslope::SoilWaterStorage& target,bool HorizontalLayers/*=false*/ ) const
{
	if (&cell == &target.cell)
	{
		return cell.Area();
	}
	else if (HorizontalLayers)
	{
		double w=cell.get_topology().flowwidth(target.cell);
		real upper=minimum(cell.Center().z - UpperBoundary(),target.cell.Center().z - target.UpperBoundary());
		real lower=maximum(cell.Center().z - LowerBoundary(),target.cell.Center().z - target.LowerBoundary());
		return upper<lower ? (lower-upper)*w : 0;
	}
	else
	{
		double w=cell.get_topology().flowwidth(target.cell);
		real upper=maximum(UpperBoundary(),target.UpperBoundary());
		real lower=minimum(LowerBoundary(),target.LowerBoundary());
		return upper<lower ? (lower-upper)*w : 0;
	}
}

real cmf::upslope::SoilWaterStorage::SaturatedDepth() const
{
	//real sd=minimum(UpperBoundary()-MatrixPotential(),LowerBoundary());
	return cell.SaturatedDepth();
// 	SoilWaterStorage
// 		* ul=Position ? &cell.Layer(Position-1) : 0,
// 		* ll=Position+1<cell.LayerCount() ? &cell.Layer(Position+1) : 0;
// 	 	if (Wetness()>=0.95)
// 	 		return ul ? ul->SaturatedDepth() : UpperBoundary();
// 	 	else
// 		{
// 			if ((ll==0) || (ll->Wetness()>=.95))
// 				return minimum(UpperBoundary()-MatrixPotential(),LowerBoundary());
// 			else
// 				return ll->SaturatedDepth();
// 		}
}

void cmf::upslope::SoilWaterStorage::StateChangeAction()
{
	m_wet.C=Soil().VoidVolume(UpperBoundary(),LowerBoundary(),cell.Area());
	m_wet.W=State()/m_wet.C;
	m_wet.theta=State()/(cell.Area()*Thickness());
	m_wet.Psi_m = Soil().MatricPotential(Wetness());
	m_wet.Ksat=Soil().K(1,0.5*(UpperBoundary()+LowerBoundary()));
	m_wet.K=Soil().K(m_wet.W,0.5*(UpperBoundary()+LowerBoundary()));
	cell.InvalidateSatDepth();
}
/************************************************************************/
/* FlexibleSizeSaturatedZone                                            */
/************************************************************************/
real cmf::upslope::FlexibleSizeSaturatedZone::UpperBoundary() const
{
	real filling=Soil().FillHeight(LowerBoundary(),cell.Area(),State());
	if (!upperLayer)
		return LowerBoundary()-filling;
	else
		return maximum(LowerBoundary()-filling,upperLayer->UpperBoundary()+0.01);
}

cmf::upslope::FlexibleSizeSaturatedZone::FlexibleSizeSaturatedZone( cmf::upslope::Cell& cell,real lowerboundary,const RCurve & r_curve ) 
: cmf::upslope::SoilWaterStorage(cell,lowerboundary-0.01,lowerboundary,r_curve,cell.LayerCount()+1),upperLayer(0)
{
	State(0.01*cell.Area()*r_curve.Porosity(lowerboundary));
}
cmf::upslope::FlexibleSizeSaturatedZone* cmf::upslope::FlexibleSizeSaturatedZone::Create(Cell& cell,real lowerboundary,const RCurve& r_curve)
{
	real maximum_upperboundary=cell.LayerCount() ? cell.Layer(-1).LowerBoundary() : 0.0;
	FlexibleSizeSaturatedZone * lowerlayer = new FlexibleSizeSaturatedZone(cell,lowerboundary,r_curve);
	FlexibleSizeLayer* upperLayer=new FlexibleSizeLayer(cell,maximum_upperboundary,lowerboundary-0.01,r_curve,lowerlayer);
	lowerlayer->upperLayer=upperLayer;
	upperLayer->Name="fs_unsat @" + cell.ToString();
	lowerlayer->Name="fs_sat @" + cell.ToString();
	cell.AddLayer(upperLayer);
	cell.AddLayer(lowerlayer);
	new cmf::upslope::connections::UnsatSatPercolation(*upperLayer,*lowerlayer);
	lowerlayer->SetPotential(cell.z-lowerboundary+0.01);
	return lowerlayer;
}

void cmf::upslope::FlexibleSizeSaturatedZone::SetPotential( real waterhead )
{
	if (waterhead>cell.z-LowerBoundary())
	{
		real filling=maximum(cell.z-waterhead,upperLayer->UpperBoundary()+0.01);
		real vol=Soil().VoidVolume(filling,LowerBoundary(),cell.Area());
		State(vol);
	}
	else
		State(0);
	if (upperLayer) upperLayer->SetPotential(waterhead);
}

cmf::upslope::FlexibleSizeLayer::FlexibleSizeLayer(cmf::upslope::Cell & cell,real upperboundary,real lowerboundary,const RCurve & r_curve,cmf::upslope::FlexibleSizeSaturatedZone* LayerBelow ) 
: SoilWaterStorage(cell,upperboundary,lowerboundary,r_curve,cell.LayerCount()-1),belowLayer(LayerBelow)
{	
}

cmf::upslope::SoilWaterStorage* cmf::upslope::AsSoilWater(cmf::water::FluxNode* node)
{
	return dynamic_cast<cmf::upslope::SoilWaterStorage*>(node);
}


