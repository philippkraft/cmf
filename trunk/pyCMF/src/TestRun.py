import pylab
import time
import cmf
from numpy import *
import visualCMF
import threading
import sys
def makescene(p):
    boxes=visualCMF.CellBoxes(p,9,9,None,5)
    visualCMF.adjustscene()
    #boxes.set_alpha(0.0)
    arrows=visualCMF.connection_arrows(p, 5, 0.01)
    boxes.frame.y=10
    arrows(cmf.Time())
    boxes.label.text="Press key to start..."
    if visualCMF.v.scene.kb.getkey()=='backspace': 
        visualCMF.v.scene.visible=0
        exit()
    return boxes,arrows
def refreshscene(t,boxes,arrows):
    if visualCMF.v.scene.visible==0:
        return 0
    if visualCMF.v.scene.kb.keys:
        return 0
    boxes()
    boxes.label.text=str(t)
    arrows(t)
    return 1
def refresh(t,boxes,arrows):
    return refreshscene(t, boxes, arrows)
    #thread=threading.Thread(target=refreshscene,args=(t,boxes,arrows))
    #thread.start()
if __name__=="__main__":
    
    visualize="v" in sys.argv
    lcount=0 if "flexsat" in sys.argv else 10
    latflux=cmf.Darcy if "darcy" in sys.argv else cmf.Richards
    nooverflow="nooverflow" in sys.argv
    sd=2.
    
    p=cmf.project()
    p.debug=0
    p.Rainfall().Location.z=8
    #vgm=cmf.VanGenuchtenMualem(2.0,0.05,0.5,5)
    bc=cmf.BrooksCoreyRetentionCurve()
    #bc.SetKsat(4,0.5)
    bc_clay=cmf.BrooksCoreyRetentionCurve(0.01,0.6,11.0,0.5)
    r_curve=cmf.RCurve(bc,1)
    z=array([0.0,0.1]+list(arange(0,20,0.5)))*1
    
    for i in range(10):
        #z=i*0.5 if i else 0.9
        c=p.NewCell(i*10,0,z[i],100)
        c.sw=c.AddStorage('','W')
        c.Transpiration().Location+=cmf.point(2,0,1)
        if lcount>0:
            for j in range(1,lcount+1):
                if (i>=0 and i<9 and j==5):
                    c.AddLayer(j*1./lcount*sd,r_curve)
                else:
                    c.AddLayer(j*1./lcount*sd,r_curve)
            cmf.Richards.use_for_cell(c)
        else:
            c.AddVariableLayerPair(sd,r_curve)
        c.saturated_depth=.5+c.z*.5
            
        cmf.MatrixInfiltration(c.layers[0],c.surface_water).thisown=0
        c.Vegetation.RootDepth=0.3
        c.Vegetation.fraction_at_rootdepth=0.8
        #cmf.SWATPercolation.use_for_cell(c)
        cmf.PenmanMonteithET.use_for_cell(c)
        if i:
            c.topology.AddNeighbor(p[-2],10)
            p[-2].topology.AddNeighbor(c,10)
    cmf.connect_cells_with_flux(p,latflux,0)
    if not nooverflow:
        cmf.connect_cells_with_flux(p,cmf.Manning,0)
    pdata=cmf.SinglePrecipitationTimeseries(100.0)
    pdata.Data.step=cmf.day*4
    for i in range(200):
        pdata.Data.Add(100.0 * (i%2))
    print "c[0].W=%f, c[1].W=%f" % (p[0].layers[0].Wetness(),p[0].layers[1].Wetness())
    p.SetPrecipitation(pdata)
    discharge=cmf.FluxNode(p)
    discharge.Location=cmf.point(-6,0,-.5)
    discharge.Name="Discharge"
    r1=p[0].AddReach(6,'T',0.1,1.)
    r1.set_outlet(discharge)
    r2=p[1].AddReach(10,'T',0.15,1.)
    r2.set_downstream(r1)
    r3=p[2].AddReach(10,'T',0.15,1.)
    r3.set_downstream(r2)
    for c in p:
        c.surface_water.State(10)
    p[-1].surface_water.State(100)
    c=p[0]
    for l in p[0].layers:
        cmf.connect(cmf.Richards,l,discharge,10*l.Thickness(),1)
  
    integ=cmf.CVodeIntegrator(p,1e-6)
    #integ=cmf.BDF2(p,1e-6)
    #integ.MaxOrder=1
    if visualize:
        boxes,arrows=makescene(p)
    integ.preconditioner='R'
    integ.reinit_always=True
    t=integ.ModelTime
    res=[]
    sw=[]
    lastprint=t()
    tstart=time.clock()
    while t()<cmf.day*14:
        flux=lambda source:source.FluxTo(discharge,t())
        res.append(array(map(flux,[p.Cell(0).surface_water]+ list(p[0].layers))).cumsum())
        sw.append([c.GetStorage(0).h() for c in p.cells])
        try:
            integ(t()+cmf.h)
        except Exception, e:
            print e
            break
        if t()-lastprint>=cmf.h*6:
            print t(),"%f sec" % (time.clock()-tstart)
            lastprint=t()
        if visualize:
            if not refresh(t(),boxes,arrows):
                break
            
    print "%f sec" % (time.clock()-tstart)
    pylab.subplot(211)
    pylab.semilogy(array(res)+1e-6)
    pylab.grid()
    pylab.subplot(212)
    pylab.plot(array(sw)+0)
    pylab.grid()
    pylab.show()