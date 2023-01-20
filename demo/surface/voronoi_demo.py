import cmf
import cmf.geometry
from cmf.geometry import irregular_grid as cmf_ig

import numpy as np
from shapely.geometry import Polygon

def create_points(n_cells, width, height, slope, roughness):
    x = np.random.uniform(0, width, n_cells)
    y = np.random.uniform(0, height, n_cells)
    z = y * slope + np.random.normal(scale=roughness, size=n_cells)
    return x, y, z


def run(project, until, dt, solver=cmf.CVodeKLU):
    for c in project:
        c.surfacewater.depth = 0.0
    if type(solver) is type:
        solver = solver(project)
    for t in solver.run(solver.t, until, dt):
        print(t)
    print(solver.get_info())
    return solver


def add_boundaries(project: cmf.project, flux=10):
    in_cell = max(project, key=lambda c: c.z)
    out_cell = min(project, key=lambda c: c.z)

    in_boundary = project.NewNeumannBoundary('inflow', in_cell.surfacewater)
    out_boundary = project.NewOutlet('outflow', out_cell.x, out_cell.y - 0.5, out_cell.z - 0.01)
    cmf.DiffusiveSurfaceRunoff(out_cell.surfacewater, out_boundary, 1)

    return in_boundary, out_boundary


if __name__ == '__main__':

    x, y, z = create_points(250, 10, 10, 0.1, 0.01)
    p = cmf.project()
    cells = cmf_ig.voronoi_cells(p, x, y, z, 0.05)
    cmf.connect_cells_with_flux(p, cmf.DiffusiveSurfaceRunoff)
    b_in, b_out = add_boundaries(p, 10.0)

    solver = run(p, cmf.min, cmf.sec, cmf.CVodeKLU)




