#include "WaterStorage.h"
#include "../project.h"

using namespace cmf::water;

struct null_deleter {	void operator()(void const *) const {}};

void WaterStorage::initializeSoluteStorages(const solute_vector& solutes)
{
	for (solute_vector::const_iterator it=solutes.begin();it!=solutes.end();++it)
	{
		std::tr1::shared_ptr<SoluteStorage> s(new SoluteStorage(this_,*it));
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
: cmf::math::StateVariable(InitialState),flux_node(_project) ,m_Concentrations(), this_(this,null_deleter())
{
	initializeSoluteStorages(_project.solutes);
}


SoluteStorage& WaterStorage::Solute( const solute& solute )
{
	return *m_Concentrations[solute.Id];
}


real WaterStorage::conc(const solute& solute) const
{
	return Solute(solute).conc();
}

std::tr1::shared_ptr<WaterStorage> WaterStorage::from_node( flux_node::ptr node )
{
	WaterStorage* ws=new WaterStorage(node->project());
	storage_pointer result(ws);

	replace_node(node,result);
	return result;
}
