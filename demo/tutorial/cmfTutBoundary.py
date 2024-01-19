# -*- coding: utf-8 -*-
"""
Created on Mon Dec 09 09:50:35 2013

@author: kraft-p
"""

import cmf
import datetime

p = cmf.project()
W1 = p.NewStorage(name="W1", x=0, y=0, z=0)
W2 = p.NewStorage(name="W2", x=10, y=0, z=0)
q = cmf.LinearStorageConnection(source=W1, target=W2, residencetime=5.0)
W1.volume = 1.0
Out = p.NewOutlet(name="Outlet", x=20, y=0, z=0)
q2 = cmf.LinearStorageConnection(source=W1, target=Out, residencetime=1.0)
qout = cmf.LinearStorageConnection(source=W2, target=Out, residencetime=.5)
# Create a Neumann Boundary condition connected to W1
In = p.NewNeumannBoundary('In', W1)
# Create a timeseries with daily alternating values.
In.flux = cmf.timeseries(begin=datetime.datetime(2012, 1, 1),
                         step=datetime.timedelta(days=1),
                         interpolationmethod=0)
for i in range(1000):
    # Add 0.0 m3/day for even days, and 1.0 m3/day for odd days
    In.flux.add(i % 2)
# Create the solver
solver = cmf.ImplicitEuler(p, 1e-9)
import datetime

# Iterate the solver hourly through the time range and return for each time step the volume in W1 and W2
result = [[W1.volume, W2.volume] for t in
          solver.run(datetime.datetime(2012, 1, 1), datetime.datetime(2012, 2, 1), datetime.timedelta(hours=1))]
import pylab as plt

plt.plot(result)
plt.xlabel('hours')
plt.ylabel('Volume in $m^3$')
plt.legend(('W1', 'W2'))
plt.show()
