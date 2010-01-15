

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

cmf::river::OpenWaterStorage::ptr cmf::river::OpenWaterStorage::from_node( cmf::water::flux_node::ptr node,real Area )
{
	cmf::river::OpenWaterStorage::ptr ows=cmf::river::OpenWaterStorage::create(node->project(),Area);
	cmf::water::replace_node(node,ows);
	return ows;
}

cmf::river::OpenWaterStorage::ptr cmf::river::OpenWaterStorage::cast( cmf::water::flux_node::ptr node )
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