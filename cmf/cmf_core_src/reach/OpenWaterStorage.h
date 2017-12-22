

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
#ifndef OpenWaterStorage_h__
#define OpenWaterStorage_h__

#include "../water/WaterStorage.h"
#include "ReachType.h"
#include "../math/real.h"
namespace cmf {
	///	Contains storages and connection for the simulation of surface water bodies, like rivers, channels, lakes and dams
	namespace river {
		/// @ingroup storages
		/// @brief An open water body. The potential is calculated from the stored water using a water table function
		class OpenWaterStorage : public cmf::water::WaterStorage
		{
		public:
			typedef std::shared_ptr<cmf::river::OpenWaterStorage > ptr;
#ifndef SWIG
			operator ptr() {return std::static_pointer_cast<OpenWaterStorage >(shared_from_this());}
#endif
		protected:
			std::unique_ptr<IVolumeHeightFunction> height_function;
			/// Creates an open water storage with a prismatic volume			
			OpenWaterStorage(cmf::project& _project,real Area);
			/// Creates an open water storage with any type of a volume
			OpenWaterStorage(cmf::project& _project, const cmf::river::IVolumeHeightFunction& base_geo);

			virtual real head_to_volume(real head) const;
			virtual real volume_to_head(real volume) const;

		public:
			/// The functional relation between volume, depth and exposed area
			virtual const IVolumeHeightFunction& get_height_function() const {
				return *height_function;
			}
			virtual void set_height_function(const IVolumeHeightFunction& val){
				IVolumeHeightFunction* new_hf = val.copy();
				height_function.reset(new_hf);
			}

			/// Returns the water table depth 
			real get_depth() const;
			void set_depth(real new_depth) {set_volume(height_function->V(new_depth));}
			/// Returns the exposed surface area in m2
			real wet_area() const {return height_function->A(maximum(0,get_volume()));}
			/// Creates an open water storage with a prismatic volume			
			static ptr create(cmf::project& _project,real Area);
			/// Creates an open water storage with any type of a volume
			static ptr create(cmf::project& _project, const cmf::river::IVolumeHeightFunction& base_geo);
			real conc(cmf::math::Time t,const cmf::water::solute& solute)	const;
			// Creates an open water storage from a flux node with a prismatic volume
			//static ptr from_node(cmf::water::flux_node::ptr node,real Area);

			real get_abs_errtol(real rel_errtol) const;
			
			/// Casts a flux node to an open water storage
			static ptr cast(cmf::water::flux_node::ptr node);
		};
	}
}

#endif // OpenWaterStorage_h__
