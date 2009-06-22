#include "WaterStorage.h"

void cmf::water::WaterStorage::initializeSoluteStorages()
{
	cmf::water::Solutes::all().SetInUse();
	for (Solutes::iterator it=cmf::water::Solutes::all().begin();it!=cmf::water::Solutes::all().end();++it)
	{
		m_Concentrations.push_back(new SoluteStorage(*this,*it));
	}
}

void cmf::water::WaterStorage::AddStateVariables( cmf::math::StateVariableVector& vector )
{
	vector.push_back(this);
	vector.insert(vector.end(),m_Concentrations.begin(),m_Concentrations.end());
}

cmf::water::WaterStorage::WaterStorage(const cmf::project& _project,double InitialState/*=0*/ ) : cmf::math::StateVariable(InitialState),cmf::water::FluxNode(_project) ,m_Concentrations()
{
	initializeSoluteStorages();
}

cmf::water::WaterStorage::WaterStorage( const WaterStorage& forcopy ) : cmf::math::StateVariable(forcopy.get_state()), cmf::water::FluxNode(forcopy),m_Concentrations()
{
	initializeSoluteStorages();
	for(SoluteStorageMap::iterator it = m_Concentrations.begin(); it != m_Concentrations.end(); ++it)
	{
		(**it).set_state(forcopy.Solute((**it).Solute).get_state());
	}
}

cmf::water::SoluteStorage& cmf::water::WaterStorage::Solute( const cmf::water::Solute& solute )
{
	return *m_Concentrations[solute.Id];
}

real cmf::water::WaterStorage::conc( const cmf::water::Solute& solute ) const
{
	if (this->get_state()>0)
		return Solute(solute).get_state()/this->get_state();
	else return 0	;
}

cmf::water::WaterQuality cmf::water::WaterStorage::conc(cmf::math::Time t)
{
	WaterQuality wq;
	size_t i=0;
	for(SoluteStorageMap::iterator it = m_Concentrations.begin(); it != m_Concentrations.end(); ++it)
	{
		wq[i++]=conc((**it).Solute);
	}
	return wq;
}
cmf::water::WaterStorage* cmf::water::AsWaterStorage(cmf::math::StateVariable * state)
{
	return dynamic_cast<cmf::water::WaterStorage*>(state);	
}
cmf::water::WaterStorage* cmf::water::AsWaterStorage(cmf::water::FluxNode* node)
{
	return dynamic_cast<cmf::water::WaterStorage*>(node);
}
