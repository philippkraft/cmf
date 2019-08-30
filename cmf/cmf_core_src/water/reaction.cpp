//
// Created by philipp on 30.08.19.
//
#include "reaction.h"
#include "SoluteStorage.h"
#include "WaterStorage.h"
#include <sstream>
using namespace cmf::water;
real SoluteDecayReaction::get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const {
    return - decay * solute_storage.get_state();
}

SoluteDecayReaction::SoluteDecayReaction(real d) {
    this->decay = d;
}

std::string SoluteDecayReaction::to_string() const {
    std::stringstream out;
    out << "linear decay reaction k=" << decay << " 1/day";
    return out.str();
}

real SoluteConstantFluxReaction::get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const {
    return flux;
}

SoluteConstantFluxReaction::SoluteConstantFluxReaction(real f) {
    this->flux = f;
}

std::string SoluteConstantFluxReaction::to_string() const {
    std::stringstream out;
    out << "constant solute flux reaction f=" << flux << " (mol | g)/day";
    return out.str();
}


real SoluteEquilibriumReaction::get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const {
    auto water_storage = solute_storage.get_water();
    real vol = water_storage.get_volume();
    real a_conc = water_storage[A].get_conc();
    real b_conc = water_storage[B].get_conc();

    if (solute_storage.Solute == A) {
        return (- k_ab * a_conc + k_ba * b_conc) * vol;
    } else if (solute_storage.Solute == B) {
        return (+ k_ab * a_conc - k_ba * b_conc) * vol;
    } else {
        return 0.0;
    }

}

std::string SoluteEquilibriumReaction::to_string() const {
    std::stringstream out;
    out << "equilibirum reaction [" << A.Name << "]<->[" << B.Name << "]";
    return out.str();
}

real Solute1stOrderReaction::get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const {
    auto water_storage = solute_storage.get_water();
    real vol = water_storage.get_volume();
    real a_conc = water_storage[A].get_conc();
    if (solute_storage.Solute == A) {
        return (- k * a_conc) * vol;
    } else if (solute_storage.Solute == B) {
        return (+ k * a_conc ) * vol;
    } else {
        return 0.0;
    }
}

std::string Solute1stOrderReaction::to_string() const {

    return "1st order reaction [" + A.Name + "]->[" + B.Name + "]";

}

real Solute2ndOrderReaction::get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const {
    auto water_storage = solute_storage.get_water();
    real vol = water_storage.get_volume();
    real a_conc = water_storage[A].get_conc();
    real b_conc = water_storage[B].get_conc();
    if (solute_storage.Solute == A) {
        return (- k * a_conc * b_conc) * vol;
    } else if (solute_storage.Solute == B) {
        return (- k * a_conc * b_conc) * vol;
    } else if (solute_storage.Solute == C) {
        return (+ k * a_conc * b_conc) * vol;
    } else {
        return 0.0;
    }
}

std::string Solute2ndOrderReaction::to_string() const {
    return "2nd order reaction [" + A.Name + "] + [" + B.Name + "]->[" + C.Name + "]";
}

