#ifndef WaterSoluteIntegrator_h__
#define WaterSoluteIntegrator_h__
#include "Integrator.h"
#include "../water/WaterStorage.h"
#include "../water/SoluteStorage.h"
namespace cmf {
	namespace math {
		class SoluteWaterIntegrator : public cmf::math::Integrator
		{
		protected:
		public:
			cmf::math::Integrator* WaterIntegrator;
			cmf::math::Integrator* SoluteIntegrator;
			void AddStatesFromOwner(cmf::math::StateVariableOwner& stateOwner)
			{
				Integrator::AddStatesFromOwner(stateOwner);
				for(cmf::math::StateVariableVector::iterator it = m_States.begin(); it != m_States.end(); ++it)
				{
					cmf::water::WaterStorage *ws = dynamic_cast<cmf::water::WaterStorage *>(**it);
					if(ws)	WaterIntegrator->AddState(*ws);
					else SoluteIntegrator->AddState(***it);
									    
				}
			}
			int Integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep)
			{
				WaterIntegrator->Integrate(MaxTime,TimeStep);
				SoluteIntegrator->Integrate(MaxTime,TimeStep);
				return 1;
			}
			virtual cmf::math::SoluteWaterIntegrator* Copy() const
			{
				return new SoluteWaterIntegrator(*WaterIntegrator,*SoluteIntegrator);
			}
			SoluteWaterIntegrator(const cmf::math::Integrator& water_integrator, const cmf::math::Integrator& solute_integrator)
				: WaterIntegrator(water_integrator.Copy()), SoluteIntegrator(solute_integrator.Copy())
			{

			}


		};
		
	}
	
}
#endif // WaterSoluteIntegrator_h__
