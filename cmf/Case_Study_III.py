# -*- coding: utf-8 -*-
"""
Case Study III: Drought event
The Case Study represents a summer wheat setup of PMF and with the
Catchment Modeling Framework (CMF) in the version cmf1d.

Weather     : Giessen,

Soil texture: Silt

Soil        : cmf1d

Atmosphere  : cmf1d,      

Simulation  : 1.1.1980 - 31.12.1980 and 

Management  : Sowing - 1.3.JJJJ, Harvest - 8.1.JJJJ.

Changing conditions: The upper soil layers dry out in the early growing season.
                    This effect is represented through increased water fluxes
                    from the upper three soil layers between 1.4.JJ And 1.5.JJ.


@author: Sebastian Multsch

@version: 0.1 (26.10.2010)

@copyright: 
 This program is free software; you can redistribute it and/or modify it under  
 the terms of the GNU General Public License as published by the Free Software  
 Foundation; either version 3 of the License, or (at your option) any later 
 version. This program is distributed in the hope that it will be useful, 
 but WITHOUT ANY  WARRANTY; without even the implied warranty of MERCHANTABILITY 
 or  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
 more details. You should have received a copy of the GNU General 
 Public License along  with this program;
 if not, see <http://www.gnu.org/licenses/>.
"""
#######################################
#######################################
### Runtime Loop

def run(t,res,plant):
    #Management
    if t.day==1 and t.month==3:
        plant_swc = PMF.connect(PMF.createPlant_SWC(),swc_fp,cmf_fp)
        plant_cmf = PMF.connect(PMF.createPlant_CMF(),cmf_fp,cmf_fp)
        plant = [plant_swc,plant_cmf]
    if t.day==1 and t.month==8:           
        plant = None  
        
    #Calculates evaporation for bare soil conditions
    baresoil(c.Kr(),0.,c.get_Rn(t, 0.12, True),c.get_tmean(t),c.get_es(t),c.get_ea(t), c.get_windspeed(t),0.,RHmin=30.,h=1.)    
    
    #plant growth
    if plant: [p(t,'day',1.) for p in plant]
    
    #swc_fp_interaction
    Wateruptake = sum(plant[0].Wateruptake) if plant else 0.
    evaporation = plant[0].et.evaporation if plant else baresoil.evaporation
    rainfall =c.cell.rain(t)
    Zr = plant[0].root.depth/100. if plant else 0.
    ETc_adj = Wateruptake + evaporation
    swc_fp(ETc_adj,evaporation,rainfall,Zr)
        
    #cmf_fp_interaction
    flux = [uptake*-1. for uptake in plant[1].Wateruptake] if plant  else zeros(c.cell.layer_count())
    flux[0] -= plant[1].et.evaporation if plant else baresoil.evaporation
    
    #influences through lateral flow
    if t.day>=1  and t.month==4 or t.month==5:
        flux[0]-=1
        flux[1]-=1
        flux[2]-=1
  
    c.flux=flux
    
   
        
    res[1].Dr.append(c.wetness)
    res[0].Dr.append(swc_fp.Dr)
    res[0].rain.append(c.cell.rain(t)) 
    res[0].radiation.append(c.get_Rn(t, 0.12, True))
    #Results
    if plant:
        res[0].TAW.append(plant[0].water.TAW)
        res[0].RAW.append(plant[0].water.RAW)
        res[0].rootdepth.append(plant[0].root.depth)
        res[1].rootdepth.append(plant[1].root.branching)
        res[1].root_growth.append(plant[1].root.actual_distribution)
        
        for i,p in enumerate(plant):
            res[i].W_shoot.append(p.shoot.Wtot)
            res[i].W_root.append(p.root.Wtot)
            res[i].LAI.append(p.shoot.leaf.LAI)
            res[i].Sh.append(p.Wateruptake)
            res[i].T.append(p.et.transpiration)
            res[i].E.append(p.et.evaporation)
            res[i].stress.append(p.water_stress)
            res[i].W_potential.append(p.biomass.pot_total)
            res[i].rootdepth_pot.append(p.root.potential_depth)
            res[i].W_leaf.append(p.shoot.leaf.Wtot)
            res[i].W_stem.append(p.shoot.stem.Wtot)
            res[i].W_storage.append(p.shoot.storage_organs.Wtot)
           
    else:
        res[0].TAW.append(0.)
        res[0].RAW.append(0.)
        res[0].rootdepth.append(0)
        res[1].rootdepth.append(zeros(c.cell.layer_count()))
        res[1].root_growth.append(zeros(c.cell.layer_count()))
        res[0].Sh.append(0.)
        res[1].Sh.append(zeros(c.cell.layer_count()))
        for r in res:
            r.W_shoot.append(0.)
            r.W_root.append(0.)
            r.LAI.append(0.)
            
            r.T.append(0.)
            r.E.append(0.)
            r.stress.append(0.)
            r.W_potential.append(0.)
            r.rootdepth_pot.append(0.)
            r.W_leaf.append(0.)
            r.W_stem.append(0.)
            r.W_storage.append(0.)
 
    c.run(cmf.day)
    return plant


