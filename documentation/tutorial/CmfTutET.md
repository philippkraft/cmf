@page cmfTutET

@tableofcontents

 
[next...](@ref cmfTutIntercept)

# Evapotranspiration (incomplete)

Several models exist to calculate water losses from the soil and
surfaces to the atmosphere.

**Evaporation** is the vaporization from a surface with no transition
through a biologically active membrane. Evaporation is thus governed
only by the physical environment. **Transpiration** is vaporization from
a biological surface, steered by active opening and closing. For our
cases only the transpiration of plants is of interest. Many models for
vaporization use the term "Evapotranspiration" which is the bulk sum of
evaporation and transpiration, since soil evaporation and plant
transpiration influence each other and are complex to differentiate.
From the water storage perspective, the main difference is, that plants
take up water from several soil layers (if the rooting zone is divided
in serveral layers), but soil evaporation takes water from the first
layer only (ok except for very, very dry regions), open water
evaporation from [OpenWaterStorage](@ref cmf::river::OpenWaterStorage)
like surface water, rivers and lakes. Water that is intercepted by
vegetation (see [next](@ref cmfTutIntercept) tutorial) either percolates
to the surface water or evaporates from the leaf surface. Since
transpiration is the main part of bulk evapotranspiration in vegetated
soils, models that deal only with bulk evapotranspiration handle the
bulk flux as we would have transpiration only. However, by including a
canopy water storage, interception of rain on the plant leaves can be
considered.

# Potential bulk evapotranspiration

Evapotranspiration is subject to many processes depending on vast amount
of environmental conditions. To get a structure, one can differ between
an energy limit and a water limitation. The energy limitation is
expressed by the term '''potential evapotranspiration'''
@f$ET_{pot}@f$ the water that would be vaporized if enough water is
available. @f$ET_{pot}@f$ is determined by the surface structure (land
cover) and meteorological conditions. If the surface structure is
assumed to be a reference surface, (short, mown grass with defined
properties), we get the '''reference evapotranspiration'''
@f$ET_{ref}@f$, which is depending on meteorological conditions only.
The '''actual evapotranspiration''' @f$ET_{act}@f$ which is limited by
water and energy, depends also the soil moisture in the rooting zone and
is usually calculated from @f$ET_{pot}@f$ using a limiting factor.
While you can use cmf to calculate @f$ET_{act}@f$ from vegetation
cover, meteorological conditions and soil moisture directly, using
different models, one can also precalculate an @f$ET_{pot}@f$
timeseries and cmf calculates @f$ET_{act}@f$ from these values. In
order to test different models, you need to have the code from
\[wiki:CmfTut1d\] at hand, since we will extend the 1D model by ET
models.

## Getting {{{@f$ET_{act}@f$}}} from {{{@f$ET_{pot}@f$}}} with cmf

In order to calculate the water loss to the atmosphere from a given or
calculated @f$ET_{pot} [\frac{mm}{day}]@f$ value, cmf needs to
partition the water demand between the layers. Since bulk ET is handled
as transpiration, the partitioning is done using the root content of a
layer:


@f[
[q_{T_{pot}}\left[\tfrac{m^3}{day}\right]=f_r|ET_{pot}\left[\tfrac{mm}{day}\right]]A_{cell}[[m^2]|\tfrac{1 m}{1000 mm}
@f]

\]

where: - @f$f_r=\frac{R_{layer}}{\sum_{i=0}^{layers}{R_i}}@f$ the root
mass in this layer per total root mass at this cell. - @f$A_{cell}@f$
is the area of the cell

To limit water uptake from the soil layer according to its water
content, cmf uses a Feddes like approach: The flux is 0, when the matrix
potential @f$\Psi_M@f$ is below wilting point. If the matrix potential
is below -5m, the limitation is linearly scaled towards the wilting
point and above -5m there is no water limitation:


@f[
[q_{T_{act}}|= q_{T_{pot}}\begin{cases} 1 & \Psi_M>-5m \\ 1+\tfrac{\Psi_M + 5}{155} & \Psi_M>-160m \\ 0 & \Psi_M<-160 m\end{cases}
@f]

\]

### Setting the root content {{{@f$f_r@f$}}}

In principle, there are two ways to define the root content, set the
root content fraction for each layer or define the root depth for the
cell

#### a) set the root content directly

Soil layers have the property `rootfraction` that can be set. It is
'''your''' responsibility to ensure that the sum of all rootfractions is
1.

~~~~~~~~~~~~~{.py}

for l in cell.layers[:5]:
    l.rootfraction = 0.2
~~~~~~~~~~~~~

If you have never set the rootfraction method b) is used.

#### b) set the root depth for the cell

If the `rootfraction` of a layer has never been set, the root fraction
of the layers are calculated from the `RootDepth` and
`fraction_at_rootdepth` properties of the cell's vegetation object.
`RootDepth` is the depth of the rooting zone of the cell in m. If
`fraction_at_rootdepth` is 1, a uniform distribution of roots until
`RootDepth` is used. If `fraction_at_rootdepth` is between 0..1,
then an exponential decline of the root content is used.
`1-fraction_at_rootdepth` of the root mass is then below
`RootDepth`.

