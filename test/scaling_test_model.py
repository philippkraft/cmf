# -*- coding: utf-8 -*-
"""
Created on Jan 09 10:39 2018
@author(s): Florian U. Jehn
"""

import datetime
import cmf
import numpy as np
import unittest


class CheckScaling(unittest.TestCase):
    """
    Determines if CMF scales correctly the following structural elements:
        - Snow
        - Kinematic Wave
        - Canopy
        - A two layer system
        
    Parameters are fixed. All models should produce the same NSE. 
    """
    def test_scaling(self):    
        # Run the models
        num_cells = [1, 2, 4, 8, 16]
        results = {}
        for num in num_cells:
            # Create the model
            model = ScalingTester(num_cells=num)
            # Get the results and convert from m³/d to m³/s
            sim = model.runmodel() / 86400   
            NSE = cmf.nash_sutcliffe(sim, model.data.Q)
            NSE = round(NSE, 3)
            results[str(num)] = NSE
    
        for key, value in results.items():
            self.assertAlmostEqual(value, 0.596, msg="NSE of {}-cell model is not the expected 0.596".format(key))
        NSE_values = list(results.values())
        self.assertTrue(NSE_values[0] == NSE_values[1] == NSE_values[2] == NSE_values[3] == NSE_values[4])



class ScalingTester:
    """
    A class to determine how CMF handles different amounts of cells. To test
    this the same model structure is run as a 1, 2, 4 and 8 cell layout. Each
    cell has the same structure and parameters.
    
    """
    # Catchment area km²
    area = 562.41
    
    def __init__(self, begin=None, end=None, num_cells=None):
        """
        Initializes the model.

        :param begin: Start year for the calibration
        :param end: Stop year
        :param num_cells: Number of cells used for this layout
        :return: None
        """

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
        self.data.Q = self.data.Q[self.begin:self.end]
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

    def setparameters(self):
        """
        Sets the parameters for all cells seperately
        :return:
        """
        # Call all cells
        for cell in self.cells:
            cell.set_parameters()

    def runmodel(self):
        """
        Runs the models and saves the results.

        :return: Simulated discharge
        """
        solver = cmf.CVodeDense(self.project, 1e-9)

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

    def set_parameters(self):
        """
        Sets the parameters for a cell instance
        :param par: Object with all parameters
        :return: None
        """
        c = self.cell
        out = self.outlet
        
        # Fill in some water
        c.layers[0].volume = 296.726 / 1000 * self.area * 1e6
        c.layers[1].volume = 77.053 / 1000 * self.area * 1e6

        # Scale to the cellsize
        V0_L1 = (185.524 / 1000) * self.area * 1e6
        V0_L2 = (150.623 / 1000) * self.area * 1e6

        # Set uptake stress
        ETV1 = 0.145 * V0_L1
        ETV0 = 0.434 * ETV1
        c.set_uptakestress(cmf.VolumeStress(ETV1, ETV0))

        # Connect layer and outlet
        cmf.PowerLawConnection(c.layers[0], out, Q0=V0_L1 / 48.823,
                               V0=V0_L1,
                               beta=2.949)

        cmf.PowerLawConnection(c.layers[0], c.layers[1], Q0=(V0_L1 /
                                                            3.198),
                               V0=V0_L1, beta=3.743)
        cmf.PowerLawConnection(c.layers[1], out, Q0=V0_L2 / 162.507,
                               V0=V0_L2,
                               beta=1.081)

        # Snow
        cmf.SimpleTindexSnowMelt(c.snow, c.layers[0], c,
                                 rate=3.957)
        cmf.Weather.set_snow_threshold(3.209)

        # Split the rainfall in interception and throughfall
        cmf.Rainfall(c.canopy, c, False, True)
        cmf.Rainfall(c.surfacewater, c, True, False)

        # Make an overflow for the interception storage
        cmf.RutterInterception(c.canopy, c.surfacewater, c)

        # Transpiration from the plants is added
        cmf.CanopyStorageEvaporation(c.canopy, c.evaporation, c)

        # Sets the paramaters for interception
        c.vegetation.LAI = 9.852

        # Defines how much throughfall there is (in %)
        c.vegetation.CanopyClosure = 0.603


class DataProvider:
    """
    Holds the forcing and calibration data
    """
    def __init__(self, file_name):
        # Load data from file using numpy magic
        data = {
                "date":["01.01.1979", "02.01.1979", "03.01.1979", "04.01.1979", 
                        "05.01.1979", "06.01.1979", "07.01.1979"],
                "tmax":[-13.07, -11.15, -7.51, -8.16, -11.57, -9.48, -5.43],
                "tmin":[-20.34, -19.17, -16.22, -15.51, -17.87, -17.59, 
                        -16.49],
                "tmean":[-15.87, -13.77, -9.65, -13.03,-14.42, -14.24,-8.86],
                "Prec":[1.14, 1.11, 0.65, 0.02, 0, 0.01, 0.01],
                "Q":[48.7, 23.2, 16.7, 14, 11.4, 9.63, 8.68]
                }

        def bstr2date(bs):
            """
            Helper function to convert date byte string to datetime object
            """
            return datetime.datetime.strptime(bs, '%d.%m.%Y')

        # Get begin, step and end from the date column
        self.begin = bstr2date(data["date"][0])
        self.step = bstr2date(data["date"][1]) - self.begin
        self.end = bstr2date(data["date"][-1])

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
    unittest.main()
