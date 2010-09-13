

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

#include "../math/statevariable.h"
#include "../math/real.h"

#include "Solute.h"
#include <map>
#include <vector>
#include "SoluteStorage.h"
#include "flux_node.h"
#include "flux_connection.h"
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
			real get_abs_errtol(real rel_errtol) const {
				return rel_errtol;
			}
			/// A character indicating the integrated variable (either 'V' for Volume or 'h' for head)
			inline char get_state_variable_content() const {return m_state_variable_content;}
			/// A character indicating the integrated variable (either 'V' for Volume or 'h' for head)
			void set_state_variable_content(char content);
			/// Returns true, since this is a storage
			virtual bool is_storage() const {return true;}
			cmf::math::state_queue get_states();
			/// creates a water storage (abstract class)
			/// @param project The project the waterstorage belongs to
			/// @param Name Name of the water storage
			/// @param InitialState Initial water content in m<sup>3</sup>
			WaterStorage(const cmf::project& project,const std::string & Name="", double InitialState=0);
			
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
			virtual real get_potential() const	{
				if (get_state_variable_content()=='h')
					return get_state();
				else
					return volume_to_head(get_state());
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
			virtual bool RecalcFluxes(cmf::math::Time t) {
				return StateIsChanged();
			}
			virtual double is_empty() const {
				return 1-piecewise_linear(get_volume(),0,get_abs_errtol(1e-4));
			}
			//@}
			static std::tr1::shared_ptr<cmf::water::WaterStorage> cast(std::tr1::shared_ptr<cmf::water::flux_node> node)
			{		
				return std::tr1::dynamic_pointer_cast<cmf::water::WaterStorage>(node);
			}
			static std::tr1::shared_ptr<cmf::water::WaterStorage> create(const cmf::project& _project, real initial_state=0.0)
			{
				return std::tr1::shared_ptr<cmf::water::WaterStorage>(new WaterStorage(_project,"unknown",initial_state));
			}

		};
		typedef std::vector<WaterStorage::ptr> storage_vector;


		/// @ingroup connections
		/// Calculates flux out of a storage as a linear function of its volume to a power.
		///
		/// \f[ q = \frac {V_{mobile}^\beta}{t_r} \f]
		/// where:
		/// - \f$V_{mobile} [m^3] = V - V_{residual}\f$ the stored mobile volume
		/// - \f$\beta [-]\f$ An empirical exponent to shape the flux function
		/// - \f$t_r [days]\f$ The residence time of the water in this storage in days
		class kinematic_wave : public flux_connection {
		protected:
			WaterStorage::ptr source;
			real calc_q(cmf::math::Time t)	{
				real V= std::max(0.0,source->get_volume()-residual_volume);
				return pow(V,exponent)/residencetime;
			}
			void NewNodes() {
				source = WaterStorage::cast(left_node());
			}
		public:
			/// Linear flow parameter traveltime in days
			real residencetime;
			/// Exponent of volume, beta
			real exponent;
			/// residual volume in m3
			real residual_volume;

			/// Creates a kinematic wave connection.
			/// \f[ q = \frac {\left(V - V_{residual}\right)^\beta}{t_r} \f]
			/// @param source Water storage from which the water flows out. Flux is a function of source.volume
			/// @param target Target node (boundary condition or storage). Does not influence the strength of the flow
			/// @param residencetime \f$t_r [days]\f$ The residence time of the water in this storage
			/// @param exponent \f$\beta [-]\f$ An empirical exponent to shape the flux function (default = 1 (linear function))
			/// @param residual_volume \f$V_{residual} [m^3]\f$ The volume of water not flowing out (default = 0 m3)
			kinematic_wave(WaterStorage::ptr source,flux_node::ptr target,real residencetime, real exponent=1.0, real residual_volume=0.0);
		};


	}
}
#endif // WaterStorage_h__
