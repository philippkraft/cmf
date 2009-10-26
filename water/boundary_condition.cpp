#include "boundary_condition.h"
void cmf::water::NeumannBoundary::connect_to( cmf::water::flux_node::ptr target )
{
	new NeumannFlux(weak_this.lock(), target);
}

real cmf::water::NeumannBoundary::operator()( cmf::math::Time t ) const
{
	if (flux.is_empty())
		return 0.0;
	else
		return scale_function(flux[t]);
}

cmf::water::NeumannBoundary::NeumannBoundary( const cmf::project& _project, cmf::math::timeseries _flux,cmf::water::SoluteTimeseries _concentration/*=cmf::water::SoluteTimeseries()*/,cmf::geometry::point loc/*=cmf::geometry::point()*/ ) 
: cmf::water::flux_node(_project,loc), flux(_flux),concentration(_concentration)
{

}

cmf::water::NeumannBoundary::NeumannBoundary( const cmf::project& _project,cmf::geometry::point loc/*=cmf::geometry::point()*/ ) 
: cmf::water::flux_node(_project,loc)
{

}

cmf::water::NeumannBoundary::NeumannBoundary( cmf::water::flux_node::ptr target ) 
: cmf::water::flux_node(target->project(),target->Location)
{
	connect_to(target);
	Name = "Boundary of " + target->Name;
}
real cmf::water::NeumannFlux::calc_q( cmf::math::Time t )
{
	real f=(*m_bc.lock())(t);
	if (f<0 && right_node()->is_empty())
		return 0.0;
	else
		return f;
}


cmf::water::DricheletBoundary::DricheletBoundary( const cmf::project& _p,real potential,cmf::geometry::point Location/*=cmf::geometry::point()*/ ) : flux_node(_p,Location),m_Potential(potential)
{

}