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
		State(Soil().VoidVolume(UpperBoundary(),LowerBoundary(),cell.get_area()));
	else
	{
		real w=Soil().Wetness(m_pot);
		real vv=Soil().VoidVolume(UpperBoundary(),LowerBoundary(),cell.get_area());
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
	m_lowerboundary(lowerboundary),Position(_cell.layer_count())
{
	Location=cmf::geometry::point(_cell.x,_cell.y,_cell.z - lowerboundary);
	m_upperboundary = cell.layer_count()>0 ? cell.get_layer(-1).LowerBoundary() : 0;
	if (m_lowerboundary-m_upperboundary<=0)
		throw std::runtime_error("0 m thickness of layer");
	SetPotential(cell.z - saturateddepth);
	std::stringstream sstr;
	sstr << "get_layer " << UpperBoundary() << "-" << LowerBoundary() << "m @(" << cell.x << "," << cell.y << "," << cell.z << ")";
	Name=sstr.str();

	cell.add_layer(this);
}
// protected constructor
cmf::upslope::SoilWaterStorage::SoilWaterStorage( cmf::upslope::Cell & _cell,real upperBoundary,real lowerboundary,const RCurve& r_curve,int _Position ) 
: cmf::water::WaterStorage(_cell.project()),cell(_cell),m_retentioncurve(r_curve),
m_lowerboundary(lowerboundary),m_upperboundary(upperBoundary),Position(_Position)
{
	Location=cmf::geometry::point(_cell.x,_cell.y,_cell.z - lowerboundary);
	if (m_lowerboundary-m_upperboundary<=0)
		throw std::runtime_error("0 m thickness of layer");

}

real cmf::upslope::SoilWaterStorage::GetFlowCrosssection( const cmf::upslope::SoilWaterStorage& target,bool HorizontalLayers/*=false*/ ) const
{
	if (&cell == &target.cell)
	{
		return cell.get_area();
	}
	else if (HorizontalLayers)
	{
		double w=cell.get_topology().flowwidth(target.cell);
		real upper=minimum(cell.z - UpperBoundary(),target.cell.z - target.UpperBoundary());
		real lower=maximum(cell.z - LowerBoundary(),target.cell.z - target.LowerBoundary());
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
	return cell.get_saturated_depth();
// 	SoilWaterStorage
// 		* ul=Position ? &cell.get_layer(Position-1) : 0,
// 		* ll=Position+1<cell.layer_count() ? &cell.get_layer(Position+1) : 0;
// 	 	if (Wetness()>=0.95)
// 	 		return ul ? ul->get_saturated_depth() : UpperBoundary();
// 	 	else
// 		{
// 			if ((ll==0) || (ll->Wetness()>=.95))
// 				return minimum(UpperBoundary()-MatrixPotential(),LowerBoundary());
// 			else
// 				return ll->get_saturated_depth();
// 		}
}

void cmf::upslope::SoilWaterStorage::StateChangeAction()
{
	m_wet.C=Soil().VoidVolume(UpperBoundary(),LowerBoundary(),cell.get_area());
	m_wet.W=State()/m_wet.C;
	m_wet.theta=State()/(cell.get_area()*Thickness());
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
	real filling=Soil().FillHeight(LowerBoundary(),cell.get_area(),State());
	if (!upperLayer)
		return LowerBoundary()-filling;
	else
		return maximum(LowerBoundary()-filling,upperLayer->UpperBoundary()+0.01);
}

cmf::upslope::FlexibleSizeSaturatedZone::FlexibleSizeSaturatedZone( cmf::upslope::Cell& cell,real lowerboundary,const RCurve & r_curve ) 
: cmf::upslope::SoilWaterStorage(cell,lowerboundary-0.01,lowerboundary,r_curve,cell.layer_count()+1),upperLayer(0)
{
	State(0.01*cell.get_area()*r_curve.Porosity(lowerboundary));
}
cmf::upslope::FlexibleSizeSaturatedZone* cmf::upslope::FlexibleSizeSaturatedZone::Create(Cell& cell,real lowerboundary,const RCurve& r_curve)
{
	real maximum_upperboundary=cell.layer_count() ? cell.get_layer(-1).LowerBoundary() : 0.0;
	FlexibleSizeSaturatedZone * lowerlayer = new FlexibleSizeSaturatedZone(cell,lowerboundary,r_curve);
	FlexibleSizeLayer* upperLayer=new FlexibleSizeLayer(cell,maximum_upperboundary,lowerboundary-0.01,r_curve,lowerlayer);
	lowerlayer->upperLayer=upperLayer;
	upperLayer->Name="Unsaturated zone";
	lowerlayer->Name="Saturated zone";
	cell.add_layer(upperLayer);
	cell.add_layer(lowerlayer);
	new cmf::upslope::connections::UnsatSatPercolation(*upperLayer,*lowerlayer);
	lowerlayer->SetPotential(cell.z-lowerboundary+0.01);
	return lowerlayer;
}
static const real unsat_min_thick=0.1;
void cmf::upslope::FlexibleSizeSaturatedZone::SetPotential( real waterhead )
{
	if (waterhead>cell.z-LowerBoundary())
	{
		real ub= (upperLayer ? upperLayer->UpperBoundary() : 0.0)+unsat_min_thick;
		real g_pot=cell.z-ub;
		real m_pot=waterhead-g_pot;
		real maxvol=Soil().VoidVolume(ub,LowerBoundary(),cell.get_area());
		if (m_pot<0)
		{
			real vol=maxvol*(1+m_pot/MaximumThickness());
			State(vol);
		}
		else
		{
			real w=Soil().Wetness(m_pot);
			State(maxvol*w);
		}
	}
	else
		State(0);
	if (upperLayer) upperLayer->SetPotential(waterhead);
}

real cmf::upslope::FlexibleSizeSaturatedZone::MaximumThickness() const
{
	return LowerBoundary() - (upperLayer ? upperLayer->UpperBoundary() : 0.0) - unsat_min_thick;
}
cmf::upslope::FlexibleSizeLayer::FlexibleSizeLayer(cmf::upslope::Cell & cell,real upperboundary,real lowerboundary,const RCurve & r_curve,cmf::upslope::FlexibleSizeSaturatedZone* LayerBelow ) 
: SoilWaterStorage(cell,upperboundary,lowerboundary,r_curve,cell.layer_count()-1),belowLayer(LayerBelow)
{	
}

cmf::upslope::SoilWaterStorage* cmf::upslope::AsSoilWater(cmf::water::FluxNode* node)
{
	return dynamic_cast<cmf::upslope::SoilWaterStorage*>(node);
}


