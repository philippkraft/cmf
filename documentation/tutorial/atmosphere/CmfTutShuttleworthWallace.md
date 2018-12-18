@page cmfTutShuttleworthWallace The Shuttleworth-Wallace method for seperated evaporation and transpiration

The separation of plant transpiration and soil evaporation is quite
complex, since a vegetation cover does intercept the incoming radiation
and changes, by transpiration the vapor pressure deficit inside the
canopy. The only method, that separates soil evaporation from plant
transpiration implemented in cmf so far, is the
[ShuttleworthWallace](@ref cmf::upslope::ET::ShuttleworthWallace). It
implements the evaporation method given by [Shuttleworth
(1985)](http://tlaxiaco.sdsu.edu/shuttleworthwallace.pdf) as used in
[BROOK90 (Federer 1995)](http://www.ecoshift.net/brook/brook90.htm). To
install all necessary connections, use:

~~~~~~~~~~~~~{.py}
cell.install_connection(cmf.ShuttleWorthWallace)
~~~~~~~~~~~~~

This installs connections between all layers and `cell.transpiration`, another
connection between the first soil layer and `cell.evaporation` as well as a connection
between the surface water and if they are

If you want to use canopy evaporation and snow sublimation, make sure
you have created the appropriate storages from the tutorials
about [interception](@ref cmfTutIntercept) and [snow](@ref cmfTutSnow) before installation.

[ShuttleworthWallace](@ref cmf::upslope::ET::ShuttleworthWallace) is
quite parameter demanding - all meteorological parameters available in
the [Weather](@ref cmf::atmosphere::Weather) object are used as
well as all parameters of the [Vegetation](@ref cmf::upslope::vegetation::Vegetation) object.

The transpiration part uses the partitioning described in a [later tutorial](@ref cmfTutETphys)



