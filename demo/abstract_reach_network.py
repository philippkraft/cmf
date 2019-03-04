import cmf
import numpy as np

class ReachNetwork:
    def __init__(self, levels=3):
        self.rtype = cmf.TriangularReach(10)
        self.project = p = cmf.project()
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
        if level <= self.levels:
            for rd in downstream:
                u1 = p.NewReach(rd.position.x - 50 / level ** 2, level * 100, level, self.rtype)
                u1.Name = rd.Name + 'l'
                u1.set_downstream(rd)
                u2 = p.NewReach(rd.position.x + 50 / level ** 2, level * 100, level, self.rtype)
                u2.Name = rd.Name + 'r'
                u2.set_downstream(rd)
                upstream.extend((u1, u2))
            return self.make_upstream(level + 1, *upstream) + upstream
        else:
            return []


class Model:
    def __init__(self, levels, solver_class, total_inflow=100.0):
        self.network = ReachNetwork(levels)
        self.network.set_inflow(total_inflow)
        self.solver = solver_class(self.network.project)
        self.solver.initialize()

    def __call__(self, time=cmf.day):
        self.solver(time)
        return self

    def depth(self):
        return np.array([r.depth for r in self.network])

    def outflux(self):
        return self.network.outlet(self.solver.t)


if __name__ == '__main__':
    dense = Model(5, cmf.CVodeDense)(cmf.day)
    sps = cmf.sparse_structure(dense.network.project.get_states())
    klu = Model(5, cmf.CVodeKLU)(cmf.day)

