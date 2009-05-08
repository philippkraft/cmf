import cmf
from pylab import *
def drawconn(upcVec,hold=1):
    was_interactive=isinteractive()
    if was_interactive: ioff()
    for c in upcVec:
        for n in c.neighbors():
            nc=n[0]
            l=array(((c.Center().x,c.Center().y),(nc.Center().x,nc.Center().y)))
            p=plot(l[:,0],l[:,1],'r:',alpha=0.5)
    if was_interactive: show()
def drawmainout(upcVec,hold=1):
    was_interactive=isinteractive()
    if was_interactive: ioff()
    for c in upcVec:
        mo=c.MainOutlet()
        if mo:
            l=array(((c.Center().x,c.Center().y),(mo.Center().x,mo.Center().y)))
            p=plot(l[:,0],l[:,1],'b:',lw=1)
    if was_interactive: show()
def drawlines(lines,style='k',alpha=1.,lw=1.,hold=1):
    was_interactive=isinteractive()
    if was_interactive: ioff()
    for p in lines:
        a=array(p[0])
        plot(a[:,0],a[:,1],style,alpha=alpha,lw=1.)
    if was_interactive: show()        
def drawpolygons(polygons,style='k',alpha=1.,hold=1):
    was_interactive=isinteractive()
    if was_interactive: ioff()
    for p in polygons:
        a=array(p[0])
        plot(a[:,0],a[:,1],style,alpha=alpha)
    if was_interactive: show()
def fillcells(upcvec,evalfunction="c.SaturatedDepth()",valuearray=None,cmap=cm.jet,alpha=1.,hold=1,vmin=None,vmax=None,time=None,whitezero=False,ec='k',lw=1,return_polygons=False):
    print "plot cells"
    if valuearray!=None:
        v=valuearray
    elif evalfunction:
        v=array([eval(evalfunction) for c in upcvec])
    else:
        raise "Either evalfunction or value array must be given!"
    if vmin==None:
        vmin=v.min()
    if vmax==None:
        vmax=v.max()
    if vmax-vmin<=0: 
        vmax=vmin+0.001
    was_interactive=isinteractive()
    if was_interactive: ioff()
    polygons=[]
    for i,c in enumerate(upcvec):
        p=c.Geometry()
        if p.size():
            a=array(p[0])
            if whitezero and v[i]==0:
                color=(1,1,1)
            else:
                color=cmap((v[i]-vmin)/(vmax-vmin))
            p=fill(a[:,0],a[:,1],fc=color,alpha=alpha,hold=hold,ec=ec,lw=lw)
            polygons.append(p[0])
            hold=1
    if was_interactive: show()
    if return_polygons:
        return polygons,vmin,vmax
    return vmin,vmax
def refillpolygons(polygons,valuearray,vmin,vmax,cmap=cm.jet):
    color=cmap((valuearray-vmin)/(vmax-vmin))
    for i,p in enumerate(polygons):
        p.set_facecolor(color[i])
            
def get_xyz_frompoints(points):
    a=array(points,float)
    return a[:,0],a[:,1],a[:,2]    
def drawreach(catchments,hold=1):
    was_interactive=isinteractive()
    if was_interactive: ioff()
    lines=[]
    for cs in catchments:
        r=cs.Reach().AsGeoReach()
        if r.Volume()<0.001 : 
            style='b:'
        else:
            style='b-'
        if r.Length()>0:
            rd=r.Type().Depth(r.Volume()/r.Length())
        else:
            rd=0         
        l=array(r.Line())
        lines.append(plot(l[:,0],l[:,1],style,lw=sqrt(rd*100)+1,hold=hold)[0])
        hold=1
    if was_interactive: show()
    return lines
def redrawreach(lines,q_out,vmax):
    for i,l in enumerate(lines):
        if q_out[i]<=0:
            l.set_linestyle(':')
            l.set_linewidth(1)
        else:
            l.set_linestyle('-')
            l.set_linewidth(sqrt(abs(q_out[i]/vmax))*10+1)

class fullwindow:
    def __init__(self,studyarea,t=cmf.Time()):
        was_interactive=isinteractive()
        if was_interactive: ioff()
        self.topmoistaxes=axes((0,0.5,0.5,0.5))
        self.topmoistpolygons=fillcells(studyarea.Cells(),"c.AverageSaturation(0.1)", vmin=0.2,vmax=1,cmap=cm.RdYlGn,lw=0, return_polygons=1)[0]
        axis('off')
        title('Top soil moisture')
        self.topmoistaxes.title.set_position((0.55,0.9))
        self.avgmoistaxes=axes((0.5,0.5,0.5,0.5))
        self.avgmoistpolygons=fillcells(studyarea.Cells(),"c.AverageSaturation()",vmin=0.2,vmax=1, cmap=cm.RdYlGn,lw=0, return_polygons=1)[0]
        axis('off')
        title('Average soil moisture')
        self.avgmoistaxes.title.set_position((0.55,0.9))
        self.satdepthaxes=axes((0,0,0.5,0.5))
        self.satdepthpolygons=fillcells(studyarea.Cells(),"c.SaturatedDepth()",vmin=0,vmax=2, cmap=cm.RdYlGn_r,lw=0, return_polygons=1)[0]
        axis('off')
        title('Saturated depth')
        self.satdepthaxes.title.set_position((0.55,0.9))
        self.runoffaxes=axes((0.5,0,0.5,0.5))
        self.runoffpolygons=fillcells(studyarea.Cells(),"-c.GetPondedFlow(time).q",time=t,cmap=cm.Blues,lw=0, return_polygons=1)[0]
        self.reachlines=drawreach(studyarea.Catchments)
        axis('off')
        title('Runoff')
        if was_interactive: show()

    def redraw(self,topmoist,avgmoist,satdepth,runoff,max_runoff,reach_q,max_reach_q):
        refillpolygons(self.topmoistpolygons, topmoist, 0, 1, cmap=cm.RdYlGn)
        refillpolygons(self.avgmoistpolygons, avgmoist, 0, 1, cmap=cm.RdYlGn)
        refillpolygons(self.satdepthpolygons, satdepth, 0, 2, cmap=cm.RdYlGn_r)
        refillpolygons(self.runoffpolygons, runoff, 0, max_runoff, cmap=cm.Blues)
        redrawreach(self.reachlines, reach_q, max_reach_q)
        if isinteractive(): draw()
               
   
    