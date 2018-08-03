@page cmfTutAbstractFluxes Abstract connections for advanced model concepts

Depending on your model design, you might need to use rather abstract concepts 
can help to realize your model - but not every model needs these connections.

## Balancing water flux at one node

### What does it mean?

The [waterbalance_connection](@ref cmf::water::waterbalance_connection) is 
a connection to route the water balance of a node somewhere else.
The connection checks the flux over all other connections of the *left* node
and routes any surplus to the *right* node, or in case of a negative water balance
of the *left* node, additional water is fetched from the right node. Or as a
mathematical expression:

@f[
q_{N} = -\sum_{i=1}^{N-1} q_{j}(V, V_{j})
@f]

The water balance connection levels always all other fluxes out: The sum of all fluxes 
(including the water balance connection) is zero.

@f[
\sum_{i=1}^{N} q_{j}(V, V_{j}) = \frac{dV}{dt} = 0
@f]

Hence, there is often no need anymore to leave the left node a storage, 
since no state change can happen there due to the water balance connection. 
You should therefore consider to change your storage into a "non-storing" node,
like a boundary condition. If your water balance connection connects to the surface of
a cell, you can (in most cases) create the cell without surface water storage

### When do I want a water balance connection

In practice, we found mainly two situations, when to use the water balance connection:

1. at the cell surface
2. to connect submodels

However, a water balance connection might solve a problem for a totally new application of cmf,
which we as the developers have not forseen. Be creative!

#### At the cell surface

When the user creates the first subsurface storage for a conceptual model with no surface water 
storage, this first layer is automatically connected with the surface via a 
water balance connection. 

~~~~~~~~~~~.py
p = cmf.project()
# Create cell without surface water storage
c = p.NewCell(0, 0, 0, 1000, with_surfacewater=False)
l = c.add_layer(1.0)  # Add layer with 1000 mm capacity
print(cmf.describe(c.surfacewater))
~~~~~~~~~~~

results in:

- {Surface water of cell #0}:
    - waterbalance connection({Surface water of cell #0}<->{Layer #0 of cell #0})
    - Rainfall({Constant rainfall of 0 mm/day}<->{Surface water of cell #0})


With this automatic construction, the mass balance of the system is ensured -
no water can vanish at the surface node, which happens when
the water balance of `c.surfacewater.waterbalance(t) != 0`.

If you choose to use an infiltration method that limits infiltration by saturation excess
or hortonian overflow (e.g. [SimpleInfiltration](@ref simple_infiltration)), and the cell
surface is still no storage, the local mass balance on the surface is not necessary zero,
and the excess gets lost. To route the surface water excess without time lag to a river
or the catchment outlet (good approximation for time steps > 1h), you can use the water
balance connection again.

The following code is meant to expand the code snippet from above:

~~~~~~~~~~~.py
cmf.SimpleInfiltration(l, c.surfacewater)
# Create outlet
out = p.NewOutlet('outlet')
# moves all infiltration excess to the outlet
cmf.waterbalance_connection(c.surfacewater, out)
print(cmf.describe(c.surfacewater))
~~~~~~~~~~~

results in:

- {Surface water of cell #0}:
    - waterbalance connection({Surface water of cell #0}<->{outlet})
    - simple infiltration({Surface water of cell #0}<->{Layer #0 of cell #0})
    - Rainfall({Constant rainfall of 0 mm/day}<->{Surface water of cell #0})


#### As a bridge between submodels

If the second example for the surface does not point to an outlet but another part
of the model setup, eg. a river, the water balance connection is the bridge between
the local (cell) sub model and the distributing river network. You can also place 
a collection node between the submodels for simplified observation of fluxes
between these different model domains.

## Keeping a flux constant as long as water is available

[ConstantFlux](@ref cmf::water::TechnicalFlux)

## Keeping a state constant as long as water is available

[ConstantState](@ref cmf::water::statecontrol_connection)