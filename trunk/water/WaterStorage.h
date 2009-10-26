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

		/// A state variable for the storage of water
		/// A class for the storage of any tracer. The state is the amount (mol, kg etc. see cmf::water) of the tracer in the storage
		/// \f{eqnarray*}
		/// \frac{dV}{dt}&=&\sum_{f=1}^{F} q_f  \\
		/// F&=& \mbox{Number of fluxes in water storage} \\
		/// q_f&=& \mbox{Water flux in } \frac{m^3}{day}	\\
		/// \f}
		/// The vector fluxes is used used by concentration to get the amount of water mixing.
		class WaterStorage : public cmf::math::StateVariable,public cmf::math::StateVariableOwner,public cmf::water::flux_node
		{
		private:
			std::tr1::shared_ptr<WaterStorage> this_;
			typedef std::vector<std::tr1::shared_ptr<cmf::water::SoluteStorage> > SoluteStorageMap;
			SoluteStorageMap m_Concentrations;
			void initializeSoluteStorages(const cmf::water::solute_vector& solutes);
		public:

			virtual bool is_storage() const {return true;}
			void AddStateVariables(cmf::math::StateVariableVector& vector);
			/// creates a water storage (abstract class)									o
			/// @param _project The project the waterstorage belongs to
			/// @param InitialState Initial water content in m<sup>3</sup>
			WaterStorage(const cmf::project& _project, double InitialState=0);
			
			static std::tr1::shared_ptr<WaterStorage> from_node(cmf::water::flux_node::ptr node);
			/// Returns the water quality of the water storage.
			SoluteStorage& Solute(const cmf::water::solute& solute);
			const SoluteStorage& Solute(const cmf::water::solute& solute) const {return *m_Concentrations[solute.Id];}
			/// Returns the concentration of the given solute
			real conc(const cmf::water::solute& solute) const;
			/// Returns the current WaterQuality (concentration of all solutes)
			real conc(cmf::math::Time t,const cmf::water::solute& solute) const
			{
				return conc(solute);
			}
			/// Sets a new concentration
			void conc(const cmf::water::solute& solute,real NewConcetration)
			{
				Solute(solute).set_state(NewConcetration*this->get_state());
			}
			/// Returns the volume of water in this storage in m<sup>3</sup>
			real get_volume() const {
				return get_state();
			}
			/// Sets the volume of water in this storage in m<sup>3</sup>
			void set_volume(real newwatercontent)	{
				set_state(newwatercontent);
			}
			virtual real Derivate(const cmf::math::Time& time) {
				return water_balance(time);
			}
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
		typedef std::tr1::shared_ptr<cmf::water::WaterStorage> storage_pointer;

	}
}
#endif // WaterStorage_h__
