# -*- coding: utf-8 -*-
"""
Case Study I: Water balance - Multi layer Richards approach
The Case Study represents a summer wheat setup of PMF and with the
Catchment Modeling Framework (CMF) in the version cmf1d:

Weather     : Giessen,

Soil texture: Silt,

Soil        : cmf1d,

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
        plant = PMF.connect(PMF.createPlant_CMF(),cmf_fp,cmf_fp)
        #plant.nitrogen.max_passive_uptake=1
        #plant.nitrogen.Km=27*14e-6
        plant.nitrogen.Km = 27 * 62e-6
        plant.nitrogen.NO3min = 0.1e-3

    if t.day==1 and t.month==8:
        plant =  None
    #Let grow
    if plant: 
        plant(t,'day',1.)
        
    #Calculates evaporation for bare soil conditions
    baresoil(c.Kr(),0.,c.get_Rn(t, 0.12, True),c.get_tmean(t),c.get_es(t),c.get_ea(t), c.get_windspeed(t),0.,RHmin=30.,h=1.)    
    flux = [uptake*-1. for uptake in plant.Wateruptake] if plant  else zeros(c.cell.layer_count())
    flux[0] -= plant.et.evaporation if plant else baresoil.evaporation
    c.flux=flux
    c.run(cmf.day)    
    
    
    
    res.water_stress.append(plant.water_stress) if plant else res.water_stress.append(0)
    res.potential_depth.append(plant.root.potential_depth) if plant else res.potential_depth.append(0)
    res.rooting_depth.append(plant.root.depth) if plant else res.rooting_depth.append(0)
    
    res.water_uptake.append(plant.Wateruptake) if plant else res.water_uptake.append(zeros(c.cell.layer_count()))
    res.branching.append(plant.root.branching) if plant else res.branching.append(zeros(c.cell.layer_count()))
    res.transpiration.append(plant.et.transpiration) if plant else res.transpiration.append(0)
    res.evaporation.append(plant.et.evaporation) if plant else  res.evaporation.append(0)
    res.biomass.append(plant.biomass.Total) if plant else res.biomass.append(0)
    res.root_biomass.append(plant.root.Wtot) if plant else res.root_biomass.append(0)
    res.shoot_biomass.append(plant.shoot.Wtot) if plant else res.shoot_biomass.append(0)
    res.lai.append(plant.shoot.leaf.LAI) if plant else res.lai.append(0)
    res.root_growth.append(plant.root.actual_distribution) if plant else  res.root_growth.append(zeros(c.cell.layer_count()))
    res.ETo.append(plant.et.Reference) if plant else res.ETo.append(0)
    res.ETc.append(plant.et.Cropspecific) if plant else res.ETc.append(0)
    res.wetness.append(c.wetness) 
    res.rain.append(c.cell.get_rainfall(t))
    res.DAS.append(t-datetime(1980,3,1)) if plant else res.DAS.append(0)
    res.temperature.append(cmf_fp.get_tmean(t))
    res.radiation.append(cmf_fp.get_Rs(t))
    res.stress.append((plant.water_stress, plant.nutrition_stress) if plant else (0,0))

    res.matrix_potential.append(c.matrix_potential)
    res.activeNO3.append(plant.nitrogen.Active)if plant else res.activeNO3.append(zeros(c.cell.layer_count()))
    res.passiveNO3.append(plant.nitrogen.Passive)if plant else res.passiveNO3.append(zeros(c.cell.layer_count()))
    res.leaf.append(plant.shoot.leaf.Wtot) if plant else res.leaf.append(0)
    res.stem.append(plant.shoot.stem.Wtot) if plant else res.stem.append(0)
    res.storage.append(plant.shoot.storage_organs.Wtot) if plant else res.storage.append(0)
    res.Rp.append(plant.Rp if plant else 0.)
    return plant

class Res(object):
    def __init__(self):
        self.water_uptake = []
        self.branching = []
        self.transpiration = []
        self.evaporation = []
        self.biomass = []
        self.root_biomass = []
        self.shoot_biomass = []
        self.lai = []
        self.root_growth = []
        self.ETo = []
        self.ETc = []
        self.matrix_potential = []
        self.wetness = []
        self.rain = []
        self.temperature = []
        self.radiation = []
        self.DAS = []
        self.stress = []
        self.activeNO3=[]
        self.passiveNO3=[]
        self.leaf=[]
        self.stem=[]
        self.storage=[]
        self.Rp=[]
        self.water_stress=[]
        self.potential_depth=[]
        self.rooting_depth=[]
        
    def __repr__(self):
        return "Shoot=%gg, Root=%gg, ETc = %gmm, Wateruptake=%gmm, Stress=%s" % (self.shoot_biomass[-1],self.root_biomass[-1],self.ETc[-1],sum(self.water_uptake[-1]),self.stress[-1])

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
    psyco.full()
    
    #Create cmf cell    
    c=cmf1d(sand=20,silt=60,clay=20,c_org=2.0,bedrock_K=0.01,layercount=20,layerthickness=0.1)
    print "cmf is setup"
    c.load_meteo(rain_factor=1)
    print "meteo loaded"
    cmf_fp = cmf_fp_interface(c.cell)
    cmf_fp.default_Nconc = .3
    #cmf_fp.default_Nconc = .1
    
    
    print "Interface to PMF"
    c.cell.saturated_depth=5.
    #Create evapotranspiration instance or bare soil conditions
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
    start_time = datetime.now()
    c.t = start
    while c.t<end:
        plant=run(c.t,res,plant)
        print c.t,res
    print 'Duration:',datetime.now()-start_time
    
#######################################
#######################################
### Show results
    timeline=drange(start,end,timedelta(1))
    subplot(311)
    imshow(transpose([r[:20] for r in res.wetness]),cmap=cm.Blues,aspect='auto',interpolation='nearest',extent=[0,len(timeline),100,0])
    ylabel('Depth [cm]')
    xlabel('Day of year')
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