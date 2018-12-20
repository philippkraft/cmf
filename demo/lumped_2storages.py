"""
A simple lumped 2 stoarge model with ET, snow but no canopy interception
"""

import cmf

from .load_climate import load_climate_data

class Cmf2StorageModel:
    def __init__(self, climate_file='data/climate.csv'):
        self.project = cmf.project()
        self.cell = self.project.NewCell(0, 0, 0, 1000)

        self.soil = self.cell.add_layer(1.0)
        self.gw = self.cell.add_layer(1.0)
        self.snow = self.cell.add_storage('Snow', 'S')

        self.outlet = self.project.NewOutlet('outlet')

        self.meteo, self.rainstation = load_climate_data(self.project, climate_file)


    def mm_to_m3(self, vol):
        return vol * self.cell.area * 1e-3

    def create_connections(self):
        # Route snow melt to surface
        cmf.SimpleTindexSnowMelt(self.cell.snow, self.cell.surfacewater, rate=7)
        # Infiltration
        cmf.SimpleInfiltration(self.soil, self.cell.surfacewater, W0=0.8)
        # Route infiltration / saturation excess to outlet
        cmf.WaterBalanceFlux(self.cell.surfacewater, self.outlet)
        # Parameterize soil water capacity
        self.soil.soil.porosity = 0.2
        C = self.soil.get_capacity()
        # Parameterize water stress function
        self.cell.set_uptakestress(cmf.VolumeStress(0.2 * C, 0 * C))
        cmf.TurcET(self.soil, self.cell.transpiration)

        # Route water from soil to gw
        cmf.PowerLawConnection(self.soil, self.gw,
                               Q0=self.mm_to_m3(50),
                               V0=0.5 * C,
                               beta=4
                               )
        # Route water from gw to outlet
        cmf.LinearStorageConnection(
            self.gw, self.outlet,
            residencetime=20,
            residual=0 * C
        )

    def run(self, begin=None, end=None):
        begin = begin or self.rainstation.data.begin
        end = end or self.rainstation.data.end

        solver = cmf.CVodeIntegrator(self.project, 1e-9)
        outlet = cmf.timeseries(begin, cmf.day)
        outlet.add(self.outlet(begin))
        for t in solver.run(begin, end, cmf.day):
            outlet.add(self.outlet.add)
            print(t)

        return outlet


