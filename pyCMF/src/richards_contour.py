#!/usr/bin/env python
# -*- coding: latin-1 -*-
import cmf
from pylab import *
p=cmf.project()
c=p.NewCell(0,0,0,100)
vgm=cmf.VanGenuchtenMualem()
wet=linspace(0.101,1.1,1000)
c.AddLayer(.1,vgm)
c.AddLayer(.2,vgm)
c[0].Soil().n=1.8
c[1].Soil().n=1.8
c[0].Soil().Ksat=1.
c[1].Soil().Ksat=1.
flux=zeros((len(wet),len(wet)),float)
for i1,w1 in enumerate(wet):
    c[0].Wetness(w1)
    for i2,w2 in enumerate(wet):
        c[1].Wetness(w2)
        flux[i1,i2]=cmf.geo_mean(c[0].K(),c[1].K())*(c[0].Potential()-c[1].Potential())/0.1
        flux[i1,i2]=max(min(flux[i1,i2],vgm.Ksat),-vgm.Ksat)
    if i1 % 25==0: print i1,
levels=arange(-1,1,0.05)*vgm.Ksat
imshow(flux,extent=(wet.min(),wet.max(),wet.min(),wet.max()),origin='bottom',interpolation='nearest')
contour(wet,wet,flux,levels,colors='k')
xlabel('Wetness of lower')
ylabel('Wetness of upper')
grid()
show()