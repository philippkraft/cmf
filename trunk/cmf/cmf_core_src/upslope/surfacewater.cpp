#include <limits>
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
	return std::tr1::dynamic_pointer_cast<SurfaceWater>(node);
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
	flux_node::ptr right = right_node();
	real d = left->get_depth() - left->get_puddledepth();
	if (d<=0.0) {
		return 0.0;
	}
	real dz = left->get_potential() - right->get_potential();
	real slope = dz/m_distance;
	real s_sqrt = sign(slope) * sqrt(abs(slope));

	// linear slope width is a value in wihcih slope range the slope should be altered
	// to prevent a singularity in dq/ds
	if (this->linear_slope_width) {
		real
			// Only a shortcut for faster writing
			s0 = this->linear_slope_width,
			// Weight of linear part
			w_lin = exp(-square((slope/s0))),
			// linear part using the slope at s0/4 
			s_lin = slope/(2.*sqrt(s0/4));
			// Weighted sum of sqrt(s) and a*s
		s_sqrt = w_lin * s_lin + (1-w_lin) * s_sqrt;
	}

	return prevent_negative_volume(m_flowwidth * pow(d,5/3.) * s_sqrt/left->get_nManning() * 86400.);
}

void DiffusiveSurfaceRunoff::NewNodes()
{
	wleft=SurfaceWater::cast(left_node());
}