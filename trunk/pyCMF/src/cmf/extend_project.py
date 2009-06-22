import cmf_core as cmf

class profilelayer:
    def __init__(self,upper_boundary,lower_boundary,r_curve):
        if lower_boundary-upper_boundary<=0:
            raise ValueError("Upper boundary must be smaller than the lower boundary, upper b. is %0.2f and lower b. is %0.2f" % ( upper_boundary,lower_boundary) )
        if not isinstance(r_curve, cmf.RetentionCurve):
            raise ValueError("Third argument must be a cmf.RetentionCurve")
        self.upper_boundary=float(upper_boundary)
        self.lower_boundary=float(lower_boundary)
        self.retentioncurve=r_curve
        self.islast=True
    @property
    def thickness(self):
        return self.lower_boundary-self.upper_boundary
    def __repr__(self):
        return "layer %g - %g m: %s" % (self.upper_boundary,self.lower_boundary,self.retentioncurve)
class profile:
    def __init__(self):
        self.layers=[]
    def append(self,lower_boundary,r_curve):
        if len(self): self.layers[-1].islast=False
        pl=profilelayer(self.layers[-1].lower_boundary if len(self) else 0.0,lower_boundary,r_curve)
        self.layers.append(pl)
    def __iter__(self):
        for l in self.layers:
            yield l
    def __len__(self):
        return len(self.layers)
    def __getitem__(self,index):
        return self.layers[index]
def add_layers_to_cells(cells,profile_map,soil_depth_map=None,min_thickness=0.05):
    """ Adds layers from a soil map containing soil profiles to a sequence of cells
        cells          : A sequence or iterator of cells (can be a cmf.project)
        profile_map    : (Required) a callable (e.g. a Map) implementing __call__(x,y,z), 
                         returning a cmf.profile
        soil_depth_map : (Optional) a callable (e.g a Map or Raster) implementing 
                         __call__(x,y,z)__ returning a number representing the soil depth in m
                         If omitted, the depth of the profile is used
        min_thickness  : (default=0.05) If the thickness of a bottom layer is below this value, it is integrated with the next layer
    """
    count=0
    for c in cells:
        profile=profile_map(c.x,c.y,c.z)
        maxdepth=soil_depth_map(c.x,c.y,c.z) if soil_depth_map else profile[-1].lower_boundary
        if not maxdepth is None:
            for l in profile: 
                if l.upper_boundary<maxdepth:
                    count+=1
                    p_lb=l.lower_boundary
                    if p_lb+min_thickness>maxdepth:
                        p_lb=maxdepth
                    lb=maxdepth if l.islast else min(maxdepth, p_lb)
                    c.add_layer(lb ,l.retentioncurve)
                    if p_lb>=maxdepth:
                        break
    return count
def add_variable_layer_pair_to_cells(cells,r_curve_map,soil_depth_map):
    """ Adds a unsaturated / saturated zone layer pait from a soil map containing retention curves to a sequence of cells
        cells          : A sequence or iterator of cells (can be a cmf.project)
        r_curve_map    : (Required) a callable (e.g. a Map) implementing __call__(x,y,z), 
                         returning a cmf.RCurve
        soil_depth_map : (Requiered) a callable (e.g a Map or Raster) implementing 
                         __call__(x,y,z)__ returning a number representing the soil depth in m
    """
    for c in cells:
        c.add_variable_layer_pair(soil_depth_map(c.x,c.y,c.z), r_curve_map(c.x,c.y,c.z))
def change_vegetation(cells,vegetation_map):
    """Sets the vegetation parameters for each cell in a sequence of cells
        cells          : A sequence or iterator of cells (can be a cmf.project)
        profile_map    : (Required) a callable (e.g. a Map) implementing __call__(x,y,z), 
                         returning a cmf.Vegetation
    """ 
    for c in cells:
        v=vegetation_map(c.x,c.y,c.z)
        if not v is None:
            c.Vegetation=v      
def connect_cells_with_flux(cells,connection,start_at_layer=0):
    """Connects all cells in cells (sequence or generator) with a flux connection
    connection is an subclass of cmf.FluxConnection which exposes the cell_connector callable 
                    (e.g. lateral subsurface fluxes and surface manning flux)
    start_at_layer : if the flux connection should only be used for deeper layers
    """
    if (hasattr(connection, 'cell_connector') and isinstance(connection.cell_connector,cmf.CellConnector)):
        cmf.connect_cells_with_flux(list(cells),connection.cell_connector,start_at_layer)
    else:
        raise TypeError("flux_connection does not implement the cell_connector protocol")        
def sum_flux(source_storages,target_storages,t):
    """ Returns the sum of fluxes from a sequence (or generator) of sources to a sequence of target storages
    A positive return value indicates flux from source to target
    """
    sum=0.0
    for source in source_storages:
        for f,target in source.fluxes(t):
            if target in target_storages:
                sum-=f
    return sum 
