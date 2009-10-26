#include "OpenWaterStorage.h"

cmf::river::open_water_storage_ptr cmf::river::OpenWaterStorage::from_node( cmf::water::flux_node::ptr node,real Area )
{
	cmf::river::OpenWaterStorage::ptr ows=cmf::river::OpenWaterStorage::create(node->project(),Area);
	cmf::water::replace_node(node,ows);
	return ows;
}

cmf::river::open_water_storage_ptr cmf::river::OpenWaterStorage::cast( cmf::water::flux_node::ptr node )
{
	return std::tr1::dynamic_pointer_cast<OpenWaterStorage>(node);
}

cmf::river::OpenWaterStorage::OpenWaterStorage( const cmf::project& _project,real Area ) 
: cmf::water::WaterStorage(_project,0), height_function(Prism(Area))
{

}

cmf::river::OpenWaterStorage::OpenWaterStorage( const cmf::project& _project, 
																							 const cmf::river::IVolumeHeightFunction& base_geo )
: cmf::water::WaterStorage(_project,0), height_function(base_geo)
{

}