

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
   
import numpy
from matplotlib import pyplot
from itertools import chain
import cmf
class hill_plot(object):    
    """
    Plots a hillslope using colored sheared rectangles for each layer and
    arrows (matplotlib.quiver) to show fluxes.
    
    Properties:
    
     - evalfunction: a function returning the value of a layer to plot. 
       The value should be a float between 0..1 for scaling. 
       The default is: lambda l: l.wetness
     - q_sub:  The matplotlib.Quiver object for subsurface fluxes
     - q_surf: The matplotlib.Quiver object for subsurface fluxes
     - title:  Title of the plot
     - scale:  The scale of q_sub and q_surf
        
    """

    def __get_snow_height(self):
        return [(c.z + c.snow.volume/(c.area*0.08) if c.snow else c.z) for c in self.cells]
    def __get_layer_shape(self,layer,c_this,c_left,c_right):
        x_left=0.5*(self.__x(c_this.x,c_this.y)+self.__x(c_left.x,c_left.y))
        x_right=0.5*(self.__x(c_this.x,c_this.y)+self.__x(c_right.x,c_right.y))
        x_center=self.__x(c_this.x,c_this.y)
        z_left=0.5*(c_this.z+c_left.z)
        z_center=c_this.z
        z_right=0.5*(c_this.z+c_right.z)
        x=(x_left,x_center,x_right,x_right,x_center,x_left)
        ub,lb=layer.boundary
        z=(z_left-ub,z_center-ub,z_right-ub,z_right-lb,z_center-lb,z_left-lb)
        return x,z
    def __x(self,x=0,y=0):
        c=self.cells[0]
        return numpy.sqrt((x-c.x)**2.0 + (y-c.y)**2.0)
    def __init__(self,cells,t,solute=None,cmap=pyplot.cm.jet):
        """
        Creates a new hill_plot on the active figure, showing the state of each layer
         - cells: The a sequence of cmf cells to use in this hill_plot. You can
                  use the whole project if you like
         - t:     Current time step. Needed to retrieve the fluxes
         - solute:The solute concentration to show. If None, the wetness of the 
                  layer will be shown
         - cmap:  a matplotlib colormap (see module cm) for coloring
        """
        was_interactive=pyplot.isinteractive()
        if was_interactive: pyplot.ioff()
        self.cells=cells
        self.layers=cmf.node_list(chain(*[c.layers for c in cells]))
        self.surfacewater=cmf.node_list(c.surfacewater for c in cells)
        x_pos=[self.__x(c.x,c.y) for c in cells]
        self.topline=pyplot.plot(x_pos,self.__get_snow_height() ,'k-',lw=2)[0]
        self.__cells_of_layer={}
        self.polys={}
        if isinstance(solute,cmf.solute):
            self.evalfunction= lambda l: l.conc(solute)
        else:
            self.evalfunction= lambda l: l.wetness
        self.cmap=cmap
        for i,c in enumerate(cells):
            c_left=cells[i-1] if i else c
            c_right=cells[i+1] if i<len(cells)-1 else c
            
            for l in c.layers:
                x,z=self.__get_layer_shape(l, c, c_left, c_right)
                self.polys[l.node_id],=pyplot.fill(x,z,fc=self.cmap(self.evalfunction(l)),ec='none',zorder=0)
                self.__cells_of_layer[l.node_id]=(c,c_left,c_right)
        layer_pos=self.layers.get_positions()
        surf_pos=self.surfacewater.get_positions()
        w=numpy.array([l.wetness for l in self.layers])
        layer_f=self.layers.get_fluxes3d(t)
        surf_f=self.surfacewater.get_fluxes3d(t)
        scale = max(numpy.linalg.norm(surf_f.X), numpy.linalg.norm(layer_f.X)) * 10
        layerX = self.__x(numpy.asarray(layer_pos.X),numpy.asarray( layer_pos.Y))
        surfX = self.__x(numpy.asarray(surf_pos.X),numpy.asarray(surf_pos.Y))
        self.q_sub=pyplot.quiver(layerX,layer_pos.Z,layer_f.X + layer_f.Y,layer_f.Z,scale=scale,minlength=0.1,pivot='middle',zorder=1)
        self.q_surf=pyplot.quiver(surfX,surf_pos.Z,surf_f.X + surf_f.Y,surf_f.Z,color='b',scale=scale,minlength=0.1,pivot='middle',zorder=1)
        self.title=pyplot.title(t)
        if was_interactive:
            pyplot.draw() 
            pyplot.ion()
    def __call__(self,t,text=''):
        """
        Updates the hill_plot at time t. You can provide a title for the figure.
        If bool(title)==False, t is shown.
        """
        layer_f=self.layers.get_fluxes3d(t)
        surf_f=self.surfacewater.get_fluxes3d(t)
        self.q_sub.set_UVC(numpy.asarray(layer_f.X),numpy.asarray(layer_f.Z))
        self.q_surf.set_UVC(numpy.asarray(surf_f.X),numpy.asarray(surf_f.Z))
        for l in self.layers:
            self.polys[l.node_id].set_fc(self.cmap(self.evalfunction(l)))
            x,z=self.__get_layer_shape(l, *self.__cells_of_layer[l.node_id])
            self.polys[l.node_id].set_xy(numpy.column_stack((x,z)))
        self.topline.set_ydata(self.__get_snow_height())
        if not text: text=str(t)         
        self.title.set_text(text)
        if pyplot.isinteractive(): pyplot.draw()
    def __set_scale(self,scale):
        self.q_sub.scale=scale
        self.q_surf.scale=scale
    def __get_scale(self):
        return self.q_sub.scale
    scale=property(__get_scale,__set_scale,"The scaling of the arrows")    
    
   
