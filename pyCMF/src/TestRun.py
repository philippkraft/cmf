import pylab
import time
import cmf
from numpy import *
import visualCMF
import threading
def makescene(p):
    boxes=visualCMF.CellBoxes(p.cell_list(),9,9,None,5)
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
        return
    if visualCMF.v.scene.kb.keys:
        return
    boxes()
    boxes.label.text=str(t)
    arrows(t)
def refresh(t,boxes,arrows):
    refreshscene(t, boxes, arrows)
    #thread=threading.Thread(target=refreshscene,args=(t,boxes,arrows))
    #thread.start()
if __name__=="__main__":
    visualize=0
    p=cmf.project()
    p.debug=0
    p.Rainfall().Location.z=8
    #vgm=cmf.VanGenuchtenMualem(2.0,0.05,0.5,5)
    bc=cmf.BrooksCoreyRetentionCurve()
    bc.SetKsat(4,0.5)
    bc_clay=cmf.BrooksCoreyRetentionCurve(0.01,0.6,11.0,0.5)
    r_curve=cmf.RCurve(bc,1)
    r_curve_map=cmf.SingleValueMap_RetentionCurve(r_curve)
    z=array([0.0,0.1]+list(arange(0,20,0.5)))*1
    for i in range(10):
        #z=i*0.5 if i else 0.9
        c=p.NewCell(i*10,0,z[i],100)
        c.sw=c.AddStorage('','W')
        c.Transpiration().Location+=cmf.point(2,0,1)
        lcount=10
        sd=2.
        for j in range(1,lcount+1):
            if (i>=0 and i<9 and j==5):
                c.AddLayer(j*1./lcount*sd,bc_clay)
            else:
                c.AddLayer(j*1./lcount*sd,r_curve)
            c[-1].SetPotential(c.z*.5-.5)
        cmf.MatrixInfiltration(c.layers[0],c.surface_water).thisown=0
        c.Vegetation.RootDepth=0.3
        c.Vegetation.fraction_at_rootdepth=0.8
        cmf.Richards.use_for_cell(c)
        #c.SurfaceWater().State(5)
        cmf.PenmanMonteithET.use_for_cell(c)
        if i:
            c.AddNeighbor(p.Cell(-2),10)
            p.Cell(-2).AddNeighbor(c,10)
    #c.SurfaceWater().State(30)
    #p.Connect(cmf.Darcy.cell_connector(),0)      
    p.Connect(cmf.Richards.cell_connector,0)
    p.Connect(cmf.Manning.cell_connector,0)
    pdata=cmf.SinglePrecipitationTimeseries(100.0)
    pdata.Data.step=cmf.day*4
    for i in range(200):
        pdata.Data.Add(100.0 * (i%2))
    print "c[0].W=%f, c[1].W=%f" % (p.Cell(0)[0].Wetness(),p.Cell(0)[1].Wetness())
    p.SetPrecipitation(pdata)
    discharge=cmf.FluxNode(p)
    discharge.Location=cmf.point(-6,0,-.5)
    discharge.Name="Discharge"
    r1=cmf.Reach(p.Cell(0),discharge, 6,'T',0.1,1.)
    r2=cmf.Reach(p.Cell(1),r1, 10,'T',0.15,1.)
    r3=cmf.Reach(p.Cell(2),r2, 10,'T',0.05,1.)
    #rect_channel=cmf.RectangularReach(10)
    for i in range(0,10,1):
        p.Cell(i).surface_water.State(10)
    p.Cell(-1).surface_water.State(100)
    c=p.Cell(0)
    c.GetStorage(0).State(0)
    for l in p.Cell(0):
        cmf.connect(cmf.Richards,l,discharge,10*l.Thickness(),1)
        #l.SetPotential(c.z)

    integ=cmf.CVodeIntegrator(p,1e-6)
    #integ=cmf.BDF2(p,1e-6)
    #integ.MaxOrder=2
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
        res.append(array(map(flux,[p.Cell(0).surface_water]+ list(p.Cell(0)))).cumsum())
        sw.append([c.GetStorage(0).h() for c in p.cells()])
        try:
            integ(t()+cmf.h)
        except Exception, e:
            print e
            break
        if t()-lastprint>=cmf.h*6:
            print t(),"%f sec" % (time.clock()-tstart)
            lastprint=t()
        if visualize:
            refresh(t(),boxes,arrows)
            
    print "%f sec" % (time.clock()-tstart)
    pylab.subplot(211)
    pylab.semilogy(array(res)+1e-6)
    pylab.grid()
    pylab.subplot(212)
    pylab.plot(array(sw)+0)
    pylab.grid()
    pylab.show()