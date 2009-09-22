#include "OpenWaterStorage.h"
cmf::river::OpenWaterStorage* cmf::river::OpenWaterStorage::FromNode( cmf::water::FluxNode& node,real Area )
{
	OpenWaterStorage* ows=new OpenWaterStorage(node.project(),Area);
	cmf::water::replace_node(node,*ows);
	return ows;
}
cmf::river::OpenWaterStorage* cmf::river::AsOpenWater(cmf::water::FluxNode* node)
{
	return dynamic_cast<cmf::river::OpenWaterStorage*>(node);
}

