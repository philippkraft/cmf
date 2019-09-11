//
// Created by philipp on 30.08.19.
//

#ifndef CMF_CORE_REACTION_H
#define CMF_CORE_REACTION_H
#include "Solute.h"
#include "../math/real.h"
#include "../list.h"
#include <initializer_list>
#include <string>

namespace cmf {
    namespace water {

        class SoluteStorage;
        class WaterStorage;
        /// @ingroup Solutes
        /// @brief Abstract class for a solute reaction. Can be derived with python.
        class SoluteReaction {
        public:
            /// Calculates the reactive flux from / to the given solute storage at time t
            virtual real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const = 0;

            typedef std::shared_ptr<SoluteReaction> ptr;

            /// Calculates the reactive flux from / to the given solute storage at time t
            real operator()(const SoluteStorage &solute_storage, const cmf::math::Time &t) const {
                return get_flux(solute_storage, t);
            }

            virtual std::string to_string() const {
                return "reaction";
            }

            /// Tests if the reaction is compatible to a solute storage. Returns true if not overwritten by a child class
            virtual bool is_compatible(const SoluteStorage &solute_storage) {
                return true;
            }

            virtual ~SoluteReaction() = default;

        };

        /// @ingroup Solutes
        /// Adds a constant flux to the solute storage.
        ///
        /// Normally used for operator split model coupling. Use this to replace the old `source` parameter of
        /// a solute storage
        ///
        /// Usage:
        /// @code
        /// p = cmf.project('X')
        /// X = p.solutes[0]
        /// S = p.NewStorage('S')
        /// S[X].reactions = [cmf.SoluteConstantReaction(1.0)]
        /// assert S[X].dxdt(cmf.Time()) == 1.0
        /// @endcode
        class SoluteConstantFluxReaction : public SoluteReaction {
        public:
            real flux;

            explicit SoluteConstantFluxReaction(real f);

            real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const override;

            std::string to_string() const override;
        };

        /// @ingroup Solutes
        /// @brief Adds a linear decay to solute storages.
        ///
        /// Normally used for operator split model coupling. Use this to replace the old `source` parameter of
        /// a solute storage
        ///
        /// Usage:
        /// @code
        /// p = cmf.project('X')
        /// X = p.solutes[0]
        /// S = p.NewStorage('S')
        /// S[X].set_state(2.0)
        /// S[X].reactions = [cmf.SoluteDecayReaction(0.5)]
        /// assert S[X].dxdt(cmf.Time()) = S[X].state * 0.5
        /// @endcode
        class SoluteDecayReaction : public SoluteReaction {
        public:
            real decay;

            explicit SoluteDecayReaction(real d);

            real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const override;

            std::string to_string() const override;
        };
        /// @ingroup Solutes
        /// @brief An equilibrium reaction between two solutes A<->B
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
            SoluteEquilibriumReaction(const solute &A, const solute &B, real k_ab, real k_ba);

            bool is_compatible(const SoluteStorage &solute_storage) override;

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

            bool is_compatible(const SoluteStorage &solute_storage) override;

        };

        /// @ingroup Solutes
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
            solute A, ///< Educt 1
                   B, ///< Educt 2
                   C; ///< Product
            real k;

            real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const override;

            /// Creates a 2nd order kinetic reaction.
            Solute2ndOrderReaction(const solute &A, const solute &B, const solute &C, real k);

            std::string to_string() const override;

            bool is_compatible(const SoluteStorage &solute_storage) override;

        };


        /// @ingroup Solutes
        /// @warning Experimental feature!
        /// @brief Calculates a diffusive flux between solute storages
        ///
        /// \f[ q = \alpha \cdot \left([A]_1 - [A]_2\right) V_2 \f]
        /// - \f$ [A]_n \f$ Concentration of solute A in storage n in mol/m³ or a similar unit
        /// - \f$ \alpha \f$ Diffusion velocity in 1/day. Depends on the distance between the storages.
        ///   To calculate it from a distance independent diffusion velocity \f$ D [m/day] \f$ over a distance d use
        ///   \f$ \alpha = D / d \f$
        /// - \f$ V_2 \f$ the water volume of the source water storage
        class SoluteDiffusiveTransport: public SoluteReaction {
        public:
            /// Diffusion rate in 1/day
            real alpha;

            /// SoluteStorages that are connected
            const SoluteStorage& left;
            const SoluteStorage& right;

            real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const override;

            std::string to_string() const override;

            SoluteDiffusiveTransport(real alpha, const SoluteStorage &left, const SoluteStorage &right);

        };

        typedef cmf::List<SoluteReaction::ptr> SoluteReactionList;

        /// @ingroup Solutes
        /// @brief Attaches reactions to all solutes of a waterstorage.
        void attach_reactions_to_waterstorage(
                std::shared_ptr<cmf::water::WaterStorage> waterstorage,
                const SoluteReactionList& reactions);

        /// @ingroup Solutes
        /// @brief Clear all reactions of a water storage
        void clear_reactions_of_waterstorage(std::shared_ptr<cmf::water::WaterStorage> waterstorage);

    }

}
#endif //CMF_CORE_REACTION_H
