from geometry import *
import pylab

def plotPolygons(shapes=None,values=[],linewidth=1,cmap=pylab.cm.jet,hold=True,faceted=True,filled=True,vmin=None,vmax=None,lc='k'):
    c=pylab.array(values)
    if vmin!=None:
        cmin=vmin
    else:
        cmin=c.min()
    if vmax!=None:
        cmax=vmax
    else:
        cmax=c.max()
    was_interactive=pylab.isinteractive()
    if was_interactive: pylab.ioff()
    
    for i,p in enumerate(shapes):
        if hasattr(p,'parts'):
            for part in p.parts:
                loc=pylab.array(part.asList())
                if part.IsRing() and filled:
                    color=cmap((c[i]-cmin)/(cmax-cmin))
                    pylab.fill(loc[:,0],loc[:,1],fc=color,ec=lc,lw=linewidth)
                else:
                    pylab.plot(loc[:,0],loc[:,1],c=lc,lw=linewidth)
                
    if was_interactive: pylab.show()
    
def plotPoints(points=None,c=None,s=25,**kwargs):
    loc=pylab.array([tuple(p) for p in points])
    pylab.scatter(loc[:,0],loc[:,1],c=c,s=s,**kwargs)
    