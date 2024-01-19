//
// Created by kraft-p on 01.04.2019.
//
#include "explicit_euler.h"



cmf::math::ExplicitEuler_fixed::ExplicitEuler_fixed(const cmf::math::state_list & states)
        : Integrator(states,0.0),
          dxdt(get_system().size())
{}

cmf::math::ExplicitEuler_fixed::ExplicitEuler_fixed()
        : Integrator(1e-9)
{	}

cmf::math::Integrator *cmf::math::ExplicitEuler_fixed::copy() const {
    return new ExplicitEuler_fixed(*this);
}

int cmf::math::ExplicitEuler_fixed::integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep) {
    if (get_system().size()==0)
        throw std::out_of_range("No states to integrate!");
    else if (get_system().size() != dxdt.size()) {
        dxdt.resize(get_system().size());
    }
    m_dt=TimeStep;
    if (m_dt>MaxTime-get_t())
        m_dt=MaxTime-get_t();
    // copy derivates multipied with time step to dxdt
    get_system().copy_dxdt(get_t(),dxdt,m_dt.AsDays());
    // Update time step with delta x
    get_system().add_values_to_states(dxdt);
    m_t += m_dt;

    return 1;
}


cmf::math::HeunIntegrator::HeunIntegrator(const cmf::math::state_list & states, real Alpha)
        : Integrator(states,0.0),alpha(Alpha)
{}

cmf::math::HeunIntegrator::HeunIntegrator(real Alpha)
        : Integrator(),alpha(Alpha)
{	}

cmf::math::HeunIntegrator::HeunIntegrator(const cmf::math::Integrator &copy)
        : Integrator(copy) {
    auto heun = dynamic_cast<const cmf::math::HeunIntegrator*>(&copy);
    if (heun) {
        alpha = heun->alpha;
    } else {
        alpha = 0.5;
    }


}

cmf::math::Integrator *cmf::math::HeunIntegrator::copy() const {
    return new HeunIntegrator(*this);
}
int cmf::math::HeunIntegrator::integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep) {
    size_t size = get_system().size();
    if (size==0)
        throw std::out_of_range("No states to integrate!");


    m_dt=TimeStep;
    if (m_dt>MaxTime-get_t())
        m_dt=MaxTime-get_t();
    get_system().copy_states(old_states);
    // get f(y_i)dt
    get_system().copy_dxdt(get_t(),dxdt0,m_dt.AsDays());
    // Update time step with delta x
    get_system().add_values_to_states(dxdt0);
    if (alpha>0)
    {
        // get f(y_i+1)dt
        get_system().copy_dxdt(get_t(),dxdt1,m_dt.AsDays() * alpha);
        // reset states to y_i
        get_system().set_states(old_states);
        // update states to y_i+1
        if (alpha<1)
        {
            dxdt0 *= (1-alpha);
            get_system().add_values_to_states(dxdt0);
        }
        get_system().add_values_to_states(dxdt1);
    }
    m_t += m_dt;

    return 1;
}

void cmf::math::HeunIntegrator::reset() {
    size_t size = get_system().size();
    if (size != dxdt0.size() || size != dxdt1.size() || size != old_states.size()) {
        dxdt0.resize(size);
        dxdt1.resize(size);
        old_states.resize(size);
    }
}
