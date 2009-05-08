# -*- coding=utf-8 -*-
from numpy import *
import pylab
import visual as v
import cmf
import os
import time
import threading

class LayerBox :
    def __init__(self,frame,cell,l,width,length,colorscale=pylab.cm.RdYlBu,zScale=1.):
        self.layer=l
        self.colorscale=colorscale
        self.zScale=zScale
        self.box=v.box(frame=frame,up=v.vector(0,0,1),width=width,length=length)
        self()
    def __call__(self,X=None,cmax=None) :
        l=self.layer
        zScale=self.zScale
        u=l.cell
        h=(l.LowerBoundary()-l.UpperBoundary())*zScale
        self.center=v.vector(u.Center().x,u.Center().y,u.Center().z*zScale)
        self.center.z-= 0.5*(l.UpperBoundary()+l.LowerBoundary())*zScale
        self.box.height=h
        self.box.pos=self.center
        if (X):
            c=self.colorscale(self.layer.conc(X)/cmax)
        else:
            c=self.colorscale(self.layer.Wetness())
        self.box.color=c[:3]
    def set_alpha(self,alpha):
        self.box.opacity=alpha
class OpenWaterBox:
    def __init__(self,frame,waterstorage,diameter,zScale=1.):
        self.waterstorage=waterstorage
        self.color=(0.5,0.6,1.0)
        self.cyl=v.cylinder(frame=frame,radius=diameter*.5,pos=tuple(self.waterstorage.Location),color=self.color,axis=(0,0,1))
        self.cyl.pos.z*=zScale
        h=waterstorage.h()
        self.cyl.length=max(h,1e-6)*zScale
        self.cyl.opacity=h>1e-6
        self.zScale=zScale
        self.alpha=1
        
    def __call__(self,X=None,cmax=None):
        self.cyl.color=self.color
        h=self.waterstorage.h()
        self.cyl.length=max(h,1e-6)*self.zScale
        self.cyl.opacity=(h>1e-6)*self.alpha
    def set_alpha(self,alpha):
        self.alpha=alpha
        self()
class connection_arrow:
    def __init__(self,node1,node2,frame,zScale=1.,qscale=0.1,length=0.75,color=(0.6,0.6,1.0)):
        self.nodes=(node1,node2)
        self.length=length
        self.color=color
        self.qscale=qscale
        self.zScale=array((1,1,zScale))
        self.arrow=v.arrow(pos=array(node1.Location)*self.zScale,color=color)
        
        #print "arrow for %s to %s created" % self.nodes
        #self(cmf.Time())
    def __call__(self,t,X=None,cmax=None):
        n1,n2=self.nodes
        q=n1.FluxTo(n2,t)
        source=n1 if (q>=0) else n2
        target=n2 if (q>=0) else n1
        a=self.arrow
        src_v=array(source.Location)*self.zScale
        tgt_v=array(target.Location)*self.zScale
        if (tgt_v-src_v).any():
            pos=src_v +(1-self.length)*0.5*(tgt_v-src_v)
            #print source,src_v,target,tgt_v
            a.pos=pos
            a.axis=self.length*(tgt_v-src_v)
            minshaftwidth=a.length/50.
            shaftwidth=sqrt(abs(q)*self.qscale)
            
            a.opacity=min(1,max(0,shaftwidth/minshaftwidth))
            a.shaftwidth=max(shaftwidth,minshaftwidth)
        else:
            a.opacity=0
                   
def GetLabel(cells,zScale=1.,text='Datum',frame=None):
    p=v.vector(0,0,0)
    l=len(cells)
    for c in cells:
        p.x+=c.Center().x/l
        p.y+=c.Center().y/l
        if c.Center().z*zScale*1.1>p.z : p.z = 1.1*c.Center().z*zScale
    return v.label(frame=frame,pos=p,text='Datum')
class CellBoxes :
    def __init__(self,cells=[],width=10,length=10,lbl=None,zScale=1.) :
        self.boxes=[]
        self.cells=cells
        self.solver=cmf.CVodeIntegrator(1e-6)
        #self.solver=cmf.BDF2(1e-6)
        self.solver.UseEulerAtTmin=0
        self.solver.preconditioner='R'
        #self.solver.maxl=5
        #self.solver.LinearSolver=0
        #self.solver.MaxOrder=5
        self.solver.MaxErrorTestFailures=20
        self.solver.MaxConvergenceFailures=15
        self.solver.MaxNonLinearIterations=10
        self.zScale=zScale
        print "frame",
        self.frame=v.frame()
        print "ok"
        for u in cells :
            self.solver.AddStatesFromOwner(u)
            if (hasattr(u.SurfaceWater(),'h')):
                self.boxes.append(OpenWaterBox(self.frame,u.SurfaceWater(),min(length,width),zScale))
            for l in u:
                self.boxes.append(LayerBox(self.frame,u,l,width,length,pylab.cm.jet_r,zScale))
        if lbl :
            self.label=lbl
            self.label.frame=self.frame
        else:
            print "label",
            self.label=GetLabel(cells,zScale,'Datum',self.frame)
            print "ok"
#    def __del__(self):
#        for o in self.frame.objects :
#            o.visible=False
    def __call__(self,X=None,cmax=0):
        if not X:
            for b in self.boxes:
                t=threading.Thread(target=b)
                t.start()
        else:                
            if not cmax:
                for c in self.cells:
                    for l in c:
                        concX=l.conc(X)
                        if concX>cmax: cmax=concX
            for b in self.boxes:
                b(X,cmax)
    def set_alpha(self,alpha):
        for b in self.boxes:
            b.set_alpha(alpha)
class connection_arrows:
    def __init__(self,project,zScale=1.,qscale=0.1,length=0.75,color=(0.6,0.6,1.0)):
        self.cons=list(project.get_connections())
        self.frame=v.frame()
        makearrow=lambda con:connection_arrow(con[0],con[1],self.frame,zScale,qscale,length,color)
        self.arrows=[]
        for i,c in enumerate(self.cons):
            #print "%s [%i/%i]" % (c,i+1,len(self.cons))
            self.arrows.append(makearrow(c))
    def __call__(self,t):
        for a in self.arrows:
            tt=threading.Thread(target=a,args=(t,))
            tt.start()
        
                         

def adjustscene():
    v.scene.forward=v.vector(0.5,0.9,-0.1)
    v.scene.up=v.vector(0,0,1)
    v.scene.autocenter=1
    v.scene.autoscale=1
    v.scene.exit=0
    #v.scene.lights[1]=vector(0.2,0.9,0.1)
    v.scene.ambient=0.7
def clearscene():
    for obj in v.scene.objects:
        obj.visible=0             
