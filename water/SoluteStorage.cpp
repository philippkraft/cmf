#include "SoluteStorage.h"
#include "WaterStorage.h"
real cmf::water::SoluteStorage::Derivate( const cmf::math::Time& time )
{
 	// Sums up the fluxes as water fluxes (mol/day)
	//WaterFlux sum_in=Storage.sum_inflow(time);
	//real sum_mix_flux=sum_in.matterflux(this->Solute);
	// Add the additional rate (might be negative)
	//real sum_reaction_flux=AdditionalFlux;
	// sum up the reactive fluxes
// 	if (Reactions.size())
// 	{
// 		WaterQuality wq=this->Storage.conc(time);
// 		for (cmf::water::reaction::ReactionVector::iterator it=Reactions.begin();it!=Reactions.end();++it)
// 			sum_reaction_flux+=(**it).ReactiveFlux(wq) * this->Storage.water();
// 	}

	return AdditionalFlux;
}

real cmf::water::SoluteStorage::conc() const
{
	if (Storage.water()>0)
		return get_state()/Storage.water();
	else
		return 0.0;
}



