@page CmfTutSoluteTransport1D

[index...](@ref tutorial) [back...](@ref CmfTut1d)

# 1D Richards equation model with tracer transport

In this tutorial, we will introduce tracer in the 1D column model from
the [last tutorial](@ref CmfTut1d). Tracer transport in cmf is up to now
only advective using the equations given
[here](wiki:FiniteVolumeMethod#solutetransport).

The amount of a tracer in a water storage is stored in another state
variable, the [SoluteStorage](@ref cmf::water::SoluteStorage). Solute
storages are created automatically with their corresponding water
storages, when the tracer is installed at project creation. For this
example, we will use three tracers, X, Y and Z.

### Creating a project with tracer

~~~~~~~~~~~~~{.py}

from __future__ import division, print_function  # Python 2/3 compatible code
import cmf
# Create project with space delimited tracer names
p = cmf.project('X Y Z')
# Get the tracers as variables
X,Y,Z = p.solutes
~~~~~~~~~~~~~

### Setting up the 1D column model

Now we set up the simple 1D column model from the [last
tutorial](@ref CmfTut1d) with a constant rainfall rate. For every water
storage that is created, three solute storages are created
automatically:

~~~~~~~~~~~~~{.py}

# Create a single cell c with a surfacewater storage, which references 3 solute storages
c = p.NewCell(0,0,0,1000,with_surfacewater = True)
# Create 50 layers with 2cm thickness
for i in range(50):
    # Add a layer. Each layer will reference 3 solute storages
    c.add_layer((i+1)*0.02, cmf.VanGenuchtenMualem())
# Use Richards equation
c.install_connection(cmf.Richards)
# Use a constant rainfall of 100 mm to wash out the tracers fast
c.set_rainfall(100.)
# Make a groundwater boundary condition
gw = p.NewOutlet('gw',0,0,-1.5)
cmf.Richards(c.layers[-1],gw)
~~~~~~~~~~~~~

## Solving a system with tracer transport

There are two possibilities to solve a system with tracer transport:

1\. Solving the whole system as one including the water storages and the
solute storages as state variables 1. Use one solver for the water
transport and one solver for each tracer.

### 1\. One solver

Since our system is stiff (see [here](@ref CmfTutSolver) and [last
tutorial](@ref CmfTut1d)) the best choice is the CVode solver. The setup
is simple:

~~~~~~~~~~~~~{.py}

solver = cmf.CVodeIntegrator(p, 1e-9)
print(solver.size())
~~~~~~~~~~~~~

The print command shows, that the system to be solved is for a single
soil column quite big (`(50 soillayers + 1 surface water storage)\*(1
water storage + 3 solute storages) = 204 storages`). A second problem
with this approach is, that closely related storages (soillayers) are
not directly beneath each other in the list of storages. This makes the
numerical solution of the system much more computational demanding.
However, the error checking capabilities of the solver is used for the
whole system, thus the result is numerically reliable.

### 2\. Seperated solvers for water and tracers

The setup for seperated solvers is only a bit more complicated, if one
uses the
[SoluteWaterIntegrator](@ref cmf::math::SoluteWaterIntegrator) (SWI
for further use). The SWI implements the interface of cmf integrators,
but uses in our case for solvers internally, one for the water
transport, and 3 for the 3 tracers. The SWI is created using a template
for the water solver and one template for the solute solvers. When the
SWI should integrate over a given timespan (eg. one hour), the water
integrator is run first for a single timestep, that might be shorter
than one hour for convergence reasons. Then the solute sovers are run
after that for the same timestep. Since the boundary conditions of the
solute system change at each time step by the water transport, multistep
methods, like CVode or [BDF2](@ref cmf::math::BDF2) need to be reset
at each internal timestep. Hence, single step methods, like
[ImplicitEuler](@ref cmf::math::ImplicitEuler) are a good choice for
the solute transport. However, you encouraged to do your own tests with
different solvers.

Be aware, when doing such an operator split, the error tolerance given
for each solver is not globally valid, the numerical error for the whole
system will be larger. In general, the numerical dispersion of tracers
seem to increase a bit by using a SWI instead of a single solver for the
whole system.

~~~~~~~~~~~~~{.py}

# Template for the water solver
wsolver = cmf.CVodeIntegrator(p,1e-9)
# Template for the solute solver
ssolver = cmf.ImplicitEuler(p,1e-9)
# Creating the SWI, the storage objects of the project are internally assigned to the correct solver
solver = cmf.SoluteWaterIntegrator(p.solutes, wsolver,ssolver,p)
~~~~~~~~~~~~~

## Run the model

### Inital conditions

As initial conditions, we use the hydrostatic equilibrium for 1.5 m
groundwater level, and a solute concentration of 1 g/m3 for X in the
first layer, 1 g/m3 for Y in the 10th layer, 1g/m3 for Z in the 20th
layer

~~~~~~~~~~~~~{.py}

c.saturated_depth = 1.5
c.layers[[0].conc(X,1.)|c.layers[10]].conc(Y,1.)
c.layers[20].conc(Z,1.)
~~~~~~~~~~~~~

### Run the model

The model runtime can be like this:

~~~~~~~~~~~~~{.py}

# Save wetness and concentration of all layers
conc = [[]|wetness = []]
# save groundwater recharge
recharge=[]
# save concentration of recharge
crecharge=[]
# Run for one week with hourly time step
for t in solver.run(solver.t,solver.t + cmf.week,cmf.h):
    # Get concentration of all layers
    conc.append([[l.conc(T) for T in p.solutes] for l in c.layers])
    wetness.append([[l.wetness|for l in c.layers]])
    # Get water balance of groundwater
    recharge.append(gw.waterbalance(t))
    crecharge.append([[gw.conc(t,T)|for T in p.solutes]])
~~~~~~~~~~~~~

### Plot results

The results should show 5 subplots. The first shows the recharge and the
concentration of the recharge, the second the soil moisture distribution
and the third to fifth the concentration distribution over time. The x
axis shows always time.

~~~~~~~~~~~~~{.py}

# Plot the result
import numpy as np
import pylab as plt

# Plot gw recharge and its concentration
ax1 = plt.subplot(511)
plt.plot(recharge,'k:',lw=2)
plt.legend(['water'],loc=2)
plt.ylabel('recharge in mm/day')

# Plot concentration of recharge in the same plot
plt.twinx()
plt.plot(crecharge)
plt.legend(['X','Y','Z'],loc=1)
plt.ylabel('Conc. in g/m3')

# Plot wetness profile over time
plt.subplot(512,sharex=ax1)
plt.imshow(np.transpose(wetness),cmap=plt.cm.jet_r,aspect='auto')
plt.ylabel('wetness')

# Plot concentration profiles over time. To see more, the color scale is limited to 0.1 g/m3.
for i in range(3):
    plt.subplot(513+i,sharex=ax1)
    plt.imshow(np.transpose(conc)[[i],cmap=plt.cm.copper,aspect='auto',vmax=0.1)|    plt.ylabel(p.solutes[i]])
plt.show()    
~~~~~~~~~~~~~

author: philipp, version: 3 Mon Oct 17 11:45:27 2016
