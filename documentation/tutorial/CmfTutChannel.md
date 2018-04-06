@page CmfTutChannel

[index...](@ref tutorial)

# Channelled flow in cmf

In this chapter, we will create a simple 1D river model using the
kinematic wave equation with Manning's formula as the friction term. The
approach is very similar to surface runoff, hence
\[wiki:CmfTutSurfaceRunoff\] should be read before.

## Manning's formula

[Mannig's formula](WikiPedia:Manning_formula) is an empirical formula to
describe the flow velocity in a channel as a function of the channel
geometry, depth, roughness and slope.


@f[
[q_{Manning}=|A R^{2/3}\frac{\sqrt{s}} {n}
@f]

\]

where:

- @f$A = \frac V l@f$ : Crosssectional area of the wetted
crossection, Volume per length) - @f$R = \frac A {P(d)}@f$ : the
hydraulic radius, given by the wetted crossection area per wetted
perimeter - @f$P(d)@f$ : the perimeter of the wetted crosssection, a
function of reach depth, depending on channel geometry - @f$d(V)@f$ :
the depth of the reach, a function of the volume depending on channel
geometry - @f$s = \frac{\|z_1 - z_2\|}{l}@f$ : Slope of the reach -
@f$n@f$ : Manning friction roughness.

Since the perimeter depends on the geometry of the channel crossection,
this needs to be described. The way cmf handles channel geometry is
given [here](wiki:CmfTutCell#Riversandreaches)

## Kinematic vs. Diffusive

When you are using CMF, you have the choice between the kinematic wave
approach, where the slope of the reach is given by the topographic
slope, or the diffusive wave approach, where the slope is given by the
slope of the water surface. Although the diffusive approach is more
realistic, you are getting serious numerical trouble when using the
diffusive wave. As you can see in the formula above, the flux depends on
the square root of the slope. If the water surface levels out, the
derivative of the square function goes to infinity and becomes hence
infinite sensitive against value. As a result, any error controlled
solver in cmf will fail for diffusive wave approaches when the water
surface slope may go to zero. Unless we find a fix for this behaviour,
please do **not** use diffusive wave approaches.

## Implementation

Create a 1km river with 100 reaches along the x axis and a constant
slope of 1%

~~~~~~~~~~~~~{.py}

from __future__ import division, print_function #  Make code python 2/3 compatible
import cmf
from datetime import datetime,timedelta
p=cmf.project()
# Create a triangular reach crosssection for 10 m long reaches with a bankslope of 2
shape = cmf.TriangularReach(10.,2.)
# Create a 1km river with 100 reaches along the x axis and a constant slope of 1%
reaches = [[p.NewReach(i,0,i*.01,shape,False)|for i in range(0,1000,10)]]
~~~~~~~~~~~~~

Now we have 100 unconnected reaches in our river. To connect them with
kinematic wave equation

~~~~~~~~~~~~~{.py}

for r_lower, r_upper in zip(reaches[:-1],reaches[1:]):
    r_upper.set_downstream(r_lower)
~~~~~~~~~~~~~

Creating a solver and setting initial conditions:

~~~~~~~~~~~~~{.py}

# Create a solver
solver = cmf.CVodeIntegrator(p,1e-9)
# Initial condition: 10 cmf of water in the most upper reach
reaches[-1].depth=0.1
~~~~~~~~~~~~~

As always, the next part is the runtime code, saving the river depth.

~~~~~~~~~~~~~{.py}

# We store the results in this list
depth = [[r.depth for r in reaches]]
# Run the model for 3 h with dt=1 min
for t in solver.run(datetime(2012,1,1),datetime(2012,1,1,3), timedelta(minutes=1)):
    depth.append([[r.depth|for r in reaches]])
    print(t)
~~~~~~~~~~~~~

Finaly we are plotting the dynamic of the river depth over the run time

~~~~~~~~~~~~~{.py}

# Plot the result (matplotlib needs to be installed)
from matplotlib.pylab import plot, show
plot(depth)
show()
~~~~~~~~~~~~~

author: philipp, version: 10 Mon Oct 17 12:24:38 2016
