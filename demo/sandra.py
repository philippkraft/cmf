from __future__ import division
from pylab import frange,draw,fill,figure,subplot,axis,cm
import sys
import cmf
fig = figure()

def maize_growth(t):
    DOY = t.AsDate().DOY()
    if DOY<90:
        return 0.0
    elif DOY<180:
        return (DOY-90)/90
    elif DOY<240:
        return 1.0
    else:
        return 0.0
        
    

p=cmf.project()
c_maize=p.NewCell(0,0,0,1000)
c_forest=p.NewCell(0,0,0,1000)
p.rainfall_stations.add('giessen', cmf.timeseries.from_file('giessen.rain'),(0,0,0))
landuse="Wald","Mais"
c_forest.vegetation.LAI = 6.0
c_forest.vegetation.Height = 18.0
c_forest.vegetation.RootDepth = 0.8
c_forest.vegetation.albedo = 0.19
c_maize.vegetation.LAI = 3.0
c_maize.vegetation.RootDepth = 0.4
c_maize.vegetation.albedo = 0.25
c_maize.vegetation.Height = 1.5 
rects=[]
for i,c in enumerate(p):
    for d in frange(0.1,2.0,0.05):
        c.add_layer(d, cmf.BrooksCoreyRetentionCurve([30,10][i]))
    c.install_connection(cmf.Richards)
    p.rainfall_stations[0].use_for_cell(c)
    c.saturated_depth=1.0
    c.install_connection(cmf.CanopyOverflow)
    c.install_connection(cmf.SimpleTindexSnowMelt)
    c.install_connection(cmf.ShuttleworthWallace)
    subplot(121 + i)
    rects.append([fill([0,1,1,0],[-l.boundary[0],-l.boundary[0],-l.boundary[1],-l.boundary[1]],ec='none',fc='none')[0] for l in c.layers])
    axis('off') 

solver=cmf.CVodeIntegrator(p,1e-6)
solver.t= cmf.Time(1,1,1981)
timelabel = fig.text(0.5,0.05,solver.t)
fig.text(0.25,0.1,"Wald")
fig.text(0.75,0.1,"Mais")

def refresh(t):
    for c_r,c in zip(rects,p):
        for r,l in zip(c_r,c.layers):
            r.set_fc(cm.jet_r(l.wetness))
    timelabel.set_text(t)
    f_maize= maize_growth(t)
    c_maize.vegetation.LAI = 3.0 * f_maize
    c_maize.vegetation.RootDepth = 0.4 * f_maize
    c_maize.vegetation.albedo = 0.25 * f_maize
    c_maize.vegetation.Height = 1.5 * f_maize
    draw()

if "run" in sys.argv:
    for t in solver.run(cmf.Time(1,1,1980),cmf.Time(1,1,2003),cmf.h):
        if not t % cmf.day:
            refresh(t)


