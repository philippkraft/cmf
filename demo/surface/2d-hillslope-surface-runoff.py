# -*- coding: utf-8 -*-
"""
Created on Fri Dec  9 10:11:47 2022

@author: philipp
"""

import cmf
import numpy as np
from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
import time

cmf.set_parallel_threads(2)
l_per_s = 86.4


def random_grid(length=6, width=3, cellsize=0.1, slope=0.1, roughness=0.01, gap_fraction=0.0):
    """
    Creates a random grid

    Parameters
    ----------
    length : float, optional
        Length of slope in m. The default is 6.
    width : float, optional
        width of slope in m. The default is 3.
    cellsize : float, optional
        Resolution in m. The default is 0.1.
    slope : float, optional
        Slope of the hillslope in m/m. The default is 0.1.
    roughness : float, optional
        Standard deviation of noise in m. The default is 0.01.
    gap_fraction : float, optional
        The frequency of obstacles 0..1 [-]. The default is 0 - no obstacles.

    Returns
    -------
    np.ndarray
        An 3d array[3,...,...] with the x, y and z 2d grids.

    """
    y, x = np.mgrid[0:length:cellsize, 0:width:cellsize]
    z = y * -slope + np.random.normal(scale = roughness, size=x.shape)
    if gap_fraction:
        gaps = np.random.uniform(size=x.shape) < gap_fraction
        z[gaps] = np.NaN
    
    return np.array([x, y, z])
    

class IWGBerghausen:
    
    def __init__(self, grid, runoff=23.0, solver_class=None, solute=None):
        
        self.project = cmf.project(solute or '')
        
        self.cells, self.shape = self.create_grid(grid) 
        
        self.create_upper_boundary(runoff)
        self.create_lower_boundary()
        solver_class = solver_class or cmf.CVodeAdams

        self.solver = solver_class(self.project)
        if hasattr(self.solver, 'initialize'):
            self.solver.initialize()
        
    def scaling(self):
        cellsize = self.cells[0,1].x - self.cells[0,0].x, self.cells[1,0].y - self.cells[0,0].y
        return cellsize + (
            self.cells[0, self.shape[1]-1].x - self.cells[0, 0].x + cellsize[0],
            self.cells[self.shape[0]-1, 0].y - self.cells[0, 0].y + cellsize[1]
        )
        
        
    def create_grid(self, grid):
        x, y, z = grid[:3]
        cellsize_x = x[0, 1] - x[0, 0]
        cellsize_y = y[1, 0] - y[0, 0]
        cells = {
            (r, c): self.project.NewCell(x[r, c], y[r, c], z[r, c], cellsize_x * cellsize_y, True)
            for c in range(y.shape[1])
            for r in range(y.shape[0])
            if not np.isnan(z[r, c])
        }
        
        for r, c in cells:
            for pos, cellsize in zip([(r, c-1), (r-1,c)], [cellsize_x, cellsize_y]):
                if pos in cells:
                    cells[r, c].topology.AddNeighbor(cells[pos], cellsize)
        
        if grid.shape[0] > 3:
            for r,c in cells:
                cells[r, c].surfacewater.depth = grid[-1, r, c]
                
        for X in self.project.solutes:
            for r, c in cells:
                cells[r, c].surfacewater[X].conc = 0.1
                cells[r, c].surfacewater[X].set_abs_errtol(100)
            
            
        cmf.connect_cells_with_flux(self.project.cells, cmf.DiffusiveSurfaceRunoff)
        for c in self.project.cells:
            c.surfacewater.puddledepth = 0.0
            c.surfacewater.nManning = 0.05
                    
        return cells, x.shape
    
    def create_upper_boundary(self, runoff):
        cellsize_x, cellsize_y, width, length = self.scaling()
        # Create Neumann boundary in center of first row
        self.upper_boundary = self.project.NewNeumannBoundary('Pump', self.project.cells[0].surfacewater) 
        self.upper_boundary.position = (width / 2, - cellsize_x / 2, 0)
        self.upper_boundary_cells = [self.cells[r, c] for r, c in self.cells if r==0 ]
        # Transform runoff in l/s total to m³/day/cell
        self.upper_boundary.flux = runoff * l_per_s / len(self.upper_boundary_cells)
        for X in self.project.solutes:
            self.upper_boundary.concentration[X] = 1.0
            
        for c in self.upper_boundary_cells:
            self.upper_boundary.connect_to(c.surfacewater)
            
    def create_lower_boundary(self):
        cellsize_x, cellsize_y, width, length = self.scaling()
       
        self.lower_boundary = self.project.NewOutlet(
            'Outlet', 
            width / 2, #  x
            length,    #  y
            min(c.z for c in self.project)  # z
        )
        self.lower_boundary_cells = [self.cells[r, c] for r, c in self.cells if r == self.shape[0] - 1 ]
        
        for c in self.lower_boundary_cells:
            cmf.WaterbalanceFlux(c.surfacewater, self.lower_boundary)
            
    def getoutflow(self):
        return self.lower_boundary(self.solver.t) / l_per_s

        
    def getdepth(self):
        res = np.zeros(self.shape, dtype=float) * np.NAN
        for pos, cell in self.cells.items():
            res[pos] = cell.surfacewater.depth
        return res
    
    def getconc(self):
        X = self.project.solutes[0]
        res = np.zeros(self.shape, dtype=float) * np.NAN
        for pos, cell in self.cells.items():
            res[pos] = cell.surfacewater[X].conc
        return res
        
            
    def gethead(self):
        res = np.zeros(self.shape, dtype=float) * np.NAN
        for pos in self.cells:
            cell = self.cells[pos]
            res[pos] = cell.surfacewater.depth + cell.z
        return res
    
    def getflux(self, t=None):
        return cmf.cell_flux_directions(self.project, t or self.solver.t)
    
    def extent(self):
        cs05 = self.cellsize / 2
        return -cs05, self.width - cs05, -cs05, self.length - cs05
    
