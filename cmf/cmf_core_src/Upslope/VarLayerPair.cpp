#include "VarLayerPair.h"
#include "connections/VarLayerPercolation.h"
#include "../math/real.h"
/************************************************************************/
/* VariableLayerSaturated                                            */
/************************************************************************/

static const real unsat_min_thick=0.1;


real cmf::upslope::VariableLayerSaturated::get_upper_boundary() const
{
	
	return get_lower_boundary()-calc_thickness(get_state());
}

cmf::upslope::VariableLayerSaturated::VariableLayerSaturated( cmf::upslope::Cell& cell,real lowerboundary,const RetentionCurve & r_curve ) 
: cmf::upslope::SoilLayer(cell,lowerboundary-0.01,lowerboundary,r_curve,cell.layer_count()+1)
{
	set_state(0.01*cell.get_area()*r_curve.Porosity(lowerboundary));
}
std::tr1::shared_ptr<cmf::upslope::VariableLayerSaturated> cmf::upslope::VariableLayerSaturated::Create(Cell& cell,real lowerboundary,const RetentionCurve& r_curve)
{
	real maximum_upperboundary=cell.layer_count() ? cell.get_layer(-1)->get_lower_boundary() : 0.0;
	std::tr1::shared_ptr<VariableLayerSaturated>   lowerlayer(new VariableLayerSaturated(cell,lowerboundary,r_curve));
	std::tr1::shared_ptr<VariableLayerUnsaturated> upperLayer(new VariableLayerUnsaturated(cell,maximum_upperboundary,lowerboundary-0.01,r_curve,lowerlayer));
	lowerlayer->m_upperLayer=upperLayer;
	upperLayer->Name="Unsaturated zone (" + cell.to_string() + ")";
	lowerlayer->Name="Saturated zone (" + cell.to_string() + ")";
	new cmf::upslope::connections::VarLayerPercolationRichards(upperLayer,lowerlayer);
	lowerlayer->set_potential(cell.z-lowerboundary+0.01);
	return lowerlayer;
}

void cmf::upslope::VariableLayerSaturated::set_potential( real waterhead )
{
	std::tr1::shared_ptr<cmf::upslope::VariableLayerUnsaturated> upperLayer=m_upperLayer.lock();
	if (waterhead>cell.z-get_lower_boundary())
	{
		real ub= (upperLayer ? upperLayer->get_upper_boundary() : 0.0)+unsat_min_thick;
		real low_pot=cell.z-get_lower_boundary();
		real maxvol=get_soil().VoidVolume(ub,get_lower_boundary(),cell.get_area());

		real states[2] = {0.0,maxvol};
		real fillings[2] = {calc_potential(0.01)-waterhead,calc_potential(maxvol)-waterhead};

		for (int i = 0; i < 1000 ; ++i)
		{
			if (fabs(states[0]-states[1])<1e-9) break;
			states[i % 2] = states[(i+1) % 2] - (states[(i+1) % 2]-states[i % 2])/(fillings[(i+1) % 2]-fillings[i % 2])*fillings[(i+1) % 2];
			fillings[i % 2]=calc_potential(states[i % 2])-waterhead;
		}
		set_state(states[0]);
	}
	else
		set_state(0);
	if (upperLayer) upperLayer->set_potential(waterhead);
}

real cmf::upslope::VariableLayerSaturated::MaximumThickness() const
{
	return get_lower_boundary() - (!m_upperLayer.expired() ? m_upperLayer.lock()->get_upper_boundary() : 0.0) - unsat_min_thick;
}

real cmf::upslope::VariableLayerSaturated::get_potential() const
{
	return calc_potential(get_state());
}

real cmf::upslope::VariableLayerSaturated::get_thickness_change_rate() const
{
	return 1e6 * (calc_thickness(get_state()) - calc_thickness(get_state()-1e-6));
}

real cmf::upslope::VariableLayerSaturated::calc_thickness( real state ) const
{
	real filling=get_soil().FillHeight(get_lower_boundary(),cell.get_area(),state);
	real max_thick=MaximumThickness();
	return filling; //hyperbola::rounded_linear(filling,max_thick,1,0.1);

}

real cmf::upslope::VariableLayerSaturated::calc_potential( real state ) const
{
	std::tr1::shared_ptr<cmf::upslope::VariableLayerUnsaturated> upperLayer=m_upperLayer.lock();
	real pot_upper=upperLayer ? upperLayer->get_potential() : cell.z-get_lower_boundary();
	if (state<=0) return pot_upper;
	real f_empty=pow(1-calc_thickness(state)/MaximumThickness(),100);
	real thickness=calc_thickness(state);
	real lb=get_lower_boundary();

	real ub=lb - thickness;
	if (lb-ub<1e-12)
		return pot_upper;
	else
	{
		real w=state/maximum(get_soil().VoidVolume(ub,lb,cell.get_area()),1e-12);
		real pot_lower=cell.z-ub+get_soil().MatricPotential(w);
		real pot_mixed=f_empty*pot_upper+(1-f_empty)*pot_lower;
		return minimum(pot_mixed,pot_lower);
	}

}

std::tr1::shared_ptr<cmf::upslope::VariableLayerSaturated> cmf::upslope::VariableLayerSaturated::get_from_cell( cmf::upslope::Cell& cell )
{
	for (int i = 0; i < cell.layer_count() ; ++i)
	{
		std::tr1::shared_ptr<VariableLayerSaturated> p_result = std::tr1::dynamic_pointer_cast<VariableLayerSaturated>(cell.get_layer(i));
		if(p_result)
		{
			return p_result;
		}
	}
	return std::tr1::shared_ptr<VariableLayerSaturated>();
}
/************************************************************************/
/* Unsaturated zone                                                     */
/************************************************************************/

cmf::upslope::VariableLayerUnsaturated::VariableLayerUnsaturated(cmf::upslope::Cell & cell,real upperboundary,real lowerboundary,const RetentionCurve & r_curve,std::tr1::shared_ptr<cmf::upslope::VariableLayerSaturated> LayerBelow ) 
: SoilLayer(cell,upperboundary,lowerboundary,r_curve,cell.layer_count()-1),belowLayer(LayerBelow)
{	
}

real cmf::upslope::VariableLayerUnsaturated::get_potential() const
{
	return cmf::upslope::SoilLayer::get_potential();
	// 	real f_empty=/*square*/(piecewise_linear(-get_thickness(),-0.1,-0.01));
	// 	real pot=get_gravitational_potential() + get_soil().MatricPotential(get_wetness());
	// 	return pot;
	// 	if (f_empty>0)
	// 		return f_empty*(0.5*(belowLayer->get_potential()+cell.get_surfacewater().get_potential())) + (1-f_empty)*pot;
	// 	else
	// 		return pot;
}

real cmf::upslope::VariableLayerUnsaturated::get_wetness() const
{
	return cmf::upslope::SoilLayer::get_wetness();
	real f_empty=/*square*/(piecewise_linear(-get_thickness(),-0.1,-0.01));
	return f_empty * belowLayer.lock()->get_wetness() + (1-f_empty)*get_state()/get_capacity();
}

real cmf::upslope::VariableLayerUnsaturated::get_lower_boundary() const
{
	real sat_ub=!belowLayer.expired() ? belowLayer.lock()->get_upper_boundary() : get_upper_boundary()+0.001;
	return maximum(sat_ub,unsat_min_thick);
}