//
// Created by kraft-p on 01.04.2019.
//

#ifndef CMF_SPARSE_STRUCT_H
#define CMF_SPARSE_STRUCT_H

#include <vector>
#include "statevariable.h"
namespace cmf {
    namespace math {
        struct sparse_structure {
            std::vector<size_t> indexvalues;
            std::vector<size_t> indexpointers;
            /// @brief Creates the sparse row compressed structure from states variable list
            sparse_structure();
            size_t generate(const cmf::math::StateVariableList& states);
            size_t NNZ, NP, N;
        };

    }
}

#endif //CMF_SPARSE_STRUCT_H
