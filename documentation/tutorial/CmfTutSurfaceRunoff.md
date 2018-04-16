@page cmfTutSurfaceRunoff

 [next...](@ref cmfTut2d)

# Surface runoff

## Surface water storage

Cells can own a distinct storage for surface water, and usually this
storage should exist. However, water that does not infiltrate directly
might be routed to an outlet (model boundary), a river (see later part
of the tutorial) or to the surface water of another cell.

The surface water of a cell has the class
[SurfaceWater](@ref cmf::upslope::SurfaceWater), and can either be
created directly withe the cell:

~~~~~~~~~~~~~{.py}

cell = project.NewCell(x=0,y=0,z=0,area=1000,with_surfacewater=True) 
~~~~~~~~~~~~~

or afterwards, if the cell has not been created with surface water

~~~~~~~~~~~~~{.py}

cell = project.NewCell(x=0,y=0,z=0,area=1000) 
cell.surfacewater_as_storage()
~~~~~~~~~~~~~

To route the water to the target, one can use conceptual connections
like the
[waterbalance_connection](@ref cmf::water::waterbalance_connection),
if you want to ensure the surfacewater to be empty at all time, the
generic [kinematic_wave](@ref cmf::water::kinematic_wave) connection
for linear and polynomal storages or the
[KinematicSurfaceRunoff](@ref cmf::upslope::connections::KinematicSurfaceRunoff)
for a more "physical" approach. This type of connection is deemed to be
the "normal" case and the
[SurfaceWater](@ref cmf::upslope::SurfaceWater) exposes some
properties to be used by that connection.

Using a diffusive wave, where sinks can fill up and water flows over the
topographic gradient, is not numerically stable. Contributions are
welcome.

## Kinematic surface runoff

Surface runoff in cmf is usually modelled using the kinematic wave
approximation of the St. Vernant equation for sheet flow using Manning
friction loss:


@f[
[q|= d\ w\ d^{2/3} \frac{\sqrt{\tan \beta}}{n} 86400 \frac{sec}{day}
@f]

\]

where: - @f$q@f$ is the surface flow in @f$\frac{m^3}{day}@f$ -
@f$d@f$ is the "active" water depth in @f$m@f$ - @f$w@f$ is the
width of the sheet flow - @f$\tan \beta@f$ is the topographic slope -
@f$n@f$ is the Manning roughness

The "active" water depth is the average water depth where runoff starts.
The idea behind a passive and an active water depth is, that prior to
runof subscale sinks need to be filled up in form of puddles. Only when
the puddles are filled, the runoff starts. The
[SurfaceWater](@ref cmf::upslope::SurfaceWater) storage has a property
`puddledepth` to characterize the average water depth needed to fill up
the puddles. If runoff starts, when 20% of the surface is covered by (on
average) 1cm deep puddles, the `puddledepth` is @f$0.01m \cdot 0.2
= 0.002m@f$. The property `nManning` of the
[SurfaceWater](@ref cmf::upslope::SurfaceWater) is also used.

### Example: A simple over flow routing to a boundary condition

~~~~~~~~~~~~~{.py}

import cmf
from numpy import transpose
from pylab import plot,show,subplot

p = cmf.project()
# Create cell with 1000m2 and surface water storage
c = p.NewCell(0,0,1,1000,True)
# Set puddle depth to 2mm
c.surfacewater.puddledepth = 0.002
# Add a thick layer, low conductivity. Use Green-Ampt-Infiltration
c.add_layer(0.1, cmf.VanGenuchtenMualem(Ksat=0.1))
c.install_connection(cmf.GreenAmptInfiltration)
# Create outlet, 10 m from cell center, 1m below cell
outlet = p.NewOutlet('outlet',10,0,0)
# Create connection, distance is calculated from position
con = cmf.KinematicSurfaceRunoff(c.surfacewater,outlet,flowwidth=10)
# set rainfall, a good shower to get surface runoff for sure (100mm/day)
c.set_rainfall(100.)
# Create a solver
solver = cmf.CVodeIntegrator(p,1e-8)

# Calculate results
Vsoil, Vsurf, qsurf,qinf = transpose([[(c.layers[0].volume,|c.surfacewater.volume, outlet(t), c.layers[0]](t)) 
                             for t in solver.run(cmf.Time(1,1,2012),cmf.Time(2,1,2012),cmf.min)])
# Present results
ax1=subplot(211)
plot(Vsurf,label='Surface')
plot(Vsoil,label='Soil')
ylabel('Water content in mm')
legend(loc=0)
subplot(212,sharex=ax1)
plot(qsurf,label='Surface')
plot(qinf,label='Soil')
ylabel('Flux in mm/day')
xlabel('Time in minutes')
legend(loc=0)
~~~~~~~~~~~~~

@author philipp, version: 2 Tue Jan 21 10:38:14 2014
