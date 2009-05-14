from shapely.geometry import Polygon,MultiPolygon,Point,LineString,MultiLineString,MultiPoint,GeometryCollection
import pylab
from cmf.cell_factory import geometry as geoms
import cmf.cell_factory

def drawcells(cells,colorfunction=lambda c:c.z,cmap=pylab.cm.jet,hold=True,vmin=None,vmax=None,**kwargs):
    was_interactive=pylab.isinteractive()
    if was_interactive: pylab.ioff()
    geos=[]
    maxvalue=-1e300 if vmax is None else vmax
    minvalue=1e300 if vmin is None else vmin
    
    for c in cells:
        shape=geoms[c]
        if hasattr(shape, "geoms"):
            shapes=shape.geoms
        else:
            shapes=[shape]
        value=colorfunction(c)
        if vmax is None:
            maxvalue=max(value,maxvalue)
        if vmin is None:
            minvalue=min(value,minvalue)
        for s in shapes:
            geos.append((s,value))
    print minvalue,maxvalue
    if minvalue>=maxvalue: minvalue=maxvalue-1
    for s,v in geos:
        if isinstance(s, Polygon):
            s=s.exterior
        c=cmap(float(v-minvalue)/float(maxvalue-minvalue))
        a=pylab.array(s)
        pylab.fill(a[:,0],a[:,1],fc=c,hold=hold,**kwargs)
        hold=1
    pylab.axis('equal')
    if was_interactive:
        pylab.draw() 
        pylab.ion()
def drawobjects(objects,style=None,hold=1,**kwargs):
    was_interactive=pylab.isinteractive()
    if was_interactive: pylab.ioff()
    for i,o in enumerate(objects):
        shape=geoms.get(o)
        if hasattr(shape, "geoms"):
            shapes=list(shape.geoms)
        else:
            shapes=[shape]
        for s in shapes:
            if isinstance(s, Polygon):
                s=s.exterior
            a=pylab.array(s.coords)
            if style is None:
                pylab.plot(a[:,0],a[:,1],hold=hold,**kwargs)
            else:
                pylab.plot(a[:,0],a[:,1],style,hold=hold,**kwargs)
            hold=1
    pylab.axis('equal')
    if was_interactive:
        pylab.draw() 
        pylab.ion()
    
def draw_cell_arrows(cells,width_function,color='k',hold=1,**kwargs):
    """Draws arrows indicating fluxes from one cell to another
    
    Since this function loops through each cell and each neighbor of 
    a cell, the function is rather slow. Use smaller cell lists if appropriate
    
    cells: a "bunch" (sequence or generator) of cells
    width_function: a function f(cell1,cell2) returning the width of the arrow in map coordinates
    color: a pylab color
    hold: if False, clears the screen prior to drawing
    
    Additional keyword arguments:
       - see pylab.arrow
    """
    was_interactive=pylab.isinteractive()
    if was_interactive: pylab.ioff()
    if hold==0: pylab.clf()
    for c in cells:
        for n,w in c.neighbors:
            x,y,dx,dy=c.x,c.y,n.x-c.x,n.y,c.y
            v=weight_function(c,n)
            if v>0.001:
                pylab.arrow(x,y,dx/2,dy/2,c=color,width=v,hold=1,**kwargs)
    if was_interactive:
        pylab.draw() 
        pylab.ion()
    
def connector_matrix(cells,size=(500,500)):
    """Returns a matrixx
    """
    allstates=[]
    for c in cells:
        allstates.extend(c.storages)
    posdict={}
    jac=pylab.zeros(size,dtype=int)
    l=len(allstates)
    for i,a in enumerate( allstates):
        posdict[a.node_id]=i
    for i,a in enumerate( allstates):
        for f,t in a.fluxes(cmf.Time()):
            j=posdict.get(t.node_id)
            if j:
                jac[i*size[0]/l,j*size[1]/l]+=1
    return jac   
        

               
        

    

    