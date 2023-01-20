"""

"""

import numpy as np
import cmf
import rasterio
import time


def load_dem(filename='dem10.dem') -> rasterio.DatasetReader:
    return rasterio.open(filename, mode='r', driver='AAIGrid')


class Setup:

    def __call__(self, dt=cmf.day, solver_type=cmf.CVodeKLU):
        if not self.solver:
            self.solver = solver_type(self.project)
            self.solver.t = cmf.Time(1, 1, 2019)
        self.solver(self.solver.t + dt)
        return self

    def __init__(self, dem: rasterio.DatasetReader, runoff_class=cmf.KinematicSurfaceRunoff):

        self.solver = None
        self.project = p = cmf.project()
        self.cells = {}
        self.shape = dem.shape
        dd = dem.read(1, masked=True)
        for row in range(dem.shape[0]):
            for col in range(dem.shape[1]):
                if np.isfinite(dd[row, col]):
                    x, y = dem.xy(row, col)
                    new_cell = self.project.NewCell(x, y, dd[row, col], dem.res[0] * dem.res[1], True)
                    self.cells[(row, col)] = new_cell
                    for r, c in [(0, -1), (-1, -1), (-1, 0), (-1, +1)]:
                        n = self.cells.get((row + r, col + c))
                        if n:
                            f = 0.25 if (r and c) else 0.75
                            new_cell.topology.AddNeighbor(n, dem.res[1] * f)

        for c in self.project:
            l = c.add_layer(c.z)
            l.soil.Ksat = 1.0
            l.soil.porosity = 0.1
            l.volume = 0
            l.volume = l.get_capacity() * 0.9
            cmf.GreenAmptInfiltration(l, c.surfacewater)

        self.outcell = min(self.project, key=lambda c: c.z)
        cmf.connect_cells_with_flux(p, cmf.Darcy)
        for c in p:
            if c.main_outlet:
                runoff_class(c.surfacewater, c.main_outlet.surfacewater, c.topology.flowwidth(c.main_outlet))

        self.outlet = p.NewOutlet('outlet', self.outcell.x, self.outcell.y - 10, self.outcell.z - 0.11)
        runoff_class(self.outcell.surfacewater, self.outlet, 7.5)

    def __getitem__(self, index):
        return self.project[index]

    def __str__(self):
        sd = [c.saturated_depth for c in self.project]
        sd_min = min(sd)
        sd_max = max(sd)
        sd_mean = np.mean(sd)
        if self.project.reaches:
            out_depth = self.project.reaches[-1].depth
        else:
            out_depth = self.outcell.surfacewater.depth
        return ', '.join([
            f'{self.t:%Y-%m-%d %H:%M:%S}',
            f'{self.outlet(self.t):0.2f} m³/day',
            f'runoff depth = {out_depth * 100:0.1f} cm',
            f'sat-depth = {sd_max:0.2f}..{sd_mean:0.2f}..{sd_min:0.2f} m'
        ])

    def __get_t(self):
        if self.solver:
            return self.solver.t
        else:
            return cmf.Time()

    def __set_t(self, t: cmf.Time):
        if self.solver:
            self.solver.t = t

    def make_reaches(self, contrib_threshold=2e3):
        p = self.project
        cmf.Topology.calculate_contributing_area(p.cells)
        reach_cells = [c for c in p if c.contributing_area >= 2e3]
        reaches = {}
        for rc in reach_cells:
            if rc.main_outlet:
                l = cmf.distance(rc, rc.main_outlet)
                reaches[rc] = self.project.NewReach(rc.x, rc.y, rc.z - 0.1, cmf.TriangularReach(l))
                reaches[rc].Name = f'R{rc.Id}'
                rc.surfacewater.connection_to(rc.main_outlet.surfacewater).kill_me()
            elif rc == self.outcell:
                l = cmf.distance(rc, self.outlet.position)
                reaches[rc] = self.project.NewReach(rc.x, rc.y, rc.z - 0.1, cmf.TriangularReach(l))
                reaches[rc].Name = f'R{rc.Id}'
                rc.surfacewater.connection_to(self.outlet).kill_me()
            else:
                print(f"WARNING: {rc} has no outlet and is not the outcell")

        for rc in reach_cells:
            if rc.main_outlet in reaches:
                cmf.KinematicSurfaceRunoff(rc.surfacewater, reaches[rc], 10, 5)
                reaches[rc].set_downstream(reaches[rc.main_outlet])
            elif rc == self.outcell:
                cmf.KinematicSurfaceRunoff(rc.surfacewater, reaches[rc], 10, 5)
                reaches[rc].set_outlet(self.outlet)
            else:
                print(f"WARNING: {reaches[rc]} has no outlet reach")

    def as_raster(self, predicate):
        res = np.zeros(self.shape) / 0.0
        for row, col in self.cells:
            res[row, col] = predicate(self.cells[(row, col)])
        return res

    t = property(__get_t, __set_t)


if __name__ == '__main__':
    threads = 1
    cmf.set_parallel_threads(threads)
    dem = load_dem()
    p = Setup(dem)
    p.make_reaches()
    tstart = time.time()
    p(cmf.day, cmf.CVodeKLU)
    duration = time.time() - tstart
    # print(p)
    print(f'{p.solver.to_string()}, {threads} thread(s): {duration:0.3f} s')
    # print(p.solver.info)
