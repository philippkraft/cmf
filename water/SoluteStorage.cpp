#include "SoluteStorage.h"
#include "WaterStorage.h"
#include "flux_connection.h"
real cmf::water::SoluteStorage::Derivate( const cmf::math::Time& time )
{
 	// Sums up the fluxes as water fluxes (mol/day)
	
	cmf::water::connection_vector connections=m_water->get_connections();
	real inflow=0, outflow=0;
	for (cmf::water::connection_vector::iterator it = connections.begin();it!=connections.end();++it)
	{
		cmf::water::flux_connection& con=**it;
		real q=con.q(*m_water,time);
		if (q>0)
		{
			inflow+= q * con.conc(time,Solute);
		}
		else if (q<0)
		{
			outflow+=q * con.conc(time,Solute);
		}
	}
	real source_term = source + source_concentration * m_water->get_state();
	real decay_term = this->get_state() * decay;
	return inflow + outflow + source_term - decay_term;
}

real cmf::water::SoluteStorage::conc() const
{
	if (m_water->get_state()>0)
		return get_state()/m_water->get_state();
	else
		return 0.0;
}



