@page CmfTutTestData

[index](@ref tutorial), [next](@ref CmfTutMeteostation)

## Available data

Models that reveal a more realistic insight than the previous one, need
data. For the next test cases we are going to use meteorological data
from the region, where cmf is developed: Gießen, Germany (50°35' N,
8°40' E). Download the meteorological data as a simple .csv file here.
The data contains daily timesteps from 1985 to 2006.

Available fields: - date in YYYY-MM-DD format - Rainfall, mean = 1.586
mm/day = 578 mm/a - Daily min Temperature, mean = 5.6 °C - Daily max
Temperature, mean = 13.96 °C - mean daily relative Humidity, mean = 77%
- Windspeed, mean = 2.48 m/s - Sunshine fraction n/N, mean = 0.33 h/h,
where n is the measured sunshine duration and N is the astronomical
potential sunshine duration.

## Loading data from a simple .csv file into cmf

cmf does not enforce the usage of a particualar data format - data is
loaded by the user script. Hence, the usage of text files in any format,
relation databases (with the appropriate database connector for Python),
netCDF with the netCDF4 package, and web services using the socket
library can be used to retrieve data.

In this example we are going to read the test data set from an csv file
using only standard Python. However, using pandas Dataframes or just the
csv module from the standard library is working equally well.

First we create a function to load our meteorological data and we create
an empty [timeseries](@ref cmf::math::timeseries) for the rain fall
and a [MeteoStation](@ref cmf::atmosphere::MeteoStation) to hold the
timeseries for the other meteorological observations. The **station**
concept is explained in more detail in the [next
tutorial](@ref CmfTutMeteostation). An introduction to timeseries is
[here](@ref CmfTutSpaceTime).

~~~~~~~~~~~~~{.py}

import cmf
def loadmeteo(project,csvfilename):
    """Loads the meteorology from a csv file.
    This is just one example for access your meteo data.
    Thanks to Python, there is no problem to access most type of datasets,
    including relational databases, hdf / netcdf files or what ever else.
    """
    
    # Create a timeseries for rain - timeseries objects in cmf is a kind of extensible array of 
    # numbers, with a begin date, a timestep.
    begin = datetime(1980,1,3)
    rain = cmf.timeseries(begin = begin, step = timedelta(days=1))

    # Create a meteo station
    meteo=project.meteo_stations.add_station(name = 'Giessen',position = (0,0,0))

    # Meteorological timeseries, if you prefer the beginning of the timeseries
    # read in from file, just go ahead, it is only a bit of Python programming
    meteo.Tmax      = cmf.timeseries(begin = begin, step = timedelta(days=1))
    meteo.Tmin      = cmf.timeseries(begin = begin, step = timedelta(days=1))
    meteo.rHmean    = cmf.timeseries(begin = begin, step = timedelta(days=1))
    meteo.Windspeed = cmf.timeseries(begin = begin, step = timedelta(days=1))
    meteo.Sunshine  = cmf.timeseries(begin = begin, step = timedelta(days=1))
~~~~~~~~~~~~~

Now we read in the file and append the read values to the timeseries.

~~~~~~~~~~~~~{.py}

    # Load climate data from csv file
    # could be simplified with numpy's 
    csvfile =  file(csvfilename) 
    csvfile.readline() # Read the headers, and ignore them
    for line in csvfile:
        # split the line in to columns using commas
        columns = line.split(',')
        # Get the values, but ignore the date, we have begin and step
        # of the data file hardcoded
        rain.add(float(columns[[1]))|        meteo.Tmax.add(float(columns[2]]))
        meteo.Tmin.add(float(columns[[3]))|        meteo.rHmean.add(float(columns[4]]))
        meteo.Windspeed.add(float(columns[[5]))|        meteo.Sunshine.add(float(columns[6]]))
~~~~~~~~~~~~~

The last part is to create a station (see [next
tutorial](@ref CmfTutMeteostation)) for the rainfall data and apply the
stations to the cells of the project.

~~~~~~~~~~~~~{.py}           

    # Create a rain gauge station
    project.rainfall_stations.add('Giessen',rain,(0,0,0))
        
    # Use the rainfall for each cell in the project
    project.use_nearest_rainfall()
    # Use the meteorological station for each cell of the project
    project.use_nearest_meteo()
~~~~~~~~~~~~~

Depending on your needs, any data source you can read with Python (which
is virtually any) is suitable to read forcing data for cmf models.

author: philipp, version: 6 Fri Mar 7 13:52:06 2014
