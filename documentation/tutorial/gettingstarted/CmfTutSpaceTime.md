@page cmfTutSpaceTime

@tableofcontents

 
[back...](@ref cmfTutUnits) [next...](@ref cmfTutCell)

# Space and time in cmf

## Note for Python 2 users

To show our results, we are using Python's print statement / function.
However, "print" has changed between python 2 and 3. To run our examples
python 2 users should always start their programs with the following
line:

~~~~~~~~~~~~~{.py}

from __future__ import print_function, division
~~~~~~~~~~~~~

This import the behaviour of the python3 print function as well as the
improved handling of integer division into python 2. Using this line in
python3 is fine and changes nothing.

## Space

Many objects in cmf have a position in space given as coordinates. Such
spatial position is crucial for spatial explicit models, like
distributed models. You can either use a local reference system (you
declare a point as the origin of the coordinate system) or you can use a
spatial reference system with GPS coordinates The spatial reference
system you prefer is your choice, as long as the following conditions
are met:

- The reference system must be a cartesian system (projected). - Length
needs to be measured in m

Or, simply put, UTM coordinates are fine, decimal degrees are not.

If you need to provide a position in cmf, in most cases a
[point](@ref cmf::geometry::point) is expected. However, any sequence
(tuple, list, numpy array) with three numbers and any object providing
x,y and z properties will be accepted by cmf as a point. In the
tutorials, a tuple like `(54321,2456.2,3.2)`, or more often
`(0,0,0)` is used.

## Time

CMF uses the [Time](@ref cmf::math::Time) and the
[Date](@ref cmf::math::Date) class for calculations with time.

Time is used both for absolute time stamps as well as for time
differences. CMF accepts python's `datetime` and `timedelta` objects
also and converts the m internally to cmf.Time. The Date class exists
only for some features that are valid for absolute time spans. Every
Time is convertable to a Date and vice versa.

The cmf.Time can be used with some operators, like +,-,\*,/,% (modulo).
Not all operations are sensible, of course. The addition of two absolute
timestamps is not meaningful, but the addition of an absolute timestamp
and a time span. To simplify programming some time span constants are
defined: `cmf.year, cmf.month, cmf.week, cmf.day, cmf.hour, cmf.min,
cmf.sec, cmf.ms`.

## Timeseries

Forcing data for hydrological models is organized in timeseries. To use
time series data in cmf effectivly, cmf provides the
[timeseries](@ref cmf::math::timeseries) class. Timeseries objects in
cmf have always a fixed step size (`step`) and a defined time for the
first data entry (`begin`). The data is stored as a sequence of
values. The end time of the time series is calculated from `begin`,
`step` and the number of entries in the series.

`timeseries` objects are queried with a time and return the value for
that time. If the time is before `begin` or after `end`, the first
resp. last entry in the dataset is returned. Hence, constant
"timeseries", with only one scalar entry containing the constant can be
used. These "timeseries" return the scalar value regardless of the time.
Values between the timesteps are interpolated using a linear or a
nearest neighbor interpolation.

### Creating timeseries

#### The standard way

The simplest and most common way to use timeseries in cmf is to create
an empty timeseries (no entry) with a begin and a step size. After
creation the `add` method is used to fill the timeseries with data.

~~~~~~~~~~~~~{.py}

import cmf
from datetime import datetime,timedelta
# Create an empty timeseries, with a 30 min time step. Alternativly you can write the 
# time step as step = timedelta(minutes=30). 
# The interpolation method is by default linear interpolation
data = cmf.timeseries(begin = datetime(2010,1,1), step = cmf.min * 30)
# Add data for two days, alternating between 0 and 1
for i in range(96):
    data.add(i % 2)
# Print some values
# Value at a certain time step
print(data[datetime(2010,1,1,6)]) # returns 0.0
# Value between time steps, 15 minutes late
print(data[datetime(2010,1,1,6,15)]) # returns 0.5
# Value before begin
print(data[datetime(2009,12,24)]) # returns 0.0
# Value after end
print(data[datetime(2010,1,5)]) # returns 1.0
~~~~~~~~~~~~~

#### Scalar timeseries

Objects that expect a timeseries, but should have in your model a
constant in time are represented as a single value timeseries. To reduce
the number of lines needed to define a constant timeseries, the static
method `from_scalar` exists:

~~~~~~~~~~~~~{.py}

# Create a constant timeseries
data = cmf.timeseries.from_scalar(0.5)
~~~~~~~~~~~~~

However, some objects expecting timeseries will do the conversion from a
scalar to a timeseries gracefully behind the scences and accept a number
instead of the timeseries object. Just try it

#### From numpy arrays and other sequences

If the data you want to store in a timeseries is in a Python iterable
(list, tuple, generators, etc.) the timeseries can be created in one
step, with out the need to write a loop yourself. A shorter replacement
for the example above is:

~~~~~~~~~~~~~{.py}

