

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
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
#ifndef real_h__
#define real_h__

#include <string>
#include <limits>
#include <cstddef>
typedef double real;
#ifndef SWIG
const real REAL_MAX = std::numeric_limits<real>::max();
#endif


// Some helper functions
/// Returns the minimum of two values
real minimum(real a,real b);
real maximum(real a,real b);
real minmax(real x,real min,real max);
real mean(real a,real b);
real geo_mean(real a,real b);
real harmonic_mean(real a,real b);
real piecewise_linear(real x,real xmin,real xmax,real ymin=0,real ymax=1);

/// The boltzmann function, used in cmf at several places where a s-shaped curve is needed
///
/// \f[f(x,x_{1/2},\tau)=\frac{1}{1+e^{-\frac{x-x_{1/2}}{tau}}}\f]
real boltzmann(real x,real x_half,real tau);
real sign(real x);
real square(real x);

const real Pi=3.141592654;

namespace cmf {
    typedef std::string bytestring;



}


#endif // real_h__
