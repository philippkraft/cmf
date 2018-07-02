@page cmfTutAbstractFluxes Abstract connections for advanced model concepts

Depending on your model design, you might need to use rather abstract concepts 
can help to realize your model - but not every model needs these connections.

## Balancing water flux at one node

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

## Keeping a flux constant as long as water is available

[ConstantFlux](@ref cmf::water::TechnicalFlux)

## Keeping a state constant as long as water is available

[ConstantState](@ref cmf::water::statecontrol_connection)