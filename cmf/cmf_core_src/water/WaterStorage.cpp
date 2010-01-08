#include "WaterStorage.h"
#include "../project.h"

using namespace cmf::water;

struct null_deleter {	void operator()(void const *) const {}};

void WaterStorage::initializeSoluteStorages(const solute_vector& solutes)
{
	for (solute_vector::const_iterator it=solutes.begin();it!=solutes.end();++it)
	{
		std::tr1::shared_ptr<SoluteStorage> s(new SoluteStorage(this,*it));
		m_Concentrations.push_back(s);
	}
}

void WaterStorage::AddStateVariables( cmf::math::StateVariableVector& vector )
{
	vector.push_back(this);
	for(SoluteStorageMap::const_iterator it = m_Concentrations.begin(); it != m_Concentrations.end(); ++it)
	{
	    vector.push_back(it->get());
	}
}

WaterStorage::WaterStorage(const cmf::project& _project,double InitialState/*=0*/ ) 
: cmf::math::StateVariable(InitialState),flux_node(_project) ,m_Concentrations() //, this_no_delete(this,null_deleter())
{
	initializeSoluteStorages(_project.solutes);
}


SoluteStorage& WaterStorage::Solute( const solute& _Solute )
{
	return *m_Concentrations[_Solute.Id];
}


real WaterStorage::conc(const solute& _Solute) const
{
	return Solute(_Solute).conc();
}

std::tr1::shared_ptr<WaterStorage> WaterStorage::from_node( flux_node::ptr node )
{
	WaterStorage* ws=new WaterStorage(node->project());
	WaterStorage::ptr result(ws);

	replace_node(node,result);
	return result;
}
