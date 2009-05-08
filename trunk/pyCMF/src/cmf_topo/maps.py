# -*- coding=utf-8 -*-
import shapely.geometry as geometry
import math
import raster
def distance(p1,p2):
    return math.sqrt((p1[0]-p2[0])**2+(p1[1]-p2[1])**2)
class Map(object):
    """
    A Map is the base class for different spatial data distributions.
    The base version contains returns always the default value
    
    A Map should implement the following functions:
    __iter__, returns an iterator over the items of a map
    values(), returns an iterator over the different objects of a map
    __call__(x,y,z), returns the object belonging to the position x,y,z
    """
    @property
    def default(self):
        return self._default
    def __init__(self,default=None):
        self._default=default
    def __iter__(self):
        yield self.default
    @property
    def values(self):
        yield self.default
    def __call__(self,x,y,z=0):
        """ returns default, regardless of the position given """
        return self.default
        
class nearest_neighbor_map(Map):
    """A map (spatial distribution of data) returning the nearest neighbor to the queried position
    Stores position referenced objects.
    
    z_weight is a weight, how important vertical differences are for neighborhood.
              0 means only horizontal distance and a high value only uses the height as a distance measure.
    The distance to be minimized is calculated as: sqrt((x1-x2)**2+(y1-y2)**2)+abs(z1-z2)*z_weight 
    """
    def __init__(self,z_weight=0):
        Map.__init__(self)
        self._objects={}
        self.z_weight=z_weight
    def append(self,position,object):
        self._objects[position]=object
    def remove(self,position):
        self._objects.pop(position)
    def __iter__(self):
        for p in self._objects:
            yield (p,self._objects[p])
    @property        
    def values(self):
        for v in self._objects.itervalues():
            yield v
    def __call__(self,x,y,z=0):
        """ returns the nearest neighbor object to the given position
            The distance to be minimized is calculated as: sqrt((x1-x2)**2+(y1-y2)**2)+abs(z1-z2)*z_weight 
        """
        mindist=1e32
        nearest=None
        for p in self._objects:
            z_dist=abs(z-p[2])
            actdist=distance((x,y,z), p)+self.z_weight*z_dist
            if (actdist<mindist):
                nearest=self._objects[p]
                mindist=actdist
        return nearest

class polygon_map(Map):
    """A map of polygons. Each object is referenced with a shapely polygon. 
    Returns the object of the first polygon, within the query position lays. 
    """
    def __init__(self):
        Map.__init__(self)
        self._objects={}
    def append(self,polygon,object):
        self._objects[polygon]=object
    def remove(self,polygon):
        self._objects.pop(polygon)
    def __iter__(self):
        for p in self._objects:
            yield (p,self._objects[p])
    @property
    def values(self):
        for v in self._objects.itervalues():
            yield v
    def __call__(self,x,y,z=0):
        loc=geometry.Point(x,y,z)
        for p in self._objects:
            if loc.within(p):
                return self._objects[p]        
        return self.default

class raster_map(Map):
    """A map based on an integer raster and a lookup dictionary referencing the map objects with the raster values 
    """
    def __init__(self,source):
        """Expects an integer raster or a filename"""
        Map.__init__(self)
        if isinstance(source, raster.Raster):
            self._raster=source
        elif isinstance(source,str):
            self._raster=raster.Raster(source, 'i')
        else:
            raise ValueError('Expected Raster of string as a source')
        self._objects={}
    @property
    def raster(self):
        return self._raster
    def append(self,rastervalue,object):
        self._objects[rastervalue]=object
    def remove(self,rastervalue):
        self._objects.pop(rastervalue)
    def __iter__(self):
        for rv in self._objects:
            yield (rv,self._objects[rv])
    @property
    def values(self):
        for v in self._objects.itervalues():
            yield v
    def __call__(self,x,y,z=0):
        v=self._raster(x,y)
        if v!=self._raster.nodata and v in self._objects:
            return self._objects[v]
        else:
            return self.default
            
            
                
        