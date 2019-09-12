#!/usr/bin/env python3

import cmf
import numpy as np

class Model(cmf.project):

    def __init__(self, size=100):
        super().__init__('X')
        X, = self.solutes
        self.storages = []

        for i in range(size):
            self.storages.append(self.NewStorage(f'S{i:03d}', i, 0, 0))
            S = self.storages[i]
            S.volume = 1.0
            if i:
                S0 = self.storages[i-1]
                cmf.LinearStorageConnection(S0, S, 1/6.0, residual=0.0)
                r = cmf.SoluteDiffusiveTransport(24.0, S0[X], S[X])
                S[X].reactions.append(r)
                S0[X].reactions.append(r)

        self.storages[size // 5][X].conc = 1.0
        self.solver = cmf.CVodeDense(self)
        self.inlet = self.NewNeumannBoundary('inlet', self.storages[0])
        self.inlet.flux = 1.0
        self.outlet = self.NewOutlet('out', *self.storages[-1].position)
        cmf.LinearStorageConnection(self.storages[-1], self.outlet, 3.0, residual=1.0)

    @property
    def conc(self):
        X, = self.solutes
        return np.array([S[X].conc for S in self.storages])

    def run(self, dt=cmf.h):
        self.solver(self.solver.t + dt)
        return self.conc



if __name__ == '__main__':
    m = Model()
    X, = m.solutes
