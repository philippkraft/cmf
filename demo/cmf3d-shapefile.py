"""
A demonstration, how shapefiles can be used in cmf.

The API is likely to change some day
"""

import cmf
import numpy as np
from cmf.geos_shapereader import Shapefile
from cmf.cell_factory import cells_from_polygons

from datetime import datetime, timedelta

def load_meteo(project):
    """Loads the meteorology from a csv file.
    This is just one example for access your meteo data.
    Thanks to Python, there is no problem to access most type of datasets,
    including relational databases, hdf / netcdf files or what ever else.
    """

    # Create a timeseries for rain - timeseries objects in cmf is a kind of extensible array of
    # numbers, with a begin date, a timestep.
    begin = datetime(1980, 1, 3)
    rain = cmf.timeseries(begin=begin, step=timedelta(days=1))

    # Create a meteo station
    meteo = project.meteo_stations.add_station(name='Giessen', position=(0, 0, 0))

    # Meteorological timeseries, if you prefer the beginning of the timeseries
    # read in from file, just go ahead, it is only a bit of Python programming
    meteo.Tmax = cmf.timeseries(begin=begin, step=timedelta(days=1))
    meteo.Tmin = cmf.timeseries(begin=begin, step=timedelta(days=1))
    meteo.rHmean = cmf.timeseries(begin=begin, step=timedelta(days=1))
    meteo.Windspeed = cmf.timeseries(begin=begin, step=timedelta(days=1))
    meteo.Sunshine = cmf.timeseries(begin=begin, step=timedelta(days=1))
    meteo.T = (meteo.Tmax + meteo.Tmin) * 0.5

    # Load climate data from csv file
    # could be simplified with numpy's
    csvfile = open('data/climate.csv')
    csvfile.readline()  # Read the headers, and ignore them
    for line in csvfile:
        # split the line in to columns using commas
        columns = line.split(',')
        # Get the values, but ignore the date, we have begin and steo
        # of the data file hardcoded
        # If you don't get this line - it is standard Python, I would recommend the official Python.org tutorial
        for timeseries, value in zip([rain, meteo.Tmax, meteo.Tmin, meteo.rHmean, meteo.Windspeed, meteo.Sunshine],
                                     [float(col) for col in columns[1:]]):
            # Add the value from the file to the timeseries
            timeseries.add(value)

    # Create a rain gauge station
    project.rainfall_stations.add('Giessen', rain, (0, 0, 0))

    # Use the rainfall for each cell in the project
    project.use_nearest_rainfall()
    # Use the meteorological station for each cell of the project
    project.use_nearest_meteo()


def soiltype(depth):
    """
    Creates a retention curve for a depth below ground
    using an exponential Ksat decline
    :param depth: depth below ground in m
    :return: Retention curve
    """
    return cmf.BrooksCoreyRetentionCurve(ksat=15 * np.exp(-depth),
                                         porosity=0.5,
                                         _b=5.5,
                                         theta_x=0.35)

def build_cell(c, layercount=0):
    """
    Shapes and makes a cell, by adding layers and connections
    :param c: cmf.Cell
    :return: None
    """

    for d in [0.05, 0.1, 0.2, 0.3, 0.5, 0.75, 1.0, 1.3, 1.7, 2.][-layercount:]:
        c.add_layer(d, soiltype(d))
    c.saturated_depth = 5.
    c.surfacewater_as_storage()
    # use Richards connection
    c.install_connection(cmf.Richards)
    c.install_connection(cmf.MatrixInfiltration)
    c.install_connection(cmf.CanopyOverflow)
    c.install_connection(cmf.SimpleTindexSnowMelt)
    c.install_connection(cmf.PenmanMonteithET)



def create_project(subsurface_lateral_connection,
                   surface_lateral_connection,
                   layercount=0):

    p = cmf.project()

    shp = Shapefile('data/soil_lu_gw.shp')
    cells = cells_from_polygons(p, shp)

    for c in p:
        build_cell(c, layercount)

    cmf.connect_cells_with_flux(p, subsurface_lateral_connection)
    cmf.connect_cells_with_flux(p, surface_lateral_connection)

    load_meteo(p)

    return p

p = create_project(cmf.DarcyKinematic, cmf.KinematicSurfaceRunoff)

print(cmf.describe(p))