class Results():
    def __init__(self):
        self.W_shoot=[]
        self.W_root=[]
        self.W_leaf=[]
        self.W_stem=[]
        self.W_storage=[]
        self.rootdepth=[]
        self.LAI=[]
        self.Sh=[]
        self.T=[]
        self.E=[]
        self.stress=[]
        self.TAW=[]
        self.RAW=[]
        self.Dr=[]
        self.rain=[]
        self.W_potential=[]
        self.rootdepth_pot=[]
        self.radiation=[]
        self.root_growth=[]
        
        
    def __repr__(self):
        return "Shoot = %gg, Root =% gg, LAI = %gm2/m2, Wateruptake =% gmm, T = %gmm, E = %gmm, Stress = %g" % (self.W_shoot[-1],self.W_root[-1],self.LAI[-1],sum(self.Sh[-1]),self.T[-1],self.E[-1],self.stress[-1])

#######################################
#######################################
### Setup script

if __name__=='__main__':
    from pylab import *
    from datetime import *
    import PMF
    import cmf
    from cmf_setup import cmf1d
    from cmf_fp_interface import cmf_fp_interface
    import psyco
    import time
    psyco.full()
    
    r1=Results()
    r2=Results()
    res = [r1,r2]
    
    #Create Evaporation modul
    baresoil = PMF.ProcessLibrary.ET_FAO([0.,0.,0.,0.],[0.,0.,0.,0.],kcmin = 0.)
    
    #Create cmf cell    
    c=cmf1d()
    c.load_meteo(rain_factor=.75)
    cmf_fp = cmf_fp_interface(c.cell)
    c.cell.saturated_depth=5.
    
    #Create Soilwater container
    swc_fp = PMF.SWC()
    
    #Create management
    sowingdate = set(datetime(i,3,1) for i in range(1980,2100))
    harvestdate = set(datetime(i,8,1) for i in range(1980,2100))
    #Simulation period
    start = datetime(1980,3,1)
    end = datetime(1980,12,31)
    
    plant = None
    c.t = start
    while c.t<end:
        plant=run(c.t,res,plant)
        
        #shoot_data[i] += res[0].W_shoot[-1]
        #shoot_line = make_show(shoot_line,shoot_data)
        #i+=1
        
        print c.t

   
#######################################
#######################################
### Show results    


subplot(311)
title('Root biomass in each soil layer [g]')
imshow(transpose([r[:20] for r in res[1].rootdepth]),cmap=cm.Greens,aspect='auto',interpolation='nearest',extent=[60,200,100,0])
ylabel('Depth [cm]')
colorbar()
subplot(312)
title('Root growth in each soil layer [g]')
imshow(transpose([r[:20] for r in res[1].root_growth]),cmap=cm.Greens,aspect='auto',interpolation='nearest',extent=[60,200,100,0])
ylabel('Depth [cm]')
colorbar()
subplot(313)
title('Wetness [m3 m-3]')
imshow(transpose([r[:20] for r in res[1].Sh]),cmap=cm.Blues,aspect='auto',interpolation='nearest',extent=[60,200,100,0])
ylabel('Depth [cm]')
xlabel('Day of year')
colorbar()
        