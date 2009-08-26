#include "SoilWaterStorage.h"

#include "Topology.h"

/************************************************************************/
/* SoilWaterStorage                                                     */
/************************************************************************/
// Actual volumetric water content of the soil (m3 H2O/m3 get_soil)
// Actual conductivity
// total get_potential
real cmf::upslope::SoilWaterStorage::get_potential() const
{
	real
		mp=get_matrix_potential(),
		gp=get_gravitational_potential();
	return mp+gp;
}


void cmf::upslope::SoilWaterStorage::set_potential( real waterhead )
{
	real m_pot=waterhead - get_gravitational_potential();
	real w=get_soil().Wetness(m_pot);
	real vv=get_soil().VoidVolume(get_upper_boundary(),get_lower_boundary(),cell.get_area());
	set_state(vv*w);
}

real cmf::upslope::SoilWaterStorage::get_gravitational_potential() const
{
	return cell.z-get_upper_boundary();
}

// public ctor
cmf::upslope::SoilWaterStorage::SoilWaterStorage( cmf::upslope::Cell & _cell,real lowerboundary,const RetentionCurve& r_curve,real saturateddepth/*=-10*/ ) 
: cmf::water::WaterStorage(_cell.project(), 0),cell(_cell),m_retentioncurve(r_curve.copy()),
	m_lowerboundary(lowerboundary),Position(_cell.layer_count())
{
	Location=cmf::geometry::point(_cell.x,_cell.y,_cell.z - lowerboundary);
	m_upperboundary = cell.layer_count()>0 ? cell.get_layer(-1).get_lower_boundary() : 0;
	if (m_lowerboundary-m_upperboundary<=0)
		throw std::runtime_error("0 m thickness of layer");
	set_potential(cell.z - saturateddepth);
	std::stringstream sstr;
	sstr << "Layer #" << Position << " " << get_upper_boundary() << "-" << get_lower_boundary() << "m of cell #" << cell.Id;
	Name=sstr.str();

	cell.add_layer(this);
}
// protected constructor
cmf::upslope::SoilWaterStorage::SoilWaterStorage( cmf::upslope::Cell & _cell,real upperBoundary,real lowerboundary,const RetentionCurve& r_curve,int _Position ) 
: cmf::water::WaterStorage(_cell.project()),cell(_cell),m_retentioncurve(r_curve.copy()),
m_lowerboundary(lowerboundary),m_upperboundary(upperBoundary),Position(_Position)
{
	Location=cmf::geometry::point(_cell.x,_cell.y,_cell.z - lowerboundary);
	if (m_lowerboundary-m_upperboundary<=0)
		throw std::runtime_error("0 m thickness of layer");

}

real cmf::upslope::SoilWaterStorage::get_flow_crosssection( const cmf::upslope::SoilWaterStorage& target,bool HorizontalLayers/*=false*/ ) const
{
	if (&cell == &target.cell)
	{
		return cell.get_area();
	}
	else if (HorizontalLayers)
	{
		double w=cell.get_topology().flowwidth(target.cell);
		real upper=minimum(cell.z - get_upper_boundary(),target.cell.z - target.get_upper_boundary());
		real lower=maximum(cell.z - get_lower_boundary(),target.cell.z - target.get_lower_boundary());
		return upper<lower ? (lower-upper)*w : 0;
	}
	else
	{
		double w=cell.get_topology().flowwidth(target.cell);
		real upper=maximum(get_upper_boundary(),target.get_upper_boundary());
		real lower=minimum(get_lower_boundary(),target.get_lower_boundary());
		return upper<lower ? (lower-upper)*w : 0;
	}
}

real cmf::upslope::SoilWaterStorage::get_saturated_depth() const
{
	//real sd=minimum(get_upper_boundary()-get_matrix_potential(),get_lower_boundary());
	return cell.get_saturated_depth();
// 	SoilWaterStorage
// 		* ul=Position ? &cell.get_layer(Position-1) : 0,
// 		* ll=Position+1<cell.layer_count() ? &cell.get_layer(Position+1) : 0;
// 	 	if (get_wetness()>=0.95)
// 	 		return ul ? ul->get_saturated_depth() : get_upper_boundary();
// 	 	else
// 		{
// 			if ((ll==0) || (ll->get_wetness()>=.95))
// 				return minimum(get_upper_boundary()-get_matrix_potential(),get_lower_boundary());
// 			else
// 				return ll->get_saturated_depth();
// 		}
}

void cmf::upslope::SoilWaterStorage::StateChangeAction()
{
	m_wet.C=get_soil().VoidVolume(get_upper_boundary(),get_lower_boundary(),cell.get_area());
	m_wet.W=maximum(get_state(),0)/m_wet.C;
	m_wet.theta=maximum(get_state(),0)/(cell.get_area()*get_thickness());
	m_wet.Psi_m = get_soil().MatricPotential(get_wetness());
	m_wet.Ksat=get_soil().K(1,0.5*(get_upper_boundary()+get_lower_boundary()));
	m_wet.K=get_soil().K(m_wet.W,0.5*(get_upper_boundary()+get_lower_boundary()));
	cell.InvalidateSatDepth();
}

cmf::upslope::SoilWaterStorage* cmf::upslope::AsSoilWater(cmf::water::FluxNode* node)
{
	return dynamic_cast<cmf::upslope::SoilWaterStorage*>(node);
}


