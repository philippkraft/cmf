@page cmfTutRetentioncurve

@tableofcontents

 ,
[next...](@ref cmfTutVolumeHeight)

# Hydraulic head and stored water volume

The state of a water storage in cmf is always the volume of the actually
stored water, and the change rate of the state is the sum of the fluxes
in an out of the water storage (finite volume, see FiniteVolumeMethod).
However, physical descriptions of water flux is often based on the
gradient of the [hydraulic head](WikiPedia:Hydraulic_head) in space,
like [Darcian](WikiPedia:Darcy's_law) or
[Richards](WikiPedia:Richards_equation) approaches, and the 2nd St.
Venant equation for surface water. To make such approaces available in
cmf, a functional relation between volume and hydraulic head (or
potential) needs to be available. In cmf, the term potential (symbol
@f$\Psi@f$) is used.

For unsaturated porous media, the relation between water content and
head is given by a retention curve, for saturated porous media this
relation is often modelled by a compressability term (eg. in MODFLOW).
For surface water, the relation of volume to head is given by the height
of the watertable above a model wide reference height, often sea level.

The potential or hydraulic head for porous media is commonly defined as:

~~~~~~~~~~~~~ 

#!td style="vertical-align: top"

{{{#!latex
@f[
[\Psi_{tot}|= z + \Psi_M(\theta) + p + \Psi_O
@f]
~~~~~~~~~~~~~

\] }}}

~~~~~~~~~~~~~ 

#!td

where:
- {{{@f$\Psi_{tot}@f$}}}
 is the total potential of the water storage in m water column
- {{{@f$z@f$}}} is the height above the reference height in m, the gravitaional potential
- {{{@f$\Psi_M(\theta)@f$}}} is the matrix potential, a function of the water content {{{@f$\theta)@f$}}}, also called the suction pressure in m. {{{@f$\Psi_M<0m@f$}}} for unsaturated media and {{{@f$\Psi_M=0m@f$}}} for saturated conditions
- {{{@f$p@f$}}} is the hydrostatic pressure from upper water storages in m
- {{{@f$\Psi_O@f$}}} is the osmotic potential in m
~~~~~~~~~~~~~

# Using subsurface potential in cmf

## Introduction and definitions

cmf follows the definition of water potential from above, with two
important exceptions: 1. The osmotic potential @f$\Psi_O@f$ is
ignored. 2. The hydrostatic pressure from water storages above the used
one is not implemented. Instead the compressability of water is
exagerated. In lower saturated conditions, the water storage is modelled
as oversaturated, thus gaining a matric potential above 0m and thus
accounting for the hydrostatic pressure. With these definitions,
@f$\Psi_{tot}@f$ is in cmf only a function of the position of the
water storage in space and its water content, which is again a function
of the stored water volume in m³ @f$\Psi_{tot} = z +
\Psi_M(\theta)@f$.

## Getting and setting the potential in cmf

Every flux node in cmf has a property `potential` returning the
absolut potential @f$\Psi_{tot}@f$ of that flux node. For soil layers,
one can query the gravitational potential @f$z@f$ and the matrix
potential @f$\Psi_M@f$ also. The gravitational potential for layer
`l`is defined as the height of the upper layer boundary above
reference height

~~~~~~~~~~~~~{.py}

z = l.gravitational_potential = l.position.z + l.thickness/2. = l.cell.z - l.upper_boundary
~~~~~~~~~~~~~

The matrix potential is depending on the water content by the retention
curve (see below).

~~~~~~~~~~~~~{.py}

Psi_M = l.matrix_potential
Psi_tot = z + Psi_M
Psi_tot == l.potential
~~~~~~~~~~~~~

## Saturated depth

To get an easy to interprete indicator for the vertical potential
distribution in a cell, the cell has a property `saturated_depth`. If
you query the saturated depth of a cell you get the potential of the
first saturated layer @f$\Psi_M \geq 0m@f$ relative to the cell
height. Hence

~~~~~~~~~~~~~{.py}

def getsaturateddepth(cell):
    return cell.saturated_depth
~~~~~~~~~~~~~

