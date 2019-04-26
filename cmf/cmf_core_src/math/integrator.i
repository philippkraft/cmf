%{
#include "math/statevariable.h"
#include "math/integrators/integrator.h"
#include "math/integrators/bdf2.h"
#include "math/integrators/explicit_euler.h"
#include "math/integrators/implicit_euler.h"
#include "math/integrators/RKFintegrator.h"
#include "math/integrators/cvode.h"
#include "math/integrators/multiintegrator.h"
#include "math/integrators/WaterSoluteIntegrator.h"
%}

%rename(__getitem) cmf::math::Integrator::operator[];
%state_downcast(cmf::math::StateVariable::ptr cmf::math::Integrator::operator[]);

%include "math/integrators/integrator.h"
%include "math/integrators/bdf2.h"
%include "math/integrators/explicit_euler.h"
%include "math/integrators/implicit_euler.h"
%include "math/integrators/RKFintegrator.h"
%include "math/integrators/cvode.h"
%include "math/integrators/multiintegrator.h"
%include "math/integrators/WaterSoluteIntegrator.h"


%extend__repr__(cmf::math::Integrator);
%extend_pysequence(cmf::math::Integrator);
%extend cmf::math::Integrator { 
    %pythoncode {
    t = property(get_t,set_t,doc="Sets the actual time of the solution")
    dt = property(get_dt,doc="Get the current time step of the solver")
    def __call__(self, t, dt=None, reset=False):
        """
        Advances the integration until `t`

        A shortcut to .integrate_until

        Parameters
        ----------
        t : cmf.Time
            The time step to advance to. If t < current time, the solver will
            advance to self.t + t
        dt : cmf.Time, optional
            The timestep for the integration. If not given try to integrate in one step
        reset : bool, optional
            If True, the solver will perform a reset before starting

        Returns
        -------
        cmf.Time
            The new time stamp
        """
        if t < self.t:
            t += self.t
        if dt is None:
            dt = t - self.t
        self.integrate_until(t, dt, reset=reset)
        return self.t

    def run(self, start=None, end=None, step=day*1, max_errors=0, reset=False):
        """
        Returns an iterator over the timesteps start..end

        **Examples:**

        >>> solver=cmf.CVodeIntegrator(...)
        >>> for t in solver.run(solver.t, solver.t + cmf.week, cmf.h):
        >>>    print(t, solver[0].state)
        or with list comprehension
        >>> states = [solver[0].state for t in solver.run(solver.t, solver.t + cmf.week, cmf.h)]

        Parameters
        ----------
        start : cmf.Time, optional
                Start time for the solver iteration
        end : cmf.Time, optional
                End time of the iteration
        step : cmf.Time, optional
                Step size for the integration
        max_errors: int
                Number of tolerated errors. If >0, up to these number of runtime errors
                will be saved with their time and the integration proceeds after a reset
                of the solver. Some systems operate with values close to their physical
                limits and inifinite values in the integration can easily occur. For
                these kind of systems set max_errors to eg. 10. A larger number of errors
                should be eliminated usually.
        reset: bool
                If True, the solver performs a `reset` at every time step

        Yields
        ------
        cmf.Time
             the actual timestep
        """
        from logging import warning
        if not start is None:
            self.t = start
        if end is None:
            end = self.t + 100*step
        errors = []
        t = self.t
        while self.t < end:
            try:
                t = self(self.t+step, step, reset=reset)
            except Exception as e:
                if len(errors) < max_errors:
                    errors.append((t, e))
                    self.reset()
                    warning(str(t) + ': ' + str(e))
                else:
                    raise
            yield t
}
}

%extend__repr__(cmf::math::CVodeInfo);

%extend cmf::math::CVodeBase {
    %pythoncode {
    def get_jacobian(self):
        return self._get_jacobian().reshape((self.size(), self.size()), order='F')
    info = property(get_info)
    }
}
