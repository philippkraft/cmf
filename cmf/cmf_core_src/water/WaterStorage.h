

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
#ifndef WaterStorage_h__
#define WaterStorage_h__

#include "../math/statevariable.h"
#include "../math/real.h"

#include "Solute.h"
#include <map>
#include <vector>
#include "SoluteStorage.h"
#include "flux_node.h"
#include "flux_connection.h"
#include <memory>

namespace cmf {
	namespace water {
		/// @defgroup storages Storages of water
		/// @ingroup nodes

		/// @ingroup storages
		/// @brief A state variable for the storage of water
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
		class WaterStorage : public cmf::math::StateVariable, public cmf::water::flux_node
		{
		public:
			typedef std::shared_ptr<cmf::water::WaterStorage> ptr;
#ifndef SWIG
			operator ptr() {return std::static_pointer_cast<WaterStorage>(shared_from_this());}

#endif
		private:
			// @brief Character indicating if Volume ('V') or head ('h') is the state variable
			char m_state_variable_content;
			typedef std::vector<std::shared_ptr<cmf::water::SoluteStorage> > SoluteStorageMap;
			SoluteStorageMap m_Concentrations;
			void initializeSoluteStorages(const cmf::water::solute_vector& solutes);

		protected:
			virtual real head_to_volume(real head) const;
			virtual real volume_to_head(real volume) const;
	
		public:
			/// @brief A character indicating the integrated variable (either 'V' for Volume or 'h' for head)
			 char get_state_variable_content() const {return m_state_variable_content;}
			/// @brief A character indicating the integrated variable (either 'V' for Volume or 'h' for head)
			void set_state_variable_content(char content);
			/// @brief Returns true, since this is a storage
			bool is_storage() const override;
			/// @brief Returns True if this waterstorage is effected by another state
			virtual bool is_connected(const cmf::math::StateVariable& other) const;
#ifndef SWIG
			virtual void add_connected_states(cmf::math::StateVariable::list& states);
            operator cmf::math::state_list();
#endif
			/// @brief creates a water storage (abstract class)
			/// @param project The project the waterstorage belongs to
			/// @param Name Name of the water storage
			/// @param InitialState Initial water content in m<sup>3</sup>
			/// @param scale A kind of "standard size" in m3 of the water storage to scale tolerances, default 1m3
			WaterStorage(cmf::project& project,const std::string & Name="", 
				double InitialState=0, double scale=1);
			
			static std::shared_ptr<WaterStorage> from_node(cmf::water::flux_node::ptr node);
			/// @brief Returns the water quality of the water storage.
			SoluteStorage& Solute(const cmf::water::solute _Solute);
			const SoluteStorage& Solute(const cmf::water::solute _Solute) const {return *m_Concentrations[_Solute.Id];}
#ifndef SWIG
			SoluteStorage& operator[](cmf::water::solute _Solute) {
				return *m_Concentrations[_Solute.Id];
			}
			const SoluteStorage& operator[](cmf::water::solute _Solute) const {
				return *m_Concentrations[_Solute.Id];
			}

#endif // !SWIG


			/// @brief Returns the concentration of the given solute
			real conc(const cmf::water::solute& _Solute) const;
			/// @brief Returns the current WaterQuality (concentration of all solutes)
			real conc(cmf::math::Time t,const cmf::water::solute& _Solute) const override;
			/// @brief Sets a new concentration
			void conc(const cmf::water::solute& _Solute,real NewConcetration);
			/// @brief Returns the volume of water in this storage in m<sup>3</sup>
			virtual real get_volume() const;
			real get_potential(cmf::math::Time t = cmf::math::never) const override;

			/// @brief Sets the volume of water in this storage in m<sup>3</sup>
			virtual void set_volume(real newwatercontent);
			void set_potential(real newpotential) override;
			virtual real dxdt(const cmf::math::Time& time);
			///@name Overrides of flux_node
			//@{
			virtual bool RecalcFluxes(cmf::math::Time t) {
				return StateIsChanged();
			}
			virtual double is_empty() const {
				return 1-piecewise_linear(get_volume(),0,get_abs_errtol(1e-4));
			}
			virtual std::string to_string() const {
				return cmf::water::flux_node::to_string();
			}
			//@}
			static std::shared_ptr<cmf::water::WaterStorage> cast(std::shared_ptr<cmf::water::flux_node> node)
			{		
				return std::dynamic_pointer_cast<cmf::water::WaterStorage>(node);
			}
			static std::shared_ptr<cmf::water::WaterStorage> create(cmf::project& _project, real initial_state=0.0, real scale=1.0)
			{
				return std::shared_ptr<cmf::water::WaterStorage>(new WaterStorage(_project,"unknown",initial_state, scale));
			}

		};
		typedef std::vector<WaterStorage::ptr> storage_vector;




	}
}
#endif // WaterStorage_h__