class Plot:
    def __init__(self, model, callback=None, 
                 clim=None, cmap=plt.cm.Blues,
                 scale=None):
        self.model = model
        self.callback = callback or self.model.getdepth
        p = model.project
        self.fig, self.ax = plt.subplots(figsize=(8,12))
        data = self.callback()
        self.clim = clim or (np.nanmin(data), np.nanmax(data))
        self.img = self.ax.imshow(data, cmap=cmap, aspect='equal', origin='lower',
                        vmin=self.clim[0], vmax=self.clim[1], interpolation='none',
                        extent=model.extent())
        pos = cmf.cell_positions(p.cells)
        fluxdir = model.getflux()
        self.quiver = self.ax.quiver(pos.X, pos.Y,
                           fluxdir.X, fluxdir.Y,
                           scale=scale, zorder=10, pivot='middle', color='w')
        self.title = self.ax.set_title(model.solver.t, size=16)
        self.colorbar = self.fig.colorbar(self.img)
        
    def __call__(self, t=None):        
        self.img.set_array(self.callback())
        self.colorbar.vmin = self.clim[0]
        self.colorbar.vmax = self.clim[1]
        self.img.set_clim(self.clim)
        t = t or self.model.solver.t
        fluxdir = cmf.cell_flux_directions(self.model.project, t)
        self.quiver.set_UVC(fluxdir.X, fluxdir.Y)
        self.title.set_text('%s/%s %g l/s' % (t, self.model.solver.dt, self.model.getoutflow()))
        print(self.title.get_text())
        return self.img, self.quiver, self.title
    
    def animate(self, until, dt=cmf.sec):
        
        iterator = iter(self.model.solver.run(cmf.Time(), until, dt))
        return FuncAnimation(self.fig, self, frames=iterator, repeat=False)


def run_to_array(m, until, dt):
    t0 = time.time()
    res = np.zeros((int(until / dt), *m.shape)) * np.NAN
    def getvalue(m):
        if m.project.solutes:
            return m.getconc()
        else:
            return m.getdepth()
    res[0] = getvalue(m)
    for i, t in enumerate(
            m.solver.run(cmf.sec * 0, until, dt)
        ):
        print(f'{t} : out: {m.getoutflow():0.3f}')
        res[i] = getvalue(m)
    print('-' * 40)
    tt = time.time() - t0
    print(f'{tt:0.2f} s for {m.shape} cells')
    return res
        
        
if __name__ == '__main__':

    until = cmf.sec * 60
    dt = cmf.sec * 0.1
    #grid = random_grid(6, 3, 0.1, slope=0.1, roughness=0.01, gap_fraction=0)
    grid = np.load('berghausen_25cm_steadystate.npy')
    m = IWGBerghausen(grid, runoff=10, solver_class=cmf.CVodeKLU, solute='X')
    #a = run_to_array(m, until, dt)
    print(type(m.solver).__name__)
    print(m.solver.get_info())
    # p = Plot(m, clim=(0,0.1), cmap=plt.cm.cividis, scale=3000)

    # anim = p.animate(cmf.sec * 60, cmf.sec * 0.1)
    # anim.save('waterdepth.apng')
    #plt.show()
    # np.save('waterdepth.npy', run_to_array(until, dt))