#include "integrator.h"
#ifdef _OPENMP
#include <omp.h>
#endif



void cmf::math::Integrator::integrate_until( cmf::math::Time t_max,cmf::math::Time dt/*=Time()*/,bool reset_solver/*=false*/ )
{
	m_Iterations=0;
	int i=0;
	Time start = m_t;
	if (reset_solver) reset();
	if (!dt) dt=m_dt;
	if (reset_integratables) integratables.reset(start);
	while (m_t < t_max) {
		
		integrate(t_max, dt);
		integratables.integrate(m_t);
		++i;
	}
	if (i>0) m_dt = (t_max - start)/i;
}

cmf::math::Integrator::Integrator(real epsilon)
: Epsilon(epsilon) {

}

cmf::math::Integrator::Integrator(const cmf::math::state_list &states, real epsilon)
	: Epsilon(epsilon), m_system(states) {
}

cmf::math::Integrator::Integrator(const cmf::math::Integrator &other)
: m_system(), Epsilon(other.Epsilon), m_t(other.m_t), m_dt(other.m_dt)
{

}

void cmf::math::Integrator::set_system(const cmf::math::state_list &states) {
	m_system = ODEsystem(states);
	reset();
}

size_t cmf::math::Integrator::size() const {
	return m_system.size();
}

cmf::math::StateVariable::ptr cmf::math::Integrator::operator[](ptrdiff_t index) const {
    return m_system[index];
}

cmf::math::Integrator::operator cmf::math::state_list() {
    return m_system.states;
}

cmf::math::ODEsystem &cmf::math::Integrator::get_system() {
    return m_system;
}

const cmf::math::ODEsystem &cmf::math::Integrator::get_system() const {
    return m_system;
}


