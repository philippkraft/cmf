from HillSlope import *
from pylab import *


class hillplot:
    """Creates a plot of the layers of the cell 
    """    
    def __init__(self,cells,celllength,Solute=None,vmax=1,cmap=cm.jet_r):
        self.polygons={}
        for j,c in enumerate(cells):
            if type(c)==cmf.LayeredCell:
                cell=c.AsLayeredCell()
            elif type(c)==cmf.VerticalHomogeneousCell:
                cell=c.AsVerticalHomogeneousCell()
            
            if hasattr(celllength,'__getitem__'):
                cl=celllength[j]
            else:
                cl=celllength
            x1=c.Center().x-cl/2.
            x2=c.Center().x+cl/2.
            
            # Get left height
            if j: # If its not the first cell use average height of this and previous cell 
                y1=c.Height()-(c.Height()-cells[j-1].Height())/2
            else: # For first cell take height of cell itself
                y1=c.Height()
            
            #Get right height
            if j<len(cells)-1: # If it is not last cell use average height of this and next cell
                y2=c.Height()-(c.Height()-cells[j+1].Height())/2
            else:
                y2=c.Height()
            
            #Create Layer plots:
             
            for layer in cell:
                ylt=y1-layer.UpperBoundary()
                yrt=y2-layer.UpperBoundary()
                ylb=y1-layer.LowerBoundary()
                yrb=y2-layer.LowerBoundary()
                color=cmap(layer.Wetness())
                if Solute:
                    color=cmap(layer.conc(Solute)/vmax)
                    ylabel('['+Solute.Name+'] in mol/m3')
                else:
                    ylabel('Water saturation []')
                self.polygons[layer]=fill([x1,x2,x2,x1],[ylt,yrt,yrb,ylb],fc=color)[0]
    def Recolor(self,Solute=None,vmax=1,cmap=cm.jet_r):
        for layer in self.polygons:
            color=None
            if Solute:
                color=cmap(layer.conc(Solute)/vmax)
            elif type(layer)==cmf.Layer :
                color=cmap(layer.Wetness())
            if color: self.polygons[layer].set_fc(color)

            
