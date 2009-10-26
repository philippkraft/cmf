#ifndef FixpointImplicitEuler_h__
#define FixpointImplicitEuler_h__




#include "Integrator.h"
#include "../real.h"

namespace cmf {
	namespace math {
		/// An implicit (backward) Euler integrator using fixpoint iteration
		class ImplicitEuler : public Integrator
		{
			//@}
		private:
			///@name Internal data storages
			//@{
			/// Array to store the new states
			num_array compareStates;
			///Vector of the state variable in the beginning of the integration
			num_array oldStates;
			///Vector of derivatives
			num_array dxdt;
			//@}



		public:
			void AddStatesFromOwner(cmf::math::StateVariableOwner& stateOwner);
			/// Constructs a new FixPointImplicitEuler from a pointer to a vector of state variables
			/// @note The Integrator becomes the owner of states
			/// @param states Statevariables of the system
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			ImplicitEuler(StateVariableOwner& states, real epsilon=1e-9,cmf::math::Time tStepMin=Time::Seconds(10));
			/// Constructs a new FixPointImplicitEuler
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			ImplicitEuler(real epsilon=1e-9,cmf::math::Time tStepMin=Time::Seconds(10));

			/// Copy constructor
			ImplicitEuler(const Integrator&);

			virtual Integrator * Copy() const
			{
				return new ImplicitEuler(*this);
			}
			///Integrates the vector of state variables
			/// @param MaxTime To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed timestep, and changes it into the effictivly used timestep according to the local stiffness of the problem and MaxTime
			int Integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep);
		};
	}
}

#endif // FixpointImplicitEuler_h__
