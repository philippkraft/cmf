'''
Created on 09.10.2009

@author: philkraf
'''
import cmf
from time import clock

class hillslope(object):
    def __init__(self,id,cells):
        self.id=id
        self.cells=cells
        self.cells.sort(key=lambda c: c.contributing_area,reverse=True)
        self.__downstream=None
        self.upstream=set()
        self.__integrator=None
    def __contains__(self,cell):
        return cell in (c.Id for c in self.cells)
    @property
    def downstream(self):
        return self.__downstream
    @downstream.setter
    def downstream(self,value):
        assert isinstance(value[0], cmf.Cell)
        assert isinstance(value[1], hillslope)
        self.__downstream=value
        value[1].upstream.add(self)
    @property
    def integrator(self):
        return self.__integrator
    @integrator.setter
    def integrator(self,template):
        self.__integrator=template.copy()
        for c in self.cells:
            self.__integrator.AddStatesFromOwner(c)
            
            

class connected_hillslopes(object):
    def connect_two_cells(self,cell1,cell2):
        """Connects to cells using their geometry"""
        # Get the intersection of the current cell and its outlet
        shape1=self.shapes[cell1]
        shape2=self.shapes[cell2]
        intersect = shape1.intersection( shape2)
        if (intersect.length>0):
            cell1.topology.AddNeighbor(cell2,intersect.length)
            cell2.topology.AddNeighbor(cell1,intersect.length)
            print "connected %s with %s width: %0.1fm" % (cell1,cell2,intersect.length)  

            return True
        else:
            print "!!! Connection %s with %s failed" % (cell1,cell2)              
            return False

    def get_hillslope(self,cell):
        return self.__hillslope_dict[cell.Id]
        
    def __init__(self, features, get_id,get_shape,get_hillslope,get_outlet,get_center,get_area,solutes=""):
        """Creates a project from features connecting only hillslopes
        Parameters:
        features: A list of features referencing an a shapely polygon and a height
        get_id       : A callable returning a Id for a feature
        get_shape    : A callable returning a shapely polygon for a feature
        get_hillslope: A callable returning a hillslope Id
        get_outlet   : A callable returning a feature Id of the feature this feature 
                        is connected to across a hillslope boundary 
        get_center   : A callable returning a 3-tuple holding the coordinates of
                        the center of the feature (x,y,z)
        get_area     : A callable returning the area of a feature
        solutes      : A string representing the solutes for this project (e.g. "N DOC"),
                        see help(cmf.project)
        """
        # Create a (Id,Feature) dictionary for later use
        feature_dict=dict(((get_id(f),f) for f in features))
        
        # Create a (HillslopeID,list(features)) dictionary to group the features
        hillslope_features={}
        for f in features:
            hillslope_features.setdefault(get_hillslope(f),[]).append(f)
        
        # Create the cmf project
        p=cmf.project(solutes)
        
                
        for id,feats in hillslope_features.iteritems():
            # Create and mesh the cells for each hillslope
            print "Processing hillslope #%i with %i cells, %f ha" % (id,len(feats),1e-4 * sum((get_area(f) for f in feats))) 
            cmf.cells_from_polygons(p, feats, 
                                    shape_callable=get_shape, 
                                    id_callable=get_id, 
                                    center_callable=get_center,
                                    area_callable=get_area)
        
        cmf.Topology.calculate_contributing_area(p.cells)
        # Create a (Id,Cell) dictionary for look up
        cells=dict(((c.Id,c) for c in p))
        get_cell=lambda f: cells.get(get_id(f))
        # Create a (HillslopeID,hillslope) dictionary to group the cells to hillslopes
        self.hillslopes=[ hillslope(id,[get_cell(f) for f in h]) for id,h in hillslope_features.iteritems() ]
        self.shapes=dict(((get_cell(f),f.shape) for f in features))
        self.__hillslope_dict={}
        for h in self.hillslopes:
            for c in h.cells:
                self.__hillslope_dict[c.Id]=h
                
        # Connect last cells of hillslopes with their outlet
        for f in features:
            # Get current cell from feature
            cell=get_cell(f)
            # Get the node feature 
            node_f=feature_dict.get(get_outlet(f))
            # if the outlet exists
            if node_f: 
                # Mesh up current cells and its outlet
                self.connect_two_cells(cell,  get_cell(node_f))
                # Mesh up their hillslopes
                upper_hillslope=self.get_hillslope(cell)
                lower_hillslope=self.get_hillslope(get_cell(node_f))
                upper_hillslope.downstream=get_cell(node_f), lower_hillslope
        self.project=p
        self.hillslopes.sort(key=lambda h: len(h.upstream))
    def create_integrators(self,template=None):
        if template is None:
            w_integ=cmf.CVodeIntegrator(1e-6)
            w_integ.preconditioner='R'
            s_integ=cmf.CVodeIntegrator(1e-6)
            s_integ.preconditioner='R'
            template=cmf.SoluteWaterIntegrator(w_integ,s_integ)   
        for h in self.hillslopes:
            h.integrator=template
    
    @property
    def t(self):
        return self.hillslopes[0].integrator.t
    @t.setter
    def t(self,value):
        for h in self.hillslopes:
            h.integrator.t=value
    
    def __call__(self,until):
        now=self.t
        start=clock()
        try:
            for h in self.hillslopes:
                h.integrator(until)
        except RuntimeError,e:
            self.t=now
            raise e
        return cmf.sec * (clock()-start)
    