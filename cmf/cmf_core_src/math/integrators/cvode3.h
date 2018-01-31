// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   cmf is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with cmf.  If not, see <http://www.gnu.org/licenses/>.

#ifndef cvode3_h__
#define cvode3_h__

#include <memory>

#include "integrator.h"
#include "../time.h"

namespace cmf {
	namespace math {

		class CVode3 : public Integrator {
		private:
			class Impl;
			friend Impl;

			std::unique_ptr<Impl> _implementation;
			std::string _error_msg;
		public:
			virtual int integrate(cmf::math::Time t_max, cmf::math::Time dt);
			virtual void reset();
			CVode3(cmf::math::StateVariableOwner& states, real epsilon = 1e-9);
			void set_error_msg(std::string error);
			CVode3 * copy() const;

		};
	}
}

#endif