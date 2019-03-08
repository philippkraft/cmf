@page cmfTutBoundary2 Head based boundary conditions

[In the first tutorial about boundary conditions](@ref cmfTutBoundary) 
an outlet was created to let water flow out
of the system. Outlets created with

~~~~~~~~~~~~~{.py}

outlet = p.NewOutlet('name',position=position)
~~~~~~~~~~~~~

are in fact [DirichletBoundary](@ref cmf::water::DirichletBoundary)
objects. Most connection types can be used to connect on the right side
with an outlet. However, some connections in are gradient based (eg. the
[lateral darcian connections](@ref cmfTutDarcianLateralFlow)). For these
connections, the outlet position is meaningful for the calculation of
the distance and the potential head of the boundary condition matters.
By default, the head of an outlet equals its z position. If this does
not match your model well, eg. your boundary is a river or lake with a
more or less fixed water level, the potential of the boundary condition
can be changed:

~~~~~~~~~~~~~{.py}

# Set the outlet 0.5 m above the outlet height
outlet.potential = outlet.position.z + 0.5
~~~~~~~~~~~~~

The potential of the outlet can be adjusted during run time in the run
time loop by reading from a file of predefined heads or from another
model running in the same run time loop.

But what happens if the potential of the left side of the connection is
lower than the potential of the boundary condition? As a default,
outlets are not a water source, therefore no flux occurs. In the case
illustrated above, when the boundary condition is something carrying
water (river, like, groundwater table) one might prefer to use the
boundary condition not only as a sink, but also a source, depending on
the state of the connected water storages. In this case, cmf needs to be
informed to handle the boundary condition as a source:

~~~~~~~~~~~~~{.py}

outlet.is_source=True
~~~~~~~~~~~~~


