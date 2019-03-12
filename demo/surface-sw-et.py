#! /usr/bin/env python3
"""
Calculates the behaviour how puddles in a forest dry out
"""
import cmf


class Model:

    def __init__(self):
        self.p = cmf.project()
        self.c = self.p.NewCell(0, 0, 0, 1000, with_surfacewater=True)
        self.c.add_layer(1.0)
        self.c: cmf.Cell
        cmf.RutterInterception.use_for_cell(self.c)
        self.et = cmf.ShuttleworthWallace.use_for_cell(self.c)
        cmf.MatrixInfiltration(self.c.layers[0], self.c.surfacewater)

    def __call__(self, days=7):
        self.c.vegetation.LAI = 7
        self.c.vegetation.height = 10
        self.c.vegetation.CanopyClosure = 0.1
        l = self.c.layers[0]
        self.c.surfacewater.depth = 0.05
        l.soil.Ksat = 0.02
        l.volume = 100
        self.c.canopy.volume = self.c.vegetation.LAI * self.c.vegetation.CanopyCapacityPerLAI
        self.c.set_uptakestress(cmf.VolumeStress(10, 0))
        Vc0 = self.c.canopy.volume
        Vl0 = self.c.layers[0].volume
        Vs0 = self.c.surfacewater.volume
        solver = cmf.CVodeIntegrator(self.p, 1e-9)
        vol = []
        flux = []
        resistance = []
        mpot = []
        self.et.refresh(cmf.Time())
        for t in solver.run(cmf.Time(), cmf.day * days, cmf.min*6):
            #print(f'{t}: {self.c.surfacewater.depth:0.3f}m')
            vol.append((
                self.c.canopy.volume / Vc0,
                self.c.surfacewater.volume / Vs0,
                self.c.layers[0].volume / Vl0
            ))
            flux.append((
                self.c.canopy.flux_to(self.c.evaporation, t),
                self.c.surfacewater.flux_to(self.c.evaporation, t),
                self.c.layers[0].flux_to(self.c.evaporation, t),
                self.c.layers[0].flux_to(self.c.transpiration, t),
                self.c.surfacewater.flux_to(self.c.layers[0], t),
            ))
            resistance.append((
                self.et.RAA, self.et.RAC, self.et.RAS,
                self.et.RSC, self.et.RSS
            ))
            mpot.append(self.c.surface_water_coverage())
        return vol, flux, resistance, mpot

if __name__ == '__main__':
    print(cmf.__version__)
    m = Model()
    vol, flux, resistance, mpot = m(10)
    from matplotlib import pylab as plt
    fig, ax = plt.subplots(3, 1, sharex='all')
    plt.sca(ax[0])
    plt.plot(vol)
    plt.legend(['Canopy', 'Surfacewater', 'Layer'], loc=0)
    plt.sca(ax[1])
    plt.plot(flux)
    plt.legend(['E_Canopy', 'E_Surfacewater', 'E_Layer', 'T_Layer', 'Infiltration'], loc=0)
    plt.sca(ax[2])
    plt.plot(resistance)
    plt.legend('RAA RAC RAS RSC RSS'.split(), loc=0)
    plt.show()




