#include "Solute.h"
#include <iostream>
cmf::water::Solutes cmf::water::Solutes::m_All=cmf::water::Solutes();

const cmf::water::Solute& cmf::water::Solutes::add( const std::string& name,const std::string& unit/*=""*/,double uptake/*=1*/ )
{
	if (InUse) throw std::runtime_error("Solute list already in use by model, cannot create more solutes");

	Solute s(m_Solutes.size(),name,unit,uptake);
	m_Solutes.push_back(s);
	return m_Solutes.back();
}

void cmf::water::Solutes::SetInUse()
{
	if (!InUse)
	{
		std::cout << "Solutes in use, not possible to add or remove solutes" << std::endl;
	}
	InUse=true;
}
cmf::water::Solute& cmf::water::Solute::operator=( const Solute& copy )
{
	Name=copy.Name;
	Uptake=copy.Uptake;
	Unit=copy.Unit;
	m_Id=copy.Id;
	return *this;
}

const cmf::water::Solute& cmf::water::Solute::create( const std::string& name,const std::string& unit/*=""*/,double uptake/*=1*/ )
{
	return Solutes::all().add(name,unit,uptake);
}
void cmf::water::WaterQuality::clear()
{
	for(Concentrations::iterator it = conc.begin(); it != conc.end(); ++it)
		*it=0;
}

