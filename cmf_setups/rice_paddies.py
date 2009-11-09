# -*- coding: utf-8 -*-
"""
Created on Thu Nov 05 09:10:18 2009

@author: philkraf
"""
import cmf
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

def make_rice_paddies(project,count=3,cells_per_paddy=10,dam_height=1.0):
    h = (count+3) * dam_height
    x = 0
    c=None
    pool_of_cell={}
    pools=[]
    for i in range(count):
        c,x=add_cell(project,x,h,c)
        h-=dam_height
        if i>0: h-=dam_height
        pool=cmf.OpenWaterStorage.create(project,cells_per_paddy * 10)
        pool.Name = "Paddy #%s" % i
        pool.Location = cmf.point(x + cells_per_paddy*0.5,0,h)
        pools.append(pool)
        cmf.connect(cmf.PenmanEvaporation, pool,c.evaporation, c.meteorology)       
        for j in range(cells_per_paddy):
            c,x=add_cell(project,x,h,c)
            pool_of_cell[c]=pool
        h+=dam_height
        c,x=add_cell(project,x,h,c)
    
    c,x=add_cell(project, x, 2 * dam_height, c)
    bc=cmf.BrooksCoreyRetentionCurve(0.5,0.5,6,0.35)    
    for c in project:
        for d in arange(0.0,c.z,0.1):
            c.add_layer(d+0.1,bc)
            c.layers[-1].Location.z =c.z - (d+0.05)
        c.install_connection(cmf.Richards)
        c.install_connection(cmf.PenmanMonteithET)
        cmf.connect(cmf.Richards,c.layers[0],c.surfacewater)
        c.saturated_depth=c.z - dam_height*1.9
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

def draw_fig(storages,t=cmf.Time(),qscale=100,alpha=1.0):
    ioff()
    r=cmf.Raster(shape=(max(c.layer_count() for c in p),len(p)),
                 corner=(-0.5,0),cellsize=(1,0.1))
    for s in storages:
        if isinstance(s, cmf.SoilLayer):
            r[s.Location.x, s.Location.z] = s.wetness
    r.draw(hold=0,cmap=cm.RdYlBu,alpha=alpha,vmax=1.0,zorder=1)
    axis('tight')
    pos=storages.get_positions()
    f=storages.get_fluxes3d(t)
    q=quiver(array(pos.X),array(pos.Z),array(f.X),array(f.Z),
            scale=qscale,minlength=0.1,zorder=2)
    
    draw()
    ion()
def equalize_surfacewater(cells):
    mean_d=sum(c.surfacewater.depth for c in cells)/len(cells)
    for c in cells:
        c.surfacewater.depth = mean_d
if __name__=='__main__':
    p=cmf.project()
    pools,dbc,pool_cell_dict=make_rice_paddies(p,3,5,1)
    storages=cmf.node_list.from_sequence(pools)
    storages.extend(cmf.get_layers(p))
    integ=cmf.CVodeIntegrator(p,1e-6)
    integ.preconditioner='R'
    pools[1].depth=0.9
    b = lambda : bar(left=[.5,7.5,14.5],bottom=[5,4,3],width=5,height=[pool.depth for pool in pools],alpha=0.6)
    scale=100
    def run(for_t=cmf.min*15):
        for c,P in pool_cell_dict.iteritems():
            c.surfacewater.potential = P.potential
            c.surfacewater.is_source = not P.is_empty()
        integ(integ.t+for_t)
        for c,P in pool_cell_dict.iteritems():
            P.volume += c.surfacewater.water_balance(integ.t) * for_t.AsDays()
        draw_fig(storages,integ.t,scale)
        b()
        return integ.t, [c for c in p if c.surfacewater.is_source]
    run(cmf.min)
    RRR=b()
    show()
 