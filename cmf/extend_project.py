

# Copyright 2010 by Philipp Kraft
# This file is part of cmf.
#
#   cmf is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 2 of the License, or
#   (at your option) any later version.
#
#   cmf is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
#
from __future__ import print_function, unicode_literals, absolute_import, division

from . import cmf_core as cmf

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
    def __init__(self, retention_curves=[], depth=[]):
        self.layers=[]
        if depth and retention_curves:
            for i,d in enumerate(depth):
                self.append(d,retention_curves[min(i,len(retention_curves)-1)])                                          
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
    def __call__(self,x,y,z):
        return self

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
