import cmf
import typing
import numpy as np
import pandas as pd
import os


def load_hourly_rainfall():

    ts = pd.read_csv('data/hourly_rainfall_giessen.csv', parse_dates=[0], index_col=0).iloc[:, 0]
    return cmf.timeseries.from_pandas(ts, True)


class Model(cmf.project):

    def __init__(self, connection):
        super().__init__()
        self.tstart = cmf.Time()
        self.dt = cmf.day
        self.tend = cmf.week * 13
        self.connection_type = connection
        self.outlet = self.NewStorage('outlet', 0, 0, 0)

    def run(self, solver, tstart=None, tend=None, dt=None, *, verbose=False):
        tstart = tstart or self.tstart
        tend = tend or self.tend
        dt = dt or self.dt
        for t in solver.run(tstart, tend, dt):
            o = self.outlet(t)
            if verbose:
                print(f'{t:%Y-%m-%d %H} {o*1000:12.5g} l/day')
            yield t.as_datetime(), o

    def initialize(self, startvol=1.0):
        for c in self:
            for stor in c.layers:
                stor.wetness = startvol
        return self

    def create_cell(self, x, y, z):
        c = self.NewCell(x, y, z, 1, False)
        stor = c.add_layer(2.0, cmf.VanGenuchtenMualem(Ksat=1, phi=0.5, n=2, alpha=0.1))
        cmf.WaterbalanceFlux(c.surfacewater, stor)
        return c, stor

    def get_connections(self):
        cons = []
        for c in self:
            for con in c.layers[0].connections:
                if con not in cons and not str(con).startswith('waterbalance'):
                    cons.append(con)
        return cons

    @property
    def rainfall(self):
        if self.rainfall_stations:
            return self.rainfall_stations[0].data
        else:
            return None

    @rainfall.setter
    def rainfall(self, rain_ts: cmf.timeseries):
        if not self.rainfall_stations:
            self.rainfall_stations.add(str(self), rain_ts, self.outlet.position)
        else:
            self.rainfall_stations[0].data = rain_ts
        self.use_nearest_rainfall()
        self.tstart = rain_ts.begin
        self.tend = rain_ts.end
        self.dt = rain_ts.step

    def set_rainfall(self, rain_ts: cmf.timeseries):
        self.rainfall = rain_ts
        return self


    def __repr__(self):
        return '{}({}, N={}, C={})'.format(
            type(self).__name__,
            self.connection_type.__name__,
            len(self), len(self.get_connections())
        )


class Cascade(Model):

    def __init__(self, connection, *args, **kwargs):
        super().__init__(connection)
        self.storages = []
        for i in range(128):
            c, stor = self.create_cell(i + 1, 0, (i + 1) * 0.1)
            if self.storages:
                connection(stor, self.storages[-1], *args, **kwargs)
            else:
                connection(stor, self.outlet, *args, **kwargs)
            self.storages.append(stor)
        self.initialize()


class XCascade(Model):

    def __init__(self, connection, *args, **kwargs):
        super().__init__(connection)
        c, stor = self.create_cell(1, 1, 0.05)
        self.storages = [stor]
        connection(stor, self.outlet, *args, **kwargs)
        for i in range(127):
            c, stor = self.create_cell(i + 1, 0, (i + 1) * 0.1)
            if i:
                connection(stor, self.storages[-1], *args, **kwargs)
            connection(stor, self.storages[0], *args, **kwargs)
            self.storages.append(stor)
        self.initialize()


class Network(Model):

    def __init__(self, connection, *args, **kwargs):
        super().__init__(connection)
        cells: typing.Dict[tuple, cmf.Cell] = {}
        for i in range(16):
            for j in range(8):
                c, stor = self.create_cell(i + 1, j + 1, (i + j + 1) * 0.1)
                cells[i, j] = c
                if i:
                    connection(stor, cells[i - 1, j].layers[0], *args, **kwargs)
                if j:
                    connection(stor, cells[i, j - 1].layers[0], *args, **kwargs)
        connection(cells[0, 0].layers[0], self.outlet, *args, **kwargs)

        self.initialize()


class Tree(Model):

    def __init__(self, connection, *args, **kwargs):
        super().__init__(connection)
        cells = {}
        for i in range(7):
            for j in range(2**i):
                c, stor = self.create_cell(i + 1, j + 1, i * 0.1)
                cells[i, j] = c
                if i:
                    connection(stor, cells[i - 1, j // 2].layers[0], *args, **kwargs)
        connection(cells[0, 0].layers[0], self.outlet, *args, **kwargs)

        self.initialize()


def plot_cells(model, f: typing.Callable):

    x = [c.x for c in model]
    y = [c.y for c in model]
    v = [f(c) for c in model]

    from matplotlib.pyplot import scatter, plot
    for con in model.get_connections():
        plot([con.left_node().position.x, con.right_node().position.x],
             [con.left_node().position.y, con.right_node().position.y],
             c='0.5', ls='-')
    scatter(x, y, s=20, c=v, marker='o')


connections = [
    dict(connection=cmf.LinearStorageConnection, residencetime=1 / 24),
    dict(connection=cmf.PowerLawConnection, Q0=1, beta=2, V0=0.5),
    dict(connection=cmf.Darcy, FlowWidth=1, Distance=1),
    dict(connection=cmf.Richards_lateral, FlowWidth=1, Distance=1)
]

model_types = [Cascade, XCascade, Tree, Network]


if __name__ == '__main__':
    cmf.options.richards_lateral_base_flow = False
    cmf.set_parallel_threads(1)

    jacs = {}
    timeseries = {}
    models = {}
    for i, mt in enumerate(model_types):
        for j, con in enumerate(connections):
            model = mt(**con)
            models[i, j] = model
            print(model)
            timeseries[i, j], solver = model.run(cmf.CVodeDense)
            jacs[i, j] = np.log10(np.abs(solver.get_jacobian()))

    def plot_jacs(ax, i, j):
        ax.matshow(jacs[i, j])

    def plot_timeseries(ax, i, j):
        ax.plot(timeseries[i, j])


    def plot_results(result_function, models):

        from matplotlib import pyplot as plt
        fig, ax = plt.subplots(len(model_types), len(connections))
        for j, con in enumerate(connections):
            ax[-1, j].set_xlabel(con['connection'].__name__)

        for i, mt in enumerate(model_types):
            ax[i, 0].set_ylabel(mt.__name__)
            for j, con in enumerate(connections):
                ax[i, j].set_title(str(models[i, j]), fontsize=6)
                result_function(ax[i, j], i, j)


    plot_results(plot_timeseries, models)
    plot_results(plot_jacs, models)











