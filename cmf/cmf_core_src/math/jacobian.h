#ifdef EXPERIMENTAL
#ifndef jacobian_h__
#define jacobiab_h__

#include <map>
#include "statevariable.h"

namespace cmf {
	namespace math {
		/// @brief A class to hold the Jacobian of the system as a sparse matrix
		///
		/// 
		class SparseMatrix {
		private:
			typedef std::map< long, double > _sparse_jacobian_map_type;
			_sparse_jacobian_map_type _this_map;
			long _get_position(int row, int col) const {
				return row * 1000000 + col;
			}
		public:
			double get_value(int row, int col) const;
			double set_value(int row, int col, double value) {
				_this_map[_get_position(row, col)] = value;
			}

			void clear() {
				_this_map.clear();
			}
		};

		int initialize_jacobian(const StateVariableList& states, SparseMatrix& jacobian);
	}

}

#endif
#endif