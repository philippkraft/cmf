@page semi_distributed

# Semi distributed models in CMF

Creating a lumped and a semi distributed model in CMF can be handled
relatively similar. The main difference is that a lumped model contains
only one cell, while a semi distributed model contains several cells
(one for every subatchment). This leads to a more difficult construction
and parametrization of the semi distributed model. The most
straightforward way to adress this, is to split the code in severall
classes. The main model class calls all the other classes and has the
interface to Spotpy (or handles the manual calibration). The second
class is a kind of template for a cell. In this class all CMF elements
which are the same for all subcatchments are defined. This class
inherits then to the classes of the several subcatchments. In those
classes for the subcatchments all CMF elements are added which are
different to the other subcatchments. This is also a good place to add
the weather data for the subcatchments.

Important: In lumped models with a cell area other than 1000 m² and
especially in semi distributed models, where every cell has another
size, some parameters need to be adjusted for the size of the cell.
Examples would be V0 in the PowerLaw or ETV1 in the evapotranspiration.

Following is a example how one might construct a semi distributed model.
The example allows the user to choose the amount of cells in the model.
All cells here use the same data and parameters. This would have to be
changed for other usage.

The model itself and its driving data are attached to this site.

~~~~~~~~~~~~~{.py}

# -*- coding: utf-8 -*-
"""
Created on Jan 09 10:39 2018
@author(s): Florian U. Jehn
"""

import datetime
import cmf
import spotpy
from spotpy.parameter import Uniform
import os
import numpy as np
import pandas as pd


class ScalingTester:
    """
    A class to determine how CMF handles different amounts of cells. To test
    this the same model structure is run as a 1, 2, 4 and 8 cell layout. Each
    cell has the same structure and parameters.
    """
    # Catchment area km²
    area = 562.41

    # General storage parameter
    V0_L1 = Uniform(10, 300)
    V0_L2 = Uniform(10, 300)

    # ET parameter
    fETV1 = Uniform(0.01, 1, doc='if V<fETV1*V0, water uptake stress for '
                    'plants starts [%]')
    fETV0 = Uniform(0, 0.9, doc='if V<fETV0*fETV1*V0, plants die of drought '
                                '[%]')

    # Outflow parameters
    tr_L1_out = Uniform(0.1, 200, doc='Residence time of water in storage '
                                      'when '
                    'V=V0 [days]')
    tr_L1_L2 = Uniform(0.1, 200)
    tr_L2_out = Uniform(0.1, 200)

    beta_L1_out = Uniform(0.5, 4, doc="Exponent for scaling the outflow[]")
    beta_L1_L2 = Uniform(0.5, 4)
    beta_L2_out = Uniform(0.4, 4)


    # Snow parameters
    snow_meltrate = Uniform(3, 10, doc="Meltrate of the snow [[(mm*degC)/day]")|    snow_melt_temp = Uniform(0, 3.5, doc="Temperature at which the snow [degC]]"
                             "melts")

    # Canopy Parameters
    LAI = Uniform(1, 12, doc="Leaf Area Index")
    CanopyClosure = Uniform(0.1, 0.9, doc="Closure of the Canopy [%]")

    def __init__(self, begin=None, end=None, num_cells=None):
        """
        Initializes the model.

        :param begin: Start year for the calibration
        :param end: Stop year
        :param num_cells: Number of cells used for this layout
        :return: None
        """
        self.dbname = "scaling_tester_num_cells_" + str(num_cells)

        # load driver data
        self.data = DataProvider("fulda_kaemmerzell_climate_79_89.csv")
        # Create cells and project
        self.project, self.outlet = self.create_project()
        self.num_cells = num_cells
        self.cells = self.create_cells()

        # Add the data and set the parameters with random value, so the
        # complete structure can be described.
        self.data.add_stations(self.project)
        self.begin = begin or self.data.begin
        self.end = end or self.data.end
        self.setparameters()

    def create_project(self):
        """
        Creates and CMF project with an outlet and other basic stuff and
        returns it.
        :return: cmf project and cmf outlet
        """
        # Use only a single thread, that is better for a calibration run and
        # for small models
        cmf.set_parallel_threads(1)

        # make the project
        p = cmf.project()

        # make the outlet
        outlet = p.NewOutlet("outlet", 10, 0, 0)
        return p, outlet

    def create_cells(self):
        """
        Creates a 'num_cells' amount of cells for the project
        :return:
        """
        # Adjust the cellsize to the amount of cells
        area = self.area / self.num_cells
        # Create all the cells!
        cells = []
        for num in range(self.num_cells):
            cells.append(CellTemplate(self.project, self.outlet, area,
                                      num))
        return cells

    def setparameters(self, par=None):
        """
        Sets the parameters for all cells seperately
        :return:
        """
        # Create tje parameters
        par = par or spotpy.parameter.create_set(self)
        # Call all cells
        for cell in self.cells:
            cell.set_parameters(par)

    def runmodel(self):
        """
        Runs the models and saves the results.

        :return: Simulated discharge
        """
        solver = cmf.CVodeIntegrator(self.project, 1e-9)

        # Result timeseries
        res_q = cmf.timeseries(self.begin, cmf.day)

        try:
            # Start solver and calculate in daily steps
            for t in solver.run(self.data.begin, self.end, cmf.day):
                res_q.add(self.outlet.waterbalance(t))
        except RuntimeError:
            return np.array(self.data.Q[
                            self.data.begin:self.data.end + datetime.timedelta(
                                days=1)])*np.nan

        return res_q

    def simulation(self, vector=None):
        """
        Sets the parameters of the model and starts a run
        :return: np.array with runoff in mm/day
        """
        self.setparameters(vector)
        result_q = self.runmodel()
        result_q /= 86400
        return np.array(result_q[self.begin:self.end])

    def evaluation(self):
        """Returns the evaluation data"""
        return np.array(self.data.Q[self.begin:self.end])

    def objectivefunction(self, simulation, evaluation):
        """Calculates the objective function"""
        return spotpy.objectivefunctions.nashsutcliffe(evaluation, simulation)


