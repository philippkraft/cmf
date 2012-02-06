# -*- coding: utf-8 -*-
from __future__ import division
"""
This file creates a single cell to calculate a soil column using a 1D Richards equation
"""
import cmf
import sys
try:
    if "noshow" in sys.argv:
        raise ImportError()
    else:
        import pylab
except ImportError:
    pylab=None
from math import exp
def load_meteo(project):
    # Load rain timeseries (doubled rain of giessen for more intersting results)
    rain=cmf.timeseries.from_file('giessen.rain')

    # Create a meteo station
    meteo=project.meteo_stations.add_station(name = 'Giessen',position = (0,0,0))

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
    

# Create a retention curve (used for the whole profile)
def soiltype(depth):
    return cmf.BrooksCoreyRetentionCurve(ksat=15*exp(-d),
                                         porosity=0.5,
                                         _b=5.5,
                                         theta_x=0.35)

# Create a project with one solute X
p=cmf.project()

# Create a cell at position (0,0,0) with 1000m2 size (making conversion from m3 to mm trivial)
c= p.NewCell(0,0,0,1000)

# Customize cell
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

# Make an outlet (ditch, 30cm deep)
outlet=p.NewOutlet('outlet',-10, 0, -.3)
# Connect outlet to soil
p[0].connect_soil_with_node(outlet,cmf.Richards_lateral,10.0,10.0)
# Connect outlet to surfacewater (overbank flow)
cmf.Manning_Kinematic(p[0].surfacewater, outlet, 
                      cmf.Channel('R',10.0,10.0))
# load meteorological data
load_meteo(p)
# Make solver
solver=cmf.CVodeIntegrator(p,1e-6)
solver.t=cmf.Time(1,1,1980)
# Integrates the waterbalance over each internal substep
out_integ=cmf.waterbalance_integrator(outlet)
solver.integratables.append(out_integ)

def run(until=cmf.year,dt=cmf.day):
    """Runs a the model, and saves the outflow"""
    sw=cmf.StopWatch(solver.t,solver.t+until if until<solver.t else until)
    outflow = cmf.timeseries(solver.t,dt)
    wetness = []
    until = until if until>solver.t else solver.t+until
    for t in solver.run(solver.t,until,dt):
        ele,tot,rem= sw(t)
        outflow.add(out_integ.avg())
        wetness.append(c.layers.wetness)
        print "%s - %6.2fm3/day (%s/%s)" % (t,outlet(t),ele*cmf.sec,tot*cmf.sec)
    return outflow,wetness
if "run" in sys.argv:
    outflow,wetness=run(2*cmf.year)
    if pylab:
        from pylab import *
        subplot(211)
        cmf.draw.plot_timeseries(outflow)
        title('Outflow')
        axis('tight')
        subplot(212)
        wetness=array(wetness)
        contourf([date2num(t.AsPython()) for t in outflow.iter_time()],
                  c.layers.thickness*0.5-c.layers.lower_boundary, 
                  transpose(wetness),
                  levels=linspace(wetness.min(),1,20),
                  cmap=cm.jet_r,
                  extend='both',
                  )
        title('Wetness')
        gca().xaxis_date()
        axis('tight')
        show()