is equal to

~~~~~~~~~~~~~{.py}

def getsaturateddepth(cell):
    for l in cell.layers:
        if l.matrix_potential>=0:
            return cell.z - l.potential
    # No layer is saturated, return potential of last layer
    return cell.z - cell.layers[-1].potential
~~~~~~~~~~~~~

This works quite fine as an indicator of cell saturation, as long as you
do not have perched water tables. In case of a temporary existent
perched water table over a confining layer, you may have sharp jumps in
`saturated_depth`, also the potential of each layer has a smooth
behaviour.

It is also possible to set the `saturated_depth` of a cell for
simplified setting of initial conditions. All layers of the cell will be
put into vertical hydrostatic equilibrium '''for that cell''' with the
same potential. Hence

~~~~~~~~~~~~~{.py}

def setsaturateddepth(cell,value):
    cell.saturated_depth = value
~~~~~~~~~~~~~

is equal to

~~~~~~~~~~~~~{.py}

def setsaturateddepth(cell,value):
    for l in cell.layers:
        l.potential = cell.z - value
~~~~~~~~~~~~~

If you want to set a whole landscape into hydrostatic equilibrium in a
gradient based model, you need to set the `saturated_depth` for each
cell with care to their respective height above reference height.

# Retention curves

A number of parametric retention curves exists, however the
VanGenuchten-Mualem (VanGenuchten 1980) retention curve is the most
widely used. Another widely used, older retention curve is the
Brooks-Corey curve (Brooks & Corey 1964). Both curves are consist of the
pressure - water content relation and another function, based on the
same parameters to calculate the unsaturated hydraulic conductivity.
Additional retention curves can be easily implemented in cmf on request,
however they must meet the following conditions: 1. it must be possible
to calculate the matrix potential from water content, and not only the
other way round like the Durner dual porosity curve in Hydrus 1D. 2. The
curve must be extrapolated for oversaturation.

Until now the VanGenuchten-Mualem curve and the Brooks-Corey curve are
implemented in cmf. For more conceptual models the so called linear
retention curve can be used.

All curves share the definition of the water content @f$\theta@f$ and
the water filled pore space (wetness) @f$W@f$:

~~~~~~~~~~~~~ 

#!td

{{{#!latex
@f[
[\theta|= \frac V {A \Delta z}
@f]
~~~~~~~~~~~~~

\] }}}

~~~~~~~~~~~~~ 

#!td

where:
* {{{@f$V@f$}}}
 is the stored water volume of the soil layer in m³
* {{{@f$A@f$}}} is the area of the soil column in m²
* {{{@f$\Delta z@f$}}} is the thickness of the soillayer
~~~~~~~~~~~~~

|                   |
| ----------------- |
| ---------------- |

~~~~~~~~~~~~~ 

#!td

{{{#!latex
@f[
[W|= \frac{\theta - \theta_r}{\Phi - \theta_r}
@f]
~~~~~~~~~~~~~

\] }}}

~~~~~~~~~~~~~ 

#!td

where:
* {{{@f$\theta_r@f$}}}
 is the residual water content in m³/m³
* {{{@f$\Phi@f$}}} is the porosity of the soil in m³/m³
~~~~~~~~~~~~~

For the comparisons of the different retention curves, we define a
function to plot the curves. You can play around with the paramters and
see the different effects.

~~~~~~~~~~~~~{.py}

def plot_rc(retcurve,Psi_M):
    """Plots the retention curve retcurve for the matrix potential values in the array Psi_M
    and the function of K(theta)"""
    
    # Make upper plot
    subplot(2,1,1)
    # Calculate the wetness at the matrix potential
    W = retcurve.Wetness(Psi_M) 
    # plot the retention curve. The methods vgm.Wetness(matrixpotential), vgm.MatricPotential(wetness),
    # vgm.K(wetness) accept numbers as well as arrays for the values
    plot(Psi_M, W * retcurve.Porosity(0))
    # label axis
    xlabel('Matric potential [m]')     ylabel(r'water content $\theta [\frac{m^3}{m^3}]$')
    grid()
    # Make lower plot (K(W))
    subplot(2,1,2)
    semilogy(Psi_M, retcurve.K(W))
    xlabel('Matric potential [m]')     ylabel(r'$K(\theta) [\frac{m}{day}]$')
    grid()
