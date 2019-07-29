//
// Created by kraft-p on 01.04.2019.
//

#include "sparse_struct.h"
#include "statevariable.h"
#include <algorithm>
#include <stdexcept>
#include <map>

cmf::math::sparse_structure::sparse_structure()
        : N(0), NP(0), NNZ(0) {
}

size_t cmf::math::sparse_structure::generate(const cmf::math::state_list & states)
{
    this->errors.clear();
    N = states.size();
    StateVariable::list pointer_list;
    std::map<cmf::math::StateVariable*, size_t> pointer_positions;
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
                errors.push_back(ptr->to_string() + " is connected with " + col_state.to_string() + " but not part of the provided states");
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
