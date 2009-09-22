'''
Created on 28.08.2009

@author: philkraf
'''
import time
import cmf
class unsat1d_mi(object):
    class element(object):
        def __init__(self,cell,solver,solver_id):
            self.cell=cell
            self.solver_id=solver_id
            self.solver=solver
            
            self.cell.install_connection(cmf.Richards)
            for s in list(self.cell.surface_storages) + list(self.cell.layers)[:-1]:
                self.solver.add_states_to_integrator(s,self.solver_id)
            self.cell.layers[-2].remove_connection(self.cell.layers[-1])
            self.lower_boundary_condition=cmf.DricheletBoundary(self.cell.project(),self.cell.layers[-1].potential,self.cell.position)
            self.lower_boundary_condition.Name="lower boundary of " + str(cell)
            self.lower_boundary_condition.is_source=True
            cmf.connect(cmf.Richards, self.cell.layers[-2], self.lower_boundary_condition, self.cell.area, 1.0) 
        @property
        def t(self):  return self.solver.t
        @t.setter
        def t(self,T): self.solver.t=T    
        @property
        def lower_boundary_potential(self):
            return self.lower_boundary_condition.potential
        @lower_boundary_potential.setter
        def lower_boundary_potential(self,potential):
            self.lower_boundary_condition.potential=potential
        @property
        def lower_boundary_flux(self):
            return self.lower_boundary_condition.water_balance(self.t)
    
    def __init__(self,project,solver_template=cmf.CVodeIntegrator(1e-6),cells=None):
        '''
        Constructor
        '''
        if cells is None:
            self.cells=project.cells
        else : self.cells=cells
        self.project=project
        self.element={}
        self.solver=cmf.MultiIntegrator(solver_template,len(cells))
        self.solver.use_OpenMP=not solver_template.use_OpenMP
        print "MultiSolver created"
        for i,c in enumerate(self.cells):
            self.element[c]=unsat1d_mi.element(c,self.solver,i)
            
    def set_lower_potential(self,cell,potential):
        self.element[cell].lower_boundary_potential=potential
    def get_lower_flux(self,cell):
        return self.element[cell].lower_boundary_flux
    @property
    def t(self): return self.solver.t
    @t.setter
    def t(self,T):
        self.solver.t=T
    def __call__(self,until):
        self.solver(until)
    
class unsat1d(object):
    '''
    classdocs
    '''
    class element(object):
        def __init__(self,cell,solver_template=cmf.CVodeIntegrator(1e-6),solver=None):
            self.cell=cell
            if solver:
                self.solver=solver
            else:
                self.solver=solver_template.Copy()
            self.cell.install_connection(cmf.Richards)
            for s in self.cell.surface_storages:
                self.solver.AddStatesFromOwner(s)
            for l in list(self.cell.layers)[:-1]:
                self.solver.AddStatesFromOwner(l)
            self.cell.layers[-2].remove_connection(self.cell.layers[-1])
            self.lower_boundary_condition=cmf.DricheletBoundary(self.cell.project(),self.cell.layers[-1].potential,self.cell.position)
            self.lower_boundary_condition.Name="lower boundary of " + str(cell)
            self.lower_boundary_condition.is_source=True
            cmf.connect(cmf.Richards, self.cell.layers[-2], self.lower_boundary_condition, self.cell.area, 1.0) 
        @property
        def t(self):  return self.solver.t
        @t.setter
        def t(self,T): self.solver.t=T    
        @property
        def lower_boundary_potential(self):
            return self.lower_boundary_condition.potential
        @lower_boundary_potential.setter
        def lower_boundary_potential(self,potential):
            self.lower_boundary_condition.potential=potential
        @property
        def lower_boundary_flux(self):
            return self.lower_boundary_condition.water_balance(self.t)
    
    def __init__(self,project,solver_template=cmf.CVodeIntegrator(1e-6),cells=None,single_solver=False):
        '''
        Constructor
        '''
        if cells is None:
            self.cells=project.cells
        else : self.cells=cells
        self.project=project
        self.element={}
        if single_solver:
            self.solver=cmf.CVodeIntegrator(1e-6)
            self.solver.LinearSolver=2                
            for c in self.cells:
                self.element[c]=unsat1d.element(c, solver=self.solver)
        else:
            self.solver=None
            for c in self.cells:
                self.element[c]=unsat1d.element(c, solver_template)
            
    def set_lower_potential(self,cell,potential):
        self.element[cell].lower_boundary_potential=potential
    def get_lower_flux(self,cell):
        return self.element[cell].lower_boundary_flux
    @property
    def t(self): return self.element[self.cells[0]].solver.t
    @t.setter
    def t(self,T):
        for e in self.element.itervalues():
            e.solver.t=T
    def __call__(self,until):
        if self.solver:
            self.solver(until)
        else:
            for s in self.element.itervalues():
                try:
                    s.solver(until)
                except RuntimeError,e:
                    print "Error at ",s.cell
                    print e
        
class darcy2d(object):
    def __init__(self,project,solver_template=cmf.CVodeIntegrator(1e-6),cells=None):
        '''
        Constrructor
        '''
        if cells is None:
            self.cells=project.cells
        else: self.cells=cells
        self.project=project
        self.solver=solver_template.Copy()
        self.layers=cmf.node_list()
        self.layers.extend((c.layers[-1] for c in self.cells))
        self.solver.AddStatesFromOwner(self.layers)
        self.top_boundary={}
        for c in self.cells:
            b=cmf.NeumannBoundary(self.project,c.layers[-2].position)
            b.connect_to(c.layers[-1])
            b.Name='Node to unsaturated zone of ' + str(c)
            self.top_boundary[c]=b
        cmf.connect_cells_with_flux(self.project.cells,cmf.Darcy,-1)
    @property
    def t(self):
        return self.solver.t
    @t.setter
    def t(self,T): self.solver.t=T
    def __call__(self,until):
        self.solver(until)
    def set_top_flux(self,cell,flux):
        self.top_boundary[cell].flux=cmf.timeseries(flux)

