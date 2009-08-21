#include "boundary_condition.h"

void cmf::water::NeumannBoundary::connect_to( cmf::water::FluxNode& target )
{
	new NeumannFlux(*this,target);
}

real cmf::water::NeumannBoundary::operator()( cmf::math::Time t ) const
{
	if (flux.is_empty())
		return 0.0;
	else
		return scale_function(flux[t]);
}
real cmf::water::NeumannFlux::calc_q( cmf::math::Time t )
{
	real f=(*m_bc)(t);
	if (f<0 && m_right->is_empty())
		return 0.0;
	else
		return f;
}