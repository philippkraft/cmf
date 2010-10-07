# -*- coding: utf-8 -*-
from __future__ import division
"""
This file creates a hillslope to calculate a transsect using a 2D Richards equation
"""
import cmf
import sys
import cmf
import sys
import numpy as np
try:
    if "noshow" in sys.argv:
        raise ImportError()
    else:
        import pylab
except ImportError:
    pylab=None

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
    print project
    project.rainfall_stations.add('Giessen',rain,(0,0,0))
    # Use the rainfall for each cell in the project
    project.use_nearest_rainfall()
    # Use the meteorological station for each cell of the project
    project.use_nearest_meteo()
   

cellcount = 20 # numbers of cells
celllength= 10 # length of cells in m
# A function to shape the hillslope (e.g. "lambda x: 0.05*x" would make linear slope)
def z(x): return 10/(1+exp((x-100)/30))
# Create a retention curve (used for the whole profile)
def soiltype(depth):
    return cmf.BrooksCoreyRetentionCurve(ksat=15*exp(-d),
                                         porosity=0.5,
                                         _b=5.5,
                                         theta_x=0.35)


# Create a project with one solute X
p=cmf.project()
#X, = p.solutes


# Create a cells at position x
for x in np.arange(0,cellcount*celllength,celllength):
    c= p.NewCell(x,0,z(x),celllength**2)
# Make cell topology
for i,c in enumerate(p[:-1]):
    c.topology.AddNeighbor(p[i+1],celllength)

# Customize cells
for c in p:    
    for d in [0.05,0.1,0.2,0.3,0.5,0.75,1.0,1.3,1.7,2.]:
        c.add_layer(d, soiltype(d))
    c.saturated_depth=2.
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
outlet=p.NewOutlet('outlet',-celllength, 0, -.3)
# Connect outlet to soil
p[0].connect_soil_with_node(outlet,cmf.DarcyKinematic,10.0,5.0)
# Connect outlet to surfacewater (overbank flow)
cmf.Manning_Kinematic(p[0].surfacewater, outlet, 
                      cmf.Channel('R',celllength,celllength))

# Load meteorological data
load_meteo(p)
# Make solver
solver=cmf.CVodeIntegrator(p,1e-6)
solver.t=cmf.Time(1,1,1980)
# Integrates the waterbalance over each internal substep
out_integ=cmf.waterbalance_integrator(outlet)
solver.integratables.append(out_integ)
def run(until=cmf.year,dt=cmf.day):
    sw=cmf.StopWatch(solver.t,solver.t+until if until<solver.t else until)
    outflow = cmf.timeseries(solver.t,dt)
    for t in solver.run(solver.t,solver.t+cmf.year,cmf.day):
        outflow.add(out_integ.avg())
        ele,tot,rem= sw(t)
        print "%s - %6.2fm3/day (%s/%s)" % (t,outflow[-1],ele*cmf.sec,tot*cmf.sec)
    return outflow
if "run" in sys.argv:
    outflow=run()
    if pylab:
        cmf.draw.plot_timeseries(outflow)
        pylab.show()