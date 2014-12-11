#ifndef RK23_MPI_H_
#define RK23_MPI_H_

#include "integrator.h"

#ifdef MPI
	#include <mpi.h>
#endif

namespace cmf {
namespace math {


/// An embedded explicit Runge Kutta integrator of order 3(2), known as the Bogacki-Shampine method,
/// with automatic step size control that supports MPI.
/// Does not use Integrator's m_dt variable for its step size, but rather an internal variable tau,
/// so that no external changes to m_dt (e.g. by integrate_until) mess up the automatic step size
/// control.
class RK23Integrator : public Integrator
{
public:
	/// Create a new RK23MPI integrator.
	/// @param states State variables to be added to the integrator.
	/// @param abstol Absolute tolerance for error estimator and step size control.
	/// @param reltol Relative tolerance for error estimator and step size control.
	/// @param dt_min Minimum step size.
	RK23Integrator(cmf::math::StateVariableOwner& states, double abstol=1e-6, double reltol=1e-6,
	        Time dt_min=cmf::math::sec);

	RK23Integrator( const cmf::math::RK23Integrator& other);
	/// Destructor.
	virtual ~RK23Integrator();

	/// Do not use this method. It is not implemented.
	virtual Integrator* copy() const
	{
		return new RK23Integrator(*this);
	}
	
	/// Add state variables from a StateVariableOwner
	void add_states(cmf::math::StateVariableOwner& stateOwner);

	/// Integrates the vector of state variables, either till time t_max or a point of time before
	/// that, if a small step size is required.
	/// @param t_max Upper boundary for integration interval.
	/// @param dt    Ignored.
	int integrate(cmf::math::Time t_max, cmf::math::Time dt);

	void reset();

private:
	/// Estimate error of approximation y based on the difference between y and y_e.
	double estimate_error();


	double abstol;  ///< Absolute tolerance for error estimator and step size control.
	double reltol;  ///< Relative tolerance for error estimator and step size control.

	Time   dt_min;  ///< Minimum step size.

	bool initialized;

	num_array old_states;  ///< Saves current state values.
	num_array y;    ///< Result of the 3rd order method.
	num_array y_e;  ///< Result of the embedded 2nd order method.

	/// constants for step size control
	static const double fac     /*= 0.9*/;  ///< safety factor
	static const double facmax_ /*= 2.0*/;  ///< prevent too large step size increases
	static const double facmin  /*= 0.1*/;  ///< prevent too large step size decreases
	double facmax;  ///< actually used facmax value; not const because it will be temporarily
	                ///< set to 1 after step size rejection (see Hairer/Wanner)

	/// constants for the 3rd order method
	static const real c2, c3, c4;

  // Butcher tableau
  static const real a21,
                         a32,
                    a41, a42, a43;

  // 3rd order method:
  // first same as last (FSAL), i.e. a4i = bi

	// 2nd order embedded method
	static const real b1e, b2e, b3e, b4e;

	/// variables for Runge Kutta stages
	num_array k1, k2, k3, k4;
};


}  // namespace math
}  // namespace cmf


#endif  // RK23_MPI_H_
