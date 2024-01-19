@page cmfTutSolver Choosing a solver

cmf uses the [finite volume method](@ref finiteVolumeMethod) to set up a wide range of models of
water flow through your study area. The result of the finite volume
method discretization in space is an ordinary differential equation (ODE)
system, that needs to be integrated in time. Several solvers are
available for this task, each with its own advantages and disadvantages.

## Euler's method

Generally the simplest method to solve an ODE system is the explicit
Euler's method. It advances the system state to the next time step by a
linear extrapolation of the current change rate of the system.


@f[
\vec{u}(t_{i+1}) = \vec{u}(t_{i}) + h \vec{f}(\vec{u}(t_{i}),t_{i})
@f]

where: 
- @f$\vec{u}(t)@f$ is the system state at the given timestep
(water and solute storages) 
- @f$t_i@f$ is the actual time step 
- @f$t_{i+1}@f$ is the time step to be calculated 
- @f$h = t_{i+1}-t_{i}@f$ is the length of the time step 
- @f$ \frac{d\vec u}{dt} = \vec{f}(\vec{u}(t),t)@f$ is the ODE.

Since Euler's method is so simple, it is the fastest method to solve an
ODE. However, by using a finite time step, an error occurs if the system
does not behave linearly. The solver is not error controlled, hence
these errors might accumulate and create a completely chaotic behavior.
The solver is not stable. In fact, many (if not all) systems set up with
cmf, are so badly extrapolated, that even extremely small timesteps will
still behave unstable. However, if you want to try the simple Euler
method, [ExplicitEuler_fixed](@ref cmf::math::ExplicitEuler_fixed)
can be used.

There are two approaches to solve this problem:

- Use several time steps to make a polynomial based prediction of the
  future time step (a solver with an higher **order**) 
- or use an **implicit** formulation of the next timestep, where @f$u(t_{i+1})@f$
   appears on both sides of the equation.

Both approaches can be combined by using a higher order, implicit
method.

## Stiff systems 

