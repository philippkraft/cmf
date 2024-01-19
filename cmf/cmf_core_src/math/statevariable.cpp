#include "statevariable.h"
#include <algorithm>
#include <stdexcept>
#include <map>

using namespace cmf::math;
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

state_list cmf::math::operator+(const state_list &left, const state_list &right) {
    state_list res;
    res += left;
    res += right;
    return res;
}

state_list &state_list::operator+=(const state_list &food) {
	this->extend(food);
	return *this;
}

state_list &state_list::extend(const state_list &svl) {
	insert(end(), svl.begin(), svl.end());
	return *this;
}

state_vector::iterator state_list::begin() {return state_vector::begin();}

state_list::state_list() : state_vector() {}

state_list::state_list(const state_list &other) {
    insert(end(), other.begin(), other.end());
}

size_t state_list::size() const {
    return state_vector::size();
}

state_list::operator bool() const {return size()>0;}

