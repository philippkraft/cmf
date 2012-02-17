# -*- coding: utf-8 -*-
from __future__ import division
"""
cmf3d_Richards.py
"""
import cmf
import sys
from cmf.raster import Raster
import cmf
import sys
import numpy as np

from datetime import datetime, timedelta
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
    meteo.T         = (meteo.Tmax + meteo.Tmin) * 0.5

    # Load climate data from csv file
    # could be simplified with numpy's 
    csvfile =  file('climate.csv') 
    csvfile.readline() # Read the headers, and ignore them
    for line in csvfile:
        # split the line in to columns using commas
        columns = line.split(',')
        # Get the values, but ignore the date, we have begin and steo
        # of the data file hardcoded
        # If you don't get this line - it is standard Python, I would recommend the official Python.org tutorial
        for timeseries,value in zip([rain,meteo.Tmax,meteo.Tmin,meteo.rHmean,meteo.Windspeed,meteo.Sunshine],
                                    [float(col) for col in columns[1:]]):
            # Add the value from the file to the timeseries
            timeseries.add(value)
             
    # Create a rain gauge station
    project.rainfall_stations.add('Giessen',rain,(0,0,0))
        
    # Use the rainfall for each cell in the project
    project.use_nearest_rainfall()
    # Use the meteorological station for each cell of the project
    project.use_nearest_meteo()
    

# Create a retention curve (used for the whole profile)
def soiltype(depth):
    return cmf.BrooksCoreyRetentionCurve(ksat=15*exp(-d),
                                         porosity=0.5,
                                         _b=5.5,
                                         theta_x=0.35)


# Create a project with one solute X
p=cmf.project()


# Loading a height model from an ESRI ASCII raster dataset
dem=Raster('dem10.dem')
# Creates cells for each grid cell
cells = cmf.cells_from_dem(p,dem,1)
# Customize cells
for c in p:  
    # Add the soil layers
    for d in [0.05,0.1,0.2,0.3,0.5,0.75,1.0,1.3,1.7,2.]:
        c.add_layer(d, soiltype(d))
    c.saturated_depth=1
    c.surfacewater_as_storage()
    # use Richards connection
    c.install_connection(cmf.Richards)
    c.install_connection(cmf.MatrixInfiltration)
    c.install_connection(cmf.CanopyOverflow)
    c.install_connection(cmf.SimpleTindexSnowMelt)
    c.install_connection(cmf.PenmanMonteithET)

# Use Richards equation
cmf.connect_cells_with_flux(p,cmf.DarcyKinematic)
cmf.connect_cells_with_flux(p,cmf.Manning_Kinematic)


# Make an outlet (ditch, 30cm deep)
outcell = min(p,key=lambda c: c.z)
outlet=p.NewOutlet('outlet',outcell.x-10, outcell.y-10, outcell.z-.3)
# Connect outlet to soil
p[0].connect_soil_with_node(outlet,cmf.Richards_lateral,10.0,5.0)
# Connect outlet to surfacewater (overbank flow)
cmf.Manning_Kinematic(p[0].surfacewater, outlet, 
                      cmf.Channel('R',celllength,celllength))
load_meteo(p)
# Make solver
solver=cmf.CVodeIntegrator(p,1e-6)
solver.t=cmf.Time(1,1,1980)
if __name__ == '__main__':
    sw=cmf.StopWatch(solver.t,solver.t+cmf.year)
    for t in solver.run(solver.t,solver.t+cmf.year,cmf.day):
        ele,tot,rem= sw(t)
        print "%s - %6.2fm3/day (%s/%s)" % (t,outlet(t),ele*cmf.sec,tot*cmf.sec)