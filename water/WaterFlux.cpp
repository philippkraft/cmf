#include "WaterFlux.h"
#include "WaterStorage.h"

cmf::water::WaterFlux& cmf::water::WaterFlux::Update(real waterflux,const cmf::water::WaterStorage& origin,bool uptake/*=false*/ )
{
	q=waterflux;
	size_t i=0;
	for(cmf::water::Solutes::const_iterator it = cmf::water::Solutes::all().begin(); it != cmf::water::Solutes::all().end(); ++it)
	{
		const cmf::water::SoluteStorage& solstor=origin.Solute(*it);
		if (uptake)
			c[i++]= solstor.conc() * solstor.Solute.Uptake;
		else
			c[i++]=solstor.conc();
	}
	return *this;
}

cmf::water::WaterFlux& cmf::water::WaterFlux::operator+=(const cmf::water::WaterFlux& add )
{
	this->Add(add.q,add.c);
	return *this;
}

cmf::water::WaterFlux& cmf::water::WaterFlux::Add( real flux,const cmf::water::WaterStorage& origin )
{
	if (this->q*flux<0)
		throw std::runtime_error("Can't add fluxes with differing signs");
	if (abs(flux+q)>1e-12)
	{
		// Mix the water of the concentrations
		size_t i=0;
		for(cmf::water::Solutes::const_iterator it = cmf::water::Solutes::all().begin(); it != cmf::water::Solutes::all().end(); ++it)
		{
			const cmf::water::SoluteStorage& solstor=origin.Solute(*it);
			c[i]=(solstor.conc()*flux+c[i]*q)/(flux+q);
			++i;
		}
	}
	q+=flux;
	return *this;

}

cmf::water::WaterFlux& cmf::water::WaterFlux::Add( real flux,const cmf::water::WaterQuality& concentrations )
{
	if (this->q*flux<0)
		throw std::runtime_error("Can't add fluxes with differing signs");
	if (abs(flux+q)>1e-12)
	{
		// Mix the water of the concentrations
		int i=0;
		for (cmf::water::WaterQuality::const_iterator it=concentrations.begin();it!=concentrations.end();++it)
		{
			real thisMass=q*c[i];
			c[i]=(thisMass+(*it)*flux)/(q+flux);
			++i;
		}
	}
	q+=flux;
	return *this;

}

cmf::water::WaterFlux::WaterFlux( real flux,const cmf::water::WaterStorage& origin,bool uptake )	: q(flux),c()
{
	size_t i=0;
	for(cmf::water::Solutes::const_iterator it = cmf::water::Solutes::all().begin(); it != cmf::water::Solutes::all().end(); ++it)
	{
		const cmf::water::SoluteStorage& solstor=origin.Solute(*it);
		if (uptake)
			c[i++]= solstor.conc() * solstor.Solute.Uptake;
		else
			c[i++]=solstor.conc();
	}
}