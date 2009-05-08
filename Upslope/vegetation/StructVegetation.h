#ifndef StructVegetation_h__
#define StructVegetation_h__
#include <cmath>
namespace cmf {
	namespace upslope{
		namespace vegetation {
			/// Holds the vegetation parameters for the calculation of ET and fractionating rainfall. Not every ET method uses all parameters
			struct Vegetation{
				double 
					LAI, ///< Leaf area index
					Height, ///< Height of the vegetation in m
					albedo, ///< Fraction of reflected radiation
					CanopyCapacityPerLAI, ///< Capacity of interception storage per LAI unit in mm
					RootDepth, ///< Depth of rooting zone in m, see fraction_at_rootdepth
					fraction_at_rootdepth, ///< Fraction of roots in the rooting zone. If 1 (default), a uniform root distribution is used, otherwise an exponential decay of the root density with depth is used.
					StomatalResistance, ///< optimal stomatal resistence of the plant tissue for vapor, in s/m
					CanopyClosure, ///< Fraction of bare soil to the surface, influences the routing of rainfall
					LeafWidth;     ///< Average width of leaves in m (only for Shuttleworth-Wallace ET)
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
						return abs(fraction_to_lower-fraction_to_upper);
					}
				}
				Vegetation(double _LAI=12,double _Height=0.12,double _RootDepth=0.25,double _StomatalResistance=100,
					double _albedo=0.23,double _CanopyClosure=1,double _CanopyCapacityPerLAI=0.1,double _fraction_at_rootdepth=1.0)
					: LAI(_LAI),Height(_Height),albedo(_albedo),
					  CanopyCapacityPerLAI(_CanopyCapacityPerLAI),
						RootDepth(_RootDepth),StomatalResistance(_StomatalResistance),
						CanopyClosure(_CanopyClosure)	, LeafWidth(0.05),fraction_at_rootdepth(_fraction_at_rootdepth)
				{
				}
			};			

		}		
	}	
}

#endif // StructVegetation_h__
