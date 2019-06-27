#include "simple_connections.h"
#include <stdexcept>

void cmf::water::set_flux( flux_node::ptr source,flux_node::ptr target,real flux_value)
{
	auto* con = dynamic_cast<ExternallyControlledFlux*>(source->connection_to(*target));
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
	auto* con = dynamic_cast<ExternallyControlledFlux*>(source->connection_to(*target));
	return con != 0;
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
	
cmf::water::ConstraintLinearStorageFlux::ConstraintLinearStorageFlux(
	cmf::water::WaterStorage::ptr source,
	cmf::water::WaterStorage::ptr target,
	real _residencetime,
	real _Vlmin, real _Vrmax)
	: flux_connection(source, target, "constrained lin. storage"),
	residencetime(_residencetime), Vlmin(_Vlmin), Vrmax(_Vrmax)
{
	NewNodes();
}

real cmf::water::ConstraintLinearStorageFlux::calc_q(cmf::math::Time t)
{
	real V = std::max(0.0, source->get_volume() - Vlmin);
	real C = std::max(0.0, Vrmax - target->get_volume());
	return V / residencetime * (C / Vrmax);

}

 void cmf::water::ConstraintLinearStorageFlux::NewNodes() {
	source = WaterStorage::cast(left_node());
	target = WaterStorage::cast(right_node());
}

cmf::water::ConstantStateFlux::ConstantStateFlux( cmf::water::WaterStorage::ptr controlled_storage, cmf::water::flux_node::ptr other_end,
															 real _target_state, cmf::math::Time _reaction_time ) 
: flux_connection(controlled_storage, other_end, "State controlling flux"), target_state(_target_state), reaction_time(_reaction_time)
{
	NewNodes();
}

void cmf::water::ConstantStateFlux::NewNodes()
{
    source = cmf::water::WaterStorage::cast(this->left_node());
}

real cmf::water::ConstantStateFlux::calc_q( cmf::math::Time t )
{
	real dV = source.lock()->get_state() - target_state ;
	if (dV > 0) // If water is to be removed from the controlled storage
		return (1-left_node()->is_empty()) * dV/reaction_time.AsDays();
	else // If the controlled storage is to be refilled
		return (1-right_node()->is_empty()) * dV/reaction_time.AsDays();
}

real cmf::water::LinearGradientFlux::calc_q( cmf::math::Time t )
{
	flux_node::ptr 
		left = left_node(),
		right = right_node();
    real
        lpot = left->get_potential(t),
        rpot = right->get_potential(t),
	    q = Q1 * (lpot - rpot) / d;
	return prevent_negative_volume(q);
}

cmf::water::LinearGradientFlux::LinearGradientFlux( cmf::water::flux_node::ptr left,
	cmf::water::flux_node::ptr right, real _Q1,real _d/*=1.0*/)
	: flux_connection(left,right,"linear gradient connection"), Q1(_Q1)
{
    /* TODO: Reinclude when #71 is solved
    if (!left) {
        use_count = shared_this.use_count();
        // kill_me();
        use_count = shared_this.use_count();
        throw std::runtime_error("left node is null");
    }
    if (!right) {
        use_count = shared_this.use_count();
        // kill_me();
        use_count = shared_this.use_count();
        throw std::runtime_error("right node is null");
    }
    if (!(left->is_storage() || right->is_storage())) {
        use_count = shared_this.use_count();
        // kill_me();
        use_count = shared_this.use_count();
        throw std::runtime_error("both nodes without storage");
    }


    if (d <= 0) {
        kill_me();
        throw std::runtime_error("Cannot create a LinearGradientFlux with a distance <= 0 m");
    }
    if (Q1 <= 0) {
        kill_me();
        throw std::runtime_error("LinearGradientFlux.Q1 <= 0");
    }
     */
    d = _d <= 0 ? left->position.distanceTo(right->position) : _d;

}

cmf::water::WaterbalanceFlux::WaterbalanceFlux(flux_node::ptr source, flux_node::ptr target)
	: flux_connection(source, target, "waterbalance connection")
{
    // Check if this is the first waterbalance connection
	/*
	cmf::water::connection_list source_cons = source->get_connections();
	for (auto con: source->get_connections())
	{
		if (con->type == "waterbalance connection" && con.get() != this) {
		    kill_me();
			throw cmf::water::flux_connection_construction_error(shared_this, source->to_string() +
				" has already a waterbalance connection: " + 
				con->to_string());
		}
	}
	 */
	RecalcAlways = true;
}

real cmf::water::WaterbalanceFlux::calc_q(cmf::math::Time t) {
    real q = left_node()->waterbalance(t,this);
    if (q > 0) return q;
    else return 0.0;
}
