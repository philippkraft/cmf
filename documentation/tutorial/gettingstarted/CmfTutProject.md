@page cmfTutProject Project

The cmf project is the container for the nodes of the water flux
network. For any kind of cmf model, the first step is to create a
project:

~~~~~~~~~~~~~{.py}

import cmf
p = cmf.project()
~~~~~~~~~~~~~

The project represents the study area and holds the cells (horizontal
discretization of the soil), the reaches (discretized river sections),
"free" water storages (like lakes, dams etc.) and the boundary
conditions. While models for channeled flow may only contain reaches,
but no cells, every model spatial explicit description of subsurface
flux needs to have cells.

Have a look at the description of the programming interface for
[project](@ref cmf::project).

Adding Cells, Rivers, free water storages and boundary conditions is
explained in the next parts of the tutorial. Bear in mind, that nearly
every cmf object is somehow bound to a project.
Creating a project is therefore always the second step in using cmf
after import of the library.

The [next tutorial](@ref cmfTutFirstModel) contains a first simple,
conceptual model (Nash-Boxes).


