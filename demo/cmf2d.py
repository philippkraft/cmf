# -*- coding: utf-8 -*-
from __future__ import division, print_function
"""
This file creates a hillslope to calculate a transsect using a 2D Richards equation
"""
import time
import cmf
import numpy as np
from argparse import ArgumentParser
from datetime import datetime, timedelta

try:
    import pylab
    import cmf.draw
except:
    pylab = None


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
    csvfile = open('data/climate.csv')
    csvfile.readline() # Read the headers, and ignore them
    for line in csvfile:
        # split the line in to columns using commas
        columns = line.split(',')
        # Get the values, but ignore the date, we have begin and step
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


def z(x):
    """
    A function to shape the hillslope (e.g. "0.05*x" would make linear slope)
    :param x: distance from outlet
    :return: height over outlet
    """
    return cmf.boltzmann(x, 100, 25)

def w(x, form='S'):
    """
    A function to get the width of a cell in relation to the x distance to the outlet
    :param x:
    :return:
    """
    if form == 'S':
        return 10
    elif form == 'C':
        return 10 + 0.5 * x
    elif form == 'D':
        return 10 * np.exp(-x / 100)
    else:
        raise KeyError('Form key not in {S,C,D}')

def soiltype(depth):
    """
    Creates a retention curve for a depth below ground
    using an exponential Ksat decline
    :param depth: depth below ground in m
    :return: Retention curve
    """
    return cmf.BrooksCoreyRetentionCurve(ksat=15*np.exp(-depth),
                                         porosity=0.5,
                                         _b=5.5,
                                         theta_x=0.35)

def build_cell(c):
    """
    Shapes and makes a cell, by adding layers and connections
    :param c: cmf.Cell
    :return: None
    """

    for d in [0.05, 0.1, 0.2, 0.3, 0.5, 0.75, 1.0, 1.3, 1.7, 2.]:
        c.add_layer(d, soiltype(d))
    c.saturated_depth = 5.
    c.surfacewater_as_storage()
    # use Richards connection
    c.install_connection(cmf.Richards)
    c.install_connection(cmf.MatrixInfiltration)
    c.install_connection(cmf.CanopyOverflow)
    c.install_connection(cmf.SimpleTindexSnowMelt)
    c.install_connection(cmf.PenmanMonteithET)


def create_hillslope(subsurface_lateral_connection,
                     surface_lateral_connection,
                     form='S',
                     cellcount=20, celllength=10):
    """
    Creates a cmf hillslope
    :param cellcount: Number of cells
    :param celllength: x dimension of cell
    :param cellwidth: y dimension of a cell
    :return: project, outlet
    """
    # Create a project
    p = cmf.project()

    # Create a cells at position x
    for i in range(cellcount):
        x = i * celllength + celllength/2
        p.NewCell(x, 0, z(x), celllength * w(x, form))

    # Make cell topology
    for u, d in zip(p[:-1], p[1:]):
        u.topology.AddNeighbor(d, 0.5 * (w(u.x, form) + w(d.x, form)))

    # Customize cells
    for c in p:
        build_cell(c)

    # This connects the layers of all adjacent cells laterally
    cmf.connect_cells_with_flux(p, subsurface_lateral_connection)
    if surface_lateral_connection:
        cmf.connect_cells_with_flux(p, surface_lateral_connection)

    # Make an outlet (ditch, 30cm deep)
    outlet = p.NewOutlet('outlet',-celllength, 0, 0)

    # Connect outlet to soil
    p[0].connect_soil_with_node(outlet, subsurface_lateral_connection, w(0, form), p[0].x - outlet.position.x)

    # Connect outlet to surfacewater (overbank flow)
    if surface_lateral_connection:
        surface_lateral_connection(p[0].surfacewater, outlet, w(0, form))

    load_meteo(p)

    return p, outlet


def run(p, outlet, until,dt=cmf.day):
    solver = cmf.CVodeIntegrator(p, 1e-9)
    solver.t = cmf.Time(1, 1, 1980)
    outflow = cmf.timeseries(solver.t, dt)
    for t in solver.run(solver.t, solver.t+until, dt):
        outflow.add(outlet(t))
        print("%20s - %6.1f l/day" % (t, outlet(t)*1e3))
    return outflow, solver.get_rhsevals()

subsurface_lateral_dict = dict(R=cmf.Richards_lateral, D=cmf.Darcy,
                               K=cmf.DarcyKinematic, T=cmf.TopographicGradientDarcy)

surface_lateral_dict = dict(K=cmf.KinematicSurfaceRunoff, D=cmf.DiffusiveSurfaceRunoff, N=None)


if __name__ == '__main__':
    parser = ArgumentParser(description='Runs a 2D hillslope')

    parser.add_argument('subsurface', choices='RDKT', default='R',
                        help='The type of the lateral subsurface connection:'+
                             'R - Richards_lateral, D - Darcy, K - KinematicDarcy, T - TopographicGradientDarcy')
    parser.add_argument('surface', choices='KDN', default='K',
                        help='The type of the lateral surface water connection:'+
                             'K - KinematicSurfaceRunoff, D - DiffusiveSurf..., N - None')

    parser.add_argument('--form', choices='SCD', default='S',
                        help='Form of the hillslope: S - Straight, C - Converging, D - Diverging')

    parser.add_argument('--cellcount', '-n', default=5, type=int,
                        help='Number of cells in hillslope')
    parser.add_argument('--celllength', '-l', default=10, type=float,
                        help='Length of cell in m')

    args=parser.parse_args()
    sublat = subsurface_lateral_dict.get(args.subsurface)
    surflat = surface_lateral_dict.get(args.surface)

    p, o = create_hillslope(sublat, surflat, args.form, args.cellcount, args.celllength)

    print(cmf.describe(p))
    tstart = time.time()
    outflow, rhs_evals = run(p, o, cmf.year, cmf.day)
    print('{:g} s, {} rhs evaluations'.format(time.time()-tstart, rhs_evals))