[Stiffness](http://en.wikipedia.org/wiki/Stiff_equation) is a property
of an ODE, which occurs often when equations of different timescales
need to be solved together in one system. Due to the high order of the
conductivity/water content relation of porous media, any cmf model using
"physical" water retention is a stiff system. Such a system cannot be
solved effectively with an explicit solver, regardless of its order.
Implicit methods of a higher order than 2 also do have a
constraint stability and may fail on a stiff system. The simplest way to
decide how stiff your cmf model is, is not to try to calculate the
stiffness ratio, as given in the Wikipedia link, but just to try out
different methods. If one of the implicit methods solves your system
faster than the [RKFIntegrator](@ref cmf::math::RKFIntegrator), your
system is stiff enough to use an implicit method. If the RKF solver is
faster, you can try the
[ExplicitEuler_fixed](@ref cmf::math::ExplicitEuler_fixed) with a
timestep near to the lowest time step the RKF method has used, but in
most cases the timestep will be so small, that using the explicit Euler
method is not an effective option.

## Higher order 

A solver that is predicting the next timestep using a polynom of a
certain order can work in two different ways. One option is for the
solver to remember the last states and fit the polynomial in the last
states (so called [linear multistep methods](http://en.wikipedia.org/wiki/Linear_multistep_method) or
Adams-Bashfort method) the other one is to introduce sub-steps in
between the actual step and the "real" next step, the family of
[Runge-Kutta methods](http://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods).

In cmf the well known
[Runge-Kutta-Fehlberg 4/5](http://en.wikipedia.org/wiki/Runge%E2%80%93Kutta%E2%80%93Fehlberg_method)
method is implemented as
[RKFIntegrator](@ref cmf::math::RKFIntegrator). This method uses a
Runge Kutta method of order 4 to predict the next timestep and a Runge
Kutta method of order 5 for comparison. If the two methods yield
different results, the time step is reduced until the difference between
both methods is below a given error tolerance. As such, the error of the
integration is controlled and the step size is chosen adaptivly. Hence,
this method is solver of choice for non-stiff problems.

[Preditor-Corrector
methods](http://en.wikipedia.org/wiki/Predictor%E2%80%93corrector_method)
are somewhat in between explicit and implicit methods. These methods do
one timestep with an explicit method and repeat the timestep using the
states of the initial guess as an corrector of the first timestep. CMF
implements the simple PECE method after Heun as
[HeunIntegrator](@ref cmf::math::HeunIntegrator). However, this solver
has no adaptive timestepping scheme, like the RKF method, hence the
evaluation of the integration error using RKF or an implicit method
shoud be done.

## Implicit solvers 

The equation for a 1st order, implicit solver (implicit Euler's method)
is only marginally different to the explicit formulation:


@f[
\vec{u}(t_{i+1}) = \vec{u}(t_{i}) + h \vec{f}(\vec{u}(t_{i+1}),t_{i+1})
@f]

Clark and Kavetski (2010) state that even for lumped models,
implicit schemes should be used.

However, the equation above cannot be directly solved, since
@f$\vec{u}(t_{i+1})@f$ is on both sides of the equation sign. To solve
the equation, one needs an iterative approach which raises the
computational demand. For stiff systems, the higher demand for
a single step is compensated by the larger step size.

In principle, the iteration to yield @f$\vec{u}(t_{i+1})@f$ can be
done using a Newton iteration or fixpoint iteration.

### Fixpoint iteration

The [fixpoint iteration](http://en.wikipedia.org/wiki/Fixpoint) is
simpler to implement and needs only the right hand side function
@f$f(\vec u,t)@f$, however usually more iteration steps are needed. In
that case you start with an explicit method, and repeat the left hand
side by exchanging @f$\vec u@f$ in @f$f(\vec u,t)@f$ with the newly
generated state, until the @f$\vec u@f$ is not changing anymore (comes
to a fix point). However, if the first guess is not "near enough" to the
fixpoint, the iteration may not converge. In that case the process
starts new with a smaller timestep. The implicit Euler method with
fixpoint iteration is implemented in cmf as
[ImplicitEuler](@ref cmf::math::ImplicitEuler), and a 2nd order Gears
method with fixpoint iteration is implemented as
[BDF2](@ref cmf::math::BDF2). Both methods are a good choice for
systems not using the Richards equation with solute transport.

### Newton iteration

The other option is to iterate using the [Newton
iteration](http://en.wikipedia.org/wiki/Newton%27s_method). To use it,
the Jacobian matrix @f$\frac{d\vec f}{d\vec u}@f$ of the system needs
to be known. Since it is quite difficult to calculate the exact Jacobian
for a flexible system like cmf, only an numerical approximation is
feasible to use. A second problem is, that the size of the Jacobian is
@f$N^2@f$, where @f$N@f$ is the number of states. On the other hand,
only few places of the Jacobian may have values different from zero (the
connections). Advanced solvers can make use of the sparse structure to
reduce the computational demand. One general purpose solver for ODE's
using a simplified Newton iteration and an advanced error control
mechanism is the CVODE solver by Hindmarsh et al. (2005).
It uses an up to order 5 Gears method, but checks the stability
constraint to reduce the order in very stiff cases. The freely available
CVODE code is integrated into cmf. The solver is called and managed by
the derivations of the [CVodeBase](@ref cmf::math::CVodeBase) class.

Due to the high stiffness of models using the Richards equation, the
CVODE solver is the choice for all models with physical based transport
with variable saturation.

## References 

- Clark, M. P. and Kavetski, D.: Ancient numerical
  daemons of conceptual hydrological modeling: 1. Fidelity and efficiency
  of time stepping schemes, Water Resour. Res., 46(10), W10510, 2010. 2.
- Hindmarsh, A. C., Brown, P. N., Grant, K. E., Lee, S.
  L., Serban, R., Shumaker, D. E. and Woodward, C. S.: SUNDIALS: Suite of
  nonlinear and differential/algebraic equation solvers, ACM Trans. Math.
  Softw., 31(3), 363–396, <doi:10.1145/1089014.1089020>, 2005.

author: konrad, version: 6 Tue Dec 15 13:17:28 2015
