@page CmfTutCell

@tableofcontents

 [index...](@ref CmfTutStart)

# Cells

Cells are the units of the horizontal discretization. Cells are defined
by the position of their center in space and their area. Each cell
references its soil layers, a surfacewater storage, a snow storage, a
water storage for the vegetation canopy. Vegetational parameters are
also defined for cells, and used by models for evapotranspiration. Upon
creation, the cell consists of a boundary condition for evaporation and
transpiration, and a flux node for the surface water (not a water
storage). The geometric shape of a cell is not saved and not relevant
for cmf. For models with lateral fluxes, saved information on the
topology (spatial relation) of the cells can be helpful. The API
documentation can be found here: [Cell](@ref cmf::upslope::Cell).

## Creation

Cells are created for a project with the following command. The position
in space is given with the x,y,z coordinates (cartesian coordinates in
m, eg. UTM or from a user defined origin) and the area is given in m².
For 1D models, an area of 1000m² is helpful to use any volume in m³ as
mm.

``` {.py}

c = p.NewCell(x=0.0,y=0.0,z=0.0,area=1000.0)
```

The cell `c` has a standard vegetation (short grass) which can be
changed with `c.vegetation` and the boundary conditions noted above.

### Adding soil layers

``` {.py}

c.add_layer(d=0.1,r_curve=cmf.VanGenuchtenMualem())
```

Adds a new soil layer up to the depth of 10cm below ground (d in m) with
a default paramterized van Genuchten - Mualem retention curve. The usage
of retention curves in cmf is discussed [here](@ref CmfTutRetentioncurve),
but it is recommended to read about that later in the [CMF-1D
tutorial](@ref CmfTut1d)

### Adding a surface water storage

``` {.py}

c.surfacewater_as_storage()
```

converts the flux node surfacewater into an openwater storage. This is
'''strongly recommended''' for all model setups, except for very
conceptual models. If you are not sure, create it. Routing is much
simpler to understand, if a surfacewater storage is present, '''even if
you do not expect to have infiltration or saturation excess'''. And cmf
is much better tested for models with surfacewater storage.

## Topology

Lateral flux connections between water storages and boundary conditions,
like soil layers, surface water, and outlets are usually defined by just
creating them (see eg. [here](@ref CmfTutFirstModel)). Most of the flux
connections require a notion of the distance between the water storages,
and the crosssection of the flux. Calculating these values can be
cumbersome, if many lateral connections need to be created. To
facilitate this task, you can save the topologic relation of cells.
There are tools available (see CmfFluxCreationTools), to create many
connections based on the topological information.

Cell topology defines neighborhood relation between 2 cells by saving
the length of the shared boundary of the cells.

``` {.py}

c1.topology.AddNeighbor(c2,5.0)
```

defines c1 and c2 as neighbors with a shared boundary of 5m. Topology is
used in \[\[wiki:CmfTutDarcianLateralFlow\]|and
\[wiki:CmfTutSurfaceRunoff\]\].

# Rivers and reaches

Rivers can cross cells or run along cells, depending your setup, or if
you only need a 1D model of flow in rivers, they can be created without
cells, see \[\[wiki:CmfTutChannel\].|Rivers need to be discretized to
reaches according to your needs of numerical precision, the heterogenity
of the channel form and to match the size of the cells. The reaches are
implemented in cmf by the [Reach](@ref cmf::river::Reach)\] water
storage.

## Crossectional geometry

If you create a reach for the project, you must give the crossectional
geometry of the river. The different basic geometries are: -
[TriangularReach](@ref cmf::river::TriangularReach) - T, a river with
a triangular crosssection -
[SWATReachType](@ref cmf::river::SWATReachType) - S, a river with a
trapezoidal crossection, with a flat flood plain, as in the model SWAT -
[RectangularReach](@ref cmf::river::RectangularReach) - R, a
rectangular crosssection - [PipeReach](@ref cmf::river::PipeReach) -
P, a circular pipe

More crossection types can be implemented on demand.

## Creating a river

### Creating the reaches

As a general type, [Channel](@ref cmf::river::Channel) is used,
denoting the crosssection by the shortcuts listed above:

``` {.py}

r1=p.NewReach(x=0,y=0,z=1.0,length=100,Type='S',width=0.5,depth=0.1,diffusive=False)
r2=p.NewReach(x=100,y=0,z=0,length=100,Type='S',width=0.5,depth=0.1,diffusive=False)
```

This command creates a new reach with a trapezoidal crossection, a bank
width of 50cm, a depth of 10 cm using (per default) a kinematic wave
approach. The center of the reach is located at x,y,z.

### Connecting the reaches

The two reaches can be connected with any connection suitable for open
water storages. However, in most cases a kinematic or diffusive wave
approach, using Manning's roughness as the friction term will be used
for modelling channeled flow (see CmfTutChannel), hence the two reaches
can be connected by definining \!r2 as the downstream reach of \!r1:

``` {.py}

r1.set_downstream(r2)
```

\!r2 should be connected to the area outlet:

``` {.py}

outlet = p.NewOutlet('outlet',x=200,y=0,z=-1.0)
r2.set_outlet(outlet)
```

CmfTutChannel will show a full running example.

[index...](@ref CmfTutStart)

author: giovanny, version: 5 Tue May 5 11:22:32 2015