data = cmf.timeseries.from_sequence(begin=datetime(2010,1,1), 
                                    step=cmf.min*30, 
                                    sequence=(i % 2 for i in range(96)))
~~~~~~~~~~~~~

If you have already stored your data in a numpy array, a very fast
(C/C++ based) method exists to copy the array into a timeseries object.
As an example we store random normal distributed values for 50 days in
30 minute stepsize in a timeseries:

~~~~~~~~~~~~~{.py}

import cmf
import numpy as np
data = cmf.timeseries.from_array(begin=datetime(2010,1,1), 
                                 step=cmf.min*30, 
                                 data = np.random.normal(0,1.,2400))
~~~~~~~~~~~~~

However, you do not gain speed or simplicity if you fill the array
beforehand with a Python loop.

#### From a binary file

In general, cmf is file format agnostic. As shown above you can read
your data from any datasource and add it with your code to the
timeseries objects. However, if you are calibrating a fast conceptual
model on a super computer, reading from the files can be quite an
overhead. You can (but do not have to) write timeseries in a specific
binary format to disk and read it afterwards.

~~~~~~~~~~~~~{.py}

# Load a timeseries from the file named 'my.timeseries'
data = cmf.timeseries.from_file('my.timeseries')
# Save the timeseries to a file named 'my2nd.timeseries'
data.to_file('my2nd.timeseries')
~~~~~~~~~~~~~

The file format is described in the docstring of the `to_file`
method. For interprocess exchange, the `to_buffer` and
`from_buffer` method uses the same format as for files, but write the
data to a binary string in memory instead of the file.

### Removing no-data values within timeseries

Because your model will create odd results with common no-data values as
e.g. ‘-999’, you should first examine your data and in case interpolate
it. CMF gives the following function:

~~~~~~~~~~~~~{.py}

data.remove_nodata(nodata_value)
~~~~~~~~~~~~~

By using this function, CMF will take the preceding and following value
of the no-data value in the timeseries and return the mean of it.

### Calculations with timeseries

#### Arithmetic operation

You can do calculations with timeseries. This especially handy, if you
need to transform a timeseries from one unit to another. Let us assume a
timeseries of rainfall data in mm/h. Instead of changing your database,
you can just write in your script:

~~~~~~~~~~~~~{.py}

rain = data * 24
# Or if you do not need the old timeseries for something else:
data *= 24
~~~~~~~~~~~~~

The basic arithmetic operators (+,-+,\*,/) are defined for timeseries
and scalars, as well as for timeseries on both side of the operand. If
you calculate with two timeseries, the timestep and begin of the left
side operand is used. The values of the right side are sampled using the
inter- and extrapolation rules from above.

Beside the arithmetic operators, there are methods to apply the
exponential function (`exp`), logarithm (`log`,`log10`), to raise
the timeseries to a scalar power (`pow`) and to get the inverse of the
timeseries (`inv`).

~~~~~~~~~~~~~{.py}

exp_data=data.exp()
square_data = data.pow(2.)
...
~~~~~~~~~~~~~

#### Statistical analysis

To get the statistics of a timeseries, use the `mean`, `max`,
`min`. For other statistical information, numpy treats cmf timeseries
as an array, therefore all functions for numpy arrays are available for
cmf timeseries, eg. `np.std(data)`, `np.percentile(data,0.1)` and
all functions from the scipy.stats module (if scipy is available). If
you need the time axis for your analysis (or for plotting) you can get
the time axis in days since 30.12.1899 as a list by

~~~~~~~~~~~~~{.py}

data_t = [t/cmf.day for t in data.iter_time()]
~~~~~~~~~~~~~

To smooth timeseries the methods `floating_avg`,
`floating_max`,`floating_min` are available for floating window
statistics. The functions need the number of entries to be included in
the window.

If you need to reduce the frequency of the timeseries, use the methods
`reduce_avg`, `reduce_max`, `reduce_min` to retrieve a new
timeseries with the average, resp. min or max value in the new
timesteps. A common use case is the retrieval of the daily min/max
temperature from high frequency data for meteorological station data.

For comparison of timeseries containing model results and observations,
you can use the [nash_sutcliffe](@ref cmf::math::nash_sutcliffe)
efficiency integrated in cmf:

~~~~~~~~~~~~~{.py}

E  = cmf.nash_sutcliffe(model, observation)
~~~~~~~~~~~~~

### Plotting timeseries

Using matplotlib, you can plot timeseries. To make this more convient,
cmf has plotting functions integrated, dedicated for timeseries, a
function for line graphs and a function for bar graphs. The helper
functions to draw cmf objects using matplotlib are in the namespace
`cmf.draw` you need to import seperately.

~~~~~~~~~~~~~{.py}

import cmf.draw
cmf.draw.plot_timeseries(data)
cmf.draw.bar_timeseries(data)
~~~~~~~~~~~~~