In this example we will get the same result as in a):

~~~~~~~~~~~~~{.py}

cell.vegetation.RootDepth = 0.5
~~~~~~~~~~~~~

Here we will have 80% of the root content in the upper 40cm, and the
other 20% below using an exponential decline of the root content:

~~~~~~~~~~~~~{.py}

cell.vegetation.RootDepth = 0.4
cell.vegetation.fraction_at_rootdepth = 0.8
~~~~~~~~~~~~~

If you do nothing, the `RootDepth` default value of 0.25 is used.

## Using precalculated {{{@f$ET_{pot}@f$}}} timeseries

![](@ref CmfTut1d-ET1.png)

As a first step, you need to have your values stored in a
[timeseries](@ref cmfTutSpaceTime), for the following code named ET. The
timeseries needs to have the unit of mm/day. To get @f$ET_{act}@f$,
you will connect every soil layer in the rooting zone with the
`transpiration` boundary condition of the cell with the
[timeseriesETpot](@ref cmf::upslope::ET::timeseriesETpot) connection.

The following code snippet is an extensions the setup code from
[1D model](@ref cmfTut1d) without the groundwater boundary condition. Think
about the position of the snippet in the code. For simplicity, we are
using a constant ETpot value. And to avoid changing the run time
duration and see nice effects anyway, the constant ETpot is set to the
absurd high value of 20 mm/day. The initial condition
[saturated depth](@ref cmfTutRetentioncurve) is also changed to 4m.

~~~~~~~~~~~~~{.py}

cell.saturated_depth = 4.
ETpot = cmf.timeseries.from_scalar(20.0) # constant ETpot of 20 mm/day (I know that this is too much)

# Set root content
cell.vegetation.RootDepth = .4
cell.vegetation.fraction_at_rootdepth = 0.8
for layer in cell.layers:
    # connect each layer with the transpiration boundary condition using ETpot
    cmf.timeseriesETpot(layer, cell.transpiration, ETpot)
~~~~~~~~~~~~~

The potential plot is not so nice, hence let us rather look at the pF
values in the layers. The pF value are calculated from the potential by
`pF = log10(-100\*array(potential))`. With this change, the result is
shown on the right side.

## Calculating {{{@f$ET_{pot}@f$}}} from data

Up to now, to different methods to calculate @f$ET_{pot}@f$ are
available in cmf [Hargreaves](@ref cmf::upslope::ET::HargreaveET) and
[PenmanMonteithET](@ref cmf::upslope::ET::PenmanMonteithET).

### Penman-Monteith (FAO) (inclomplete)

In order to calculate @f$ET_{pot}@f$ from the actual
[Weather](@ref cmf::atmosphere::Weather) and
[Vegetation](@ref cmf::upslope::vegetation::Vegetation)
according to the FAO Penman-Monteith method
([Allen, 1998](http://www.fao.org/docrep/x0490e/x0490e00.htm#Contents)),
use [PenmanMonteithET](@ref cmf::upslope::ET::PenmanMonteithET). It is
installed by:

~~~~~~~~~~~~~{.py}

cell.install_connection(cmf.PenmanMonteithET)
~~~~~~~~~~~~~

### Hargreaves

Implements the ET method by Hargeaves in the variation by [Samani
(2000)](http://www.zohrabsamani.com/research_material/files/Hargreaves-samani.pdf).
This method calculates only @f$ET_{ref}@f$ and thus ignores the cell's
[Vegetation](@ref cmf::upslope::vegetation::Vegetation) object.

# Separating evaporation from transpiration

The separation of plant transpiration and soil evaporation is quite
complex, since a vegetation cover does intercept the incoming radiation
and changes, by transpiration the vapor pressure deficit inside the
canopy. The only method, that separates soil evaporation from plant
transpiration implemented in cmf so far, is the
[ShuttleworthWallace](@ref cmf::upslope::ET::ShuttleworthWallace). It
implements the evaporation method given by [Shuttleworth
(1985)](http://tlaxiaco.sdsu.edu/shuttleworthwallace.pdf) as used in
[BROOK90 (Federer 1995)](http://www.ecoshift.net/brook/brook90.htm). To
install all necessary connections, use:

~~~~~~~~~~~~~{.py}

cell.install_connection(cmf.ShuttleWorthWallace)
~~~~~~~~~~~~~

If you want to use canopy evaporation and snow sublimation, make sure
you have created the appropriate storages from the
[next](@ref cmfTutIntercept) tutorial before installation.

[ShuttleworthWallace](@ref cmf::upslope::ET::ShuttleworthWallace) is
quite parameter demanding - all meteorological parameters available in
the [Weather](@ref cmf::atmosphere::Weather) object are used as
well as all parameters of the
[Vegetation](@ref cmf::upslope::vegetation::Vegetation) object .

@author philipp, version: 16 Thu Nov 9 15:40:49 2017
