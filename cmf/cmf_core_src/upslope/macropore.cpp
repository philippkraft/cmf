#include "cell.h"
#include "SoilLayer.h"
#include "Topology.h"
#include "macropore.h"
using namespace cmf::geometry;
using namespace cmf::upslope;



cmf::upslope::MacroPore::MacroPore( SoilLayer::ptr layer,real _porefraction_min, real _Ksat,real _density, real _porefraction_max, real _K_shape) 
	:	WaterStorage(layer->cell.get_project(),"Macro pores for " + layer->Name,0.0),
	m_layer(layer),porefraction_min(_porefraction_min),density(_density), Ksat(_Ksat), porefraction_max(_porefraction_max), K_shape(_K_shape), crack_wetness(1.0)
{
	position = layer->position;
	if (porefraction_max<0) porefraction_max = porefraction_min;
}

real cmf::upslope::MacroPore::head_to_volume( real head ) const
{
	SoilLayer::ptr l = get_layer();
	real C = get_capacity(),
		 d = l->get_thickness(),
		 z = l->get_gravitational_potential(),
		 fill = head - (z - d);
	if (fill > l->get_thickness()) {
		return C + (fill - d)/100.;
	} else if (fill>0) {
		return C * fill/d;
	} else {
		return 0.0;
	}
}

real cmf::upslope::MacroPore::volume_to_head( real volume ) const
{
	SoilLayer::ptr l = get_layer();
	real C = get_capacity(), // macropore capacity in m3
		d = l->get_thickness(), // layer thickness in m
		z = l->get_gravitational_potential(), // upper layer boundary in m a.s.l
		fill = volume / C * d; // fill height in m
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

real cmf::upslope::MacroPore::get_porefraction() const
{
	real 
		w = get_layer()->get_wetness(),
		wwilt = get_layer()->get_soil().Wetness_pF(4.2),
		open = minmax(1-(w-wwilt)/(crack_wetness - wwilt),0.,1.);
	// real w_eff = minmax(get_layer()->get_soil().Wetness_eff(w),0,1);
	return porefraction_min + open * (porefraction_max-porefraction_min);
}
void cmf::upslope::connections::BaseMacroFlow::NewNodes()
{
	MacroPore::ptr 
		Mp1=cmf::upslope::MacroPore::cast(left_node()),
		Mp2=cmf::upslope::MacroPore::cast(right_node());
	if (!(Mp1) && !(Mp2)) 
		throw std::runtime_error("One of the end points for " + this->type + " needs to be a MacroPore");
	mp1 = Mp1;
	mp2 = Mp2;
	c2=cmf::upslope::conductable::cast(right_node());
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
		Mp2=mp2.lock(),
		Mp =Mp1 ? Mp1 : Mp2;
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
	real area = Mp->get_cell().get_area();
	real k_flow = linear_gradient * overflow * area;
	return prevent_negative_volume(k_flow);

}

real cmf::upslope::connections::JarvisMacroFlow::calc_q(cmf::math::Time t) {

	using namespace cmf::upslope;
	using namespace cmf::geometry;
	MacroPore::ptr 
		Mp1=mp1.lock(),
		Mp2=mp2.lock(),
		Mp = Mp1 ? Mp1 : Mp2;
	// cmf::water::WaterStorage::ptr ws1 = cmf::water::WaterStorage::cast(left_node());
	real 
		w = Mp->get_crackwidth(), // m
		rho = 1000.,  // kg m-3, density of water at 20degC
		G = 9.81, // m s-2, earth accell.
		nu = 1.0, // kg m-1 s-1, viscosity of water at 20 degC
		pf_r = porefraction_r>=0 ? porefraction_r : Mp->porefraction_min,
		e_v = std::max((Mp->get_porefraction() - pf_r)/(1-pf_r),0.0), 
		Sc = Mp1 ? Mp1->get_filled_fraction() : 1 - left_node()->is_empty();

	// Jarvis and Harrison, 1987, JSS p. 491, eq 11
	// Potential flow in m/s

	//    m/s    kg m-3   m/s2  kg/(m*s)   m*m
	real q_pot_ms = rho   *   G /  (12*nu)  * w*w  * e_v * pow(Sc,beta);
	real overflow = Mp2 ? std::max( 1 - Mp2->get_filled_fraction(),0.0) : 1;			
	real area = Mp->get_cell().get_area();
	real q = q_pot_ms * overflow * area * 86400;
	return prevent_negative_volume(q);

}

cmf::upslope::connections::JarvisMacroFlow::JarvisMacroFlow( cmf::water::WaterStorage::ptr left,cmf::water::flux_node::ptr right, real _beta/*=1.*/, real _porefraction_r )
	: BaseMacroFlow(left,right,"Jarvis & Leeds-Harrison 1987 clay crack flow"), beta(_beta),porefraction_r(_porefraction_r)
{}

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
	sl = SoilLayer::cast(right_node());
	mp = MacroPore::cast(left_node());
}

cmf::upslope::connections::DiffusiveMacroMicroExchange::DiffusiveMacroMicroExchange(cmf::upslope::MacroPore::ptr left,cmf::upslope::SoilLayer::ptr right, real _omega,real _pFrmi)
	: flux_connection(left,right,"Macro to micro pores exchange"),omega(_omega),pFrmi(_pFrmi)
{
	NewNodes();
}

real cmf::upslope::connections::DiffusiveMacroMicroExchange::calc_q( cmf::math::Time t )
{
	MacroPore::ptr MP = mp.lock();
	SoilLayer::ptr SL = sl.lock();
	const RetentionCurve& rt = SL->get_soil();
	real 
		// Macropore filled fraction
		Wma = MP->get_volume()/MP->get_capacity(),
		// Micropore filled fraction
		Wmi = SL->get_wetness(),
		Wmi_eff = rt.Wetness_eff(Wmi,pFrmi),

		q = omega
		   *   (Wma - Wmi_eff) 
		   *	SL->get_thickness() * SL->cell.get_area();
	return prevent_negative_volume(q);

}


real cmf::upslope::connections::MACROlikeMacroMicroExchange::calc_q( cmf::math::Time t )
{
	MacroPore::ptr MP = mp.lock();
	SoilLayer::ptr SL = sl.lock();
	const RetentionCurve& rt = SL->get_soil();
	// Check if this fits to MACRO theory
	real theta_b = rt.Porosity() * rt.Wetness(.01);
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

