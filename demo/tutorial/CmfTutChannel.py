# -*- coding: utf-8 -*-
"""
Code from the channeld flow tutorial http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/CmfTutChannel

Creates 100 reaches of 10m length and calculates a single wave through the river.

Inital condition:
River is empty (r.volume=0.0), except for the first element with a depth of 10cm

Created on Wed Mar 05 15:44:00 2014

@author: kraft-p
"""
from __future__ import print_function, division
import cmf
import time
from datetime import datetime, timedelta
import pylab
import numpy as np

p = cmf.project()

slope = 0.0001
segment_length = 10
# Make a river geometry
river_geom = cmf.TriangularReach(segment_length, 2)
# Sets the Manning n for the geometry
river_geom.set_nManning(0.035)

# Create a 1km river with 100 reaches along the x axis and a constant slope of 1%
# Copies river_geom into the new reach
reaches = []
for x in pylab.arange(0, 1000, segment_length):
    r = p.NewReach(x=x, y=0, z=x * slope,
                   # Copy the river crosssection geometry into the reach
                   shape=river_geom,
                   # Use a kinematic wave for set_downstream connections
                   diffusive=True)
    r.Name = 'R%03i' % int(x)
    reaches.append(r)

# connect the reaches    
for r_lower, r_upper in zip(reaches[:-1], reaches[1:]):
    r_upper.set_downstream(r_lower)

# Make an outlet at the lowest reach (reaches[0])
r_out = reaches[0]
outlet = p.NewOutlet('Out', x=r_out.position.x - r_out.length, y=0, z=r_out.position.z - r_out.length * slope)
# Connect river to outlet
r_out.set_outlet(outlet)

# Initial condition: 
# Empty river
for r in reaches:
    r.depth = r.position.x / 1000.
# 10cm of water in the most upper reach
reaches[-1].depth = 0.1

# Create a solver
solver = cmf.CVodeKrylov(p, 1e-9)
# We store the results in this list
depth = [[r.potential for r in reaches]]
flux = [[r_upper.flux_to(r_lower, solver.t) for r_upper, r_lower in zip(reaches[:-1], reaches[1:])]]
wet_area = [[np.mean([r.wet_area() for r in rr]) for rr in zip(reaches[:-1], reaches[1:])]]
# Track runtime
tstart = time.time()
# Run the model for 3 h with dt=1 min
for t in solver.run(datetime(2012, 1, 1), datetime(2012, 1, 8, 0), timedelta(minutes=30)):
    depth.append([r.potential for r in reaches])
    flux.append([r_upper.flux_to(r_lower, solver.t) for r_upper, r_lower in zip(reaches[:-1], reaches[1:])])
    wet_area.append([np.mean([r.wet_area() for r in rr]) for rr in zip(reaches[:-1], reaches[1:])])
    print('{t:16s} depth at outlet: {d:0.3f}cm'.format(t=str(t), d=r_out.depth * 100))

print('Calculation time: {:0.1f}s, RHS-evaluations: {}'.format(time.time() - tstart, solver.info.rhs_evaluations))

depth = np.array(depth)
# Plot the result (matplotlib needs to be installed)
x = [r.position.x for r in reaches]
for t in range(0, len(depth), 60):
    pylab.plot(x, depth[t])
pylab.xlabel('river length in m')
pylab.ylabel('Water level in m')
pylab.show()
