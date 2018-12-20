@page cmfTutET Evapotranspiration

Several models exist to calculate water losses from the soil and
surfaces to the atmosphere, and naming of processes and concepts 
is not completely fixed. Some definitions:

**Evaporation** is the vaporization from a surface with no transition
through a biologically active membrane. Evaporation is thus governed
only by the physical environment. 

**Transpiration** is vaporization from
a biological surface, steered by active opening and closing. For our
cases only the transpiration of plants is of interest. 

Many models for vaporization use the term "Evapotranspiration" which is the bulk sum of
evaporation and transpiration, since soil evaporation and plant
transpiration influence each other and are complex to differentiate.
From the water storage perspective, the main difference is, that plants
take up water from several soil layers (if the rooting zone is divided
in serveral layers), but soil evaporation takes water from the first
layer only (except for very, very dry regions), open water
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
expressed by the term **potential evapotranspiration** @f$ET_{pot}@f$ the water that would be vaporized if enough water is
available. @f$ET_{pot}@f$ is determined by the surface structure (land
cover) and meteorological conditions. If the surface structure is
assumed to be a reference surface, (short, mown grass with defined
properties), we get the **reference evapotranspiration**
@f$ET_{ref}@f$, which is depending on meteorological conditions only.

The **actual evapotranspiration** @f$ET_{act}@f$ which is limited by
water and energy, depends also on the soil moisture in the rooting zone and
is usually calculated from @f$ET_{pot}@f$ using a limiting factor.
While you can use cmf to calculate @f$ET_{act}@f$ from vegetation
cover, meteorological conditions and soil moisture directly, using
different models, one can also precalculate an @f$ET_{pot}@f$
timeseries and cmf calculates @f$ET_{act}@f$ from these values. 

In the following, we use a simple lumped conceptual model with
one cell, and a single soil layer, without any runoff. 
This model can be extended to create direct runoff or runoff via groundwater storages,
as shown in the [next chapter](@ref conceptual) and [connected to a rain station](@ref cmfTutMeteostation).
This tutorial is also mostly applicable to physical models with multiple layers in the 
root zone by making [assumptions about the root distribution in the soil](@ref cmfTutETphys).

~~~~~~~~~~~~~~~~~~{.py}
from __future__ import division, print_function
import cmf

# Create project with 1 cell and 1 water storage of 1000mm capacity
p = cmf.project()   
cell = p.NewCell(0, 0, 0, 1000)
layer = cell.add_layer(1.0)
# Put 200mm water into the soil, quite dry, good to show water stress 
layer.volume = 200.0
~~~~~~~~~~~~~~~~~~

## Using precalculated @f$ET_{pot}@f$ timeseries

As a first step, you need to have your values stored in a
[timeseries](@ref cmfTutSpaceTime), for the following code named ET. The
timeseries needs to have the unit of mm/day. To get @f$ET_{act}@f$,
you will connect every soil layer in the rooting zone with the
`transpiration` boundary condition of the cell with the
[timeseriesETpot](@ref cmf::upslope::ET::timeseriesETpot) connection.

~~~~~~~~~~~~~{.py}
ETpot = cmf.timeseries.from_scalar(5) # constant ETpot of 20 mm/day (I know that this is too much)

# connect each layer with the transpiration boundary condition using ETpot
et_pot_connection = cmf.timeseriesETpot(layer, cell.transpiration, ETpot)
~~~~~~~~~~~~~

## Calculating @f$ET_{pot}@f$ from meteorological data

Cells provide always weather information, by default they have an eternal spring day in central Europe. 
To change the weather of a cell dynamically you can use [meteorological stations](@ref cmfTutMeteostation),
to change the weather of a cell to a static nice summer day one can do (Rs is global radiation in MJ/(m² day)):

~~~~~~~~~~~~~~~{.py}
summer = cmf.Weather(Tmin=16, Tmax=28, rH=50, wind=1.0, sunshine=0.9, daylength=16, Rs=26)
cell.set_weather(summer)
~~~~~~~~~~~~~~~

Or, if you prefer winter in Gießen:
~~~~~~~~~~~~~~~{.py}
winter = cmf.Weather(Tmin=1, Tmax=2, rH=80, wind=6, sunshine=0.0, daylength=8, Rs=1.8)
cell.set_weather(winter)
~~~~~~~~~~~~~~~

Which of the weather information is actually used to calculate @f$ET_{pot}@f$ depends on the
method. Currently the following methods to calculate @f$ET_{pot}@f$ from the weather are available:

- [cmf.HargreavesET](@ref cmf::upslope::ET::HargreaveET), using @f$T_{min}, T_{max}@f$ and the day of year 
- [cmf.TurcET](@ref cmf::upslope::ET::TurcET), using @f$T, rH@f$ and global incoming radiation
- [cmf.PriestleyTaylorET](@ref cmf::upslope::ET::PriestleyTaylorET), using net radiation and Temperature 
- [cmf.PenmanMonteithET](@ref cmf::upslope::ET::PenmanMonteithET) with a high demand of atmospheric variables (see below)

Using these evapotranspiration models works all the same, therefore it is only shown here for the Turc ET Method:

