#ifndef OpenWaterStorage_h__
#define OpenWaterStorage_h__

#include "../water/WaterStorage.h"
#include "VolumeHeightRelation.h"
#include "ReachType.h"
#include "../math/real.h"
#include "../Upslope/cell.h"
namespace cmf {
	namespace river {
		class OpenWaterStorage : public cmf::water::WaterStorage
		{
		private:
			std::auto_ptr<VolumeHeightRelation> v_to_height;
		protected:
			OpenWaterStorage(const cmf::project& _project,cmf::river::VolumeHeightRelation* base_geo) :WaterStorage(_project), v_to_height(base_geo) {}
		public:
			void SetBaseTopography(const cmf::river::VolumeHeightRelation& new_base_topo) 
			{v_to_height.reset(new_base_topo.copy());}
			const cmf::river::VolumeHeightRelation& BaseTopography() const
			{return *v_to_height;}
			real h() const {return v_to_height->h(maximum(0,get_state()));}
			real wet_area() const {return v_to_height->A(maximum(0,get_state()));}
			real get_potential() const {return Location.z+h();} 
			OpenWaterStorage(const cmf::project& _project,real Area) : cmf::water::WaterStorage(_project,0), v_to_height(new cuboid(Area)) {}
			OpenWaterStorage(const cmf::project& _project,const cmf::river::VolumeHeightRelation& base_geo) : cmf::water::WaterStorage(_project,0), v_to_height(base_geo.copy()) {}
			static OpenWaterStorage* FromNode(cmf::water::FluxNode& node,real Area);
			static OpenWaterStorage* cast(cmf::water::FluxNode& node)
			{
				OpenWaterStorage* res=dynamic_cast<OpenWaterStorage*>(&node);
				return res;
			}
		};
		cmf::river::OpenWaterStorage* AsOpenWater(cmf::water::FluxNode* node);
	}
}

#endif // OpenWaterStorage_h__
