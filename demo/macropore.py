from __future__ import print_function, division
import cmf
import numpy as np
import sys



# Parameters

# sat. conductivity of the matrix
Ksat_mx = 1.
# sat. conductivity of the macropores
Ksat_mp = 10.
# Macrapore fraction
porefraction = .05
# Rainfall rate
rainfall = 50.0
rainfall_duration = cmf.day
# The MacroPoreList class defined here in pure Python might get implemented in C++
# as layerlist - to get a faster access on its properties
class MacroPoreList(list):
    @property
    def wetness(self):
        return np.array([mp.filled_fraction for mp in self])
    @property
    def potential(self):
        return np.array([mp.potential for mp in self])
    @property
    def capacity(self):
        return np.array([mp.get_capacity() for mp in self])
    @property
    def volume(self):
        return np.array([mp.volume for mp in self])
    
    def percolation(self,t):
        mpups = [self[0].layer.cell.surfacewater] + self[:-1]
        return np.array([up.flux_to(down,t) for up,down in zip(mpups,self)])
    def aggregate_infiltration(self,t):
        return np.array([mp.flux_to(mp.layer,t) for mp in self])

# Create project and a cell with surfacewater
if 'X' in sys.argv:
    p=cmf.project('X')
    X,=p.solutes
else:
    p=cmf.project()
    X=None
c=p.NewCell(0,0,0,1000,True)

# Add some layers
vgm = cmf.VanGenuchtenMualem(Ksat=Ksat_mx,phi=0.5)
for d in np.arange(0,1,0.02):
    c.add_layer(d+.02, vgm)

# Use Richards equation for matrix transport
c.install_connection(cmf.Richards)
c.install_connection(cmf.MatrixInfiltration)

# Create Macropore storages for each layer
macropores = MacroPoreList(cmf.MacroPore.create(l,
                                   porefraction=porefraction,
                                   Ksat=Ksat_mp,density=0.1) 
                           for l in c.layers)
# Macro pore infiltration
cmf.JarvisMacroFlow(c.surfacewater, macropores[0])
# Macro pore percolation
for upper,lower in zip(macropores[:-1],macropores[1:]):
    cmf.KinematicMacroFlow(upper, lower)
# Macro / Micro exchange
for mp,layer in zip(macropores,c.layers):
    #cmf.GradientMacroMicroExchange(layer,mp)
    mme = cmf.DiffusiveMacroMicroExchange(mp,layer,1.)
    
# Inital / boundary conditions
c.saturated_depth = 1.0
# Add constant rainfall 
rain = cmf.NeumannBoundary.create(c.surfacewater)
rain.flux = rainfall
if X:
    rain.concentration[X] = cmf.timeseries.from_scalar(1.)
# A deep groundwater as boundary condition for the matrix
groundwater = p.NewOutlet('groundwater',c.x,c.y,c.z - 1.)
#cmf.Richards(c.layers[-1],groundwater)

# Create solver
solver = cmf.CVodeIntegrator(p,1e-9)

# some lists to save data during runtime
wetness_mx = [] # Saturation of the matrix
wetness_mp = [] # Saturation of the macropores

conc_mx=[]
conc_mp = []

perc_mx=[] # percolation through the matrix
perc_mp=[] # percolation through the macropores
ex_mp_mx=[] # flux from macropores to micropores
# Run...
t0 = cmf.Time(1,1,2012)
tend = t0 + cmf.day * 10
dt = cmf.min * 5
for t in solver.run(t0,tend,dt):
    if t-t0>rainfall_duration:
        rain.flux = 0.0
    wetness_mx.append(c.layers.wetness)
    wetness_mp.append(macropores.wetness)
    if X:
        conc_mx.append([l.conc(X) for l in c.layers])
        conc_mp.append([mp.conc(X) for mp in macropores])
    perc_mp.append(macropores.percolation(t))
    perc_mx.append(c.layers.get_percolation(t))
    ex_mp_mx.append([mp.flux_to(mp.layer,t) for mp in macropores])
    print(t)

# plot results
import pylab as plt
plt.figure(figsize=(16,9))
ax1=plt.subplot(3,2,1)
plt.imshow(plt.transpose(perc_mx),cmap=plt.plt.cm.Blues,aspect='auto',interpolation='nearest',vmin=0.0,vmax=rainfall)
plt.xticks(plt.arange(0,10 * 24 * 12,24*12),plt.arange(0,10))
plt.xlabel('days')
plt.colorbar()
plt.title('Matrix percolation')
plt.subplot(3,2,2,sharex=ax1)
plt.imshow(plt.transpose(perc_mp),cmap=plt.plt.cm.Blues,aspect='auto',interpolation='nearest',vmin=0.0,vmax=rainfall)
plt.xticks(plt.arange(0,10 * 24 * 12,24*12),plt.arange(0,10))
plt.xlabel('days')
plt.colorbar()
plt.title('Macropore percolation')
ax1=plt.subplot(3,2,3,sharex=ax1)
plt.imshow(plt.transpose(wetness_mx),cmap=plt.cm.jet_r,aspect='auto',interpolation='nearest')
plt.xticks(plt.arange(0,10 * 24 * 12,24*12),plt.arange(0,10))
plt.xlabel('days')
plt.colorbar()
plt.title('Matrix saturation')
plt.subplot(3,2,4,sharex=ax1)
plt.imshow(plt.transpose(wetness_mp),cmap=plt.cm.jet_r,aspect='auto',interpolation='nearest')
plt.xticks(plt.arange(0,10 * 24 * 12,24*12),plt.arange(0,10))
plt.xlabel('days')
plt.colorbar()
plt.title('Macropore saturation')
plt.subplot(3,2,5,sharex=ax1)
plt.imshow(plt.transpose(conc_mx),cmap=plt.cm.jet,aspect='auto',interpolation='nearest',vmin=0.0,vmax=1.0)
plt.xticks(plt.arange(0,10 * 24 * 12,24*12),plt.arange(0,10))
plt.xlabel('days')
plt.colorbar()
plt.title('Matrix concentration')
plt.subplot(3,2,6,sharex=ax1)
plt.imshow(plt.transpose(conc_mp),cmap=plt.cm.jet,aspect='auto',interpolation='nearest',vmin=0.0,vmax=1.0)
plt.xticks(plt.arange(0,10 * 24 * 12,24*12),plt.arange(0,10))
plt.xlabel('days')
plt.colorbar()
plt.title('Macropore concentration')

plt.subplots_adjust(0.05,0.05,0.95,0.9,0.02,0.3)


plt.suptitle('$K_{sat}^{mx}=%g m/day, K_{sat}^{mp}=%g m/day, \Phi_{mp}=%g$' % (c.layers[0].Ksat,macropores[0].Ksat,porefraction),size=20)
plt.savefig('Ksmx-%g-Ksmp-%g-MP%g.png' % (c.layers[0].Ksat,macropores[0].Ksat,porefraction))
plt.show()