class CellTemplate:
    """
    Template, which provides
    """
    def __init__(self, project, outlet, area, cell_num):
        self.project = project
        self.outlet = outlet
        self.area = area
        self.cell = self.project.NewCell(cell_num, 0, 0, area * 1e6)
        self.basic_set_up()

    def basic_set_up(self):
        """
        Creates the basic storages, that are to be connected in set_parameters.
        :return:
        """
        # Add layers
        self.cell.add_layer(2.0)
        self.cell.add_layer(4.0)
        # Install a connection for the ET
        cmf.HargreaveET(self.cell.layers[0], self.cell.transpiration)
        # Add Snow
        self.cell.add_storage("Snow", "S")
        cmf.Snowfall(self.cell.snow, self.cell)
        # Create a storage for Interception
        self.cell.add_storage("Canopy", "C")

    def set_parameters(self, par):
        """
        Sets the parameters for a cell instance
        :param par: Object with all parameters
        :return: None
        """
        c = self.cell
        out = self.outlet

        # Scale to the cellsize
        V0_L1 = (par.V0_L1 / 1000) * self.area * 1e6
        V0_L2 = (par.V0_L2 / 1000) * self.area * 1e6

        # Set uptake stress
        ETV1 = par.fETV1 * V0_L1
        ETV0 = par.fETV0 * ETV1
        c.set_uptakestress(cmf.VolumeStress(ETV1, ETV0))

        # Connect layer and outlet
        cmf.PowerLawConnection(c.layers[0], out, Q0=V0_L1 / par.tr_L1_out,
                               V0=V0_L1,
                               beta=par.beta_L1_out)

        cmf.PowerLawConnection(c.layers[[0],|c.layers[1]], Q0=(V0_L1 /
                                                            par.tr_L1_L2),
                               V0=V0_L1, beta=par.beta_L1_L2)
        cmf.PowerLawConnection(c.layers[1], out, Q0=V0_L2 / par.tr_L2_out,
                               V0=V0_L2,
                               beta=par.beta_L2_out)

        # Snow
        cmf.SimpleTindexSnowMelt(c.snow, c.layers[0], c,
                                 rate=par.snow_meltrate)
        cmf.Weather.set_snow_threshold(par.snow_melt_temp)

        # Split the rainfall in interception and throughfall
        cmf.Rainfall(c.canopy, c, False, True)
        cmf.Rainfall(c.surfacewater, c, True, False)

        # Make an overflow for the interception storage
        cmf.RutterInterception(c.canopy, c.surfacewater, c)

        # Transpiration from the plants is added
        cmf.CanopyStorageEvaporation(c.canopy, c.evaporation, c)

        # Sets the paramaters for interception
        c.vegetation.LAI = par.LAI

        # Defines how much throughfall there is (in %)
        c.vegetation.CanopyClosure = par.CanopyClosure


