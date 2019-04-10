# -*- coding: utf-8 -*-
"""
Created on Thu Apr 10 21:33:35 2014

@author: kraft-p
"""

import cmf
# project with three tracers
p = cmf.project('No Linear Freundlich Langmuir')
N, X, Y, Z = p.solutes

# A water storage without specific properties
ws = p.NewStorage('storage',0,0,0)
# 1m3 water as initial condition
ws.state = 1.0

# The storage should contain 1 unit of every tracer for the beginning
for s in p.solutes:
    ws[s].conc = 1.0

# Tracer N does not adsorb, nothing to change
# Tracer X has a linear isotherm xa/m=Kc, with K = 1 and sorbent mass m = 1
ws[X].set_adsorption(cmf.LinearAdsorption(1, 1))
# Tracer Y has a Freundlich isotherm xa/m=Kc^n, 
# with K = 1 and n=0.5 and sorbent mass m = 1
# ws[Y].set_adsorption(cmf.FreundlichAdsorbtion(1., 1., 1.0))
# Tracer Y has a Langmuir isotherm xa/m=Kc/(1+Kc), 
# with K = 1 and sorbent mass m = 1
ws[Z].set_adsorption(cmf.LangmuirAdsorption(1., 1.))

# Now we put a constant clean water flux into the storage
inflow = cmf.NeumannBoundary.create(ws)
inflow.flux = 1.0 # 1 m3/day
for s in p.solutes:
    inflow.concentration[s] = 0.0
# And an outlet, a linear storage term with a retention time of 1 day
outlet = p.NewOutlet('out', 0, 0, 0)
cmf.LinearStorageConnection(ws, outlet, 1.)

# Create a solver
solver = cmf.CVodeAdams(p)

# Rinse the storage for 1 week and get the outlet concentration at every hour
# and the remaining tracer in the storage
result = [[outlet.conc(t, s) for s in p.solutes]
          + [ws[s].state for s in p.solutes]
          for t in solver.run(solver.t, 2 * cmf.week, cmf.h)
          ]
# Plot result        ]
from pylab import *
result = array(result)
conc = result[:, :len(p.solutes)]
load = result[:, len(p.solutes):]
subplot(211)
plot(conc)
grid()
ylabel('Concentration')
xlabel('Time [h]')
legend([str(s) for s in p.solutes])
subplot(212)
plot(load)
grid()
ylabel('Load in storage')
xlabel('Time [h]')
legend([str(s) for s in p.solutes])

show()
