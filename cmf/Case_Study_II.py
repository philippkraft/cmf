# -*- coding: utf-8 -*-
"""
Case Study II: Water balance - Single layer Storage approach
The Case Study represents a summer wheat setup of PMF and with the
SoilWaterContainer (SWC) as water balance model:

Weather     : Giessen,

Soil texture: Silt

Soil        : SWC,

Atmosphere  : cmf1d,      

Simulation  : 1.1.1980 - 31.12.1980 and 

Management  : Sowing - 1.3.JJJJ, Harvest - 8.1.JJJJ.


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
    if t.day==1 and t.month==3:
        plant = PMF.connect(PMF.createPlant_SWC(),soil,atmosphere)
        plant.nitrogen.Km = 27 * 62e-6
        plant.nitrogen.NO3min = 0.1e-3

    if t.day==1 and t.month==8:
        plant =  None
    #Let grow
    if plant: 
        plant(t,'day',1.)
     
    
    #Calculates evaporation for bare soil conditions
   
    ETc_adj = sum(plant.Wateruptake)+plant.et.evaporation if plant else baresoil.evaporation
    evaporation = plant.et.evaporation if plant else baresoil.evaporation
    rainfall = atmosphere.cell.get_rainfall(t)
    Zr = plant.root.depth/100. if plant else 0.
    soil(ETc_adj,evaporation,rainfall,Zr)
    
    res.water_stress.append(plant.water_stress) if plant else res.water_stress.append(0)
    res.potential_depth.append(plant.root.potential_depth) if plant else res.potential_depth.append(0)
    res.rooting_depth.append(plant.root.depth) if plant else res.rooting_depth.append(0)
    res.water_uptake.append(plant.Wateruptake) if plant else res.water_uptake.append([0])
    res.transpiration.append(plant.et.transpiration) if plant else res.transpiration.append(0)
    res.evaporation.append(plant.et.evaporation) if plant else  res.evaporation.append(0)
    res.biomass.append(plant.biomass.Total) if plant else res.biomass.append(0)
    res.root_biomass.append(plant.root.Wtot) if plant else res.root_biomass.append(0)
    res.shoot_biomass.append(plant.shoot.Wtot) if plant else res.shoot_biomass.append(0)
    res.lai.append(plant.shoot.leaf.LAI) if plant else res.lai.append(0)
    res.ETo.append(plant.et.Reference) if plant else res.ETo.append(0)
    res.ETc.append(plant.et.Cropspecific) if plant else res.ETc.append(0)
    res.rain.append(atmosphere.cell.get_rainfall(t))
    res.DAS.append(t-datetime(1980,3,1)) if plant else res.DAS.append(0)
    res.temperature.append(atmosphere.get_tmean(t))
    res.radiation.append(atmosphere.get_Rs(t))
    res.stress.append(plant.water_stress if plant else 0.)
    res.leaf.append(plant.shoot.leaf.Wtot if plant else 0.)
    res.stem.append(plant.shoot.stem.Wtot if plant else 0.)
    res.storage.append(plant.shoot.storage_organs.Wtot if plant else 0.)
    res.Dr.append(soil.Dr)
    res.TAW.append(plant.water.TAW if plant else 0.)
    res.RAW.append(plant.water.RAW if plant else 0.)
    atmosphere.run(cmf.day) 
    return plant

class Res(object):
    def __init__(self):
        self.water_uptake = []
        self.transpiration = []
        self.evaporation = []
        self.biomass = []
        self.root_biomass = []
        self.shoot_biomass = []
        self.lai = []
        self.ETo = []
        self.ETc = []
        self.rain = []
        self.temperature = []
        self.radiation = []
        self.DAS = []
        self.leaf=[]
        self.stem=[]
        self.storage=[]
        self.Dr=[]
        self.TAW=[]
        self.RAW=[]
        self.stress=[]
        self.fc=[]
        self.wp=[]
        self.rooting_depth=[]
        self.potential_depth=[]
        self.water_stress=[]
        
    def __repr__(self):
        return "Shoot=%gg, Root=%gg, ETc = %gmm, Wateruptake=%gmm, Stress=%s" % (self.shoot_biomass[-1],self.root_biomass[-1],self.ETc[-1],sum(self.water_uptake[-1]),self.stress[-1])
if __name__=='__main__':
#######################################
#######################################
### Setup script   

    from pylab import *
    from datetime import *
    import PMF
    import cmf
    from cmf_setup import cmf1d
    import psyco
    psyco.full()
    
    #Create cmf cell    
    atmosphere=cmf1d()
    atmosphere.load_meteo(rain_factor=1)
    
    soil = PMF.ProcessLibrary.SWC()
    baresoil = PMF.ProcessLibrary.ET_FAO([0.,0.,0.,0.],[0.,0.,0.,0.],kcmin = 0.)
    #set management
    sowingdate = set(datetime(i,3,1) for i in range(1980,2100))
    harvestdate = set(datetime(i,8,1) for i in range(1980,2100))
    #Simulation period
    start = datetime(1980,1,1)
    end = datetime(1982,12,31)
    #Simulation
    res = Res()
    plant = None
    print "Run ... "    
    #start_time = datetime.now()
    atmosphere.t = start
    while atmosphere.t<end:
        plant=run(atmosphere.t,res,plant)
        print "time: %s" % atmosphere.t
    
#######################################
#######################################
### Show results

timeline=drange(start,end,timedelta(1))
subplot(311)
plot_date(timeline,res.RAW,'k',label='Readily available Water')
plot_date(timeline,res.Dr,'r--',label='Depletion')
legend(loc=0)
ylabel('Water balance [mm]')   
subplot(312)
plot_date(timeline,res.water_stress,'b',label='Drought stress')
ylabel('Stress index [-]')
ylim(0,1)
legend(loc=0)
subplot(313)
plot_date(timeline,[-r for r in res.rooting_depth],'g',label='Actual')
plot_date(timeline,[-r for r in res.potential_depth],'k--',label='Potential')
ylabel('Rooting depth [mm]')
legend(loc=0)
show()
    
    
    
    
    
   
    
    