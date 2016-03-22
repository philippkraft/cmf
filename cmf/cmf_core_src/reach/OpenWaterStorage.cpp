

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
#include "OpenWaterStorage.h"

// cmf::river::OpenWaterStorage::ptr cmf::river::OpenWaterStorage::from_node( cmf::water::flux_node::ptr node,real Area )
// {
// 	cmf::river::OpenWaterStorage::ptr ows=cmf::river::OpenWaterStorage::create(node->get_project(),Area);
// 	cmf::water::replace_node(node,ows);
// 	return ows;
// }

cmf::river::OpenWaterStorage::ptr cmf::river::OpenWaterStorage::cast( cmf::water::flux_node::ptr node )
{
	return std::dynamic_pointer_cast<OpenWaterStorage>(node);
}

cmf::river::OpenWaterStorage::OpenWaterStorage(cmf::project& _project,real Area ) 
: cmf::water::WaterStorage(_project,"Unnamed OpenWaterStorage",0), height_function(new Prism(Area))
{

}

cmf::river::OpenWaterStorage::OpenWaterStorage(cmf::project& _project, const cmf::river::IVolumeHeightFunction& base_geo )
: cmf::water::WaterStorage(_project,"Unnamed OpenWaterStorage",0), height_function(base_geo.copy())
{

}

inline real cmf::river::OpenWaterStorage::head_to_volume(real head) const
{
	return get_height_function().V(head - this->position.z);
}
inline real cmf::river::OpenWaterStorage::volume_to_head(real volume) const
{
	return get_height_function().h(volume) + this->position.z;
}

real cmf::river::OpenWaterStorage::get_depth() const
{
	if (get_state_variable_content()=='h') 
		return std::max(get_state() - position.z,0.0);
	else
		return get_height_function().h(std::max(0.0,get_state()));
}


real cmf::river::OpenWaterStorage::conc( cmf::math::Time t,const cmf::water::solute& solute ) const
{
	return is_empty()    * cmf::water::flux_node::conc(t,solute)
		+ (1-is_empty()) * cmf::water::WaterStorage::conc(t,solute);
}

cmf::river::OpenWaterStorage::ptr cmf::river::OpenWaterStorage::create(cmf::project& _project, const cmf::river::IVolumeHeightFunction& base_geo )
{
	return ptr(new OpenWaterStorage(_project,base_geo));
}

cmf::river::OpenWaterStorage::ptr cmf::river::OpenWaterStorage::create(cmf::project& _project,real Area )
{
	ptr res(new OpenWaterStorage(_project,Area));
	return res;
}

real cmf::river::OpenWaterStorage::get_abs_errtol( real rel_errtol ) const
{
	return rel_errtol * get_height_function().V(1);
}