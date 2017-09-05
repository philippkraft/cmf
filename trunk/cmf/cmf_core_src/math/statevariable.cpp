#include "statevariable.h"
#include <algorithm>
#include <stdexcept>
using namespace cmf::math;
void integratable_list::append( cmf::math::integratable::ptr add )
{
	m_items.push_back(add);
}


cmf::math::num_array cmf::math::integratable_list::avg() const
{
	num_array res(size());
	for (size_t i = 0; i < size() ; ++i)
		res[i] = m_items[i]->avg();
	return res;
}

cmf::math::num_array cmf::math::integratable_list::sum() const
{
	num_array res(size());
	for (size_t i = 0; i < size() ; ++i)
		res[i] = m_items[i]->sum();
	return res;

}

void cmf::math::integratable_list::reset( Time t )
{
	for (size_t i = 0; i < size() ; ++i)
		m_items[i]->reset(t);
	
}

void cmf::math::integratable_list::integrate( Time t )
{
	for (size_t i = 0; i < size() ; ++i)
		m_items[i]->integrate(t);
}

void cmf::math::integratable_list::remove( cmf::math::integratable::ptr rm )
{
	integ_vector::iterator it =std::find(m_items.begin(),m_items.end(),rm);
	if (it!=m_items.end()) 
		m_items.erase(it);
	else 
		throw std::out_of_range("Can't remove item. Integratable not in list");

}
void cmf::math::StateVariable::set_state(real newState) {
	if (!std::isfinite(newState)) {
		throw std::runtime_error("New state of " + this->to_string() + " is not finite");
	}
	m_State = newState;
	m_StateIsNew = true;
	if (m_StateIsNew) StateChangeAction();
}
