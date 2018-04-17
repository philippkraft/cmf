@page cmfTutIntercept

 [back...](@ref cmfTutET)
[next...](@ref cmfTutSnow)

# Interception

For this tutorial we are going to setup a simplified version (1 Layer)
of the 1D Model to observe the effect of above ground fluxes. The same
model will be used in the next tutorial about snow. As an excercise, try
to put this model together with the 1D Richards model with ET from the
last tutorial.

## Simple 1 storage model, still with out interception

### Setup code

1. we create a project and a cell (you know this stuff by now...) 
2. We add a layer of 1m water capacity
3. We add a groundwater boundary condition with free drainage

~~~~~~~~~~~~~{.py}

import cmf
p = cmf.project()
c = p.NewCell(0,0,0,1000,True)
# Add a single layer of 1 m depth
l = c.add_layer(1.0, cmf.VanGenuchtenMualem())
# Add a groundwater boundary condition
gw = p.NewOutlet('gw',0,0,-2)
# Use a free drainage connection to the groundwater
cmf.FreeDrainagePercolation(l,gw)

# Add some rainfall
c.set_rainfall(5.0)
~~~~~~~~~~~~~

### Runtime and plot code

~~~~~~~~~~~~~{.py}

solver = cmf.CVodeIntegrator(p,1e-9)
res = [l.volume for t in solver.run(solver.t,solver.t+cmf.day,cmf.min*15)]
plot(res)
~~~~~~~~~~~~~

## Create a canopy storage

![](media/canopyflow.png)

Now we will extend our model with a canopy storage. In the figure on the
right side, you can see how the canopy storage should be connected. The
incoming rainfall, that has been routed before directly to the surface
water is now split into intercepted rainfall, routed to the new canopy
storage, and throughfall, routed still routed to the surface water. The
fraction of intercepted rainfall is triggered by the variable canopy
closure of the cell's
[Vegetation](@ref cmf::upslope::vegetation::Vegetation)
`cell.vegetation.CanopyClosure`. If this variable is 1, all rainfall
is intercepted, and there is no throughfall. However, the predifined
connection between rainfall and surface water is not aware of the canopy
storage, hence both connections
([Rainfall](@ref cmf::upslope::connections::Rainfall)) need to be newly defined
(see code below).

The second connection needed, is the overflow of the canopy. The leaves
of the canopy can only store a limited volume of water, if this volume
is reached, the canopy starts dripping. One of the best known models for
the interception process is the model by Rutter et al. 1971. The
overflow term of the model is implemented in cmf as
[RutterInterception](@ref cmf::upslope::connections::RutterInterception).
A special characteristik of the model is, that drippig stops together
with rainfall. The capacity of the canopy storage is defined as the
product of the leaf area index (`cell.vegetation.LAI`) and the
capacity in mm per m2 leaf area
`cell.vegetation.CanopyCapacityPerLAI`. To define which amount of
water is directly routed to the surfaces as throughfall without being
storaged in the canopy the canopy closure can be defined
`cell.vegetation.CanopyClosure`. The default value of the canopy
closure is 1, so there is no direct throughfall if it remains unchanged.

The third term of the Rutter model is the evaporation from the wet leaf
surface. Currently only the Penman based
[CanopyStorageEvaporation](@ref cmf::upslope::ET::CanopyStorageEvaporation)
and [ShuttleworthWallace](@ref cmf::upslope::ET::ShuttleworthWallace)
do this calculation.

The code below can be used to extend the setup code for the simple model
above.

~~~~~~~~~~~~~{.py}

# Make c.canopy a water storage
c.add_storage('Canopy','C')
# Split the rainfall from the rain source (RS) between 
# intercepted rainfall (RS->canopy) and throughfall (RS-surface)
cmf.Rainfall(c.canopy,c,False,True) # RS->canopy, only intercepted rain
cmf.Rainfall(c.surfacewater,c,True,False) # RS->surface, only throughfall
# Use an overflow mechanism, eg. the famous Rutter-Interception Model
cmf.RutterInterception(c.canopy,c.surfacewater,c) 
# And now the evaporation from the wet canopy (using a classical Penman equation)
cmf.CanopyStorageEvaporation(c.canopy,c.evaporation,c)
~~~~~~~~~~~~~


