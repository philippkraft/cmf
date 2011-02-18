#include "simple_connections.h"


void cmf::water::set_flux( flux_node::ptr source,flux_node::ptr target,real flux_value)
{
	external_control_connection* con = dynamic_cast<external_control_connection*>(source->connection_to(*target));
	if (con) {
		con->flux = (con->left_node() == source ? flux_value  : -flux_value);
	} else {
		throw std::runtime_error("No external controlled connection between " 
			+ source->to_string() + " and " 
			+ target->to_string()+". \n You can create one overriding any existing connection (Python):\n>>> cmf.connect(" + source->Name + "," + target->Name + ",flux)");
	}

}

bool cmf::water::can_set_flux( flux_node::ptr source,flux_node::ptr target )
{
	external_control_connection* con = dynamic_cast<external_control_connection*>(source->connection_to(*target));
	return con != 0;
}




real cmf::water::kinematic_wave::calc_q( cmf::math::Time t )
{
	real V= std::max(0.0,source->get_volume()-residual_volume);
	return pow(V,exponent)/residencetime;
}


cmf::water::kinematic_wave::kinematic_wave( WaterStorage::ptr source,flux_node::ptr target,real _traveltime, real _exponent/*=1.0*/, real _residual_volume/*=0.0*/ ) 
: flux_connection(source,target,"kinematic wave"),residencetime(_traveltime),exponent(_exponent), residual_volume(_residual_volume)
{
	NewNodes();
}
