# -*- coding: utf-8 -*-
"""
Created on Thu Nov 05 09:10:18 2009

@author: philkraf
"""
import cmf
from DECOMP.cmf_DECOMP import DECOMPcmf
from matplotlib.pylab import *
def add_cell(p,x,h,last_cell):
    for i in range(1):
        c=p.NewCell(x+i*0.1,0,h,10)
        if last_cell:
            c.topology.AddNeighbor(last_cell,10)
        last_cell=c
    return c,x+1
def find_layer(cell,z):
    if z>cell.z:
        return cell.surfacewater
    else:
        for l in cell.layers:
            if z<=cell.z - l.upper_boundary and z>cell.z - l.lower_boundary:
                return l
    return None

def make_rice_paddies(project,count=3,cells_per_paddy=10,dam_height=1.0,terrace_height=1.0,r_curve=cmf.BrooksCoreyRetentionCurve()):
    h = (count+2) * terrace_height 
    x = 0
    c=None
    pool_of_cell={}
    pools=[]
    for i in range(count):
        c,x=add_cell(project,x,h + dam_height,c)
        if i>0: h-=terrace_height
        pool=cmf.OpenWaterStorage.create(project,cells_per_paddy * 10)
        pool.Name = "Paddy #%s" % i
        pool.Location = cmf.point(x + cells_per_paddy*0.5,0,h)
        pools.append(pool)
        cmf.connect(cmf.PenmanEvaporation, pool,c.evaporation, c.meteorology)       
        for j in range(cells_per_paddy):
            c,x=add_cell(project,x,h,c)
            pool_of_cell[c]=pool
        c,x=add_cell(project,x,h + dam_height,c)
    
    c,x=add_cell(project, x, 2 * terrace_height, c)
    for c in project:
        for d in arange(0.0,c.z,0.1):
            c.add_layer(d+0.1,r_curve)
            c.layers[-1].Location.z =c.z - (d+0.05)
        c.install_connection(cmf.Richards)
        c.install_connection(cmf.PenmanMonteithET)
        if c in pool_of_cell:
            cmf.connect(cmf.Richards,c.layers[0],c.surfacewater)
        c.saturated_depth=c.z - terrace_height*1.9
    for i,c in enumerate(project[:-1]):
        for l in c.layers:
            for n,w in c.neighbors:
                target = find_layer(n,l.Location.z)
                if target:
                    cmf.connect(cmf.Richards_lateral,l,target,w,1)
#    dbc=cmf.DricheletBoundary(project,0)
#    dbc.Name="Trench"
    c=project[-1]
#    dbc.Location = cmf.point(c.x+1,c.y,0)
    dbc=None
    #project[-1].connect_soil_with_node(dbc,cmf.Richards_lateral,10,1)
    return pools,dbc, pool_of_cell

def draw_fig(storages,t=cmf.Time(),qscale=100,alpha=1.0,tracer=None):
    ioff()
    r=cmf.Raster(shape=(max(c.layer_count() for c in p),len(p)),
                 corner=(-0.5,0),cellsize=(1,0.1))
    for s in storages:
        if isinstance(s, cmf.SoilLayer):
            r[s.Location.x, s.Location.z] = s.conc(tracer) if tracer else s.wetness 
    r.draw(hold=0,cmap=cm.RdYlGn_r if tracer else cm.RdYlBu,alpha=alpha,vmax=1.0,zorder=1,interpolation='nearest')
    axis('tight')
    pos=storages.get_positions()
    f=storages.get_fluxes3d(t)
    q=quiver(array(pos.X),array(pos.Z),array(f.X),array(f.Z),
            scale=qscale,minlength=0.1,zorder=2)
    
    draw()
    ion()
if __name__=='__main__':
    p=cmf.project("N DOC")
    pool_length=20
    dam_height=0.5
    terrace_height=0.5
    pool_count=3
    pools,dbc,pool_cell_dict=make_rice_paddies(p,pool_count,
                                               pool_length,dam_height,
                                               terrace_height)
    storages=cmf.node_list.from_sequence(pools)
    storages.extend(cmf.get_layers(p))
    for c in pool_cell_dict:
        c.layers[2].soil.SetKsat(0.1,0.0)
        c.layers[2].soil.Set_b(7.5)
    winteg=cmf.CVodeIntegrator(1e-6)
    winteg.preconditioner='R'
    sinteg=cmf.BDF2(1e-6)
    integ=cmf.SoluteWaterIntegrator(winteg,sinteg,p)    
    pools[0].depth = dam_height
    b = lambda : bar(left  =[(pool_length+2) * i +.5 for i in range(pool_count) ],
                     bottom=[(pool_count-i+2)*terrace_height for i in range(pool_count)],
                     width=pool_length,
                     height=[pool.depth for pool in pools],
                     zorder=0,fc=(0.6,0.6,1.0),ec=(0.3,0.3,1.0))
    scale=500
    
    DECOMPcells=dict((c,DECOMPcmf(c)) for c in pool_cell_dict)
    for c,D in DECOMPcells.iteritems():
        D.DECOMPlayers[0] = DECOMP.SOM(0.1*1e2,1e2)

    def run(p,for_t=cmf.min*15):
        for c,P in pool_cell_dict.iteritems():
            c.surfacewater.potential = P.potential
            c.surfacewater.is_source = not P.is_empty()
            DECOMPcells[c].run(27,for_t.AsDays())
        integ(integ.t+for_t)
        for c,P in pool_cell_dict.iteritems():
            P.volume += c.surfacewater.water_balance(integ.t) * for_t.AsDays()
        subplot(212)
        draw_fig(storages,integ.t,scale,0.85,tracer=p.solutes[0])
        b()
        subplot(211)
        draw_fig(storages,integ.t,scale,0.85)
        b()
        title(integ.t)
        return integ.t, [p.depth for p in pools]
    N,DOC=p.solutes
    show()
    
    
 