#include "ManningConnection.h"
#include "../Upslope/cell.h"
#include "../Upslope/Topology.h"
real cmf::river::Manning::calc_q( cmf::math::Time t )
{
	real 
		// Distance between source and target
		d=distance,
		// Gradient of the reach
		slope=(m_left->Potential()-m_right->Potential())/d;
	
	// Get the source of the flow
	OpenWaterStorage* source=slope>0 ? w1 : w2 && slope!=0 ? w2 : 0;
	if (source==0) return 0; // Never generate flow from a flux node
		// Wetted crossectional area use mean volume of the water storages if both sides are water storages
	real
		// Flow height between elements is the mean of the flow height, but exceeds never the flow height of the source
		h=minimum(source->h(),w2 ? mean(w1->h(),w2->h()) : w1->h());
	if (h<=0) return 0;
	real
		// Depth of the reach
		A=flux_geometry->Area(h),
		// Wetted perimeter of the reach
		P=maximum(flux_geometry->Perimeter(h),0.001),
		// Absolute flux in m3/s
		qManning=A*pow((A/P),0.6666667)*sqrt(abs(slope)/flux_geometry->nManning);
	return qManning * sign(slope) * (24*60*60);
}

void cmf::river::Manning::connect_cells( cmf::upslope::Cell& c1,cmf::upslope::Cell& c2,int dummy )
{
	real w=c1.get_topology().flowwidth(c2);
	if (w<=0) return;
	RectangularReach r_type(w);
	cmf::river::OpenWaterStorage& sw1= *dynamic_cast<cmf::river::OpenWaterStorage*>(&c1.AddStorage("SurfaceWater",'W'));
	cmf::river::OpenWaterStorage& sw2= *dynamic_cast<cmf::river::OpenWaterStorage*>(&c2.AddStorage("SurfaceWater",'W'));
	new Manning(sw1,sw2,r_type,c1.Center().distanceTo(c2.Center()));
}

const cmf::upslope::CellConnector cmf::river::Manning::cell_connector=cmf::upslope::CellConnector(&connect_cells);

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
