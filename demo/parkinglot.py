# -*- coding: utf-8 -*-
# %%
import cmf
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Create a project
p = cmf.project()
cells = {}
slope = 0.001
size = (20, 20)
length = 1.
# Create a 20x20 grid of cells with constant slope

for j in range(size[1]):
    for i in range(size[0]):
        x, y = i * length, j * length
        # Create cells with slope in x+y direction
        offset = np.random.uniform(0, .01)  # if not is_border else -0.05
        z = (x + y) * slope + offset

        c = p.NewCell(x, y, z, length ** 2)
        # Store cells with position in dictionary        
        cells[i, j] = c
        # Set up topology (4 Neighbors)
        if i:
            c.topology.AddNeighbor(cells[i - 1, j], length)
        if j:
            c.topology.AddNeighbor(cells[i, j - 1], length)

for c in p:
    c.surfacewater_as_storage()
    # 1mm puddle depth
    c.surfacewater.puddledepth = 0.001  # np.random.uniform(0.0,0.001)
    # c.surfacewater.depth = 0.002
    # Asphalt        
    c.surfacewater.nManning = 0.015
    c.add_layer(0.1, cmf.VanGenuchtenMualem(Ksat=0.005))
    c.install_connection(cmf.GreenAmptInfiltration)

cmf.DiffusiveSurfaceRunoff.set_linear_slope(1e-4)
cmf.connect_cells_with_flux(p, cmf.DiffusiveSurfaceRunoff)
outlet = p.NewOutlet('outlet', -length, 0, -slope * length)

cmf.DiffusiveSurfaceRunoff(cells[0, 0].surfacewater, outlet, length)


def setrain(rainfall):
    for c in p:
        c.set_rainfall(rainfall)


solver = cmf.CVodeKLU(p, 1e-9)

setrain(10. * 24.)


def getdepth():
    return np.array([[cells[i, j].surfacewater.depth for i in range(size[0])] for j in range(size[1])])


def getpot():
    return np.array([[cells[i, j].surfacewater.depth for i in range(size[0])] for j in range(size[1])])


# %%
fig, ax = plt.subplots()
img = ax.imshow(getdepth(), cmap=plt.cm.Blues, aspect='equal',
                vmin=0.0, vmax=0.01, origin='bottom', interpolation='nearest',
                extent=(-length / 2, (size[0] - .5) * length, -length / 2, length * (size[1] - .5)))
fluxdir = cmf.cell_flux_directions(p, solver.t)
pos = cmf.cell_positions(p.cells)
a = np.array
quiver = ax.quiver(pos.X, pos.Y,
                   fluxdir.X, fluxdir.Y,
                   scale=1000, zorder=10, pivot='middle')
title = ax.text(length, (size[1] - 2) * length, solver.t, size=16)
ax.set_xlim(-length, length * size[0])
ax.set_ylim(-length, length * size[1])
dt = cmf.sec * 10
# %%
qout = cmf.timeseries(solver.t, dt)


def run(data):
    solver(solver.t + dt)
    t = solver.t
    if t > cmf.h:
        setrain(0.0)
    img.set_array(getdepth())
    fluxdir = cmf.cell_flux_directions(p, t)
    quiver.set_UVC(fluxdir.X, fluxdir.Y)
    title.set_text('%s/%s %g mm' % (t, solver.dt, np.mean(c.surfacewater.depth) * 1000))
    qout.add(outlet(t))
    return img, quiver, title


# %%
ani = animation.FuncAnimation(fig, run, frames=int(2 * cmf.h / dt), blit=False, interval=20, repeat=False)

plt.show()
# %%
fig2, ax2 = plt.subplots()
plt.sca(ax2)
plt.plot(qout)
plt.show()
# %%
