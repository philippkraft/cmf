

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
//   
#ifndef StructVegetation_h__
#define StructVegetation_h__
#include <cmath>
#include "../../math/num_array.h"
namespace cmf {
	namespace upslope{
		namespace vegetation {
			/// Holds the vegetation parameters for the calculation of ET and fractionating rainfall. Not every ET method uses all parameters
			struct Vegetation{
				double 
					LAI, ///< Leaf area index
					Height, ///< Height of the vegetation in m
					albedo, ///< Fraction of reflected radiation
					snow_albedo, ///< Fraction of reflected radiation at snow cover
					CanopyCapacityPerLAI, ///< Capacity of interception storage per LAI unit in mm
					RootDepth, ///< Depth of rooting zone in m, see fraction_at_rootdepth
					RootContent, ///< average fine root content in kg/m3 (default 2 kg/m3) in root depth.
					fraction_at_rootdepth, ///< Fraction of roots in the rooting zone. If 1 (default), a uniform root distribution is used, otherwise an exponential decay of the root density with depth is used.
					StomatalResistance, ///< optimal stomatal resistence of the plant tissue for vapor, in s/m
					CanopyClosure, ///< Fraction of bare soil to the surface, influences the routing of rainfall
					CanopyPARExtinction, ///<  extinction coefficient for photosynthetically-active radiation in the canopy. Values usually range from 0.5 to 0.7. Values outside this range should be used very cautiously. 
					LeafWidth;     ///< Average width of leaves in m (only for Shuttleworth-Wallace ET)
				
				/// Returns the average root length in m/m2
				/// \f[ l_R \left[\frac{m}{m^2}\right]= \frac{ c_R \left[\frac{kg}{m^3}\right] z_R \left[m\right] } {0.5\left[\frac{kg}{m^3}\right] \pi \left(d_R/2\right)^2 \left[m^2\right]} \f]
				double RootLength() const {
					return RootContent /*kg/m3*/ * RootDepth /*m*/ / (0.5/*kg/m3*/ * Pi * (0.0035 * 0.0035) /*m2*/) ;
				}
				virtual double RootFraction(double upperBoundary,double lowerBoundary) const
				{
					if (fraction_at_rootdepth>=1)
					{
						if (RootDepth<=0. || upperBoundary>RootDepth || upperBoundary>lowerBoundary)
							return 0.;
						else if (lowerBoundary>RootDepth)
							return (RootDepth-upperBoundary)/RootDepth;
						else
							return (lowerBoundary-upperBoundary)/RootDepth;
					}
					else
					{
						double fraction_to_upper=pow(1/(1-fraction_at_rootdepth),-upperBoundary/RootDepth);
						double fraction_to_lower=pow(1/(1-fraction_at_rootdepth),-lowerBoundary/RootDepth);
						return fabs(fraction_to_lower-fraction_to_upper);
					}
				}
				cmf::math::num_array RootFraction(const cmf::math::num_array& thickness) const {
					cmf::math::num_array res(thickness.size());
					double ub=0,lb=0;
					for (int i = 0; i < thickness.size() ; ++i)
					{
						lb+=thickness[i];
						res[i] = RootFraction(ub,lb);
						ub=lb;
					}
					return res;
				}
				Vegetation(double _LAI=2.88,double _Height=0.12,double _RootDepth=0.25,double _StomatalResistance=100,
					double _albedo=0.23,double _CanopyClosure=1,double _CanopyCapacityPerLAI=0.1,double _fraction_at_rootdepth=1.0)
					: LAI(_LAI),Height(_Height),albedo(_albedo), RootContent(2.0),
					  CanopyCapacityPerLAI(_CanopyCapacityPerLAI),CanopyPARExtinction(0.6),
					  RootDepth(_RootDepth),StomatalResistance(_StomatalResistance),
					  CanopyClosure(_CanopyClosure), LeafWidth(0.05),fraction_at_rootdepth(_fraction_at_rootdepth), snow_albedo(0.85)
				{
				}
			};			

		}		
	}	
}

#endif // StructVegetation_h__
