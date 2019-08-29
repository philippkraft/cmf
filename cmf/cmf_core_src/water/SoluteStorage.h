

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   cmf is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
//   
#ifndef SoluteStorage_h__
#define SoluteStorage_h__

#include "Solute.h"
#include "../math/statevariable.h"
#include "../math/real.h"
#include "adsorption.h"
#include "../list.h"
#include <initializer_list>
//#include "Reaction.h"
namespace cmf {
	namespace water {
	    class SoluteStorage;
	    class SoluteReaction {
	    public:
	        virtual real get_flux(const SoluteStorage& solute_storage, const cmf::math::Time& t) const=0;
	        typedef std::shared_ptr<SoluteReaction> ptr;
	        real operator()(const SoluteStorage& solute_storage, const cmf::math::Time& t) const {
	            return get_flux(solute_storage, t);
	        }
	        virtual std::string to_string() const {
	            return "reaction";
	        }
	        virtual ~SoluteReaction() = default;
	    };

	    class SoluteConstantFluxReaction : public SoluteReaction
        {
        public:
            real flux;
            explicit SoluteConstantFluxReaction(real f);
            real get_flux(const SoluteStorage& solute_storage, const cmf::math::Time& t) const override;
            std::string to_string() const override;
	    };
	    class SoluteDecayReaction : public SoluteReaction
        {
        public:
            real decay;
            explicit SoluteDecayReaction(real d);
            real get_flux(const SoluteStorage& solute_storage, const cmf::math::Time& t) const override;
            std::string to_string() const override;
        };

	    /// An equilibrium reaction between two solutes A<->B
	    ///
	    /// @f[ A \rightleftharpoons B @f]
	    ///
	    /// @f[\frac{d[B]}{dt} = -\frac{d[A]}{dt} = k_{AB} [A] - k_{BA} [B] @f]
	    class SoluteEquilibriumReaction: public SoluteReaction
        {
        public:

            solute A, B;
            /// The reaction rate A->B in 1/day
            real k_ab;
            /// The reaction rate B->A in 1/day
            real k_ba;
            real get_flux(const SoluteStorage& solute_storage, const cmf::math::Time& t) const override;

            /// Creates a solute equilibrium reaction A<->B
            SoluteEquilibriumReaction(const solute& A, const solute& B, real k_ab, real k_ba)
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

        class Solute1stOrderReaction: public SoluteReaction
        {
        public:

            solute A, B;
            /// The reaction rate in 1/day
            real k;
            real get_flux(const SoluteStorage& solute_storage, const cmf::math::Time& t) const override;

            Solute1stOrderReaction(const solute& A, const solute& B, real k)
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
	    class Solute2ndOrderReaction: public SoluteReaction
        {
        public:
            solute A, B, C;
            real k;
            real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const override;

            /// Creates a 2nd order kinetic reaction.
            Solute2ndOrderReaction(const solute& A, const solute& B, const solute& C, real k)
                    : A(A), B(B), C(C), k(k) {}

            std::string to_string() const override;

        };

	    class SoluteReactionList:
	    public cmf::List<cmf::water::SoluteReaction::ptr>
        {

        public:
            real get_flux(const SoluteStorage &solute_storage, const cmf::math::Time &t) const;
            std::string to_string() const {
                return std::to_string(this->size()) + " reactions";
            }


            SoluteReactionList() = default;
            SoluteReactionList(const SoluteReactionList& other) = default;

#ifndef SWIG
            SoluteReactionList(std::initializer_list<SoluteReaction::ptr> l) : List<SoluteReaction::ptr>(l) {}
#endif  /* SWIG */
        };

		class WaterStorage;
		/// @brief A class for the storage of any tracer. The state is the amount (mol, kg etc. see cmf::water) 
		/// of the tracer in the storage.
		///
		/// The derivative function is given by:
		/// \f{eqnarray*}
		/// \frac{dX}{dt}&=&\sum_{f=1}^{F}\left( q_f [X]_f\right) + X_{in} - r^-X \left[\frac{mol}{day}\right]\\
		/// F&=& \mbox{Number of fluxes in water storage} \\
		/// q_f&=& \mbox{Water flux in } \frac{m^3}{day}	\\
		/// \left[X\right]_f &=& \mbox{Concentration of solute X in flux }q_f \mbox{ in } \frac{mol}{m^3} \\
		/// X_{in} &=& \mbox{Absolute source or sink term} \frac{mol}{day} \\
		/// r^- &=& \mbox{Decay rate} \frac 1{day} \\
		/// V &=& \mbox{Volume of water in water storage }\left[m^3\right]
		/// \f}
		class SoluteStorage : public cmf::math::StateVariable
		{
			SoluteStorage(WaterStorage* _water,const cmf::water::solute& _Solute, double InitialState=0) 
				: cmf::math::StateVariable(InitialState),m_water(_water), Solute(_Solute),
				  adsorption(new NullAdsorption), reactions()
			{}
			
			WaterStorage* m_water;
			std::unique_ptr<Adsorption> adsorption;
			
		public:
			void set_adsorption(const Adsorption& newadsorption,real m=-1) {
				adsorption.reset(newadsorption.copy(m));
			}
			friend class WaterStorage;
			SoluteReactionList reactions;
			/// @brief The solute, which is stored in this
			const cmf::water::solute& Solute;
			/// @brief Returns the concentration of the solute
			real get_conc() const;
			/// @brief set a new concentration of dissolved tracers. 
			///
			/// In case of adsorption functions, the isotherm is used
			void set_conc(real NewConcentration);
			/// @brief get the waterstorge of the solute storage
			const WaterStorage& get_water() const {
				return *m_water;
			}
			/// @brief Returns True if this solute storage is effected by another state
			virtual bool is_connected(const cmf::math::StateVariable& other) const;
			virtual void add_connected_states(cmf::math::StateVariable::list& states);
			virtual real dxdt(const cmf::math::Time& time);
			virtual real reactive_flux(const cmf::math::Time& time) const;
			virtual std::string to_string() const;
			typedef std::shared_ptr<SoluteStorage> ptr;
		};
	}
}

#endif // SoluteStorage_h__
