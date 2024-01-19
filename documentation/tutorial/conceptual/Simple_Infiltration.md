@page simple_infiltration Infiltration in a conceptual model

As lumped models are a quite abstract realisation of a catchment, often
a simplified kinematic wave approach is used to connect storages.
Kinematic waves are sufficient for many different flux connections, but
can lead to problems when trying to model infiltration. One way to model
the infiltration in CMF is the 
[ConceptualInfiltration](@ref cmf::upslope::connections::ConceptualInfiltration)
approach.

## Hydrological concept

a) Light rainfall on a dry soil: Incoming surface fluxes, like rain or
snow melt, infiltrate immediately and completely when the soil is dry
and the flux intensity is below infiltration capacity

b) Saturation excess: when the upper soil approaches saturation, only a
portion of the incoming fluxes infiltrate, the surplus remains in the
surfacewater storage and might be routed somewhere else. This accounts
for large cells, as in lumped or semidistributed layers, where
saturation does not need to be reached everywhere to result in surface
runoff.

c) Infiltration excess: when the incoming flux gets higher than the
saturated conductivity of the top soil layer, only the saturated
conductivity in m/day infiltrates.

## Mathematical considerations

For the approach we call the sum of incoming fluxes on the surface, like
rain, snow melt, irrigation and dew @f$\sum I@f$ in @f$m^3/day@f$,
and the effective infiltration rate @f$Q_{inf}@f$ also in
@f$m^3/day@f$. The infiltration capacity of the soil is given as
saturated conductivity @f$K_{sat}@f$ in @f$m/day@f$. The
infiltration rate is therefore limited by @f$K_{sat} A@f$, where
@f$A@f$ is the area of the cell.

To express the approach as an equation we get:


@f[
Q_{inf} = \left(1-e_{sat}\right)  Q_{inf,pot}
@f]

where:

- @f$e_{sat}@f$ Saturation excess, ranging from 0 (nowhere saturated
  soil layer) to 1 (fully saturated). 
- @f$Q_{inf,pot}= \min(\sum I, K_{sat} A)@f$ is the potential infiltration, 
  given by the incoming fluxes limited by the saturated conductivity.

For light enough rain (@f$\sum I < K_{sat} A@f$) on a dry soil,
(@f$e_{sat}=0@f$), the infiltration equals the incoming fluxes:


@f[
Q_{inf} = \sum I
@f]

### Infiltration excess

For very heavy rain or irrigation on a not well drained, but dry soil,
eg. compacted, bare, sandy loam, we get:


@f[
\sum I > K_{sat} A \rightarrow Q_{inf,pot} = K_{sat} A \rightarrow Q_{inf} = K_{sat}A
@f]

but only for the time of the event.

### Saturation excess

A naive formulation for the saturation excess is a binary switch. The
receiving top soil is either full, and all the potential infiltration
@f$Q_{inf,pot}@f$ stays in the surface water and is eventually routed
elsewhere, or, the top soil has space and all of @f$Q_{inf,pot}@f$
goes into the soil. As an equation:


@f[
e_{sat} = 1 ,\ if\ W>=1,\ else\ 0
@f]

Where @f$W@f$ is the water filled porespace, or wetness. In conceptual
models, this is expressed as stored volume per capacity @f$V/C@f$.

This naive approach assumes a homogeneous state of saturated /
unsaturated for the whole study region. This does not hold even for the
patch scale with a micro relief and is certainly false for lumped
models, where the topsoil layer represents the topsoil of a study area
of thousands of km² and hundreds of m in height difference. To take
hetereogeneity inside of the cell into account, we define
@f$e_{sat}@f$ as a continuous function of the top soil wetness
@f$W@f$. A sigmoidal function is both continuous and expresses a slow
raise of surface overflow when the soil gets wetter and approaches a no
infiltration limit for high soil wetness.

To gain an approach, that scales from patch to continental basins, the
reaction is scaled by the parameter @f$W_{1/2}@f$ that contains the
wetness, at which half of the incoming water infiltrates, and half of
the water stays back in the surface water. For the simple infiltration
connection in cmf, we have chosen the Boltzmann function:


@f[
e_{sat}(W_{soil}, W_{1/2}) = -\frac{1}{1+e^{-1/5 \omega}} \\ \omega = (W - W_{1/2})\cdot (1 - W_{1/2})
@f]

For small scales with a sharp surface runoff response, one might choose
@f$W_{1/2} > 0.9@f$ and for large and/or steep catchments lower
values, down to @f$W_{1/2}\approx 0.5@f$. The response is shown in the
figure below.

![](CmfTutConceptualInfiltration.png)

The left figure shows the relative infiltration flux @f$\frac
{Q_{inf}}{\sum I}@f$ in terms of the wetness @f$V/C@f$ for
@f$W_{1/2}=0.95@f$ (blue) and @f$W_{1/2}=0.75@f$(orange). The right
figure shows the development of the infiltration @f$Q_{inf}@f$ (solid
line) and the saturation excess @f$\sum I - Q_{inf}@f$ (dashed line)
over time with a constant inflow of the topsoil capacity in one day day
(@f$\sum I = C/day @f$) starting with a complete dry soil
(@f$W=0@f$). The lower right figure shows the development of the soil
water volume (solid line) and the surface water volume (dashed line) if
no water is lost.

## Implementation in cmf

To show the implementation, we are using the code of the right figure
above, with @f$W_{1/2}=0.9@f$

### Setup up a cell

~~~~~~~~~~{.py}

from __future__ import print_function, division
import cmf

p = cmf.project()
c = p.NewCell(0,0,0,1000)

c.surfacewater_as_storage()
l=c.add_layer(1)
~~~~~~~~~~


### Connect and adjust

**Note:** for simple infiltration the receiving node comes first, sorry.

~~~~~~~~~~{.py}

cmf.ConceptualInfiltration(c.layers[0], c.surfacewater, W0=0.9)

# adjust the saturated conductivity in m/day.
l.soil.Ksat = 1.0 # 1000 mm/day infiltration capacity - quite a lot, but not extreme

# Set the constant rainfall to C[m3]/1 day
c.set_rainfall(l.get_capacity())
~~~~~~~~~~


### Integrate

~~~~~~~~~~{.py}

solver = cmf.RKFIntegrator(p, 1e-9)

I = c.get_rainfall(t)

q_inf = [c.surfacewater.flux_to(l, t) / I]
inf_ex = [c.surfacewater.waterbalance(t) / I]

for t in solver.run(solver.t, solver.t + cmf.day * 2, cmf.h):
    q_inf.append(c.surfacewater.flux_to(l, t) / I)
    inf_ex.append(c.surfacewater.waterbalance(t) / I)
~~~~~~~~~~


### Plot

~~~~~~~~~~{.py}

from pylab import plot
plot(q_inf, '-')
plot(inf_ex, '--')
~~~~~~~~~~



