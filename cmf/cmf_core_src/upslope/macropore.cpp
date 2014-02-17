#include "cell.h"
#include "SoilLayer.h"
#include "Topology.h"
#include "macropore.h"
using namespace cmf::geometry;
using namespace cmf::upslope;



cmf::upslope::MacroPore::MacroPore( SoilLayer::ptr layer,real _porefraction, real _Ksat,real _density, real _porefraction_max, real _K_shape) 
	:	WaterStorage(layer->cell.get_project(),"Macro pores for " + layer->Name,0.0),
	m_layer(layer),m_porefraction_min(_porefraction),density(_density), Ksat(_Ksat), m_porefraction_max(_porefraction_max), K_shape(_K_shape)
{
	position = layer->position;
	if (m_porefraction_max<0) m_porefraction_max = m_porefraction_min;
}

real cmf::upslope::MacroPore::head_to_volume( real head ) const
{
	SoilLayer::ptr l = get_layer();
	real fill = head - (l->get_gravitational_potential() - l->get_thickness());
	if (fill > l->get_thickness()) {
		return get_capacity() + (fill - l->get_thickness())/100.;
	} else if (fill>0) {
		return fill * get_porefraction() * l->cell.get_area();
	} else {
		return 0.0;
	}
}

real cmf::upslope::MacroPore::volume_to_head( real volume ) const
{
	SoilLayer::ptr l = get_layer();
	real fill = volume/l->cell.get_area()/get_porefraction();
	if (volume>get_capacity())
		fill = l->get_thickness() + 100 * (fill - l->get_thickness());
	return (l->get_gravitational_potential()- l->get_thickness()) + fill;
}

real cmf::upslope::MacroPore::get_potential() const
{
	if (get_state_variable_content()=='h') {
		return this->get_state();
	} else {
		return volume_to_head(this->get_state());
	}
}

real cmf::upslope::MacroPore::get_volume() const
{
	if (get_state_variable_content()=='h') {
		return head_to_volume(this->get_state());
	} else {
		return this->get_state();
	}
}

void cmf::upslope::MacroPore::set_volume( real volume )
{
	if (get_state_variable_content()=='h') {
		this->set_state(volume_to_head(volume));
	} else {
		this->set_state(volume);
	}
}

void cmf::upslope::MacroPore::set_potential( real waterhead )
{
	if (get_state_variable_content()=='h') {
		this->set_state(waterhead);
	} else {
		this->set_state(head_to_volume(waterhead));
	}
}

real cmf::upslope::MacroPore::get_filled_fraction() const
{
	SoilLayer::ptr l=get_layer();
	real capacity = get_porefraction() * l->get_thickness() * l->cell.get_area();
	return get_volume()/capacity;
}

MacroPore::ptr cmf::upslope::MacroPore::create( SoilLayer::ptr layer,real porefraction, real Ksat,real density,real porefraction_wilt, real K_shape)
{
	MacroPore* mpp= new MacroPore(layer,porefraction,Ksat,density,porefraction_wilt,K_shape);
	MacroPore::ptr mp(mpp);
	layer->cell.add_storage(mp);
	return mp;
}

real cmf::upslope::MacroPore::get_capacity() const
{
	SoilLayer::ptr l=get_layer();
	return get_porefraction() * l->get_thickness() * l->cell.get_area();
}

real cmf::upslope::MacroPore::get_flowwidth() const
{
	return 2/density * get_cell().get_area();
}

real cmf::upslope::MacroPore::get_K() const
{
	if (K_shape<=0.)
		return Ksat;
	else {
		return Ksat * (1-exp_of_matrixpotential(K_shape));
	}
}

