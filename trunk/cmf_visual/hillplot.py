'''
Created on 10.09.2009

@author: philkraf
'''
import visual
import matplotlib.cm as cm
import cmf
class HillPlot(object):
    '''
    classdocs
    '''
    


    def __init__(self,cells,t,solute=None,colormap=cm.RdYlBu):
        '''
        Constructor
        '''
        self.cells=cells
        self.cmap=colormap
        self.layers=cmf.node_list()
        self.layers.extend(cmf.get_layers(cells))
        self.surface_water=cmf.node_list()
        self.surface_water.extend((c.surface_water for c in cells))
        self.polys={}
        self.__cells_of_layer={}
        if isinstance(solute,cmf.Solute):
            evalfunction= lambda l: l.conc(solute)
        else:
            evalfunction= lambda l: l.wetness
        for i,c in enumerate(cells):
            c_left=cells[i-1] if i else c
            c_right=cells[i+1] if i<len(cells)-1 else c           
            for l in c.layers:
                pos=self.__get_layer_shape(l, c, c_left, c_right)
                self.polys[l.node_id]= visual.convex(pos=visual.transpose(pos),color=self.cmap(evalfunction(l))[:3])
                self.__cells_of_layer[l.node_id]=(c,c_left,c_right)
    def __call__(self,t,text='',solute=None):
#        layer_f=self.layers.get_fluxes3d(t)
#        surf_f=self.surface_water.get_fluxes3d(t)
        if isinstance(solute,cmf.Solute):
            evalfunction= lambda l: l.conc(solute)
        else:
            evalfunction= lambda l: l.wetness
#        self.q_sub.set_UVC(numpy.asarray(layer_f.X),numpy.asarray(layer_f.Z))
#        self.q_surf.set_UVC(numpy.asarray(surf_f.X),numpy.asarray(surf_f.Z))
        for node in self.layers:
            l=cmf.AsSoilWater(node)
            self.polys[l.node_id].color=self.cmap(evalfunction(l))[:3]
#            x,z=self.__get_layer_shape(l, *self.__cells_of_layer[l.node_id])
            self.polys[l.node_id].set_xy(numpy.column_stack((x,z)))
        self.topline.set_ydata(self.__get_snow_height())
        if not text: text=str(t)         
        #self.title.set_text(text)

    def __get_layer_shape(self,layer,c_this,c_left,c_right):
        x_left=0.5*(c_this.x+c_left.x)
        x_right=0.5*(c_this.x+c_right.x)
        x_center=c_this.x
        y_left=0.5*(c_this.y+c_left.y)
        y_right=0.5*(c_this.y+c_right.y)
        y_center=c_this.y
        z_left=0.5*(c_this.z+c_left.z)
        z_center=c_this.z
        z_right=0.5*(c_this.z+c_right.z)
        x=(x_left,x_center,x_right,x_right,x_center,x_left)
        y=(y_left,y_center,y_right,y_right,y_center,y_left)
        ub,lb=layer.boundary
        z=(z_left-ub,z_center-ub,z_right-ub,z_right-lb,z_center-lb,z_left-lb)
        return x,y,z

        