class reach_system(object):
    class reach(object):
        def connect_downstream(self,reach_type,width,depth):
            rt=cmf.create_reachtype(reach_type,width,depth)
            print "Connect %s with %s" % (self,self.downstream)
            if isinstance(self.downstream,type(self)):
                dist=self.storage.get_distance_to(self.downstream.storage)
                self.downstream_connection=cmf.Manning(self.storage,self.downstream.storage,rt,dist)
            elif isinstance(self.downstream, cmf.FluxNode):
                dist=self.storage.get_distance_to(self.downstream)
                self.downstream_connection=cmf.Manning(self.storage,self.downstream,rt,dist)
                
        def __repr__(self):
            return str(self.storage.Name)        
        def __init__(self,cell,reaches,reach_type,width=1.0,depth=0.25):
            self.cell=cell
            self.project=self.cell.project()
            self.upstream=[n for n,w in cell.neighbors if n.topology.MainOutlet() == cell]
            self.downstream=reaches.get(cell.topology.MainOutlet())
            if self.downstream is None:
                N=self.cell.topology.neighbor_count()
                dir=cmf.point()
                for n in self.upstream:
                    dir+=self.cell.get_distance_to(n) * self.cell.get_direction_to(n) / N
                pos=self.cell.position - dir
                self.downstream=cmf.DricheletBoundary(self.project,pos.z,pos)
                self.length=self.cell.get_distance_to(self.downstream)*0.5
            else:
                self.length=self.cell.get_distance_to(self.downstream.cell)*0.5
            self.type=reach_type
            for u in self.upstream:
                self.length+=self.cell.topology.get_distance_to(u)*0.5
            if type=='R':
                self.storage=cmf.OpenWaterStorage(cell.project(),cell.area) 
            else:
                rt=cmf.create_reachtype(reach_type,width,depth)  
                channel=cmf.Channel(rt,self.length)
                self.storage=cmf.OpenWaterStorage(cell.project(),channel)
                self.storage.Location=cmf.point(self.cell.x+0.01,self.cell.y+0.01,self.cell.z-depth)
            self.storage.Name='Reach at '+str(self.cell)
            self.lateral_boundary=cmf.NeumannBoundary(self.project,cmf.point(self.cell.x+0.01,self.cell.y+0.01,self.cell.z))
            self.lateral_boundary.Name="R-Boundary at " + str(self.cell)

            self.bottom_potential=self.cell.z-depth
            self.lateral_boundary.connect_to(self.storage)
#            if self.downstream:
#                self.connect_downstream(reach_type,width,depth)
        @property
        def outlet(self):
            if isinstance(self.downstream, cmf.DricheletBoundary):
                return self.downstream
            else:
                return None
    def __init__(self,project,solver_template=cmf.ImplicitEuler(1e-6),type='T',cells=None):
        self.reaches={}
        self.outlets=cmf.node_list()
        self.storages=cmf.node_list()
        if cells is None:
            self.cells=list(project)
        else: self.cells=cells
        self.cells.sort(key=lambda c:c.z)
        for c in self.cells:
            r=self.reach(c,self.reaches,type)
            self.storages.append(r.storage)
            if r.outlet:
                self.outlets.append(r.outlet)
            self.reaches[c]=r
        self.solver=solver_template.Copy()
        self.solver.AddStatesFromOwner(self.storages)
        self.surface_water = cmf.node_list.from_sequence((c.surface_water for c in self.cells))
        self.lateral_boundaries = cmf.NeumannBoundary_list.from_sequence((self.reaches[c].lateral_boundary))
    def set_lat_flux(self,cell,flux):
        self.reaches[cell].lateral_boundary.flux=cmf.timeseries(flux)
    def __call__(self,until):
        for c in self.cells:
            self.set_lat_flux(c, c.surface_water.water_balance(self.solver.t))
        self.solver(until)
        
    @property
    def t(self): return self.solver.t
    @t.setter
    def t(self,value): self.solver.t=value
              
class saturated_distributed(object):
    def __init__(self,project,solver_template_unsat,solver_template_sat,cells=None,debug=0):
        if cells is None:
            self.cells=project.cells
        else: self.cells=cells
        self.unsat1d=unsat1d_mi(project,solver_template_unsat,self.cells)#,single_solver=1)
        self.darcy2d=darcy2d(project,solver_template_sat,self.cells)
        self.reach1d=reach_system(project,cmf.BDF2(1e-6))
        self.debug=debug
    def __call__(self,until):
        start_t=(self.t,time.clock())
        sec_per_year=cmf.sec*(cmf.year/(until-start_t[0]))

        for c in self.cells:
            self.unsat1d.set_lower_potential(c, c.layers[-1].potential)
        if self.debug: print until,"1d...",
        self.unsat1d(until)
        t1d=time.clock()-start_t[1]
        if self.debug: print "%s /  year, 2d..." % (sec_per_year*t1d),
        for c in self.cells:
            self.darcy2d.set_top_flux(c, self.unsat1d.get_lower_flux(c))
        self.darcy2d.solver(until)
        t2d=time.clock()-t1d-start_t[1]
        if self.debug: 
            print "%s / year" % (sec_per_year*t2d)
    @property
    def t(self):
        return self.darcy2d.t
    @t.setter
    def t(self,T): 
        self.darcy2d.t=T
        self.unsat1d.t=T
        print "Model time is",T
        
    def run(self,start,end,step):
        self.t=start
        while self.t<end:
            self(self.t+step)
            yield self.t
            
        
