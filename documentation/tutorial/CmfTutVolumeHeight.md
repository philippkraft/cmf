@page cmfTutVolumeHeight

 [next](@ref cmfTutBoundary2)

# Hydraulic head in surface water bodies

For unconfined surface water bodies, the hydraulic head (or potential
@f$\Psi@f$) is given by the water level above the reference height. To
use the potential as a driving force for gradient based connections, a
relation between the stored volume (the integrated state variable in
cmf) and the water level needs to be present.

[OpenWaterStorage](@ref cmf::river::OpenWaterStorage) is the class for
water storages containing free surface water. An OpenWaterStorage owns
therefore functional description of the relationship between stored
volume @f$V@f$ and the height above the relative water depth
@f$h@f$.


@f[
[\Psi|= h + z = f(V) + z
@f]

\] @f$f(V)@f$ is given by a child class of
[IVolumeHeightFunction](@ref cmf::river::IVolumeHeightFunction).

@author giovanny, version: 2 Tue May 5 13:49:06 2015
