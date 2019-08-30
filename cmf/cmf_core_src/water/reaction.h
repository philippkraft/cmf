//
// Created by philipp on 30.08.19.
//

#ifndef CMF_CORE_REACTION_H
#define CMF_CORE_REACTION_H
#include "Solute.h"
#include "../math/real.h"
#include "../list.h"
#include <initializer_list>

namespace cmf {
    namespace water {
        class SoluteStorage;

        class SoluteReaction {
        public:
            virtual real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const = 0;

            typedef std::shared_ptr<SoluteReaction> ptr;

            real operator()(const SoluteStorage &solute_storage, const cmf::math::Time &t) const {
                return get_flux(solute_storage, t);
            }

            virtual std::string to_string() const {
                return "reaction";
            }

            virtual ~SoluteReaction() = default;
        };

        class SoluteConstantFluxReaction : public SoluteReaction {
        public:
            real flux;

            explicit SoluteConstantFluxReaction(real f);

            real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const override;

            std::string to_string() const override;
        };

        class SoluteDecayReaction : public SoluteReaction {
        public:
            real decay;

            explicit SoluteDecayReaction(real d);

            real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const override;

            std::string to_string() const override;
        };

        /// An equilibrium reaction between two solutes A<->B
        ///
        /// @f[ A \rightleftharpoons B @f]
        ///
        /// @f[\frac{d[B]}{dt} = -\frac{d[A]}{dt} = k_{AB} [A] - k_{BA} [B] @f]
        class SoluteEquilibriumReaction : public SoluteReaction {
        public:

            solute A, B;
            /// The reaction rate A->B in 1/day
            real k_ab;
            /// The reaction rate B->A in 1/day
            real k_ba;

            real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const override;

            /// Creates a solute equilibrium reaction A<->B
            SoluteEquilibriumReaction(const solute &A, const solute &B, real k_ab, real k_ba)
                    : A(A), B(B), k_ab(k_ab), k_ba(k_ba) {}

            std::string to_string() const override;

        };

        /// A solute reaction of 1st order kinetics (linear decline to product) A->B
        ///
        /// @f[ A \rightarrow B @f]
        ///
        /// @f[\frac{d[B]}{dt} = -\frac{d[A]}{dt} = k [A]@f]
        ///
        /// Where A is the educt and B is the product.

        class Solute1stOrderReaction : public SoluteReaction {
        public:

            solute A, B;
            /// The reaction rate in 1/day
            real k;

            real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const override;

            Solute1stOrderReaction(const solute &A, const solute &B, real k)
                    : A(A), B(B), k(k) {}

            std::string to_string() const override;

        };

        /// A solute reaction of 2nd order kinetics A + B -> C
        ///
        /// @f[ A + B \rightarrow C @f]
        ///
        /// @f[\frac{d[C]}{dt} = -\frac{d[A]}{dt} = -\frac{d[B]}{dt} = k [A] [B]@f]
        ///
        /// Where A and B are the educts and C is the product. For a 2nd order reaction of a single educt A just write:
        /// @code
        ///  Solute2ndOrderReaction(A, A, C, k)
        /// @endcode
        class Solute2ndOrderReaction : public SoluteReaction {
        public:
            solute A, B, C;
            real k;

            real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const override;

            /// Creates a 2nd order kinetic reaction.
            Solute2ndOrderReaction(const solute &A, const solute &B, const solute &C, real k)
                    : A(A), B(B), C(C), k(k) {}

            std::string to_string() const override;

        };

        typedef cmf::List<SoluteReaction::ptr> SoluteReactionList;
    }
}
#endif //CMF_CORE_REACTION_H
