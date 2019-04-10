import numpy as np
from matplotlib import pyplot as plt
import cmf


class DarcyPlain(cmf.project):
    """
    Simulates a horizontal 1D Daracy groundwater model in a plain.
    Overlandflow is calculated as a diffusive wave

    Model settings
    ----------------
    :tstart: Start time
    :duration: Duration of the simulation
    :dt: Time step
    :cell_count: Number of sections

    Event parameters
    ----------------
    :rain_duration: duration of rain event
    :rain_amount: Total amount of rain [mm]
    :roughness: Std'dev of the surface jitter, use 0 for a float surface [m]

    Domain setting
    --------------
    :slope: Slope of the strip in m/m
    :width: Width of the strip in m
    :length: Length of the strip in m
    :depth: Depth of the soil - no flow condition below
    :Ksat: Saturated conductivity in m/day
    """
    tstart = cmf.Time(1, 7, 2018)
    duration = cmf.day * 7
    dt = cmf.min * 10
    cell_count = 100

    rain_duration = cmf.h * 2
    rain_amount = 20  # mm
    roughness = 0.002

    slope = .0 / 100
    width = 3
    length = 20.0
    depth = 1
    Ksat = 10

    def __init__(self):
        super().__init__()
        self.ET = []
        # Numerische Zauberei
        cmf.DiffusiveSurfaceRunoff.set_linear_slope(0.01)
        for i in range(100):
            x = i * self.length / 100
            c: cmf.Cell = self.NewCell(x, 0, self.depth + x * self.slope + np.random.normal(0, self.roughness),
                                       self.length / 100 * self.width, True)
            if i:
                c.topology.AddNeighbor(self[i - 1], self.width)
            rc = cmf.VanGenuchtenMualem(self.Ksat, 0.4, n=2, alpha=0.1)
            l = c.add_layer(self.depth, rc)
            l.potential = 0.0
            c.surfacewater.puddledepth = 1e-4
            self.ET.append(cmf.ShuttleworthWallace.use_for_cell(c))

        cmf.connect_cells_with_flux(self, cmf.Darcy)
        cmf.connect_cells_with_flux(self, cmf.DiffusiveSurfaceRunoff)
        self.rainfall_stations.add('', 0.0, (0, 0, 0))
        self.use_nearest_rainfall()
        self.outlet = self.NewOutlet('out', -self.length / 100, 0, 0)
        cmf.Darcy(self[0].layers[0], self.outlet, self.width)
        cmf.DiffusiveSurfaceRunoff(self[0].surfacewater, self.outlet, self.width)

    def set_rain(self):
        print('set rain')
        data = cmf.timeseries(self.tstart, self.dt)
        while data.end < self.tstart + self.rain_duration:
            data.add(self.rain_amount * cmf.day / self.rain_duration)
        data.add(0)
        self.rainfall_stations[0].data = data

    def run(self):
        solver = cmf.CVodeKrylov(self, 1e-9)
        self.set_rain()
        for t in solver.run(self.tstart, self.tstart + self.duration, self.dt):
            q = self.outlet(t)
            print(f'{t:%H:%M} {q:0.5g}')
            yield t

    @property
    def potential(self):
        return [self.outlet.potential] + [c.layers[0].potential for c in self]

    @property
    def x(self):
        return [self.outlet.position.x] + [c.x for c in self]


class Figure:

    def __init__(self, model: DarcyPlain, *args, **kwargs):
        self.fig = plt.figure(*args, **kwargs)
        self.model = model
        self.surface = plt.fill_between([c.x for c in model],
                                        [c.z for c in model],
                                        [c.z - c.soildepth for c in model],
                                        facecolor='#FFA000', edgecolor='#A08000', lw=2)

    def init(self):
        self.potential, = plt.plot(model.x, model.potential, 'b', lw=2)
        plt.ylim(-self.model.depth, 1.1 * max(c.z for c in model))
        plt.xlim(self.model.outlet.position.x, self.model[-1].x)
        self.title = plt.text(0, 1.5 * self.model.depth, self.model.tstart)
        return [self.surface, self.title, self.potential]

    def draw(self, t):
        self.potential.set_ydata(self.model.potential)
        self.title.set_text(t)
        return [self.surface, self.title, self.potential]

    def animate(self):
        from matplotlib.animation import FuncAnimation
        iter = self.model.run()
        anim = FuncAnimation(self.fig, func=self.draw,
                             frames=iter, init_func=self.init,
                             repeat=False, interval=0.0, blit=True)
        plt.show()
        return anim


if __name__ == '__main__':
    model = DarcyPlain()
    fig = Figure(model)
    fig.animate()
