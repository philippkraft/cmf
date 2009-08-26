#include "SoluteStorage.h"
#include "WaterStorage.h"
real cmf::water::SoluteStorage::Derivate( const cmf::math::Time& time )
{
 	// Sums up the fluxes as water fluxes (mol/day)
	cmf::water::connection_vector& connections=Storage.Connections();
	WaterFlux inflow, outflow;
	for (cmf::water::connection_vector::iterator it = connections.begin();it!=connections.end();++it)
	{
		cmf::water::FluxConnection& con=**it;
		real q=con.q(Storage,time);
		if (q>0)
		{
			inflow+=WaterFlux(q,con.conc(time));
		}
		else if (q<0)
		{
			outflow+=WaterFlux(q,con.conc(time));
		}
	}
	return inflow.matterflux(this->Solute) + outflow.matterflux(this->Solute) + AdditionalFlux * Storage.water();
}

real cmf::water::SoluteStorage::conc() const
{
	if (Storage.water()>0)
		return get_state()/Storage.water();
	else
		return 0.0;
}



