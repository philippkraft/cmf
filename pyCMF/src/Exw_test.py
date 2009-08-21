from pylab import *
from numpy import *
import cmf
p=cmf.project()
def plot_res(res):
    subplot(211)
    plot(map(lambda rec:rec[:3],res))
    legend('$\Psi_{surf}$ $\Psi_{unsat}$ $\Psi_{sat}$'.split(),loc=0)
    ylim(-1,0.1)
    grid()
    subplot(212)
    plot(map(lambda rec:rec[3:],res))
    legend('$I$ $q_{perc}$ $q_{lat}$'.split(),loc=0)
    grid()
    ylim(-5,5)
hold(0)
c=p.NewCell(0,0,0,100)
bc=cmf.VanGenuchtenMualem(1.8,0.01,0.5,15)

layered=0
if layered:
    c.add_layer(0.5,bc)
    c.add_layer(1.0,bc)
    c.install_connection(cmf.Richards)
else: 
    c.add_variable_layer_pair(1.0,bc)

nbc=cmf.NeumannBoundary(p)
nbc.flux.add(0.0)
c.saturated_depth=0.9
c.surfacewater_as_storage()
u,s=c.layers
c.rain.flux=cmf.timeseries(10.0)
getrecord=lambda t:(c.surface_water.potential,u.potential,s.potential,
                    -u.flux_to(c.surface_water,t), -s.flux_to(u,t),-s.flux_to(nbc,t))
def getrecord2(i,t):
    if i%24==0: print t,integ.dt, u.wetness,u.lower_boundary
    return getrecord(t)
nbc.connect_to(s)
c.install_connection(cmf.MatrixInfiltration)
#c.install_connection(cmf.ShuttleworthWallaceET)

#if not layered: cmf.connect(cmf.VarLayerPercolationSimple, c.layers[0],c.layers[1])
integ=cmf.CVodeIntegrator(p,1e-6)
integ.LinearSolver=0
#c.layers[0].potential=-0.0
res=[]
try:
    for i,t in enumerate( integ.run(cmf.day*0,cmf.day*40,cmf.h)):
        res.append(getrecord2(i,t))
except Exception,e:
    print e
plot_res(res)
show()