@page CmfTutMeteostation

[index](@ref CmfTutStart), [back](@ref CmfTutTestData), [space and time in
cmf](@ref CmfTutSpaceTime)

# Using measurement stations for atmospheric conditions

## The station concept in cmf

Meteorological data is usually measured at stations, and can be
represented as such in cmf. Cells have the method `get_weather(time)`
and `get_rainfall(time)` to get the current meteorological
observations. If you are doing nothing, this function returns a default
[Weather](@ref cmf::atmosphere::Weather). However, if the cell is
bound to one or several measurement station
([MeteoStation](@ref cmf::atmosphere::MeteoStation) or
[RainfallStation](@ref cmf::atmosphere::RainfallStation), the
functions return the condition of the bound station or an interpolated
value. To simplify this connection, the stations take a position in
project coordinates to find the distance of the cell to the stations of
the project. The [timeseries](@ref cmf::math::timeseries) containing
the forcing data are filled from any data source using standard Python.

## Rainfall stations

Rainfall stations consists of a
[timeseries](@ref cmf::math::timeseries) of rainfall intensities in
mm/day. Note that the unit is '''always''' mm/day, regardless of the
timestep of your timeseries. If you have, for example, hourly measured
rainfall data in mm/h, you have to multiply the value by 24 to get the
rainfall in mm/day. Since you can calculate with timeseries, you can
simply move the unit conversion into your data loading script.

### Create a rainfall station

In the next example, we will create rainfall station using random data.
For your application you will load your data from a file, database or
web service.

``` {.py}

import cmf
import numpy as np
from datetime import datetime,timedelta

# Create a project
project = cmf.project()

# Create a timeseries, starting Jan 1st, 2010 with 1h step, with random data
raindata = cmf.timeseries.from_array(start=datetime(2010,1,1),
                                     timedelta(hours=1),
                                     numpy.random.uniform(0,30,24*365))
# Add a rainfall station to the project
rainstation = project.rainfall_stations.add(Name='Random station', 
                                            Data=raindata,
                                            Position=(0,0,0))
```

The last command is new: Every project has a list of rainfall stations,
and you add a new station with the `add` method. The method expects a
name for the station (very useful if you have more then one), the data
as a cmf timeseries and a position in space in project coordinates. You
can connect your new rainfall station with a cell `c` by:

``` {.py}

rainstation.use_for_cell(c)
```

or you can use one of the spatial distibution methods shown at the end
of this tutorial.

## Meteorological stations

[MeteoStation](@ref cmf::atmosphere::MeteoStation) objects are much
more complex than rainfall stations. The do not hold only one
timeseries, but quite a number. The data is used for the various
evaporation, evapotranspiration and transpiration connections available
in cmf. Depending on the processes you are using in your model, not
every timeseries you can populate is really used. The more physically
based atmospheric connections, like
[PenmanMonteithET](@ref cmf::upslope::ET::PenmanMonteithET) and
[ShuttleworthWallace](@ref cmf::upslope::ET::ShuttleworthWallace) need
all values that are provided by the
[MeteoStation](@ref cmf::atmosphere::MeteoStation). However, not all
values are measured at all stations. The MeteoStation object follows
therefore the [FAO
guidelines](http://www.fao.org/docrep/x0490e/x0490e00.htm) for the
Penman Monteith method, to use surrogate values, as listed below.

### Timeseries provided by a meteo object:

T:: Timeseries of Temperature @f$ T\left[[^\circ|C\right]] @f$
'''Optional''', if missing it is calculated from `@f$ T=\frac{T_{max} +
T_{min}} 2 @f$`

Tmax:: Timeseries of daily maximum Temperature `@f$ T_{max}
\left[[^\circ|C\right]] @f$` '''Required''', but it can be generated
from T, if you have a high resolution timeseries for Temperature, Tmax
can be generated as follows: `meteo.Tmax =
meteo.T.reduce_max(meteo.T.begin, cmf.day)`

Tmin:: Timeseries of daily minimum Temperature `@f$
T_{min}\left[[^\circ|C\right]] @f$` '''Required''', but it can generate
from T. If you have a high resolution timeseries for Temperature, Tmin
can be generated as follows: `meteo.Tmin =
meteo.T.reduce_min(meteo.T.begin, cmf.day)`

Tground:: Timeseries of upper soil temperature `@f$
T_{ground}\left[[^\circ|C\right]] @f$` '''Optional''', if missing `@f$
T_{ground} = T @f$`

Windspeed:: Timeseries of windspeed at instrument height (default 2m) in
@f$m/s@f$. '''Optional''', if not available the wind speed defaults to
2 m/s

rHmean:: Timeseries of relative mean Humidity `@f$ rH_{mean}
\left[\%\right] @f$` '''Optional''', not used if rHmax or Tdew is
available. Use this field, if you have high resolution relative humidity
data.

rHmin:: Timeseries of daily minimum relative Humidity `@f$ rH_{min}
\left[\%\right] @f$` '''Optional''', only used if rHmax is used

rHmax:: Timeseries of daily maximum relative Humidity `@f$ rH_{max}
\left[\%\right] @f$`'''Optional''', not used if Tdew is available

Tdew:: Timeseries of dew point temperature `@f$
T_{dew}\left[[^\circ|C\right]]@f$` '''Optional''', if neither Tdew,
rHmax or rHmean is available then Tdew = Tmin

Sunshine:: Fractional sunshine @f$\frac{n}{N}@f$. Sunshine duration
per potential sunshine duration. '''Optional''', if not present it
defaults to 0.5. If you have the total duration of sunshine, use the
`SetSunshineFraction` method of the
[MeteoStation](@ref cmf::atmosphere::MeteoStation). If you have cloud
coverage instead of total sunshine duration you may assume that the
fractional sunshine duration equals 1-cloudfraction

Rs:: Global Radiation in @f$ \frac{MJ}{m^2 day} @f$ ''' Optional''',
if not available cmf::atmosphere::global_radiation is used to calculate
radiation from lat, lon, day of year and Sunshine.

T_lapse:: Temperature lapse, the slope of the temperature / height
regression. Typical values are `@f$ -0.0004 .. -0.001 \frac{^\circ C}{m}
@f$`, default is @f$ 0\frac{^\circ C}{m} @f$ (no temperature
adjusting)

### Creating a meteorological station

To keep the example code short, we assume, that you have your data
provided in a special binary data format for timeseries. For your model,
you would rather do something as it is shown in CmfTutTestData.

``` {.py}

meteo = project.meteo_stations.add_station(name='A station name',
                                           position=(0,0,0),
                                           latitude=50.8,
                                           longitude=8.4)
# Load some data
meteo.Tmax = cmf.timeseries.from_file('Tmax.timeseries')
meteo.Tmin = cmf.timeseries.from_file('Tmin.timeseries')
meteo.Rs   = cmf.timeseries.from_file('Rs.timeseries')
```

Have a look at the first line. To create a meteo station, the name and
position are needed as for the rainfall station, but also the position
in geographic coordinates and the time zone for the calculation of
radiation from sun inclination. If you want to have a daily average
radiation, set `meteo.daily=True`. To include diurnal changes of the
radiation (the night is dark) set `meteo.daily=False`. If you provide
`Rs` data, the geographic position is not used and the radiation
follows the given data. To assign a cell you can do
`meteo.use_for_cell(c)` or use a concept shown in the next section.

## Distributing data in space

### Using the nearest station

If the forcing data for a cell should be used from one station, the
`station.use_for_cell` idiom is fine. To assign all cells of a
project to the nearest station write in your script:

``` {.py}

project.use_nearest_meteo(z_weight=0.0)
project.use_nearest_rainfall(z_weight=0.0)
```

The distance is calculated using the `x,y,z` coordinates,of the cell
@f$x_c,y_c,z_c@f$ and the `position` argument of the station ''i'',
@f$x_i,y_i,z_i@f$ is calculated as:


@f[
 d_i(x,y,z) = w_z \left|z-z_i\right| + \sqrt{\left(x-x_i\right)^2 +\left(y-y_i\right)^2} 
@f]

where @f$w_z@f$ `z_weight` is the weight of the height difference
for the distance calculation. If `z_weight` is not given, it defaults
to 0.0 (pure euclidian hoizontal distance).

### Interpolation of forcing data with IDW

Rainfall and other meterological data can be interpolated in space using
the well known IDW (inverse distance weighted) interpolation technique.
Since many variations of this simple interpolation technique exist, the
one used in cmf sis described. Any spatial interpolation is a function
of place and known data at places. The IDW function in cmf reads as:


@f[
 f(x,y,z,t) = \sum^N_{i=1}{f_i(t) w_i(x,y,z)} 
@f]


@f[
 w_i(x,y,z) = \frac{d_i(x,y,z)^{-p}}{\sum^N_{j=0}{d_j(x,y,z)^{-p}}}
  
@f]


@f[
 d_i(x,y,z) = w_z \left|z-z_i\right| + \sqrt{\left(x-x_i\right)^2 + \left(y-y_i\right)^2} 
@f]

where:

- @f$ x,y,z @f$ is the actual spatial position - @f$ t @f$ is the
actual time step - @f$ N @f$ is the number of stations - `@f$ f_i(t)
@f$` is a data value of station @f$i@f$ and time step @f$t@f$ - `@f$
w_i(x,y,z) @f$` is a weight function of station @f$i@f$ for position
@f$ x,y,z @f$ - @f$ d_i(x,y,z)@f$ is the distance / proximity
function of station @f$i@f$ for position @f$x,y,z@f$ - @f$ p @f$
is the weighting power. A common value is @f$p=2@f$ - `@f$ x_i, y_i,
z_i @f$` is the spatial position of station @f$i@f$ - @f$ w_z @f$ is
a weight parameter. This parameter can be used change the influence of
height differences on the proximity of places. @f$w_z=0@f$ resolves in
a pure horizontal distance calculation, while very heigh values (eg. 10
000) resolve in an interpolation by height. Intermediate values (e.g.
10), depending on the height differences in the study area can lead to
realistic distributions of spatiotemporal values in the landscape.

#### Usage:

Applying IDW interpolation for all cells over all stations is used as:

``` {.py}

project.use_IDW_meteo(z_weight=0.0, power=2.)
project.use_IDW_rainfall(z_weight=0.0, power=2.)
```

with `z_weight` @f$\hat{=}\ w_z@f$, `power` @f$\hat{=}\ p@f$

author: florianjehn, version: 8 Mon Nov 20 14:24:57 2017
