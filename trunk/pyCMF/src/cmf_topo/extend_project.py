import cmf
def AddLayers(cells,profile_map,soil_depth_map=None):
    """ Adds layers from a soil map containing soil profiles to a sequence of cells
        cells          : A sequence or iterator of cells (can be a cmf.project)
        profile_map    : (Required) a callable (e.g. a Map) implementing __call__(x,y,z), 
                         returning a cmf.profile
        soil_depth_map : (Optional) a callable (e.g a Map or Raster) implementing 
                         __call__(x,y,z)__ returning a number representing the soil depth in m
                         If omitted, the depth of the profile is used
    """
    for c in cells:
        profile=profile_map(c.x,c.y,c.z)
        maxdepth=soil_depth_map(c.x,c.y,c.z) if soil_depth_map else profile.lowerboundary(-1)
        lastlayerid=profile.size()-1 
        for i in range(profile.size()):
            if profile.upperboundary(i)<maxdepth:
                c.AddLayer(min(layer.lowerboundary,maxdepth) if i<lastlayerid else maxdepth ,layer.r_curve)
def AddFlexLayerPair(project,r_curve_map,soil_depth_map):
    """ Adds a unsatureated / saturated zone layer pait from a soil map containing retention curves to a sequence of cells
        cells          : A sequence or iterator of cells (can be a cmf.project)
        r_curve_map    : (Required) a callable (e.g. a Map) implementing __call__(x,y,z), 
                         returning a cmf.RCurve
        soil_depth_map : (Requiered) a callable (e.g a Map or Raster) implementing 
                         __call__(x,y,z)__ returning a number representing the soil depth in m
    """
    for c in project:
        cmf.FlexibleSizeSaturatedZone.Create(c,soil_depth_map(c.x,c.y,c.z), r_curve_map(c.x,c.y,c.z))
def SetVegetation(cells,vegetation_map):
    """Sets the vegetation parameters for each cell in a sequence of cells
        cells          : A sequence or iterator of cells (can be a cmf.project)
        profile_map    : (Required) a callable (e.g. a Map) implementing __call__(x,y,z), 
                         returning a cmf.Vegetation
    """ 
    for c in cells:
        c.Vegetation=vegetation_map(c.x,c.y,c.z)      
            