class DataProvider:
    """
    Holds the forcing and calibration data
    """
    def __init__(self, file_name):
        # Load data from file using numpy magic
        data = pd.read_csv(file_name, encoding="ISO-8859-1", sep=";")
        # Delete first row, as it only contains the units
        data = data.iloc[1:]
        data = data.dropna(axis=0)

        def bstr2date(bs):
            """
            Helper function to convert date byte string to datetime object
            """
            return datetime.datetime.strptime(bs, '%d.%m.%Y')

        # Get begin, step and end from the date column
        self.begin = bstr2date(data[["date"].iloc[0])|        self.step = bstr2date(data["date"]].iloc[1]) - self.begin
        self.end = bstr2date(data["date"].iloc[-1])

        # Read in the data
        self.P = cmf.timeseries.from_sequence(self.begin, self.step,
                                              data["Prec"])
        self.T = cmf.timeseries.from_sequence(self.begin, self.step,
                                              data["tmean"])
        self.Tmin = cmf.timeseries.from_sequence(self.begin, self.step,
                                                 data["tmin"])
        self.Tmax = cmf.timeseries.from_sequence(self.begin, self.step,
                                                 data["tmax"])
        self.Q = cmf.timeseries.from_sequence(self.begin, self.step,
                                              data["Q"])

    def add_stations(self, project):
        """
        Creates a rainstation and a meteo station for the cmf project
        :param project: A cmf.project
        :return: rainstation, meteo
        """
        rainstation = project.rainfall_stations.add('Kaemmerzell avg', self.P,
                                                    (0, 0, 0))

        project.use_nearest_rainfall()

        # Temperature data
        meteo = project.meteo_stations.add_station('Kaemmerzell avg',
                                                   (0, 0, 0))
        meteo.T = self.T
        meteo.Tmin = self.Tmin
        meteo.Tmax = self.Tmax

        project.use_nearest_meteo()
        return rainstation, meteo


if __name__ == '__main__':
    # Get sampler
    from spotpy.algorithms import lhs as Sampler

    # Check if we are running on a supercomputer or local
    parallel = 'mpi' if 'OMPI_COMM_WORLD_SIZE' in os.environ else 'seq'

    # Run the models
    runs = 100
    num_cells = [[1,|2, 4, 8]]
    results = {}
    for num in num_cells:
        # Create the model
        model = ScalingTester(num_cells=num)
        print(cmf.describe(model.project))
        # Create the sampler
        sampler = Sampler(model, parallel=parallel, dbname=model.dbname,
                          dbformat='csv', save_sim=False)

        sampler.sample(runs)
        results[str(num)] = sampler.status.objectivefunction

    for key, value in results.items():
        print("The model with {} cell(s) has a best NS of {}".format(
                                                                    key,
                                                                    value))
~~~~~~~~~~~~~


