// This file is part of cmf, but copied and modified from codereview.stackexchange, 
// original author: Grant Williams
// Source: https://codereview.stackexchange.com/q/103762
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   cmf is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
//   

// Orignial Author's notice
/*******************************************************************************
*
* Grant Williams
*
* Version 1.0.0
* August 27, 2015
*
* Test Program for Brent's Method Function.
*
* Brent's method makes use of the bisection method, the secant method, and inverse quadratic interpolation in one algorithm.
*
* To Compile Please use icc -std=c++11 if using intel or g++ -std=c++11 if using GCC.
*
********************************************************************************/

#ifndef ROOT_FINDING_H__
#define ROOT_FINDING_H__
#include <cmath>
#include <stdexcept>

namespace cmf {
	namespace math {
		namespace root_finding {
			class sign_error : public std::runtime_error {
			public:
				sign_error(std::string msg) : std::runtime_error(msg) {}
			};
			class not_finite_error : public std::runtime_error {
			public:
				not_finite_error(std::string msg) : std::runtime_error(msg) {}
			};
			class iteration_error : public std::runtime_error {
			public:
				iteration_error(std::string msg) : std::runtime_error(msg) {}
			};
			class BrentsMethod {
				double tolerance;
				unsigned int max_iterations;
			public:
				BrentsMethod(double tolerance = 1e-12, unsigned int max_iterations = 1000);
				virtual double f(double) const = 0;
				double operator()(double lower_bound, double upper_bound, double offset = 0) const;
			};
			class Bisect {
				double tolerance;
				unsigned int max_iterations;
			public:
				Bisect(double tolerance = 1e-12, unsigned int max_iterations = 1000);
				virtual double f(double) const = 0;
				double operator()(double lower_bound, double upper_bound, double offset = 0) const;
			};


		}
	}
}

#endif // !ROOT_FINDING_H__
