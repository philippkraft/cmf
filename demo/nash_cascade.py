import cmf


class NashBoxModel:
    def __init__(self, size, residence_time=1.0):
        """

        Parameters
        ----------
        size
        residence_time
        """
        self.project = p = cmf.project()
        self.storages = []
        self.outlet = p.NewOutlet('outlet', 0, 0, 0)
        for i in range(1, size + 1):
            stor = p.NewStorage("S{}".format(i), i, 0, i)
            if self.storages:
                cmf.LinearStorageConnection(stor, self.storages[-1], residence_time / size)
            else:
                cmf.LinearStorageConnection(stor, self.outlet, residence_time / size)
            self.storages.append(stor)

    def run(self, nsteps, dt, startvol):
        """

        Parameters
        ----------
        nsteps: int
            Number of steps
        dt: cmf.Time
            Timestep length
        startvol: float
            Starting volume in the storages

        Returns
        -------

        """
        solver = cmf.ImplicitEuler(self.project)
        for s in self.storages:
            s.volume = 0.0
        self.storages[-1].volume = startvol
        return [
            self.outlet(t)
            for t in solver.run(cmf.Time(), dt * nsteps, dt)
        ]


if __name__ == '__main__':
    model = NashBoxModel(5)
    q = model.run(25, cmf.h, 1)
    print('Run for 24h')
    print(f'{q[-1]:0.5g}')
