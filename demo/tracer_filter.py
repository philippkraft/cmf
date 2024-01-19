"""
Shows the effect of a solute filtering flux connection
"""
import cmf
import datetime


p = cmf.project('no_filter filter')
NF, F = p.solutes
# Create W1 in project p 
W1 = p.NewStorage(name="W1", x=0, y=0, z=0)
# Create W2 in project p without any volume as an initial state
W2 = p.NewStorage(name="W2", x=10, y=0, z=0)
# Create a linear storage equation from W1 to W2 with a residence time tr of one day
q = cmf.LinearStorageConnection(source=W1, target=W2, residencetime=1.0)
# Set the initial state of w1 to 1m³ of water.
q.set_tracer_filter(F, 0.5)
# %%
W1.volume = 1.0
W1[NF].state = 1.0
W1[F].state = 1.0

W2.volume = 0.0
W2[NF].state = 0.0
W2[F].state = 0.0
# Create an integrator for the ODE represented by project p, with an error tolerance of 1e-9
solver = cmf.CVodeAdams(p, 1e-9)
# Import Python's datetime module

# Set the intitial time of the solver
solver.t = datetime.datetime(2012, 1, 1)
# %%
result = [[W1[NF].state, W2[NF].state, W1[F].state, W2[F].state] for t in
          solver.run(datetime.datetime(2012, 1, 1), datetime.datetime(2012, 1, 7), datetime.timedelta(hours=1))]
import pylab as plt

plt.plot(result)
plt.xlabel('hours')
plt.ylabel('Tracer mass in $g$')
plt.legend(('W1 unfiltered', 'W2 unfiltered', 'W1 filtered', 'W2 filtered'))
plt.show()
