#include <limits>
#include "project.h"
#include "surfacewater.h"
#include "cell.h"
#include "Topology.h"
using namespace cmf::upslope;
using namespace cmf::upslope::connections;
using namespace cmf::water;
using namespace cmf::river;
SurfaceWater::SurfaceWater( Cell& cell )
	: OpenWaterStorage(cell.get_project(),cell.get_area()),
	  m_cell(cell), m_nManning(0.1), m_height_function((Prism*)(height_function.get()))
{}




SurfaceWater::ptr SurfaceWater::cast( flux_node::ptr node )
{
	return std::dynamic_pointer_cast<SurfaceWater>(node);
}

const CellConnector KinematicSurfaceRunoff::cell_connector=CellConnector(KinematicSurfaceRunoff::connect_cells);

void KinematicSurfaceRunoff::connect_cells( cmf::upslope::Cell& cell1,cmf::upslope::Cell& cell2, ptrdiff_t dummy )
{
	Cell& upper = cell1.z>cell2.z ? cell1 : cell2;
	Cell& lower = cell1.z>cell2.z ? cell2 : cell1;

	SurfaceWater::ptr sw_upper = SurfaceWater::cast(upper.get_surfacewater());
	real w = upper.get_topology().flowwidth(lower.get_topology());
	if (sw_upper && w) {
		new KinematicSurfaceRunoff(sw_upper,lower.get_surfacewater(),w);
	}

}

real KinematicSurfaceRunoff::calc_q( cmf::math::Time t )
{
	SurfaceWater::ptr left = wleft.lock();
	flux_node::ptr right = right_node();
	real dz = left->position.z - right->position.z;
	real slope = dz/m_distance;
	real d = left->get_depth() - left->get_puddledepth();
	if (d<=0.0) {
		return 0.0;
	}
	return m_flowwidth * pow(d,5/3.) * sqrt(slope)/left->get_nManning() * 86400.;
}

void KinematicSurfaceRunoff::NewNodes()
{
	wleft=SurfaceWater::cast(left_node());
}


const CellConnector DiffusiveSurfaceRunoff::cell_connector=CellConnector(DiffusiveSurfaceRunoff::connect_cells);

void DiffusiveSurfaceRunoff::connect_cells( cmf::upslope::Cell& cell1,cmf::upslope::Cell& cell2, ptrdiff_t dummy )
{
	Cell& upper = cell1.z>cell2.z ? cell1 : cell2;
	Cell& lower = cell1.z>cell2.z ? cell2 : cell1;

	SurfaceWater::ptr sw_upper = SurfaceWater::cast(upper.get_surfacewater());
	real w = upper.get_topology().flowwidth(lower.get_topology());
	if (sw_upper && w) {
		new DiffusiveSurfaceRunoff(sw_upper,lower.get_surfacewater(),w);
	}

}

real DiffusiveSurfaceRunoff::calc_q( cmf::math::Time t )
{
	SurfaceWater::ptr left = wleft.lock();
	SurfaceWater::ptr sright = wright.lock();
	cmf::river::OpenWaterStorage::ptr oright = owright.lock();
	flux_node::ptr nright = right_node();
	
	// Depth of left surface water storage
	real dl = std::max(left->get_depth() - left->get_puddledepth(),0.0);

	// Depth of right node. 
	real dr;
	if (sright.get()) { 
		// For surfacewater: Use depth over puddledepth 
		dr = sright->get_depth() - sright->get_puddledepth();
	} else if (oright.get()) { 
		// For other openwaterstorage: Use depth over surface of left node
		dr = oright->get_potential(t) - left->position.z;
	} else {	
		// For other node: Use left depth
		dr = dl;
	}
	dr = std::max(0.0,dr); // Discard negative values

	// Use mean of left and right depth as intermediate flow depth
	real d = mean(dl,dr);
	//  Get potential difference
	real dPsi = left->get_potential(t) - nright->get_potential(t);
	
	// Get slope
	real grad = dPsi/m_distance;

	// Get signed square root for 
	real s_sqrt = sign(grad) * sqrt(std::abs(grad));

	// linear slope width is a value in which slope range the slope should be altered
	// to prevent a singularity in dq/ds
	if (cmf::options::diffusive_slope_singularity_protection > 0.0) {
        // Only a shortcut for faster writing
		const real & s0 = cmf::options::diffusive_slope_singularity_protection;
        // Weight of linear part
		real w_lin = exp(-square((grad/s0)));
        // linear part using the slope at s0/4
        real s_lin =grad/(2.*sqrt(s0/4));
        // Weighted sum of sqrt(s) and a*s
		s_sqrt = w_lin * s_lin + (1-w_lin) * s_sqrt;
	}

	return prevent_negative_volume(m_flowwidth * pow(d,5/3.) * s_sqrt/left->get_nManning() * 86400.);
}

void DiffusiveSurfaceRunoff::NewNodes()
{
	wleft=SurfaceWater::cast(left_node());
	wright = SurfaceWater::cast(right_node());
	owright=OpenWaterStorage::cast(right_node());
}
