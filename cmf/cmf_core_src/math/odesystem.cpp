//
// Created by kraft-p on 01.04.2019.
//

#include "odesystem.h"
#ifdef _OPENMP
#include <omp.h>
#endif

void cmf::math::ODEsystem::copy_states( num_array & destination ) const
{
    if (destination.size()!=(ptrdiff_t)size())
        destination.resize(size());
    if (use_OpenMP > 1)
    {
#pragma omp parallel for
        for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
        {
            destination[i]=get_state_value(i);
        }
    }
    else
    {
        for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
        {
            destination[i]=get_state_value(i);
        }
    }

}

void cmf::math::ODEsystem::copy_states( real * destination ) const
{
    if (use_OpenMP > 1)
    {
#pragma omp parallel for
        for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
        {
            destination[i]=states[i]->get_state();
        }
    }
    else
    {
        for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
        {
            destination[i]=states[i]->get_state();
        }

    }
}
void cmf::math::ODEsystem::copy_dxdt( Time time,num_array & destination, real factor ) const
{
    if (destination.size()!=(ptrdiff_t)size())
        destination.resize(size());
    std::string err_msg;
    bool err=false;
    if (use_OpenMP)
    {
        if (factor==1)	{
#pragma omp parallel for schedule(guided)
            for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i) {
                if (!err)
                {
                    try {
#pragma omp flush(err)
                        destination[i]=states[i]->dxdt(time);
                    } catch(std::exception& e) {
#pragma omp critical
                        {
                            err_msg = e.what();
                            err = true;
#pragma omp flush(err)
                        }
                    }
                }
            }
        } else {
#pragma omp parallel for schedule(guided)
            for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
            {
#pragma omp flush(err)
                if (!err)  {
                    try {
                        destination[i]  = states[i]->dxdt(time);
                        destination[i] *= factor;
                    } catch(std::exception& e) {
#pragma omp critical
                        err_msg = e.what();
                        err = true;
#pragma omp flush(err)
                    }
                }
            }
        }
        if (err) {
            throw std::runtime_error(err_msg);
        }
    } else {
        if (factor==1)
        {
            for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
                destination[i]=states[i]->dxdt(time);
        }
        else
        {
            for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
            {
                destination[i]=states[i]->dxdt(time);
                destination[i]*=factor;
            }
        }
    }
}

void cmf::math::ODEsystem::copy_dxdt( Time time,real * destination,real factor/*=1*/ ) const
{
    std::string err_msg;
    bool err=false;
    if (use_OpenMP)		{
        if (factor==1)	{
#pragma omp parallel for schedule(guided)
            for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i) {
                if (!err)
                {
                    try {
#pragma omp flush(err)
                        destination[i]=states[i]->dxdt(time);
                    } catch(std::exception& e) {
#pragma omp critical
                        {
                            err_msg = e.what();
                            err = true;
#pragma omp flush(err)
                        }
                    }
                }
            }
        } else {
#pragma omp parallel for schedule(guided)
            for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
            {
#pragma omp flush(err)
                if (!err)  {
                    try {
                        destination[i]  = states[i]->dxdt(time);
                        destination[i] *= factor;
                    } catch(std::exception& e) {
#pragma omp critical
                        err_msg = e.what();
                        err = true;
#pragma omp flush(err)
                    }
                }
            }
        }
        if (err) {
            throw std::runtime_error(err_msg);
        }
    }	else	{ // use_OpenMP=false
        if (factor==1)	{
            for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
                destination[i]=states[i]->dxdt(time);
        } else {
            for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)	{
                destination[i]=states[i]->dxdt(time);
                destination[i]*=factor;
            }
        }
    }
}
void cmf::math::ODEsystem::set_states(const num_array & newStates )
{
    if (use_OpenMP)
    {
#pragma omp parallel for
        for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
        {
            states[i]->set_state(newStates[i]);
        }
    }
    else
    {
        for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
        {
            states[i]->set_state(newStates[i]);
        }
    }

}

void cmf::math::ODEsystem::set_states( real * newStates )
{
    if (use_OpenMP > 1)
    {
#pragma omp parallel for
        for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
        {
            states[i]->set_state(newStates[i]);
        }
    }
    else
    {
        for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; ++i)
        {
            states[i]->set_state(newStates[i]);
        }
    }
}

void cmf::math::ODEsystem::add_values_to_states(const num_array& operands)
{
    if (use_OpenMP > 1)
    {
#pragma omp parallel for
        for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
        {
            states[i]->set_state(states[i]->get_state() + operands[i]);
        }
    }
    else
    {
        for (ptrdiff_t i = 0; i < (ptrdiff_t)size() ; i++)
        {
            states[i]->set_state(states[i]->get_state() + operands[i]);
        }
    }
}

cmf::math::num_array cmf::math::ODEsystem::get_dxdt(cmf::math::Time time) const {
    num_array result(this->size());
    copy_dxdt(time,result);
    return result;
}

cmf::math::num_array cmf::math::ODEsystem::get_state_values() const {
    num_array result(this->size());
    copy_states(result);
    return result;
}

int guess_OpenMp(int use_OpenMP, size_t system_size) {
    if (cmf::math::get_parallel_threads() > 1) {
        if (use_OpenMP >= 0) return use_OpenMP;
        else if (system_size > 10000) return 2;
        else return 1;
    }
    else return 0;
}



cmf::math::ODEsystem::ODEsystem(const cmf::math::ODEsystem &for_copy)
        : states(for_copy.states), use_OpenMP(guess_OpenMp(for_copy.use_OpenMP, for_copy.size())) {

}


cmf::math::ODEsystem::ODEsystem(const cmf::math::state_list &for_copy, int _use_OpenMP)
        : states(for_copy), use_OpenMP(guess_OpenMp(_use_OpenMP, for_copy.size())) {

}

cmf::math::StateVariable::ptr cmf::math::ODEsystem::operator[](ptrdiff_t index) const {
    return states[index];
}

real cmf::math::ODEsystem::get_state_value(ptrdiff_t index) const {
    if (index < 0) {
        index += this->size();
    }
    return states[index]->get_state();
}

void cmf::math::ODEsystem::set_state_value(ptrdiff_t index, real value) const {

    states[index]->set_state(value);
}
