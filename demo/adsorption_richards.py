# -*- coding: utf-8 -*-
"""
Created on Thu Apr 10 21:33:35 2014

@author: kraft-p
"""
from __future__ import print_function, division
import cmf
# project with three tracers
p = cmf.project('No Linear Freundlich Langmuir')
N, X, Y, Z = p.solutes

cmf.set_parallel_threads(1)
# Create a single cell c with a surfacewater storage, which references 3 solute storages
c = p.NewCell(0, 0, 0, 1000, with_surfacewater=True)
# Create 50 layers with 2cm thickness
for i in range(50):
    # Add a layer. Each layer will reference 3 solute storages
    l = c.add_layer((i+1)*0.02, cmf.VanGenuchtenMualem())
    # Tracer N does not adsorb, nothing to change
    l[N].state = 1.

    # Tracer X has a linear isotherm xa/m=Kc, with K = 1 and sorbent mass m = 1
    l[X].set_adsorption(cmf.LinearAdsorption(1,5))
    l[X].state = 1.
    # Tracer Y has a Freundlich isotherm xa/m=Kc^n, 
    # with K = 1 and n=0.5 and sorbent mass m = 1
    # l[Y].set_adsorption(cmf.FreundlichAdsorbtion(2., .5, 1.0, 1e-9))
    l[Y].state = 1.

    # Tracer Y has a Langmuir isotherm xa/m=Kc/(1+Kc), 
    # with K = 1 and sorbent mass m = 1
    l[Z].set_adsorption(cmf.LangmuirAdsorption(1.,5.))
    l[Z].state = 1.

# Use Richards equation
c.install_connection(cmf.Richards)
# Make a groundwater boundary condition
gw = p.NewOutlet('gw', 0, 0, -1.5)
cmf.Richards(c.layers[-1], gw)

solver = cmf.CVodeKrylov(p, 1e-9)

c.saturated_depth = 1.5

# Now we put a constant clean water flux into the storage
inflow=cmf.NeumannBoundary.create(c.surfacewater)
inflow.flux = 100.0 # 100 mm/day
for s in p.solutes:
    inflow.concentration[s] = 0.0

# Save wetness and concentration of all layers
conc = []
tracer_state = []
wetness = []
# save groundwater recharge
recharge = []
# save concentration of recharge
crecharge = []

# Run for one week with hourly time step
for t in solver.run(solver.t, solver.t + cmf.week, cmf.h):
    # Get concentration of all layers
    tracer_state.append([[l[T].state for T in p.solutes] for l in c.layers])
    conc.append([[l.conc(T) for T in p.solutes] for l in c.layers])
    wetness.append([l.wetness for l in c.layers])
    # Get water balance of groundwater
    recharge.append(gw.waterbalance(t))
    crecharge.append([gw.conc(t, T) for T in p.solutes])
    print(t)


# Plot the result
import numpy as np
import pylab as plt

pc = len(p.solutes) + 2
ax1 = plt.subplot(pc, 1, 1)
plt.plot(recharge, 'k:')
plt.legend(['water'], loc=2)
plt.twinx()
plt.plot(crecharge)
plt.legend([str(s) for s in p.solutes],loc=1)
plt.subplot(pc, 1, 2, sharex=ax1)
plt.imshow(np.transpose(wetness), cmap=plt.cm.RdYlBu, aspect='auto')
plt.ylabel('wetness')
for i,s in enumerate(p.solutes):
    plt.subplot(pc, 1, 3+i, sharex=ax1)
    plt.imshow(np.transpose(tracer_state)[i],
               cmap=plt.cm.viridis, aspect='auto',
               vmax=1.0, vmin=0.0)
    plt.ylabel(s)
plt.show()    
