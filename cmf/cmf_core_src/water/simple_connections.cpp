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




real cmf::water::kinematic_wave::calc_q(cmf::math::Time t)
{
	cmf::water::WaterStorage::ptr S = source.lock();
	real V = std::max(0.0, S->get_volume() - residual);
	return pow(V / V0, exponent) / residencetime;
}


cmf::water::kinematic_wave::kinematic_wave(WaterStorage::ptr source, flux_node::ptr target,
	real _traveltime, real _exponent/*=1.0*/,
	real _residual/*=0.0*/, real _V0)
	: flux_connection(source, target, "kinematic wave"), residencetime(_traveltime),
	exponent(_exponent), residual(_residual), V0(_V0)
{
	NewNodes();
}

real cmf::water::LinearStorageConnection::calc_q(cmf::math::Time t)
{
	cmf::water::WaterStorage::ptr S = source.lock();
	real V = std::max(0.0, S->get_volume() - residual);
	return V / residencetime;
}

cmf::water::LinearStorageConnection::LinearStorageConnection(
	WaterStorage::ptr source, flux_node::ptr target,
	real _residencetime, real _residual /* = 0.0 */)
	: flux_connection(source, target, "LinearStorageConnection"),
	residencetime(_residencetime), residual(_residual) 
{
	NewNodes();
}

real cmf::water::PowerLawConnection::calc_q(cmf::math::Time t)
{
	cmf::water::WaterStorage::ptr S = source.lock();
	real V = std::max(0.0, S->get_volume() - residual);
	return Q0 * pow(V / V0, beta);
}


cmf::water::PowerLawConnection::PowerLawConnection(WaterStorage::ptr source, flux_node::ptr target,
	real _Q0, real _V0, real _beta/*=1.0*/,
	real _residual/*=0.0*/)
	: flux_connection(source, target, "power law"), 
	Q0(_Q0), beta(_beta), residual(_residual), V0(_V0)
{
	NewNodes();
}

real cmf::water::ExponentialDeclineConnection::calc_q(cmf::math::Time t)
{
	cmf::water::WaterStorage::ptr S = source.lock();
	return Q0 * exp((S->get_volume() - V0) / m);
}

cmf::water::ExponentialDeclineConnection::ExponentialDeclineConnection(
	cmf::water::WaterStorage::ptr source,
	cmf::water::flux_node::ptr target,
	real _Q0, real _V0, real _m)
	: flux_connection(source, target, "exponential decline"), Q0(_Q0), V0(_V0), m(_m)
{
	NewNodes();
}
	
cmf::water::statecontrol_connection::statecontrol_connection( cmf::water::WaterStorage::ptr controlled_storage, cmf::water::flux_node::ptr other_end, 
															 real _target_state, cmf::math::Time _reaction_time ) 
: flux_connection(controlled_storage,other_end, "State controlling flux"), target_state(_target_state), reaction_time(_reaction_time)
{
	NewNodes();
}

void cmf::water::statecontrol_connection::NewNodes()
{
	source = cmf::water::WaterStorage::cast(left_node());
}

real cmf::water::statecontrol_connection::calc_q( cmf::math::Time t )
{
	real dV = source.lock()->get_state() - target_state ;
	if (dV > 0) // If water is to be removed from the controlled storage
		return (1-left_node()->is_empty()) * dV/reaction_time.AsDays();
	else // If the controlled storage is to be refilled
		return (1-right_node()->is_empty()) * dV/reaction_time.AsDays();
}

real cmf::water::generic_gradient_connection::calc_q( cmf::math::Time t )
{
	flux_node::ptr 
		left = left_node(),
		right = right_node();

	real gradient = (left->get_potential() - right->get_potential())/d;
	real q = K*A*gradient;
	return prevent_negative_volume(q);
}

cmf::water::generic_gradient_connection::generic_gradient_connection( cmf::water::WaterStorage::ptr left,
	cmf::water::WaterStorage::ptr right, real _K,real _d/*=1.0*/, real _A/*=1.0*/ )
	: flux_connection(left,right,"generic gradient connection")
{
	
}

cmf::water::constraint_kinematic_wave::constraint_kinematic_wave( WaterStorage::ptr source,WaterStorage::ptr target,real _residencetime/*=1.0*/, 
																  real _beta/*=1.0*/,real _residual/*=0.0*/,real _V0 /*= 1.0*/, 
																  real _Vrmax /*= 1.0*/, real _gamma/*=1.0*/ )
	: flux_connection(source,target,"kinematic wave"),residencetime(_residencetime),
	beta(_beta), residual(_residual), V0(_V0), Vrmax(_Vrmax),gamma(_gamma)
{
	NewNodes();
}

real cmf::water::constraint_kinematic_wave::calc_q( cmf::math::Time t )
{
	real V= std::max(0.0,source->get_volume()-residual);
	real C= std::max(0.0,Vrmax - target->get_volume());
	return pow(V/V0,beta)/residencetime * pow(C/Vrmax,gamma);
}

real cmf::water::bidirectional_kinematic_exchange::calc_q( cmf::math::Time t )
{
	cmf::water::WaterStorage::ptr S = source.lock();
	real V = S->get_volume();
	real D = Vmaxsuc-V;
	real U = V - Vminspill;
	real qsuc_a = D>0 ? qsuc * pow(D/Vmaxsuc,beta_suc) : 0;
	real qspill_a = U>0 ? qspill * pow(U/Vminspill,beta_spill) : 0;

	return qspill_a - qsuc_a;
	
}

cmf::water::bidirectional_kinematic_exchange::bidirectional_kinematic_exchange( 
	WaterStorage::ptr source,flux_node::ptr target, 
	real _Vminspill,real _Vmaxsuc, 
	real _qspill,real _qsuc, real _beta_spill,real _beta_suc )
	  :	flux_connection(source,target,"Bidirectional exchange"),
		Vminspill(_Vminspill),Vmaxsuc(_Vmaxsuc),
		qspill(_qspill),qsuc(_qsuc),
		beta_spill(_beta_spill),beta_suc(_beta_suc)
{
	NewNodes();
}


cmf::water::waterbalance_connection::waterbalance_connection(flux_node::ptr source, flux_node::ptr target)
	: flux_connection(source, target, "waterbalance connection")
{
	/*
	// Check if this is the first waterbalance connection
	cmf::water::connection_list source_cons = source->get_connections();
	for (cmf::water::connection_list::const_iterator it = source_cons.begin(); it != source_cons.end(); ++it)
	{
		if ((**it).type == "waterbalance connection") {
			throw std::runtime_error(source->to_string() + 
				" has already a waterbalance connection: " + 
				(**it).to_string());
		}
	}
	*/
	RecalcAlways = true;
}
