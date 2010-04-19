

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   cmf is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
//   
#include "SoilLayer.h"
#include "cell.h"
#include "Topology.h"

/************************************************************************/
/* SoilLayer                                                     */
/************************************************************************/
// Actual volumetric water content of the soil (m3 H2O/m3 get_soil)
// Actual conductivity
// total get_potential
real cmf::upslope::SoilLayer::get_potential() const
{
	return m_wet.h;
}


void cmf::upslope::SoilLayer::set_potential( real waterhead )
{
	if (get_state_variable_content()=='h')
		set_state(waterhead);
	else 
		set_state(head_to_volume(waterhead));
}

real cmf::upslope::SoilLayer::get_gravitational_potential() const
{
	return cell.z-get_upper_boundary();
}

// public ctor
cmf::upslope::SoilLayer::SoilLayer( cmf::upslope::Cell & _cell,real lowerboundary,const RetentionCurve& r_curve,real saturateddepth/*=10*/ ) 
:	cmf::water::WaterStorage(_cell.project(), 0),
	cell(_cell),
	m_retentioncurve(r_curve.copy()),
	m_lowerboundary(lowerboundary),
	Position(_cell.layer_count())

{
	// Get the location from the layer
	Location=cmf::geometry::point(_cell.x,_cell.y,_cell.z - lowerboundary);
	// Get the upper boundary from the upper layer
	m_upperboundary = cell.layer_count()>0 ? cell.get_layer(-1)->get_lower_boundary() : 0;
	if (m_lowerboundary-m_upperboundary<=0)
		throw std::runtime_error("0 m thickness of layer");
	set_potential(cell.z - saturateddepth);
	std::stringstream sstr;
	sstr << "Layer #" << Position << " of cell #" << cell.Id;
	Name=sstr.str();
}
// protected constructor
cmf::upslope::SoilLayer::SoilLayer( cmf::upslope::Cell & _cell,real upperBoundary,real lowerboundary,const RetentionCurve& r_curve,int _Position ) 
: cmf::water::WaterStorage(_cell.project()),cell(_cell),m_retentioncurve(r_curve.copy()),
m_lowerboundary(lowerboundary),m_upperboundary(upperBoundary),Position(_Position)
{
	Location=cmf::geometry::point(_cell.x,_cell.y,_cell.z - lowerboundary);
	if (m_lowerboundary-m_upperboundary<=0)
		throw std::runtime_error("0 m thickness of layer");

}

real cmf::upslope::SoilLayer::get_flow_crosssection( const cmf::upslope::SoilLayer& target,bool HorizontalLayers/*=false*/ ) const
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

real cmf::upslope::SoilLayer::get_saturated_depth() const
{
	return cell.get_saturated_depth();
}

void cmf::upslope::SoilLayer::StateChangeAction()
{
	m_wet.C=get_soil().VoidVolume(get_upper_boundary(),get_lower_boundary(),cell.get_area());
	if (get_state_variable_content()=='h') {
		m_wet.V = head_to_volume(this->get_state());
		m_wet.h = this->get_state();
	} else {
		m_wet.V = this->get_state();
		m_wet.h = volume_to_head(this->get_state());
	}
	m_wet.W=std::max(m_wet.V,0.)/m_wet.C;
	m_wet.theta=std::max(m_wet.V,0.)/(cell.get_area()*get_thickness());
	m_wet.Psi_m = m_wet.h - get_gravitational_potential();
	m_wet.Ksat=get_soil().K(1,0.5*(get_upper_boundary()+get_lower_boundary()));
	m_wet.K=get_soil().K(m_wet.W,0.5*(get_upper_boundary()+get_lower_boundary()));
	cell.InvalidateSatDepth();
}

real cmf::upslope::SoilLayer::head_to_volume(real head) const
{
	real 
		mp = head - this->get_gravitational_potential(),
		w  = get_soil().Wetness(mp);
	return w * this->get_capacity();
}
real cmf::upslope::SoilLayer::volume_to_head(real volume) const
{
	real
		w = volume / this->get_capacity(),
		mp = this->get_soil().MatricPotential(w);
	return mp + this->get_gravitational_potential();
}

void cmf::upslope::SoilLayer::set_theta( real Value )
{
	if (get_state_variable_content()=='h')
		set_state(get_soil().MatricPotential(get_wetness() * get_porosity()) + get_gravitational_potential());
	else
		set_state(Value*cell.get_area()*get_thickness());
}

real cmf::upslope::SoilLayer::get_porosity() const
{
	return get_capacity()/(cell.get_area() * get_thickness());
}

void cmf::upslope::SoilLayer::set_wetness( real wetness )
{
	if (get_state_variable_content()=='h')
		set_state(get_soil().MatricPotential(wetness) + get_gravitational_potential());
	else
		set_state(wetness * get_capacity());
}