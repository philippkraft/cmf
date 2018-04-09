@page CmfTutDarcianLateralFlow

 [next...](@ref CmfTutSurfaceRunoff)

# Darcian flow connections

Lateral subsurface flow in physically based models are usual based on
some variant or simplification of Darcy's law. CMF has four different
flux connections to describe the lateral subsurface flux between
variable saturated soil layers. The naming conventions of the darcian
flux connections are not chosen very well and are subject to change.

As a basis, all flux connections follow Darcy's equation:


@f[
 q = \nabla \Psi T w 
@f]

where: - @f$q@f$ is the flux between to horizontal adjacent soil
layers in @f$m^3/day@f$ - @f$\nabla \Psi@f$ is the gradient over
distance @f$d [m]@f$ - @f$T@f$ is the transmissivity between the
layers in @f$m^2/day@f$ - @f$w@f$ is the width of the shared
boundary of the layers in @f$m@f$.

The four variants of Darcy flow implemented in cmf differ in their way,
how transmissivity @f$T@f$ and the head gradient @f$\nabla \Psi@f$
of the interface of two adjacent soil layers is calculated.

The following variants are available and explained below in detail:

|| @f$T_u@f$ ||unsaturated Transmissivity || || @f$T_s@f$
||saturated Transmissivity || || @f$\nabla\Psi(\theta)@f$ ||Head based
gradient || || @f$\nabla\Psi(z)@f$ ||Topographic gradient ||

These variants lead to the four combinations of @f$T@f$ and
@f$\nabla\Psi@f$:

|| || @f$T_u@f$ || @f$T_s@f$ || || @f$\nabla\Psi(\theta)@f$ ||
[Richards_lateral](@ref cmf::upslope::connections::Richards_lateral)
|| [Darcy](@ref cmf::upslope::connections::Darcy) || ||
@f$\nabla\Psi(z)@f$ ||
[DarcyKinematic](@ref cmf::upslope::connections::DarcyKinematic) ||
[TopographicGradientDarcy](@ref cmf::upslope::connections::TopographicGradientDarcy)
||

## Transmissivity variants

### a) Unsaturated transmissivity {{{@f$T_u@f$}}}


@f[
T(\theta) = K(\theta)\ \Delta z
@f]

where: - @f$K(\theta)@f$ is the harmonic mean of the unsaturated
conductivity in @f$\frac m{day}@f$, cf. [retention
curve](@ref CmfTutRetentioncurve) - @f$\Delta z@f$ is the thickness of
the interface of the adjacent layers

This transmissivity function is used in
[Richards_lateral](@ref cmf::upslope::connections::Richards_lateral),
[DarcyKinematic](@ref cmf::upslope::connections::DarcyKinematic)

### b) Saturated transmissivity {{{@f$T_s@f$}}}


@f[
 T_{sat}(\Psi) = K_{sat}\  (\Delta z + \Psi_M)
@f]

where: - @f$K_{sat}@f$ is the saturated conductivity in @f$\frac
m{day}@f$ - @f$\Delta z + \Psi_M@f$ is the height of the water table
above the layer bottom using the matrix potential @f$\Psi_M@f$. If the
saturated depth is below the lower layer thickness, the flow depth is 0.

[Darcy](@ref cmf::upslope::connections::Darcy),
[TopographicGradientDarcy](@ref cmf::upslope::connections::TopographicGradientDarcy)

## Gradient variants

### a) Head based gradient  {{{@f$\\nabla\\Psi(\\theta)@f$}}}

The two connections types,
[Richards_lateral](@ref cmf::upslope::connections::Richards_lateral)
and [Darcy](@ref cmf::upslope::connections::Darcy) are using the real,
moisture dependent head of the connected soillayers to calculate the
gradient. This makes the more difficult to solve, yet more realistic
than option b.


@f[
 \nabla \Psi = (\Psi_1 - \Psi_2) d^{-1} 
@f]

where @f$d@f$ is the distance between the soil layers.

### b) Topographic gradient  {{{@f$\\nabla\\Psi(z)@f$}}}

To make the system simpler to solve, in this option, used by
[TopographicGradientDarcy](@ref cmf::upslope::connections::TopographicGradientDarcy)
and [DarcyKinematic](@ref cmf::upslope::connections::DarcyKinematic),
the topographic elevation of the cells is used as a proxy for the real
water pressure head. This option is much simpler to solve for the
system, however, the results are only realistic in case of water tables
roughly parallel to the surface.


@f[
 \nabla \Psi = (z_{cell,1} - z_{cell,2})\ d^{-1} 
@f]

## Connecting cell layers using a darcian lateral connection

All dedicated lateral connection provide the same interface for the
creation of a connection. Below we are going to use `cmf.Darcy` as an
example, but it works the same way with the other three connections.

~~~~~~~~~~~~~{.py}

cmf.Darcy(leftnode, rightnode, FlowWidth, distance=0)
~~~~~~~~~~~~~

- `leftnode` needs to be a
[SoilLayer](@ref cmf::upslope::SoilLayer) object - `rightnode` is
any [flux_node](@ref cmf::water::flux_node) object - `FlowWidth` is
the width of the connection in m - `distance` is the distance between
`leftnode` and `rightnode` in m. If it is left to 0 (default), the
distance is calculated from the position of the nodes.

When `rightnode` is another soillayer the flow thickness is calculated
as the common interface of the soillayers according to their depth below
ground. Otherwise the interface A is calculated as the thickness of
`leftnode` times the flow width.

However, if you have serveral cells with a defined topology (see
CmfTutCell\#Topology), one can use the topological information and
create lateral connections for all layers of all cells with a single
command:

~~~~~~~~~~~~~{.py}

cmf.connect_cells_with_flux(cells,cmf.Darcy, start_at_layer=0)
~~~~~~~~~~~~~

Here, cells is a collection of cells, eg. the whole project or a list of
cells if you want to install the connection only for a part of your
cells. The `start_at_layer` argument is optional. If it is set to a
number `n`, the connection is not installed for the `n-1` upper
layers of a cell.

@author philipp, version: 5 Tue Jan 21 10:37:02 2014
