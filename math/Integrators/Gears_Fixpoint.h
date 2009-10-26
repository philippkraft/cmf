#ifndef Gears_Fixpoint_h__
#define Gears_Fixpoint_h__


#define GEAR_MAX_ITER 200
#define GEAR_RECOVER_AFTER_STEPS 500
#include "Integrator.h"
#include "../real.h"

namespace cmf {
	namespace math {
		/// An up to 4 step Gears-Method with fixpoint iteration, needs bug fixing
		///
		/// Derived from 
		/// Roussel C. and Roussel M. (2003) "Generic Object-Oriented Differential Equation Integrators", 
		/// C/C++ User Journal, Nov. 2003, http://www.ddj.com/cpp/184401724?pgno=8
		class Gears_Fixpoint : public Integrator
		{
			//@}
		private:
			///@name Internal data storages
			//@{
			/// Array to store the states for test of convergence
			num_array compareStates;
			/// Array to store the derivatives
			num_array dxdt;

			///History of the last states
			num_array pastStatesArray[6];
			///Number of steps taken
			int stepNo;
			/// Current order of Gear formula
			int order;
			/// Maximum order of method
			int max_order;
			/// Gets one of the last states
			/// @returns   the n<sup>th</sup> state in history
			/// @param toBack Number of steps back (ranging from 1..4, no runtime check)
			num_array& pastStates(int toBack=1)
			{
				int pos = (stepNo-toBack) % max_order;
				return pastStatesArray[pos];
			}

			/// Returns true if the states are approximately equal to the given array (test for convergence)
			bool approx_equal_to_states(const num_array& compare);
			/// Returns the maximum of the error exceedance between states and a given array
			real max_rel_error_exceedance(const num_array& compare);
			//@}

			/// @name Time step management
			//@{
			int time_reduce_stage;
		public:
			///Returns int Stage oof time reduction
			int Time_reduce_stage() const { return time_reduce_stage; }			
			//@}
		private:



			/// @name Gear functions: The following functions evaluate the Gear formulas of specified order, loading the results into compareStates.
			//@{
			/// Gear formulas through an array of pointers.
			typedef void (Gears_Fixpoint::*newState_functions)(real);
			newState_functions calc_newState[4];

			void Gear1newState(real h);
			void Gear2newState(real h);
			void Gear3newState(real h);
			void Gear4newState(real h);
			void Gear5newState(real h);
			//@}


		public:

			void AddStatesFromOwner(cmf::math::StateVariableOwner& stateOwner);

			/// Constructs a new Gears_Fixpoint from a pointer to a vector of state variables
			/// @note The Integrator becomes the owner of states
			/// @param states Statevariables of the system
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			/// @param maxOrder Maximum order of method (default=4)
			Gears_Fixpoint(const StateVariableVector& states, real epsilon=1e-9,cmf::math::Time tStepMin=Time::Seconds(10),int maxOrder=4);
			/// Constructs a new Gears_Fixpoint
			/// @param epsilon relative error tolerance per time step (default=1e-9)
			/// @param tStepMin minimum time step (default=10s)
			/// @param maxOrder Maximum order of method (default=4)
			Gears_Fixpoint(real epsilon=1e-9,cmf::math::Time tStepMin=Time::Seconds(10),int maxOrder=4);
			/// Copy constructor
			Gears_Fixpoint(const Integrator&);

			virtual Integrator * Copy() const
			{
				return new Gears_Fixpoint(*this);
			}

			virtual ~Gears_Fixpoint();


			///Integrates the vector of state variables
			/// @param MaxTime To stop the model (if running in a model framework) at time steps of value exchange e.g. full hours, the next value exchange time can be given
			/// @param TimeStep Takes the proposed timestep, and changes it into the effictivly used timestep according to the local stiffness of the problem and MaxTime
			int Integrate(cmf::math::Time MaxTime,cmf::math::Time TimeStep);
		};	
		
		
		
	
	}
}

#endif // Gears_Fixpoint_h__