~~~~~~~~~~~~~

## VanGenuchten-Mualem curve

Definition from the API-documentation:
[VanGenuchtenMualem](@ref cmf::upslope::VanGenuchtenMualem)

The equation of the curve and the parameters are described in the
API-documentation. In the following, you will create a
VanGenuchten-Mualem curve with the follwing paramters in cmf:
||||=Parameter =||=value =||=unit =||=Description =|| ||
@f$K_{sat}@f$|| = || 2.5 ||@f$m/day@f$ ||Saturated conductivity ||
|| @f$\alpha@f$|| = || 0.1 ||@f$1/cm@f$ ||Inverse of water entry
potential || || @f$n@f$|| = || 1.6 ||- ||Poresize distribution
parameter || || @f$\Phi@f$|| = || 0.52 ||@f$m^3/m^3@f$ ||Porosity ||

~~~~~~~~~~~~~{.py}

import cmf
from pylab import *
vgm = cmf.VanGenuchtenMualem(Ksat=2.5, alpha=0.1, n=1.6, phi=0.52)
# Make an array of matric potential values [0..-3m]  for plotting the retention curve
Psi_M = arange(0,-3,-0.01)
plot_rc(vgm,Psi_M)
~~~~~~~~~~~~~

### Oversaturation

As noted above, cmf mimics the hydrostatic pressure by allowing a slight
oversaturation for the case of hydrostatic pressure @f$W(\Psi_M) > 1.0\
\forall\ \Psi_M>0@f$. To archive that, at a wetness near to saturation,
@f$W_0@f$ the VanGenuchten curve @f$\Psi_M(W)@f$ is replaced by a
parabolic extrapolation function @f$f_{ex}(W)@f$, which has the
following properties:

  - @f$f_{ex}(W_0) = \Psi_M(W_0)@f$: the combined curve is continuous
  - @f$\frac {df_{ex}}{dW}(W_0) = \frac{d\Psi_M}{dW}(W_0)@f$: the
    combined curve is continuously differentiable
  - @f$f_{ex}(1.0) = 0.0 m@f$: The extrapolated matrix potential at
    saturation is 0m Choosing w0 does affect the degree of
    oversaturation strongly, but has different effect for different
    parameters, as the following table shows for the VanGenuchten
    curve from the example. ||= @f$w0@f$ =||= @f$f_{ex}(+0.5\ m)@f$
    =||= @f$f_{ex}(+1.0\ m)@f$ =||= @f$f_{ex}(+2.0\ m)@f$ =|| ||=
    0.99=|| 1.06|| 1.088|| 1.128|| ||= 0.995=|| 1.038|| 1.056|| 1.081||
    ||= 0.999=|| 1.013|| 1.019|| 1.028|| ||= 0.9996=|| 1.007|| 1.01||
    1.015|| ||= 0.9999=|| 1.003|| 1.004|| 1.006|| From an accurcy view
    point, the oversaturation should be as small as possible. However,
    the steeper @f$f_{ex}(W)@f$ for W\>1, the unstable is the
    numerical solution. The user needs to define, what oversaturation is
    tolerable and set W0 accordingly. If you find a 1% oversaturation at
    @f$\Psi_M=+1.0@f$ tolerable, then set @f$W_0=0.9996@f$ for this
    curve as follows:

<!-- end list -->

~~~~~~~~~~~~~{.py}

vgm.w0 = 0.9996
~~~~~~~~~~~~~

From revision 600 and above, you can optimize w0 for a target
oversaturation using the fit_w0 function.

## Brooks & Corey curve

Definition from the API-documentation:
[BrooksCoreyRetentionCurve](@ref cmf::upslope::BrooksCoreyRetentionCurve)

## Linear retention curve

Definition from the API-documentation:
[LinearRetention](@ref cmf::upslope::LinearRetention)