~~~~~~~~~~~{.py}
et_pot_turc = cmf.TurcET(layer, cell.transpiration)
print(layer.flux_to(cell.transpiration, datetime.datetime(2018, 1, 1)))
~~~~~~~~~~~

An **alternative** way to install connections between all existing soil layers and `cell.transpiration`: 
~~~~~~~~~~~{.py}
cell.install_connection(cmf.TurcET)
~~~~~~~~~~~

# Getting @f$ET_{act}@f$ from @f$ET_{pot}@f$

Low soil moisture limits actual transpiration, as it gets more and more
difficult for a plant to extract soil moisture against a lower matrix
potential.

## Physical description of water stress

For physical models, where a soil layer is given a realistic
retention curve, the stress can be directly described as a function of
the matrix potential. This is possible in cmf, using a piecewise linear
function of the matrix potential, known as the Feddes model.
It is not possible for the model in this tutorial, since we are lacking a retention curve. The
[cmf.SuctionStress](@ref cmf::upslope::ET::SuctionStress) model is used in [this later tutorial](@ref cmfTutETphys),
which covers physical models and transpiration.

## Conceptual description of water stress

In conceptual cmf models, soil layers are just buckets of a certain size,
in this tutorial a bucket with a capacity of 1000 mm. While the matrix
potential of a layer is undefined, the water content has a defintion,
the actual water volume V divided by the layer capacity C:

@f[
\theta = \frac{V_{layer}}{C_{layer}}
@f]

An often used approach to describe water stress as a function of water
content is to define a permanent wilting point @f$\theta_{wp}@f$ and
a water content above no stress occurs @f$\theta_{d}@f$. The actual
evapotranspiration from a soil is then described as:

@f[
ET_{act} = ET_{pot} \cdot
\begin{cases}
    1 &
        \mbox{if } \theta > \theta_d \\
    \frac{\theta - \theta_{wp}}{\theta_d - \theta_{wp}} &
        \mbox{if } \theta_{wp} < \theta < \theta_d \\
    0 & \mbox{if } \theta < \theta_{wp}
\end{cases}
@f]

This is obviously a crude linearization of the function using the matrix
potential, hence the definition of @f$\theta_d@f$ is not very precise.
An often used approach in literature is to set it to the mean of wilting
point and field capacity. This approach is called
[cmf.ContentStress](@ref cmf::upslope::ET::ContentStress)

This stress model can be applied to a cell by:
~~~~~~~~~~~{.py}
cell.set_uptakestress(cmf.ContentStress(theta_d=0.3, theta_w=0.1))
~~~~~~~~~~~

However, to apply this function, the model needs to have a coherent
meaning for the "capacity" of the root zone, which is for a simple
Nash-Box model or a TOPMODEL like approach not necessarily the case.
Instead of forcing the user to define a capacity only to calculate the
stress function, water stress can also be calculated by the water volume
stored in the layer using the
[cmf.VolumeStress](@ref cmf::upslope::ET::VolumeStress), which is the
recommended stress function for simple, conceptual models.

# In code, please

![](@ref CmfTutET.png)

To put all of this long tutorial together in few lines of code, the task
is to write a model that calculates the remaining volume in a lysimeter
with a reference surface (short grass), with the following assumptions:

1. Every day is a nice summer day, no rain
  (eg. Canaries)
2. The lysimeter is water tight
3. Evapotranspiration is calculated after Turc
4. Initial water volume is 400 mm
5. Evapotranspiration decreases if less then 300 mm are left in the lysimeter
6. Evapotranspiration stops if less then 100 mm are left

~~~~~~~~~~~~~~~~~~{.py}
import cmf
from datetime import datetime, timedelta
from pylab import plot, twinx, ylabel, xlabel

# Create project with 1 cell and 1 water storage of 1000mm capacity
p = cmf.project()
cell = p.NewCell(0, 0, 0, 1000)
layer = cell.add_layer(1.0)

# Set summertime, when the living is easy... (1)
summer = cmf.Weather(Tmin=19, Tmax=28, rH=50, wind=4.0,
                     sunshine=0.9, daylength=14, Rs=26)
cell.set_weather(summer)

# Initial condition (4)
layer.volume = 400.0

# ET-Method (3)
et_pot_turc = cmf.TurcET(layer, cell.transpiration)

# Stress conditions (5, 6)
stress = cmf.VolumeStress(300, 100)
cell.set_uptakestress(stress)

# A solver, any is fine, really
solver = cmf.HeunIntegrator(p)
solver.t = datetime(2018, 5, 1)
et_act = cmf.timeseries(solver.t, cmf.day)
volume = cmf.timeseries(solver.t, cmf.day)
while solver.t < datetime(2018, 10, 1):
    et_act.add(cell.transpiration(solver.t))
    volume.add(layer.volume)
    solver(cmf.day)

# And a plot
plot(et_act, c='g')
ylabel(r'$ET_{act} \left[\frac{mm}{day}\right]$')
twinx()
plot(volume, c='b')
ylabel('Volume in mm')
~~~~~~~~~~~~~~~~~~

