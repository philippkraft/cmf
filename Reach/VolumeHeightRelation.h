#ifndef VolumeHeightRelation_h__
#define VolumeHeightRelation_h__
#include "ReachType.h"
#include <memory>
namespace cmf {
	namespace river {
		/// Volume height relations are functional objects, which return a height and a crosssectional area of a volume for different geometric bodies.
		/// This is the abstract base class, where the geometries derive from
		class VolumeHeightRelation
		{
		public:
			/// Returns the height of a given volume in the geometric body
			virtual real h(real V) const=0;
			/// Returns the crossectional area of a given volume in the geometric body
			virtual real A(real V) const=0;
			virtual VolumeHeightRelation* copy() const=0;
			real operator ()(real V)
			{
				return h(V);
			}
		};
		/// Returns the height of a volume in a cuboid with a defined base area
		class cuboid : public VolumeHeightRelation
		{
		public:
			real Area;
			cuboid(real base_area) : Area(base_area) {}
			cuboid* copy() const {return new cuboid(Area);}
			/// \f$ h(V) = \frac V A_{base} \f$
			real h(real V) const {return V/Area;}
			/// \f$ A(V) = A_{base} \f$
			real A(real V) const {return Area;}
		};
		/// A body with infinite length and an exponential decline of the base (maximum) area
		class exponential_area_decline : public VolumeHeightRelation
		{
		public:
			real Area,half_area_depth;
			/// \f$ h(V)=h_{0.5}\frac{\log{\left(\log 2 \frac V {h_{0.5}}\right)}}{\log 2} \f$
			real h(real V) const {
				real lg2=log(2.0);
				real V_rel=maximum(V,0.0001)/Area;
				return half_area_depth*log(lg2*V_rel/half_area_depth)/lg2;
			}
			real A_d(real h) const {
				return pow(2,h)*Area;
			}
			/// \f$ A(V)=A_{max}=2^{h(V)} \f$
			real A(real V) const {
				return A_d(h(V));
			}
			exponential_area_decline* copy() const
			{
				return new exponential_area_decline(Area,half_area_depth);
			}
			exponential_area_decline(real area,real _half_area_depth) : Area(area),half_area_depth(_half_area_depth) {}
		};
		class Channel : public VolumeHeightRelation
		{
		private:
			std::auto_ptr<cmf::river::ReachType> type;
		public:
			real Length;
			Channel(const cmf::river::ReachType& _type,real l) : type(_type.copy()),Length(l) {}
			Channel(char reach_type_shortcut,real l,real d=0.25,real w=1.) : type(create_reachtype(reach_type_shortcut,w,d)),Length(l) {}
			Channel* copy() const {return new Channel(*type,Length);}
			real h(real V) const {return type->Depth(V/Length);}
			real A(real V) const {return type->Width(h(V))*Length;}
		};
	}
	
}
#endif // VolumeHeightRelation_h__

