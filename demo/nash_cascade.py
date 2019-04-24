import cmf

class Cascade(cmf.project):

    def __init__(self, size, connection, *args, **kwargs):
        """
        Parameters
        ----------
        size
        residence_time
        """
        super().__init__()
        self.connection_type = connection
        self.storages = []
        self.outlet = self.NewStorage('outlet', 0, 0, 0)
        for i in range(1, size + 1):
            c: cmf.Cell = self.NewCell(i, 0, i * 0.1, 1, False)
            stor = c.add_layer(1.0, cmf.VanGenuchtenMualem(Ksat=1, phi=0.5, n=2, alpha=0.1))
            if self.storages:
                connection(stor, self.storages[-1], *args, **kwargs)
            else:
                connection(stor, self.outlet, *args, **kwargs)
            self.storages.append(stor)
        self.initialize()

    def initialize(self, startvol=1.0):
        for stor in self.storages:
            stor.potential = stor.gravitational_potential - 0.5
        self.storages[-1].wetness = startvol

    def __repr__(self):
        return 'Cascade({})'.format(self.connection_type.__name__)


class NashCascade(Cascade):
    def __init__(self, size, residence_time=1.0):
        super().__init__(size, cmf.LinearStorageConnection, residence_time / size)


class PowerLawCascade(Cascade):
    def __init__(self, size, Q0=1.0, beta=5/3):
        super().__init__(size, cmf.PowerLawConnection, Q0=Q0 * size, V0=0.25, beta=beta)


class LinearGradientCascade(Cascade):
    def __init__(self, size, K=1.0):
        super().__init__(size, cmf.LinearGradientFlux, K=K, d=1.0, A=1.0)


class DarcyCascade(Cascade):
    def __init__(self, size, K=1.0):
        super().__init__(size, cmf.Darcy, 1, 1)
        for s in self.storages:
            s.soil.Ksat = K

class RichardsCascade(Cascade):
    def __init__(self, size, K=1.0):
        super().__init__(size, cmf.Richards_lateral, 1, 1)
        for s in self.storages:
            s.soil.Ksat = K


def run(model, solvertype, nsteps, dt):
    """
    Parameters
    ----------
    nsteps: int
        Number of steps
    dt: cmf.Time
        Timestep length

    Returns
    -------

    """
    solver = solvertype(model)
    return [
        model.outlet(t)
        for t in solver.run(cmf.Time(), dt * nsteps, dt)
    ]


if __name__ == '__main__':
    size = 10
    models = [
        NashCascade(size),
        PowerLawCascade(size),
        DarcyCascade(size, 1000),
        RichardsCascade(size, 1000)
    ]
    nc, plc, lgc, rc = models

    from pylab import plot, legend, show
    import numpy as np
    dt = cmf.min
    nsteps = int(cmf.week / dt)
    t = np.linspace(0, 7, nsteps)
    for model in models:
        if model:
            q = [model.outlet(cmf.Time())]
            q += run(model, cmf.CVodeBanded, nsteps, dt)
            plot(q, label=str(model))
    legend(loc=0)
    show()


