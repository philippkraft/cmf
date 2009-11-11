'''
Created on 05.11.2009

@author: philkraf
'''
import cmf
p=cmf.project()
R1 = p.NewReach(cmf.Channel('R',1,1),1)
R2 = p.NewReach(cmf.Channel('R',1,1),1)
R1.set_downstream(R2)         
Rs=cmf.node_list.from_sequence((R1,R2))
R1.depth=0.5
R2.depth=0.1
integ=cmf.BDF2(Rs,1e-6)
integ(integ.t+cmf.min) 