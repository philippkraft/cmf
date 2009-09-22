#ifndef WaterSoluteIntegrator_h__
#define WaterSoluteIntegrator_h__
#include "Integrator.h"
#include "../../water/WaterStorage.h"
#include "../../water/SoluteStorage.h"
namespace cmf {
	namespace math {
		class SoluteWaterIntegrator : public cmf::math::Integrator
		{
		protected:
			std::auto_ptr<cmf::math::Integrator> WaterIntegrator;
			std::auto_ptr<cmf::math::Integrator> SoluteIntegrator;
			void distribute_states()
			{
				for(cmf::math::StateVariableVector::iterator it = m_States.begin(); it != m_States.end(); ++it)
				{
					cmf::water::WaterStorage *ws = dynamic_cast<cmf::water::WaterStorage *>(*it);
					if(ws)	WaterIntegrator->AddState(*ws);
					else SoluteIntegrator->AddState(**it);
				}
			}

		public:
			cmf::math::Integrator* get_water_integrator() const
			{
				return WaterIntegrator.get();
			}
			cmf::math::Integrator* get_solute_integrator() const
			{
				return SoluteIntegrator.get();
			}
			void set_water_integrator(cmf::math::Integrator* templ)
			{
				WaterIntegrator=templ->Copy();
				SoluteIntegrator=SoluteIntegrator->Copy();
				distribute_states();
			}
			void set_solute_integrator(cmf::math::Integrator* templ)
			{
				SoluteIntegrator=templ->Copy();
				WaterIntegrator=WaterIntegrator->Copy();
				distribute_states();
			}
			void AddStatesFromOwner(cmf::math::StateVariableOwner& stateOwner)
			{
				Integrator::AddStatesFromOwner(stateOwner);
				distribute_states();
			}
			int Integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
			{
				WaterIntegrator->IntegrateUntil(MaxTime,TimeStep);
				SoluteIntegrator->IntegrateUntil(MaxTime,TimeStep);
				m_Time=MaxTime;
				m_TimeStep=WaterIntegrator->TimeStep();
				return 1;
			}
			virtual cmf::math::SoluteWaterIntegrator* Copy() const
			{
				return new SoluteWaterIntegrator(*WaterIntegrator,*SoluteIntegrator);
			}
			SoluteWaterIntegrator(const cmf::math::Integrator& water_integrator, const cmf::math::Integrator& solute_integrator)
				: Integrator(), WaterIntegrator(water_integrator.Copy()), SoluteIntegrator(solute_integrator.Copy())
			{

			}
			virtual void Reset()
			{
				WaterIntegrator->ModelTime(m_Time);
				SoluteIntegrator->ModelTime(m_Time);
			}
			SoluteWaterIntegrator(const cmf::math::Integrator& water_integrator, const cmf::math::Integrator& solute_integrator, cmf::math::StateVariableOwner& states)
				: Integrator(), WaterIntegrator(water_integrator.Copy()), SoluteIntegrator(solute_integrator.Copy())
			{
				 AddStatesFromOwner(states);
			}


		};
		
	}
	
}
#endif // WaterSoluteIntegrator_h__
