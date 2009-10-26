#ifndef StateVariable_h__
#define StateVariable_h__
#include "Time.h"
#include <string>
#include <vector>
#include <cmath>
#include "num_array.h"
#include "real.h"
namespace cmf {
	/// Contains classes for numerical solving of ODE's
  namespace math {
		/*! Abstract class state variable
				
				Simple exponential system class header implementing a state variable:
				@code
				class RateGrowth
				{
				public:
					real rate;
					virtual real Derivate(const cmf::math::Time& time) {return rate*get_state();}
				};
				@endcode
		*/
		class StateVariable
		{
		private:
			bool m_StateIsNew;
			/// Holds the value of the Statevariable
			real m_State;
		protected:
			virtual void StateChangeAction() {}
			/// Sets the updated flag (m_StateIsNew) to false
			void MarkStateChangeHandled() {m_StateIsNew=false;}
		public:
			/// Returns the derivate of the state variable at time @c time
			virtual real Derivate(const cmf::math::Time& time)=0;
			/// Returns the current state of the variable
			real get_state() const {return m_State;}
			/// Gives access to the state variable
			void set_state(real newState) {
				//m_StateIsNew=m_State!=newState;
				m_State=newState;
				m_StateIsNew=true;
				if (m_StateIsNew) StateChangeAction();
			}
			/// Returns if the state was currently updated
			bool StateIsChanged() {return m_StateIsNew;}
			/// ctor
			StateVariable(real InitialState=0) : m_State(InitialState),m_StateIsNew(true) {}
		};

		/// A vector of state variables, can be solved by RKFIntegrator
		class StateVariableVector : public std::vector<StateVariable*>
		{
			void AddValuesToStates(const num_array& operands);
		public:
			bool use_OpenMP;
			/// Copies the states to a numeric vector using use_OpenMP
			void CopyStates(num_array & destination) const;
			void CopyStates(real * destination) const;
			/// Copies the new states to the actual states
			void SetStates(const num_array & newStates) ;
			void SetStates(real * newStates);
			/// Operator to add something to the states
			StateVariableVector& operator+=(const num_array& aVector)
			{
				this->AddValuesToStates(aVector);
				return *this;
			}
			/// Copies the derivatives at time step "time" to a numeric vector using use_OpenMP
			/// @param time Time at which the derivatives should be calculated
			/// @param destination Vector to be overwritten by the results
			/// @param factor A factor that is multiplied to the derivate (e.g. unit conversion or integration length)
			void CopyDerivs(Time time,num_array & destination, real factor=1) const;
			/// Copies the derivatives at time step "time" to an preallocated c array
			/// @param time Time at which the derivatives should be calculated
			/// @param destination Allocated c array
			/// @param factor A factor that is multiplied to the derivate (e.g. unit conversion or integration length)
			void CopyDerivs(Time time,real * destination,real factor=1) const;
			/// Returns the states in a numeric vector using :CopyStates, but is slower because of additional memory allocation
			num_array GetStates() const 
			{
				num_array result(this->size());
				CopyStates(result);
				return result;
			}
			/// Returns the derivatives at time step "time" in a numeric vector using :CopyDerivs, but is slower because of additional memory allocation
			num_array GetDerivs(Time time) const 
			{
				num_array result(this->size());
				CopyDerivs(time,result);
				return result;
			}
			StateVariableVector() : use_OpenMP(true) {}
		};
		
		///An abstract class, that owns one or more state variables, that can add them to a vector of state variables in a certain order
		class StateVariableOwner
		{
		public:
			/// Add the state variables, owned by an object derived from StateVariableOwner, to the given vector
			virtual void AddStateVariables(cmf::math::StateVariableVector& vector)=0;
			explicit StateVariableOwner() {}
		};
	}
}

#endif // StateVariable_h__
