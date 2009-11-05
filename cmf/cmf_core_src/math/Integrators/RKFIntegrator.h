#ifndef RKFIntegrator_h__
#define RKFIntegrator_h__



#include "Integrator.h"
#include "../real.h"

namespace cmf {
	namespace math {
		///Integrates a vector of cmf::math::StateVariable with the Runge-Kutta-Fehlberg (RKF54) method
		class RKFIntegrator : public Integrator
		{
		private:
			static const real 
				//Constants for the RKF-Method
				c[6],A[6][5],b5[6],e[6];

			///The 6 Vectors of k-Values. 
			///Since they are only needed in the Integrate method, from theory they should defined as local variable. 
			///But Allocation needs time, and we will call this often
			num_array kValues[6];
			real& k(size_t order,int stateIndex) 
			{return (kValues[order])[stateIndex];}
			num_array& k(size_t order)
			{return kValues[order]; }
			///Vector of the state variable in the beginning of the integration
			num_array oldStates;
			//@}
		public:
			void cmf::math::RKFIntegrator::AddStatesFromOwner( cmf::math::StateVariableOwner& stateOwner );

			/// Constructs a new RKFIntegrator from a pointer to a vector of state variables
			/// @note The RKF Integrator becomes the owner of states
			/// @param states Statevariables of the system
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			RKFIntegrator(StateVariableOwner& states, real epsilon=1e-9,cmf::math::Time tStepMin=Time::Seconds(10));
			/// Constructs a new RKFIntegrator
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			RKFIntegrator(real epsilon=1e-9,cmf::math::Time tStepMin=Time::Seconds(10));
			/// Copy constructor, does not copy 
			RKFIntegrator(const Integrator& forCopy);
			

			virtual Integrator * Copy() const
			{
				return new RKFIntegrator(*this);
			}
			
			///Integrates the vector of state variables
			/// @param MaxTime To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed timestep, and changes it into the effictivly used timestep according to the local stiffness of the problem and MaxTime
			int Integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep);
	

		};
		
	}
}
#endif // RKFIntegrator_h__
