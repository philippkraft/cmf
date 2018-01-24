#ifdef EXPERIMENTAL

#include "jacobian.h"

inline double cmf::math::SparseMatrix::get_value(int row, int col) const {

	SparseMatrix::_sparse_jacobian_map_type::const_iterator it = _this_map.find(_get_position(row, col));
	if (it != _this_map.end()) {
		return it->second;
	}
	else
	{
		return 0.0;
	}
}

int cmf::math::initialize_jacobian(const StateVariableList & states, SparseMatrix & jacobian) {
	jacobian.clear();

	for (ptrdiff_t i = 0; i < states.size(); ++i) {
		StateVariable::ptr state = states[i]; 
	}
	return 0;

}

#endif