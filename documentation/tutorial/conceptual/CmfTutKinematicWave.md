@page cmfTutKinematicWave Is hydrology kinematic?

... is the title of a paper by [Singh (2002)](https://doi.org/10.1002/hyp.306),
where he lists numerous examples from all scales of hydrology where a
kinematic wave function has been found to be a valid descriptor for
hydrological processes. Anyway, numerous more or less conceptional
models in hydrology use kinematic wave like formulations to calculate
fluxes. While some authors are happy to call every 1st order flux
calculation using some exponent a kinematic wave, other authors like to
see the term reserved for more "wave like" phenomens in hydrology.

Hence a number of different names exist for the same concept:

- Kinematic wave 
- Power law equation (eg. DHSVM, SUPERFLEX) 
- curve shape parameter (eg. MACRO, HBVlight)

## A simplified equation for power law equations

A generalized form for some approaches is:


@f[
q(V) = Q_0 \cdot \theta^\beta\ (1)
@f]

where:

- @f$\theta@f$ is a dimensonless variable representing a water
  content related state of a storage
- @f$\beta@f$ is a curve shape parameter 
- @f$Q_0@f$ is the flux in m³/day at unity state @f$\theta=1@f$

For cmf, using the finite volume approach, we need a consistent function
to relate the stored water volume @f$V@f$ in a water storage to the
variable @f$\theta@f$. So we define:


@f[
\theta = \frac{V - V_{res}}{V_0}\ (2)
@f]

where:

- @f$V@f$ is the water volume stored in a storage in @f$m^3@f$ 
- @f$V_{res}@f$ is an immobile water volume in the storage, that does
  not take place in this connection. It may be ommitted and set to @f$ 0
  m^3@f$. 
- @f$V_0@f$ is a reference volume, where, for any @f$\beta@f$ the runoff 
  from the water storage is @f$Q_0@f$.

The famous linear storage equation can be seen as a special case of this
approach where @f$\beta=1@f$. In that case, we can rearrange the formula
above to:

@f[
q(V) = \frac{Q_0}{V_0} \cdot (V-V_{res})\ (3)
@f]

Here the quotient @f$\frac{Q_0}{V_0}@f$ becomes the inverse of the
storage residence time @f$t_r^{-1}@f$, which simplifies (3) to  

@f[
q(V) = \frac 1 {t_r [days]} \cdot (V-V_{res})\ (4)
@f]

for the linear case

# Implementation in cmf

Due to the different meanings of parameters between the linear and
non-linear case, cmf provides now 2 distinct connections for the linear
and the non-linear case. Earlier versions (<1.0.4) provide only one,
now deprecated connection type called
[kinematic_wave](@ref cmf::water::kinematic_wave). In a transition
period, this connector is still used in some tutorials, but for clarity
the new connections should be used.

## LinearStorageConnection

~~~~~~~~~~~~~{.py}

ls = cmf.LinearStorageConnection(source, target, residencetime=3.2, residual=0)
~~~~~~~~~~~~~

The linear storage connection
[LinearStorageConnection](@ref cmf::water::LinearStorageConnection) implements (4) 
and takes the residence time in days and a residual water content in m³ as
parameters. While the residence time must be given, the residual water
content can be omitted and set to 0. Depending on system size and
structure, residence times between minutes for surface water storages or
fast drainages up to hundreds of years for large aquifers are suitable
values.

## PowerLawConnection

~~~~~~~~~~~~~{.py}

plc = cmf.PowerLawConnection(source, target, Q0=3.0, V0=5.3, beta=2.0, residual=0)
~~~~~~~~~~~~~

The power law connection
([PowerLawConnection](@ref cmf::water::PowerLawConnection)) needs the
parameters Q0, V0 and beta as given in the generalized form of the power
law equation. The reference flow @f$Q_0 [\frac{m^3}{days}]@f$ occurs
when the source water storage contains the reference volume @f$V_0@f$
as mobile water. The curve shape parameter @f$\beta@f$ bends the curve
for mobile water volumes between 0 and @f$V0@f$ down, when
@f$\beta>1@f$. The curve is bend up for @f$\beta<1@f$. If @f$V -
V_{res}>V_0@f$, the effect is directly turned around, as shown in the
figure.

The residual water content @f$V_{res} [m^3]@f$ shifts the
storage/runoff response to the right.

A residual water content makes sense eg. for percolation in simplified
models where percolation starts only above field capacity, where the
residual water content equals field capacity of the top soil, or it can
be used as the volume behind a dam. To find a justification for
@f$\beta@f$ is more challenging: @f$\beta>1@f$ describes systems,
where flow is over linear slow for low storages and gets much faster for
a large storage. Unsaturated flow can be modelled that way, if one takes
an analogy with the Brooks-Corey retention curve, betas from 4 to 15 are
reasonable. The standard beta for the "real" kinematic wave from the
Manning-Strickler equation for sheet flow in @f$\frac 5 3@f$. If you
find a real world usage for @f$\beta<1@f$, please inform the authors
of cmf. However, if @f$\beta<0.3@f$ it is reported, that numerical
instabilities will arise.

![](PowerLawConnection.png)


