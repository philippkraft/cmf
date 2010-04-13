

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
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
#ifndef WaterStorage_h__
#define WaterStorage_h__

#include "../math/StateVariable.h"
#include "../math/real.h"

#include "Solute.h"
#include <map>
#include <vector>
#include "SoluteStorage.h"
#include "flux_node.h"
#include <tr1/memory>

namespace cmf {
	namespace water {
		/// @defgroup storages Storages of water
		/// @ingroup nodes

		/// @ingroup storages
		/// A state variable for the storage of water
		///
		/// A class for the storage of water. The state is the volume of water stored in \f$m^3\f$
		/// The derivative function is given by:
		/// \f{eqnarray*}
		/// \frac{dV}{dt}&=&\sum_{f=1}^{F} q_f  \\
		/// F&=& \mbox{Number of fluxes in water storage} \\
		/// q_f&=& \mbox{Water flux in } \frac{m^3}{day}	\\
		/// \f}
		///
		/// @todo Check the head based state mode 
		class WaterStorage : public cmf::math::StateVariable,public cmf::math::StateVariableOwner,public cmf::water::flux_node
		{
		public:
			typedef std::tr1::shared_ptr<cmf::water::WaterStorage> ptr;
#ifndef SWIG
			operator ptr() {return std::tr1::static_pointer_cast<WaterStorage>(shared_from_this());}
#endif
		private:
			// Character indicating if Volume ('V') or head ('h') is the state variable
			char m_state_variable_content;
			std::tr1::shared_ptr<WaterStorage> this_no_delete;
			typedef std::vector<std::tr1::shared_ptr<cmf::water::SoluteStorage> > SoluteStorageMap;
			SoluteStorageMap m_Concentrations;
			void initializeSoluteStorages(const cmf::water::solute_vector& solutes);


		protected:
			virtual real head_to_volume(real head) const;
			virtual real volume_to_head(real volume) const;
	
		public:
			/// A character indicating the integrated variable (either 'V' for Volume or 'h' for head)
			inline char get_state_variable_content() const {return m_state_variable_content;}
			/// A character indicating the integrated variable (either 'V' for Volume or 'h' for head)
			void set_state_variable_content(char content);
			/// Returns true, since this is a storage
			virtual bool is_storage() const {return true;}
			void AddStateVariables(cmf::math::StateVariableVector& vector);
			/// creates a water storage (abstract class)
			/// @param _project The project the waterstorage belongs to
			/// @param InitialState Initial water content in m<sup>3</sup>
			WaterStorage(const cmf::project& _project, double InitialState=0);
			
			static std::tr1::shared_ptr<WaterStorage> from_node(cmf::water::flux_node::ptr node);
			/// Returns the water quality of the water storage.
			SoluteStorage& Solute(const cmf::water::solute& _Solute);
			const SoluteStorage& Solute(const cmf::water::solute& _Solute) const {return *m_Concentrations[_Solute.Id];}
			/// Returns the concentration of the given solute
			real conc(const cmf::water::solute& _Solute) const;
			/// Returns the current WaterQuality (concentration of all solutes)
			real conc(cmf::math::Time t,const cmf::water::solute& _Solute) const
			{
				return conc(_Solute);
			}
			/// Sets a new concentration
			void conc(const cmf::water::solute& _Solute,real NewConcetration)
			{
				Solute(_Solute).set_state(NewConcetration*this->get_state());
			}
			/// Returns the volume of water in this storage in m<sup>3</sup>
			virtual real get_volume() const {
				if (get_state_variable_content()=='h')
					return head_to_volume(get_state());
				else
					return get_state();

			}
			/// Sets the volume of water in this storage in m<sup>3</sup>
			virtual void set_volume(real newwatercontent)	{
				if (get_state_variable_content()=='h')
					set_state(volume_to_head(newwatercontent));
				else
					set_state(newwatercontent);
			}
			
			virtual real Derivate(const cmf::math::Time& time);
			real get_state() const
			{
				return cmf::math::StateVariable::get_state();
			}
			void set_state(real newState)
			{
				cmf::math::StateVariable::set_state(newState);
			}
			///@name Overrides of flux_node
			//@{
			virtual bool RecalcFluxes(cmf::math::Time t) {return StateIsChanged();}
			virtual bool is_empty() const {return get_state()<=0;}
			//@}
			static std::tr1::shared_ptr<cmf::water::WaterStorage> cast(std::tr1::shared_ptr<cmf::water::flux_node> node)
			{		
				return std::tr1::dynamic_pointer_cast<cmf::water::WaterStorage>(node);
			}
			static std::tr1::shared_ptr<cmf::water::WaterStorage> create(const cmf::project& _project, real initial_state=0.0)
			{
				return std::tr1::shared_ptr<cmf::water::WaterStorage>(new WaterStorage(_project,initial_state));
			}

		};
		typedef std::vector<WaterStorage::ptr> storage_vector;

	}
}
#endif // WaterStorage_h__
