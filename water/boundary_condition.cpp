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

/************************************************************************/
/* Neumann-Boundary list                                                */
/************************************************************************/

cmf::math::numVector cmf::water::NeumannBoundary_list::get_flux( cmf::math::Time t ) const
{
	cmf::math::numVector res(m_boundaries.size());
	for (int i = 0; i < int(m_boundaries.size()) ; ++i)
	{
		res[i]=m_boundaries[i]->flux[t];
	}
	return res;
}

void cmf::water::NeumannBoundary_list::set_flux( cmf::math::numVector values )
{
	if (values.size()!=m_boundaries.size())
		throw std::runtime_error("The input array with fluxes need to have the same size as this list");
	else
	{
		for (int i = 0; i < values.size() ; ++i)
		{
			set_flux(i,values[i]);
		}
	}
}

real cmf::water::NeumannBoundary_list::global_water_balance( cmf::math::Time t )	const
{
	real sum=0;
#pragma omp parallel for reduction(+ : sum)
	for (int i = 0; i < (int)size() ; ++i)
	{
		sum+=m_boundaries[i]->water_balance(t);					
	}
	return sum;
}

cmf::math::numVector cmf::water::NeumannBoundary_list::water_balance( cmf::math::Time t ) const
{
	cmf::math::numVector res(size());
#pragma omp parallel for
	for (int i = 0; i < (int)size() ; ++i)
	{
		res[i]=m_boundaries[i]->water_balance(t);
	}
	return res;
}

