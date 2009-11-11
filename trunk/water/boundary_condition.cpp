#include "boundary_condition.h"
#include "../math/real.h"
void cmf::water::NeumannBoundary::connect_to( cmf::water::flux_node::ptr target )
{
	new NeumannFlux(*this, target);
}

real cmf::water::NeumannBoundary::operator()( cmf::math::Time t ) const
{
	if (flux.is_empty())
		return 0.0;
	else
		return scale_function(flux[t]);
}

cmf::water::NeumannBoundary::NeumannBoundary(const cmf::project& _project, 
																						 cmf::math::timeseries _flux,
																						 cmf::water::SoluteTimeseries _concentration/*=cmf::water::SoluteTimeseries()*/,
																						 cmf::geometry::point loc/*=cmf::geometry::point()*/
																						) 
: cmf::water::flux_node(_project,loc), flux(_flux),concentration(_concentration)
{

}

cmf::water::NeumannBoundary::NeumannBoundary(const cmf::project& _project,
																						 cmf::geometry::point loc) 
: cmf::water::flux_node(_project,loc)
{

}

cmf::water::NeumannBoundary::ptr cmf::water::NeumannBoundary::create( cmf::water::flux_node::ptr target )
{
	cmf::water::NeumannBoundary::ptr res = cmf::water::NeumannBoundary::ptr(new NeumannBoundary(target->project(),target->Location));
	res->Name = "Boundary at " + target->Name;
	res->connect_to(target);
	return res;
}

real cmf::water::NeumannFlux::calc_q( cmf::math::Time t )
{
	real f=(*m_bc.lock())(t);
	if (f<0 && right_node()->is_empty())
		return 0.0;
	else
		return f;
}


cmf::water::DricheletBoundary::DricheletBoundary(const cmf::project& _p,real potential,
																								 cmf::geometry::point Location/*=cmf::geometry::point()*/ ) 
: flux_node(_p,Location),m_Potential(potential)
{	
}

real cmf::water::DricheletBoundary::conc( cmf::math::Time t, const cmf::water::solute& solute ) const
{
	conc_map::const_iterator it=m_concentration.find(solute);
	if (it!=m_concentration.end())
		return it->second;
	else
		return 0.0;
}

void cmf::water::DricheletBoundary::set_conc( const cmf::water::solute& solute, double value )
{
	m_concentration[solute]=value;
}