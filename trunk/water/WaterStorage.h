#ifndef WaterStorage_h__
#define WaterStorage_h__

#include "../math/StateVariable.h"
#include "../math/real.h"

#include "Solute.h"
#include <map>
#include <vector>
#include "WaterFlux.h"
#include "SoluteStorage.h"
#include "FluxConnection.h"

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
		class WaterStorage : public cmf::math::StateVariable,public cmf::math::StateVariableOwner,public cmf::water::FluxNode
		{
		private:
			typedef std::vector<cmf::water::SoluteStorage*> SoluteStorageMap;
			SoluteStorageMap m_Concentrations;
			void initializeSoluteStorages();
		public:
			virtual bool is_storage() const {return true;}
			void AddStateVariables(cmf::math::StateVariableVector& vector);
			/// creates a water storage (abstract class)
			/// @param InitialState Initial water content in m<sup>3</sup>
			WaterStorage(const cmf::project& _project, double InitialState=0);
			WaterStorage(const WaterStorage& forcopy);
			static WaterStorage* FromNode(cmf::water::FluxNode& node)
			{
				WaterStorage* result=new WaterStorage(node.project());
				replace_node(node,*result);
				return result;
			}
			/// Returns the water quality of the water storage.
			SoluteStorage& Solute(const cmf::water::Solute& solute);
			const SoluteStorage& Solute(const cmf::water::Solute& solute) const {return *m_Concentrations[solute.Id];}
			/// Returns the concentration of the given solute
			real conc(const cmf::water::Solute& solute) const;
			/// Returns the current WaterQuality (concentration of all solutes)
			WaterQuality conc(cmf::math::Time t);
			/// Sets a new concentration
			void conc(const cmf::water::Solute& solute,real NewConcetration)
			{
				Solute(solute).set_state(NewConcetration*this->get_state());
			}
			/// Returns the volume of water in this storage in m<sup>3</sup>
			real water() const {
				return get_state();
			}
			/// Sets the volume of water in this storage in m<sup>3</sup>
			void water(real newwatercontent)	{
				set_state(newwatercontent);
			}
			virtual std::string ToString() const	{
				return Name;
			}
			virtual real Derivate(const cmf::math::Time& time) {
				return water_balance(time);
			}
			///@name Overrides of FluxNode
			//@{
			virtual bool RecalcFluxes(cmf::math::Time t) {return StateIsChanged();}
			virtual bool is_empty() {return get_state()<=0;}
			virtual WaterStorage* copy() const
			{
				return new WaterStorage(*this);
			}
			//@}
		};
		cmf::water::WaterStorage* AsWaterStorage(cmf::math::StateVariable * state);
		cmf::water::WaterStorage* AsWaterStorage(cmf::water::FluxNode* node);

	}
}
#endif // WaterStorage_h__
