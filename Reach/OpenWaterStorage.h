#ifndef OpenWaterStorage_h__
#define OpenWaterStorage_h__

#include "../water/WaterStorage.h"
#include "ReachType.h"
#include "../math/real.h"
#include "../Upslope/cell.h"
namespace cmf {
	namespace river {
		class OpenWaterStorage;
		
		/// An open water body. The potential is calculated from the stored water using a water table function
		class OpenWaterStorage : public cmf::water::WaterStorage
		{
		public:
			typedef std::tr1::shared_ptr<cmf::river::OpenWaterStorage > ptr;
#ifndef SWIG
			operator ptr() {return std::tr1::static_pointer_cast<OpenWaterStorage >(shared_from_this());}
#endif
		private:
			volume_height_function height_function;
		protected:
			/// Creates an open water storage with a prismatic volume			
			OpenWaterStorage(const cmf::project& _project,real Area);
			/// Creates an open water storage with any type of a volume
			OpenWaterStorage(const cmf::project& _project, const cmf::river::IVolumeHeightFunction& base_geo);

		public:
			/// The functional relation between volume, depth and exposed area
			virtual const IVolumeHeightFunction& get_height_function() const 			{
				return height_function;
			}
			virtual void set_height_function(const IVolumeHeightFunction& val)			{
				height_function = val;
			}

			/// Returns the water table depth 
			real get_depth() const {return height_function.h(maximum(0,get_state()));}
			void set_depth(real new_depth) {set_volume(height_function.V(new_depth));}
			/// Returns the exposed surface area in m2
			real wet_area() const {return height_function.A(maximum(0,get_state()));}
			/// Returns the gravitational potential
			/// \f[ \Psi_G = z + h(V) \f]
			real get_potential() const {return Location.z+get_depth();} 
			/// Creates an open water storage with a prismatic volume			
			static ptr create(const cmf::project& _project,real Area)
			{
				ptr res(new OpenWaterStorage(_project,Area));
				return res;
			}
			/// Creates an open water storage with any type of a volume
			static ptr create(const cmf::project& _project, const cmf::river::IVolumeHeightFunction& base_geo)
			{
				return ptr(new OpenWaterStorage(_project,base_geo));
			}
			
			/// Creates an open water storage from a flux node with a prismatic volume
			static ptr from_node(cmf::water::flux_node::ptr node,real Area);
			
			/// Casts a flux node to an open water storage
			static ptr cast(cmf::water::flux_node::ptr node);
		};
	}
}

#endif // OpenWaterStorage_h__
