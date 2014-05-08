#include "groundwater.h"
#include "../project.h"
#include "algorithm.h"
inline real cmf::upslope::aquifer::volume_to_head( real volume ) const
{
	real fill = volume/(porosity * area);
	return get_base_height() + fill;
}

inline real cmf::upslope::aquifer::head_to_volume( real head ) const
{
	real fill = head - get_base_height();
	return fill * area * porosity;
}


cmf::upslope::aquifer::aquifer(cmf::project& p, cmf::geometry::point _position, 
									   real _area, real _thickness, real _porosity,real _K/*=1e-4*/ )
	:	WaterStorage(p,"Groundwater storage",0), 
		area(_area),thickness(_thickness),porosity(_porosity),K(_K,_K,_K)
{
	if (area<=0.0) throw std::runtime_error("Area of aquifer must be positive");
	if (thickness<=0.0) throw std::runtime_error("Thickness of aquifer must be positive");
	if (porosity<=0.0) throw std::runtime_error("Porosity of aquifer must be positive");
	if (_K<0.0) throw std::runtime_error("Conductivity K of aquifer must be positive or zero");

	position = _position;
}

bool cell_is_lower(cmf::upslope::Cell* c1,cmf::upslope::Cell* c2)
{
	return c1->z < c2->z;
}


cmf::upslope::aquifer::aquifer( cell_vector& cells,real _thickness,real _porosity, real _K/*=1e-4*/ )
	:	WaterStorage(cells[0].get_project(),"Groundwater storage",0), 
		area(0),thickness(_thickness),K(_K,_K,_K), porosity(_porosity)
{
	if (cells.size() == 0) {
		throw std::runtime_error("No cells in given list of cells");
	}
	area = get_area(cells);
	position = get_center(cells);
	cell_vector::iterator min_z_cell=cells.begin();
	for (cell_vector::iterator it=cells.begin(); it!=cells.end();++it)
		if (it->z<min_z_cell->z)
			min_z_cell=it;
	if (min_z_cell->layer_count()>0) 
		position.z = min_z_cell->z - min_z_cell->get_layer(-1)->get_lower_boundary() - 0.5 * thickness;
	else
		position.z = min_z_cell->z - 0.5 * thickness;

}

void cmf::upslope::aquifer::set_potential( real new_potential )
{
	if (get_state_variable_content()=='h') {
		set_state(new_potential);
	} else {
		set_state(head_to_volume(new_potential));
	}
}

real cmf::upslope::aquifer::get_K( cmf::geometry::point direction ) const
{
	return fabs(cmf::geometry::dot(direction/(direction.x+direction.y+direction.z),K));
}
real cmf::upslope::aquifer_Darcy::calc_q( cmf::math::Time t )
{
	using namespace cmf::geometry;
	point d = left_node()->position - right_node()->position;
	real 
		gradient = (left_node()->get_potential() - right_node()->get_potential())/d.length(),
		K1 = gw1->get_K(d),
		K2 = gw2 ? gw2->get_K(d) : K1,		
		K = harmonic_mean(K1,K2),
		h1 = gw1->get_potential() - gw1->get_base_height(),
		h2 = gw2 ? gw2->get_potential() - gw2->get_base_height() : h1,
		h = mean(h1,h2),
		v = K * gradient,
		q = h * flux_width * v;
	return q;

}

cmf::upslope::aquifer_Darcy::aquifer_Darcy( aquifer::ptr left, cmf::water::flux_node::ptr right, real width )
:	cmf::water::flux_connection(left,right,"Darcy flow between groundwater storages"), flux_width(width)
{
	NewNodes();
}
