"""
A demonstration, how shapefiles can be used in cmf.

The API is likely to change some day
"""

import cmf
import cmf.geometry
import numpy as np
from shapefile import Reader as Shapefile
import cmf.draw
import pylab as plt
from matplotlib.animation import FuncAnimation


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

    meteo.T = (meteo.Tmax + meteo.Tmin) * 0.5

    # Create a rain gauge station
    project.rainfall_stations.add('Giessen', rain, (0, 0, 0))

    # Use the rainfall for each cell in the project
    project.use_nearest_rainfall()
    # Use the meteorological station for each cell of the project
    project.use_nearest_meteo()

class Model:
    """
    The 3d model based on irregular polygons from a shapefile
    """

    def build_cell(self, c, layercount=0):
        """
        Shapes and makes a cell, by adding layers and connections
        :param c: cmf.Cell
        :return: the now enhanced cell
        """

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

        for d in [0.05, 0.1, 0.2, 0.3, 0.5, 0.75, 1.0, 1.3, 1.7, 2.][-layercount:]:
            c.add_layer(d, soiltype(d))
        c.saturated_depth = 1.
        c.surfacewater_as_storage()
        # use Richards connection
        c.install_connection(cmf.Richards)
        c.install_connection(cmf.GreenAmptInfiltration)
        c.install_connection(cmf.CanopyOverflow)
        c.install_connection(cmf.TempIndexSnowMelt)
        c.install_connection(cmf.PenmanMonteithET)
        return c

    @staticmethod
    def connect_outlet_cell(outlet_cell: cmf.Cell, subsurface_lateral_connection,
                            surface_lateral_connection):
        o = outlet_cell.surfacewater
        for neighbor, width in outlet_cell.neighbors:
            for l in neighbor.layers:
                subsurface_lateral_connection(l, o, width)
            try:
                surface_lateral_connection(neighbor.surfacewater, o, width)
            except TypeError:
                pass

    def get_outflow(self, t=None):
        t = t or self.t
        return sum(o.surfacewater(t) for o in self.outlet_cells) / self.area * 1000

    def get_rainfall(self, t=None):
        t = t or self.t
        return sum(c.get_rainfall(t) for c in self.project) / self.area * 1000

    def get_et(self, t=None):
        t = t or self.t
        return sum(c.transpiration(t) + c.evaporation(t) for c in self.project) / self.area * 1000

    def get_volume(self):
        return sum(sum(s.volume for s in c.storages) for c in self.project) / self.area * 1000

    @property
    def t(self):
        return self.solver.t

    @property
    def area(self):
        return sum(c.area for c in self.project)

    def __init__(self, subsurface_lateral_connection,
                       surface_lateral_connection=None,
                       layercount=0):
        """
        Creates the cmf project

        :param subsurface_lateral_connection: Type of lateral subsurface connection, eg. cmf.Darcy
        :param surface_lateral_connection:  Type of lateral surface flux connection, eg. cmf.KinematicSurfaceRunoff or None
        :param layercount: Number of layers in the subsurface
        :return:
        """

        p = cmf.project()
        shp = Shapefile('data/vollnkirchner_bach_cells.shp')

        # Create cells
        self.outlet_cells = []
        for oid, feature in enumerate(shp):
            record = feature.record
            # Create a cell for each feature in the shape file
            c = cmf.geometry.create_cell(p, feature.shape, record.HEIGHT,
                                         oid, with_surfacewater=False)

            # If it is an outlet feature, add cell to the list of outletcells
            if record.LANDUSE_CU.startswith('outlet'):
                self.outlet_cells.append(c)
            else:
                # If it is a normal upload cell, add layers
                self.build_cell(c, layercount)

        # Build topology
        cmf.geometry.mesh_project(p, verbose=True)

        # Connect cells with fluxes
        cmf.connect_cells_with_flux(p, subsurface_lateral_connection)
        if surface_lateral_connection:
            cmf.connect_cells_with_flux(p, surface_lateral_connection)

        # Connect outlets with neighbor cells
        for o_cell in self.outlet_cells:
            self.connect_outlet_cell(o_cell, subsurface_lateral_connection, surface_lateral_connection)
        # Load driver data
        load_meteo(p)
        self.project = p
        self.solver = cmf.CVodeKLU(p, 1e-9)
        self.solver.t = p.meteo_stations[0].T.begin

    def run(self, start=None, end=None, step=cmf.day):
        start = start or self.project.meteo_stations[0].T.begin
        end = end or self.project.meteo_stations[0].T.end
        print(start, end, step)
        for t in self.solver.run(start, end, step):
            print(t)
            yield t

class Animator(FuncAnimation):
    def __init__(self, model):
        self.model = model
        self.figure = plt.figure()
        def f(c): return c.layers[0].wetness if c.layers else 0.0
        self.cellmap = cmf.draw.CellMap(p.project, f, vmin=0, vmax=1)

        self.fluxmap = cmf.draw.FluxMap(self.model.project, self.model.t, zorder=2, color='w')
        self.fluxmap.scale = 10000

        self.titel = plt.title(str(self.model.t))

        super().__init__(self.figure, self.draw, self.model.run())

    def draw(self, frame=None):
        t = frame
        #print(self.fluxmap.scale)
        self.titel.set_text(str(t))
        self.cellmap()
        self.fluxmap(t)
        return [self.titel, self.fluxmap.quiver] + list(self.cellmap.get_artists())



if __name__ == '__main__':

    p = Model(cmf.Darcy, cmf.KinematicSurfaceRunoff, 1)


    # cm = cmf.draw.CellMap(p.project, lambda c: c.saturated_depth)
    anim = Animator(p)
    plt.show()

