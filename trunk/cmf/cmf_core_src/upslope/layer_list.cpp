#include "layer_list.h"

using namespace cmf::upslope;
using namespace cmf::water;


#define CMF_LAYERLIST_ARRAY_GET(getter) cmf::math::num_array layer_list::getter() const { \
	cmf::math::num_array res(size()); \
	for (size_t i = 0; i < size() ; ++i) \
		res[i] = m_layers[i]->getter(); \
	return res; }

#define CMF_LAYERLIST_ARRAY_SET(setter) void layer_list::setter(const cmf::math::num_array& Values,size_t offset) { \
	for (size_t i = offset; (i < size()) && (i < Values.size()+offset); ++i) \
		m_layers[i]->setter(Values[i]); }

CMF_LAYERLIST_ARRAY_GET( get_potential)
CMF_LAYERLIST_ARRAY_GET( get_wetness)
CMF_LAYERLIST_ARRAY_GET( get_matrix_potential)
CMF_LAYERLIST_ARRAY_GET( get_porosity)
CMF_LAYERLIST_ARRAY_GET( get_volume)
CMF_LAYERLIST_ARRAY_GET( get_thickness)
CMF_LAYERLIST_ARRAY_GET( get_gravitational_potential)
CMF_LAYERLIST_ARRAY_GET( get_K)
CMF_LAYERLIST_ARRAY_GET( get_Ksat)
CMF_LAYERLIST_ARRAY_GET( get_lower_boundary)
CMF_LAYERLIST_ARRAY_GET( get_upper_boundary)

CMF_LAYERLIST_ARRAY_SET( set_potential)
CMF_LAYERLIST_ARRAY_SET( set_volume)
CMF_LAYERLIST_ARRAY_SET( set_wetness)


layer_list::layer_list(const cmf::water::node_list& for_copy) 
{
	for(cmf::water::node_list::const_iterator it = for_copy.begin(); it != for_copy.end(); ++it)
	{
		SoilLayer::ptr l = SoilLayer::cast(**it);
		if (l) m_layers.push_back(l);
	}
}

layer_list& layer_list::append(const cmf::water::node_list& nl) { 
	for(node_list::const_iterator it = nl.begin(); it != nl.end(); ++it)
	{
		SoilLayer::ptr l = SoilLayer::cast(**it);
		if (l) m_layers.push_back(l);
	}
	return *this;
}

cmf::upslope::layer_list cmf::upslope::layer_list::get_slice( size_t first/*=0*/, size_t last/*=1000000*/, size_t step /*= 1*/ )
{
	layer_list res;
	for (size_t i = first; i < last && i < size(); i+=step)
		res.append((*this)[i]);
	return res;
}