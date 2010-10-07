# -*- coding: utf-8 -*-
from __future__ import division
"""
cmf3d_Richards.py
"""
import cmf
import sys
from cmf.raster import Raster
from pylab import *

def load_meteo(project):
    # Load rain timeseries (doubled rain of giessen for more intersting results)
    rain=cmf.timeseries.from_file('giessen.rain')
    
    # Create a meteo station
    meteo=project.meteo_stations.add_station('Giessen')
    
    # Meteorological timeseries
    meteo.Tmax=cmf.timeseries.from_file('giessen.Tmax')
    meteo.Tmin=cmf.timeseries.from_file('giessen.Tmin')
    meteo.rHmean=cmf.timeseries.from_file('giessen.rHmean')
    meteo.Windspeed=cmf.timeseries.from_file('giessen.Windspeed')
    meteo.Sunshine=cmf.timeseries.from_file('giessen.Sunshine')
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
cells = cmf.cells_from_dem(p,dem)
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
cmf.connect_cells_with_flux(p,cmf.Richards_lateral)
cmf.connect_cells_with_flux(p,cmf.Manning_Kinematic)


# Make an outlet (ditch, 30cm deep)
outcell = min(p,key=lambda c: c.z)
outlet=p.NewOutlet('outlet',outcell.x-10, outcell.y-10, outcell.z-.3)
# Connect outlet to soil
p[0].connect_soil_with_node(outlet,cmf.Richards_lateral,10.0,5.0)
# Connect outlet to surfacewater (overbank flow)
cmf.Manning_Kinematic(p[0].surfacewater, outlet, 
                      cmf.Channel('R',*dem.cellsize))
load_meteo(p)
# Make solver
solver=cmf.CVodeIntegrator(p,1e-6)
solver.t=cmf.Time(1,1,1980)
if "run" in sys.argv:
    sw=cmf.StopWatch(solver.t,solver.t+cmf.year)
    for t in solver.run(solver.t,solver.t+cmf.year,cmf.day):
        ele,tot,rem= sw(t)
        print "%s - %6.2fm3/day (%s/%s)" % (t,outlet(t),ele*cmf.sec,tot*cmf.sec)