# -*- coding: utf-8 -*-
"""
Created on Thu Dec 05 15:04:24 2013

@author: kraft-p
"""

import cmf
# Create project with space delimited tracer names
p = cmf.project('X Y Z')
# Get the tracers as variables
X,Y,Z = p.solutes

# Create a single cell c with a surfacewater storage, which references 3 solute storages
c = p.NewCell(0,0,0,1000,with_surfacewater = True)
# Create 50 layers with 2cm thickness
for i in range(50):
    # Add a layer. Each layer will reference 3 solute storages
    c.add_layer((i+1)*0.02, cmf.VanGenuchtenMualem())
# Use Richards equation
c.install_connection(cmf.Richards)
# Use a constant rainfall of 50 mm
c.set_rainfall(100.)
# Make a groundwater boundary condition
gw = p.NewOutlet('gw',0,0,-1.5)
cmf.Richards(c.layers[-1],gw)

# Template for the water solver
wsolver = cmf.CVodeIntegrator(1e-9)
# Template for the solute solver
ssolver = cmf.ImplicitEuler(1e-9)
# Creating the SWI, the storage objects of the project are internally assigned to the correct solver
solver = cmf.SoluteWaterIntegrator(p.solutes, wsolver,ssolver,p)

c.saturated_depth = 1.5
#c.surfacewater.depth = 0.5
c.layers[0].conc(X,1.)
c.layers[15].conc(Y,1.)
c.layers[30].conc(Z,1.)

# Save wetness and concentration of all layers
conc = []
wetness = []
# save groundwater recharge
recharge=[]
# save concentration of recharge
crecharge=[]
# Run for one week with hourly time step
for t in solver.run(solver.t,solver.t + cmf.week,cmf.h):
    # Get concentration of all layers
    conc.append([[l.conc(T) for T in p.solutes] for l in c.layers])
    wetness.append([l.wetness for l in c.layers])
    # Get water balance of groundwater
    recharge.append(gw.waterbalance(t))
    crecharge.append([gw.conc(t,T) for T in p.solutes])

# Plot the result
import numpy as np
import pylab as plt

ax1 = plt.subplot(511)
plt.plot(recharge,'k:')
plt.legend(['water'],loc=2)
plt.twinx()
plt.plot(crecharge)
plt.legend(['X','Y','Z'],loc=1)
plt.subplot(512,sharex=ax1)
plt.imshow(np.transpose(wetness),cmap=plt.cm.jet_r,aspect='auto')
plt.ylabel('wetness')
for i in range(3):
    plt.subplot(513+i,sharex=ax1)
    plt.imshow(np.transpose(conc)[i],cmap=plt.cm.copper,aspect='auto',vmax=0.1)
    plt.ylabel(p.solutes[i])
plt.show()    
