# -*- coding: utf-8 -*-
"""
Created on Thu Nov 07 10:37:33 2013

@author: kraft-p
"""

import cmf


p=cmf.project('X')

w1 = p.NewStorage('w1',0,0,0)
w2 = p.NewStorage('w2',10,0,0)
w3 = p.NewStorage('w3',10,10,0)
con1 = cmf.kinematic_wave(w1,w2,1)
con2 = cmf.kinematic_wave(w1,w3,0.5)
con3 = cmf.kinematic_wave(w3,w2,2.)

w1.volume=1

solver = cmf.RKFIntegrator(p,1e-9)

res = [(w1.volume,w2.volume,w3.volume) for t in solver.run(solver.t,solver.t+cmf.day*10,cmf.h)]

from pylab import *
plot(res)
legend(p.get_storages())
