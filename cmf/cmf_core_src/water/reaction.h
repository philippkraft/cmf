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

        /// @ingroup Solutes
        /// @warning Experimental feature!
        ///
        /// @brief A general solute reaction system to describe multi-species kinetics with a power law
        ///
        /// cf. to https://en.wikipedia.org/wiki/Rate_equation
        ///
        /// \f[ A + 2B \rightarrow 3C \Rightarrow 0 = -1A - 2B + 3C \f]
        ///
        /// Where \f$ -1, -2, 3 \f$ are the stoichiometric coefficients \f$v_i\f$ corresponding to the substance \f$X_i\f$.
        ///
        /// The reaction rate \f$r^+\f$ is given by a power law:
        ///
        /// \f[ r = k \prod [X_i]^{m_i} \forall v_i < 0 \f]
        /// With \f$m_i\f$ as the partial order, which is sometimes equal to the stoichiometric coefficient.
        ///
        /// Which gives the following differential equation system
        ///
        /// \f[\frac{dX_i}{dt} = v_i r^+([X]) V \f]
        ///
        /// If the opposite reaction is taking place at the same time (equilibrium reaction),
        /// with the reaction rate \f$r^-\f$ for the backwards reaction we get:
        /// \f[\frac{dX_i}{dt} = V \left(v_i r^+([X]) - v_i r^-([X])\right)\f]
        class SoluteRateReaction: public SoluteReaction {
        private:
            struct SolutePartialOrder {
                real v; ///<stoichiometric_coefficient
                real o;  ///< partial order
            };
            typedef std::map<solute, SolutePartialOrder> partial_order;
             partial_order _reactance;
            static std::string print_partial_order(const partial_order& po);
        public:
            /// Rate constant for forward reaction
            real k_forward;
            /// Rate constant for backward reaction
            real k_back;

            SoluteRateReaction(real kForward, real kBack=0.0);

            /// Use positive stoichiometric_coefficient for products and negative for educts
            void add_reactance(const solute& solute, real stoichiometric_coefficient, real partial_order=-999);

            real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const override;

            std::string to_string() const override;

            bool is_compatible(const SoluteStorage &solute_storage) override;
        };


        /// @ingroup Solutes
        /// @brief A solute reaction of 1st order kinetics (linear decline to product) A->B.
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
        /// @warning Experimental feature! The math might not be a correct diffusion equation
        ///
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
#ifndef SWIG
        /// @ingroup Solutes
        /// @brief Attaches reactions to all solutes of a waterstorage.
        void attach_reactions_to_waterstorage(
                std::shared_ptr<cmf::water::WaterStorage> waterstorage,
                const SoluteReactionList& reactions);
#endif

        /// @ingroup Solutes
        /// @brief Clear all reactions of a water storage
        void clear_reactions_of_waterstorage(std::shared_ptr<cmf::water::WaterStorage> waterstorage);
    }

}
#endif //CMF_CORE_REACTION_H
