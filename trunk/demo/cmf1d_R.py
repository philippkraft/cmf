# -*- coding: utf-8 -*-
from __future__ import division, print_function
"""
This file creates a single cell to calculate a soil column using a 1D Richards equation

It demonstrates how to use the cmf library - including how to load data
Although it should be ok for users with mediate Python knowledge, it is using
the so called list-comprehension mechanism in Python, which is sometimes ignored
by textbooks. If you have problems to follow the Python in this example,
please have a look at the tutorial at Python.org, a very good and concise ressource.
If you have no programming experience at all, I would recommend a Python book. Avoid books
with large chapters about graphic user interfaces.
"""

# loads cmf as a library
import cmf
import numpy as np
from datetime import datetime, timedelta
from math import exp
def load_meteo(project):
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

    # Load climate data from csv file
    # could be simplified with numpy's 
    csvfile =  open('climate.csv') 
    csvfile.readline() # Read the headers, and ignore them
    for line in csvfile:
        # split the line in to columns using commas
        columns = line.split(',')
        # Get the values, but ignore the date, we have begin and step
        # of the data file hardcoded
        rain.add(float(columns[1]))
        meteo.Tmax.add(float(columns[2]))
        meteo.Tmin.add(float(columns[3]))
        meteo.rHmean.add(float(columns[4]))
        meteo.Windspeed.add(float(columns[5]))
        meteo.Sunshine.add(float(columns[6]))

    meteo.T         = (meteo.Tmax + meteo.Tmin) * 0.5
             
    # Create a rain gauge station
    project.rainfall_stations.add('Giessen',rain,(0,0,0))
        
    # Use the rainfall for each cell in the project
    project.use_nearest_rainfall()
    # Use the meteorological station for each cell of the project
    project.use_nearest_meteo()
    

# A function to create a retention curve (used for the whole profile) 
# for a specific depth, using an exponential decline function for Ksat with depth
def soiltype(depth):
    return cmf.VanGenuchtenMualem(Ksat=15*exp(-d),
                                         phi=0.5,
                                         n=1.5,
                                         alpha=0.3
                                         )

# Create a project with one solute X
p=cmf.project()

# Create a cell at position (0,0,0) with 1000m2 size (making conversion from m3 to mm trivial)
c= p.NewCell(0,0,0,1000)

# Customize cell
# Create layers
for d in [0.05,0.1,0.2,0.3,0.5,0.75,1.0,1.25,1.5,1.75,2.0]:
    c.add_layer(d, soiltype(d))
# Set the head of each layer at 2 m below ground
c.saturated_depth=5.
# Create a surfacewater storage
c.surfacewater_as_storage()

# use Richards connection
for lup, ldown in zip(c.layers[:-1],c.layers[1:]):
    cmf.Richards(lup,ldown)
# Use matrix infiltration as connection between surface water and first layer
c.install_connection(cmf.MatrixInfiltration)
# Create a snow storage and use a simple Temperature index model as a connection between snow and surfacewater
c.install_connection(cmf.SimpleTindexSnowMelt)
# Use Penman-Monteith for ET
c.install_connection(cmf.ShuttleworthWallace)
c.vegetation.stomatal_resistance = 200
# Make an outlet (Groundwater as a boundary condition)
groundwater=p.NewOutlet('outlet', 0, 0, -4.4)
# Connect last layer with the groundwater, using Richards equation
cmf.Richards(c.layers[-1],groundwater)
# load meteorological data
load_meteo(p)
# Make solver
solver=cmf.CVodeIntegrator(p,1e-12)
solver.t=cmf.Time(1,11,1980)

def run(until=cmf.year,dt=cmf.day):
    """Runs a the model, and saves the outflow"""
    
    # Create a timeseries for the outflow
    outflow = cmf.timeseries(solver.t,dt)
    # Create a list to save the layer wetness 
    wetness = []
    # Get the end time
    until = until if until>solver.t else solver.t+until
    
    # The run time loop. Iterates over the outer timestep of the model
    # Internally, the model may take shorter timesteps
    for t in solver.run(solver.t,until,dt):
        # store the actual groundwater recharge      
        outflow.add(c.layers[-1].flux_to(groundwater,t))
        # store the actual wetness
        wetness.append(c.layers.wetness)
        # Print, at which time step you are
        print("{} - {:6.2f}m3/day, {} rhs-eval".format(t,groundwater(t), solver.get_nonlinear_iterations()))
    return outflow,wetness

def plotresult(outflow,wetness):
    "Plots the result using matplotlib"
    
    # import some matplotlib functionality
    import pylab
    import numpy
    import cmf.draw
    # Make the upper plot
    pylab.subplot(211)
    # Draw the timeseries. cmf.draw.plot_timeseries is a thin wrapper over pylab.plot_date
    cmf.draw.plot_timeseries(outflow)
    pylab.title('Groundwater recharge')
    pylab.axis('tight')
    # Make the lower plot
    pylab.subplot(212)
    # Convert wetness to a numpy array - for faster analysis
    wetness=numpy.array(wetness)
    # Make a times/depth contour map
    pylab.contourf([pylab.date2num(t.AsPython()) for t in outflow.iter_time()],
              c.layers.thickness*0.5-c.layers.lower_boundary, 
              wetness.T,
              levels=numpy.linspace(wetness.min(),1,50),
              cmap=pylab.cm.jet_r,
              extend='both',
              )
    pylab.title('Wetness')
    pylab.gca().xaxis_date()
    pylab.axis('tight')
    pylab.show()

# If this file is started as a script and not imported as a library
if __name__ == '__main__': 
    # Run the model for 5 years
    import time
    tstart=time.time()
    cmf.set_parallel_threads(1)
    outflow,wetness=run(cmf.year*5)
    print('{:g} s'.format(time.time()-tstart))
    #print c.vegetation
    # Try to plot the results
    try:
        plotresult(outflow,wetness)
    except ImportError:
        print("Matplotlib is not installed or has a failure. No plotting possible")
    