real cmf::upslope::connections::GradientMacroFlow::calc_q( cmf::math::Time t )
{
	using namespace cmf::upslope;
	using namespace cmf::geometry;
	// Richards flux
	MacroPore::ptr 
		Mp1=mp1.lock(),
		Mp2=mp2.lock();
	conductable::ptr C2 = c2.lock();


	real
		distance = fabs(Mp1->get_layer()->position.z - right_node()->position.z),
		Psi_t1=Mp1->get_potential(),
		Psi_t2=right_node()->get_potential(),
		gradient=(Psi_t1-Psi_t2)/distance,
		K=0.0;
	point direction =  Mp1->get_layer()->position - right_node()->position;
	if (distance == 0.0) distance = Mp1->get_layer()->get_thickness();
	//K=gradient<0 && l2 ? l2->K() : l1->K();      
	K = Mp1->get_K(direction);
	//  	if (fabs(K*gradient)>l1->get_Ksat()) K=l1->get_Ksat()/fabs(gradient);
	//  	if (l2 && fabs(K*gradient)>l2->get_Ksat()) K=l2->get_Ksat()/fabs(gradient);
	real r_flow=K*gradient*Mp1->get_layer()->cell.get_area();
	return prevent_negative_volume(r_flow);

}
real cmf::upslope::connections::KinematicMacroFlow::calc_q( cmf::math::Time t )
{
	using namespace cmf::upslope;
	using namespace cmf::geometry;
	MacroPore::ptr 
		Mp1=mp1.lock(),
		Mp2=mp2.lock();
	real 
		linear_gradient = 0.0;
	
	if (Mp1) {
		linear_gradient = Mp1->get_K() * Mp1->get_filled_fraction();
	} else {
		cmf::water::WaterStorage::ptr ws2 = cmf::water::WaterStorage::cast(left_node());
		// Use a virtual filled fraction, using the capacity of Mp2
		linear_gradient = Mp2->get_K() * ws2->get_volume() / Mp2->get_capacity();
	}

	real overflow = Mp2 ? std::max( 1 - Mp2->get_filled_fraction(),0.0) : 1;			
	real area = (Mp1 ? Mp1 : Mp2)->get_cell().get_area();
	real k_flow = linear_gradient * overflow * area;
	return prevent_negative_volume(k_flow);

}

void cmf::upslope::connections::KinematicMacroFlow::NewNodes()
{
	MacroPore::ptr 
		Mp1=cmf::upslope::MacroPore::cast(left_node()),
		Mp2=cmf::upslope::MacroPore::cast(right_node());
	if (!(Mp1) && !(Mp2)) 
		throw std::runtime_error("One of the end points for KinematicMacroFlow needs to be a MacroPore");
	mp1 = Mp1;
	mp2 = Mp2;
}

void cmf::upslope::connections::GradientMacroMicroExchange::NewNodes()
{
	sl = SoilLayer::cast(left_node());
	mp = MacroPore::cast(right_node());
}

real cmf::upslope::connections::GradientMacroMicroExchange::calc_q( cmf::math::Time t )
{
	MacroPore::ptr MP = mp.lock();
	SoilLayer::ptr SL = sl.lock();
	real
		pot = SL->get_matrix_potential(),
		Ksat = SL->get_Ksat(),
		Ksoil  = SL->get_K(),
		mp_fill = minmax(MP->get_filled_fraction(),0,1),
		K_mp = Ksoil + mp_fill * (Ksat - Ksoil),
		K = geo_mean(Ksoil,K_mp) * (pot > 0 ? 1-mp_fill : 1.0),
		d = MP->density,
		A = MP->get_flowwidth() * SL->get_thickness(),
		q = K * pot / (d*0.5) * A;
	return prevent_negative_volume(q);

}
void cmf::upslope::connections::DiffusiveMacroMicroExchange::NewNodes()
{
	sl = SoilLayer::cast(left_node());
	mp = MacroPore::cast(right_node());
}

cmf::upslope::connections::DiffusiveMacroMicroExchange::DiffusiveMacroMicroExchange( cmf::upslope::SoilLayer::ptr left, cmf::upslope::MacroPore::ptr right,real _omega ) 
	: flux_connection(left,right,"Macro to micro pores exchange"),omega(_omega)
{
	NewNodes();
}

real cmf::upslope::connections::DiffusiveMacroMicroExchange::calc_q( cmf::math::Time t )
{
	MacroPore::ptr MP = mp.lock();
	SoilLayer::ptr SL = sl.lock();
	real
		q = omega * (MP->get_volume()/MP->get_capacity() - SL->get_wetness());
	return prevent_negative_volume(q);

}


real cmf::upslope::connections::MACROlikeMacroMicroExchange::calc_q( cmf::math::Time t )
{
	MacroPore::ptr MP = mp.lock();
	SoilLayer::ptr SL = sl.lock();
	const RetentionCurve& rt = SL->get_soil();
	// Check if this fits to MACRO theory
	real theta_b = rt.Porosity();
	real theta_mi = SL->get_theta();
	real D_theta_mi = rt.Diffusivity(SL->get_wetness());
	// TODO: find the correct translation of MACRO theta_b in cmf. As a first try we assume
	// theta_b is saturation in cmf.
	real D_theta_b = rt.Diffusivity(theta_b/rt.Porosity());
	real Sma = MP->get_volume()/MP->get_capacity();
	real Dw = (D_theta_b + D_theta_mi)/2 * Sma;

	real Sw = (Gf * Dw * gamma_w)/square(MP->density) * (theta_b - theta_mi);
	return prevent_negative_volume(Sw * SL->get_thickness() * SL->cell.get_area());
}