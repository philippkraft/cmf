#include "statevariable.h"
#include <algorithm>
#include <stdexcept>
#include <map>

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

void cmf::math::StateVariable::add_connected_states(list& states)
{
	states.push_back(this);
}


cmf::math::sparse_structure::sparse_structure()
	: N(0), NP(0), NNZ(0) {
}

size_t cmf::math::sparse_structure::generate(const StateVariableList & states)
{
	N = states.size();
	StateVariable::list pointer_list;
	std::map<StateVariable*, size_t> pointer_positions;
	for (size_t i = 0; i < states.size(); ++i) {
		StateVariable * state_ptr = states[i].get();
		pointer_positions[state_ptr] = i + 1;
	}
	// Pointers of a csr sparse structure always start with 0
	indexpointers.push_back(0);
	// Better approach would be if a state can return (or append to) a list of its connected states
	for (int col = 0; col< N; ++col) {
		// Get the state to be tested for this col
		auto& col_state = *states[col];
		pointer_list.clear();
		col_state.add_connected_states(pointer_list);
		for (auto ptr : pointer_list) {
			size_t row = pointer_positions[ptr];
			if (row == 0) {
				throw std::runtime_error("sparse_structure: a connection outside the defining state list exists!");
			}
			indexvalues.push_back(row - 1);
		}
		// Write the position of the next entry (from the new row) into pointers
		indexpointers.push_back(indexvalues.size());
	}
	NP = indexpointers.size() - 1;
	NNZ = indexvalues.size();
	return NNZ;

}
