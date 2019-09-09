import cmf
import numpy as np
import time

def SoluteWaterSolver(states, epsilon=1e-9):
    wsolver = cmf.CVodeKrylov(epsilon)
    ssolver = cmf.CVodeAdams(epsilon)
    return cmf.SoluteWaterIntegrator(states.solutes, wsolver, ssolver, states)

class ReachNetwork:
    def __init__(self, levels=3):
        self.rtype = cmf.TriangularReach(100)
        self.project = p = cmf.project('X')
        self.root = p.NewReach(0, 0, 0, self.rtype)
        self.outlet = p.NewOutlet('out', 0, -100, -1)
        self.root.set_outlet(self.outlet)
        self.root.Name = f'o'
        self.levels = levels
        self.reaches = self.make_upstream(1, self.root) + [self.root]
        self.neumann_boundaries = [
            cmf.NeumannBoundary.create(r)
            for r in self.reaches
        ]

    def __len__(self):
        return len(self.reaches)

    def __iter__(self):
        return iter(self.reaches)

    def __getitem__(self, item):
        return self.reaches.__getitem__(item)

    def set_inflow(self, total_flow):
        flow = total_flow / len(self.neumann_boundaries)
        for nb in self.neumann_boundaries:
            nb.flux = flow

    def make_upstream(self, level, *downstream):
        upstream = []
        p = self.project
        X = p.solutes[0] if p.solutes else None
        if level <= self.levels:
            for rd in downstream:
                u1 = p.NewReach(rd.position.x - 50 / level ** 2, level * 100, level, self.rtype)
                u1.Name = rd.Name + 'l'
                u1.set_downstream(rd)
                u2 = p.NewReach(rd.position.x + 50 / level ** 2, level * 100, level, self.rtype)
                u2.Name = rd.Name + 'r'
                u2.set_downstream(rd)
                if X:
                    u1[X].source = 1
                    u1[X].set_abs_errtol(100)
                    u2[X].set_abs_errtol(100)
                upstream.extend((u1, u2))
            return self.make_upstream(level + 1, *upstream) + upstream
        else:
            return []


class Model:
    def __init__(self, levels, solver_class, total_inflow=100.0):
        self.network = ReachNetwork(levels)
        self.network.set_inflow(total_inflow)
        self.solver = solver_class(self.network.project)
        self.solver.use_OpenMP = False
        # self.solver.initialize()

    def __call__(self, time=cmf.day):
        self.solver(time)
        return self

    def depth(self):
        return np.array([r.depth for r in self.network])

    def outflux(self):
        return self.network.outlet(self.solver.t)


if __name__ == '__main__':
    from pylab import imshow, show

    print(f'{"solver":<25}level size {"init sec":<10}{"run sec":<10}{"method calls":<15}')
    for solver_type in [cmf.CVodeKLU, cmf.CVodeKrylov, cmf.CVodeAdams]:
        for level in range(1, 12):
            tstart = time.time()
            model = Model(level, solver_type, 100)
            tinit = time.time() - tstart
            for i in range(24):
                model(cmf.h)
                model.network.set_inflow(50 + 50 * (i % 2))
            elapsed = time.time() - tstart - tinit
            if hasattr(model.solver, 'info'):
                size = model.solver.info.size
                dxdt_method_calls = model.solver.info.dxdt_method_calls
            else:
                size = model.solver.size()
                dxdt_method_calls = -9999
            name = model.solver.to_string()
            print(f'{name:>25}{level:6d}{size:5d}{tinit:10.2f}{elapsed:10.2f}{dxdt_method_calls:15d}')
