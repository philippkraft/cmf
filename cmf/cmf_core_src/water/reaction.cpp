//
// Created by philipp on 30.08.19.
//
#include "reaction.h"
#include "SoluteStorage.h"
#include "WaterStorage.h"
#include "project.h"
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

SoluteEquilibriumReaction::SoluteEquilibriumReaction(const solute &A, const solute &B, real k_ab, real k_ba)
        : A(A), B(B), k_ab(k_ab), k_ba(k_ba) {}

bool SoluteEquilibriumReaction::is_compatible(const SoluteStorage &solute_storage) {
    return solute_storage.Solute == A || solute_storage.Solute == B;
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

bool Solute1stOrderReaction::is_compatible(const SoluteStorage &solute_storage) {
    return solute_storage.Solute == A || solute_storage.Solute == B;
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

Solute2ndOrderReaction::Solute2ndOrderReaction(const solute &A, const solute &B, const solute &C, real k)
        : A(A), B(B), C(C), k(k) {}

bool Solute2ndOrderReaction::is_compatible(const SoluteStorage &solute_storage) {
    return solute_storage.Solute == A || solute_storage.Solute == B || solute_storage.Solute == C;

}

void cmf::water::attach_reactions_to_waterstorage(
        std::shared_ptr<cmf::water::WaterStorage> waterstorage,
        const cmf::water::SoluteReactionList &reactions) {
    auto& p = waterstorage->get_project();
    for (auto& X: p.solutes) {
        auto& ss = waterstorage->operator[](X);
        for (auto& r: reactions) {
            if (r->is_compatible(ss))
                ss.reactions.append(r);
        }
    }
}

void cmf::water::clear_reactions_of_waterstorage(cmf::water::WaterStorage::ptr waterstorage) {
    auto& p = waterstorage->get_project();
    for (auto& X: p.solutes) {
        waterstorage->operator[](X).reactions.clear();
    }

}

real SoluteDiffusiveTransport::get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const {
    if        (& solute_storage == & left) {
        return alpha * (right.get_conc() - left.get_conc()) * right.get_water().get_volume();
    } else if (& solute_storage == & right) {
        return alpha * (left.get_conc() - right.get_conc()) * left.get_water().get_volume();
    } else {
        return 0.0;
    }

}

SoluteDiffusiveTransport::SoluteDiffusiveTransport(real alpha, const SoluteStorage &left, const SoluteStorage &right)
        : alpha(alpha), left(left), right(right) {

}

std::string SoluteDiffusiveTransport::to_string() const {
    return "Diffusive Transport between " + left.to_string() + " and " + right.to_string();
}
struct SolutePartialOrder {
    int v; ///<stoichiometric_coefficient
    real o;  ///< partial order
};
typedef std::map<solute, SolutePartialOrder> partial_order;



std::string SoluteRateReaction::to_string() const {
    std::stringstream out;
    out << "Reaction rate equation: "
        << print_partial_order(_reactance)
    ;
    return out.str();
}

void SoluteRateReaction::add_reactance(const cmf::water::solute &solute, real stoichiometric_coefficient,
                                  real partial_order) {
    if (partial_order == -999) partial_order = std::abs(stoichiometric_coefficient);
    _reactance[solute] = {stoichiometric_coefficient, partial_order};

}


std::string SoluteRateReaction::print_partial_order(const partial_order &po) {
    std::stringstream stream;
    for (auto& e : po) {
        if (e.first != po.begin()->first) {
            stream << " + ";
        }
        stream << e.second.v << " [" << e.first.Name << "]^" << e.second.o;
    }
    return stream.str();

}

real SoluteRateReaction::get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const {
    real r_f = k_forward;
    real r_b = k_back;
    const auto& W = solute_storage.get_water();
    const solute& X = solute_storage.Solute;
    // Calculate forward and backward reaction rate r_f and r_b
    for (auto& e : _reactance) {
        if (e.second.v < 0) {
            r_f *= std::pow(W[e.first].get_conc(), e.second.o);
        } else if (e.second.v > 0) {
            r_b *= std::pow(W[e.first].get_conc(), e.second.o);
        }
    }

    if (_reactance.count(X)) {
        const auto& po = _reactance.at(X);
        return (r_f * po.v - r_b * po.v) * W.get_volume();
    } else {
        return 0.0;
    }
}

SoluteRateReaction::SoluteRateReaction(real kForward, real kBack) : k_forward(kForward), k_back(kBack) {
}

bool SoluteRateReaction::is_compatible(const SoluteStorage &solute_storage) {

    return _reactance.count(solute_storage.Solute) > 0;
}
