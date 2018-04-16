@page cmf_lumped_without_spotpy

# Lumped model without Spotpy Tutorial

Here you will learn how to create a complete lumped model in CMF.

## Downloads

- [Precipitation](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/prec.txt)
- [Discharge](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/discharge.txt)
- [Temperature](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/max_min_avg_temp.txt)

## Getting started

The model structure we want to create in this tutorial can be downloaded
above. You will not need any additional files except the weather data,
that can be downloaded above as well.

## Importing the needed libraries and creating the project

The first thing we have to do is to import all the tools we will need
further down. Also we will create the CMF project itself and one cell.
This one cell will be the basis on which we built our lumped model

~~~~~~~~~~{.py}

# import handy tools
import numpy as np
import datetime
import cmf

# create the project
p = cmf.project()
# For the cell we do not need to specify the x,y,z data as their will only be one cell. 
# A cell size of 1000 comes in handy because then our data is in m³ the same as in mm.
c = p.NewCell(0,0,0,1000)
# Also we need to define how when our model begins and in what timesteps it should act
begin = datetime.datetime(1979,1,1)
step = datetime.timedelta(days = 1)
~~~~~~~~~~


## Creating all storages

We now have a cell and can start adding the storages and define their
starting properties.

~~~~~~~~~~{.py}

# Add the soil. The parameter is the depth of the layer. In this case the soil is 2 m deep.
soil = c.add_layer(2.0)
# Create the outlet were all the water of the catchment will be directed to. 
outlet = p.NewOutlet("Out", 10,0,0)
# Add Evaporation
cmf.HargreaveET(soil, c.transpiration)
~~~~~~~~~~


## Create all connections

As we not only want storages of water, but also want to transfer water
between them we need connections. Every connection is governed by its
parameters. As we do not use Spotpy and are thus unable to get the right
values for the parameter created for us, we need to set them our selves.
If you want to built your own model, you have to tweak the values
yourselves. As you have many possibilites in a model, you should
consider starting with a less complex one.

~~~~~~~~~~{.py}

# define all parameters
# tr_S = Residence time of the water in the soil
tr_soil = 150
# ETV1 = the Volume that defines that the evaporation is lowered because of not enough water
ETV1 = 100
# fETV0 = factor the ET is multiplied with when water is low
fETV0 = 0.5
# beta = Exponent that changes the form of the flux from the soil
beta = 1.4
~~~~~~~~~~


After we now have the parameter values we can start connecting things.
CMF offers very different types of connections. From the simple
kinematic wave to simple Infiltration and many more, much is possible.
For simplicity we stick here to the kinematic wave.

~~~~~~~~~~{.py}


# Flux from soil to outlet (interflow)
cmf.kinematic_wave(c.layers[0],outlet,tr, exponent=beta)        
~~~~~~~~~~


## Define the properties of other governing processes

After all connections have been set, we need to fine tune our model by
specifying its behaviour regarding evapotranspiration.

~~~~~~~~~~{.py}

# Adjustment of the evapotranspiration
c.set_uptakestress(cmf.VolumeStress(ETV1,ETV1 * fETV0))
~~~~~~~~~~


## Reading data in

Before we can let the model do its work we need one more thing: forcing
data. For this the three files from the start of this tutorial need to
be present in your working directory. They can be read using the
following code:

~~~~~~~~~~{.py}

# Data (plain text files)
fnQ = 'discharge.txt' # enter the name of your discharge data here in m³/day
fnT = 'max_min_avg_temp.txt' # enter the name of your max, min and average temperature here in °C
fnP = 'prec.txt' # enter the name of your precipitation data here in mm/day
# create a empty time series that starts at the begin
# and goes by step for all elements in the file
P = cmf.timeseries(begin, step)
P.extend(float(Pstr) for Pstr in open(fnP))

Q = cmf.timeseries(begin,step)
Q.extend(float(Qstr) for Qstr in open(fnQ))
# Convert m3/s to mm/day
Q *= 86400 * 1e3 / (2976.41 * 1e6)
T = cmf.timeseries(begin,step)
Tmin = cmf.timeseries(begin,step)
Tmax = cmf.timeseries(begin,step)

# Go through all lines in the file
for line in open(fnT):
    columns = line.split('\t')
    if len(columns) == 3:
        Tmax.add(float(columns[[0]))|        Tmin.add(float(columns[1]]))
        T.add(float(columns[2]))
~~~~~~~~~~


## Creating a meteorological station

After we now have the meteorological data, we have to create meteo
stations and add the data to them. The stations are CMFs way to make the
weather data accessible for the model. Rainfall stations and
meteorological stations are handled seperately.

~~~~~~~~~~{.py}

# Create the rainstation and give it the freshly loaded precipitaton data
rainstation = p.rainfall_stations.add('Rain',P,(0,0,0))
# This tells every cell in the model (in our case one) to use the rainfall station
# that is nearest to it. 
p.use_nearest_rainfall()

# Create the meteo station
meteo = p.meteo_stations.add_station('Meteo',(0,0,0))
# add all data to the station
meteo.T = T
meteo.Tmin = Tmin
meteo.Tmax = Tmax
# Tell the cell to use the nearest station
p.use_nearest_meteo()
~~~~~~~~~~


## Let it run

The structure of our model is now complete. We just need it to run. For
this we need a solver for the differential equations. A good choice it
the CVode Integrator. But you can use other solvers as well.

~~~~~~~~~~{.py}

# create the solver, tell him which project it should solve and what its 
# maximal error should be
solver = cmf.CVodeIntegrator(p, 1e-8)
# create an list to save all discharge data
discharge = []
# let the solver run for the given timeperiode (this may take some time)
for t in solver.run(begin, begin + cmf.day * len(Q), step):
    # save the discharge for every timestep
    discharge.append(outlet.waterbalance(t))
~~~~~~~~~~


## Compare the results

After our model has now run we have a calculated discharge data for
every day. As we also have the really measured discharge for every day
we can compare those two, to see how good our model worked.

~~~~~~~~~~{.py}

# import the plotting tools
import matplotlib.pyplot as plt
# plot and show the measured discharge against the simulated discharge
plt.plot(Q)
plt.plot(discharge)
plt.xlabel("Days")
plt.ylabel("m³ per day")
plt.ylim(0,20)
plt.show()
~~~~~~~~~~


As you can see the results of our model are not very good and differ
quite a lot from the real values. To quantify how good (our bad) our
model is we can calculate the percentage bias.

~~~~~~~~~~{.py}

# Transfer both timeseries to arrays for easier calculation
dis = np.array(discharge)
Q = np.array(Q)
# calculate the percentage bias
pc_bias = 100.0*sum(dis-Q)/sum(Q)
print("The percentage bias is %f" %(pc_bias))
~~~~~~~~~~


This gives us 92% as a result. Meaning that the difference between the
simulated and the observed values is almost larger than the observed
values itself. A clear indication that the parameter values we have
choosen for our model are not appropriate. Play around a bit with the
parameter values and see if you can get the bias below 100 %, 50 % or
even 10 %. Always consider what the parameter values are standing for
and what would be a realistic value for them.

## What's next?

Now you are able to built a relatively complex lumped model in CMF. Also
you have seen that calibrating your model by hand is quite a lot of
work, so be sure to check out
[Spotpy](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/) to learn how
to let the computer do this work for you.


