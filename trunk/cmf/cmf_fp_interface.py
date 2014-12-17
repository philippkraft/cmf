# -*- coding: utf-8 -*-
"""
Created on Wed Sep 30 15:40:40 2009

@author: philkraf
"""
import cmf
class cmf_fp_interface:
    """This class implements the Atmosphere and the soil interface of the PMF (Multsch et al. 2010) model
    """
    def __init__(self,cmf_cell, cmf_tracer_N=None):
        """Creates an interface for a cmf cell to flower_power
        cmf_cell : A cmf.Cell object containing the cell on which the plant growth
        cmf_tracer_N: (optional) A cmf.Solute object, representing Nitrogen
        """
        assert(isinstance(cmf_cell,cmf.Cell))
        self.cmf_cell=cmf_cell
        self.N = cmf_tracer_N
        self.default_Nconc=100
        self.bulk_density=1.5
    def get_pressurehead(self,depth):
        """ Depth in cm; Returns the capillary suction for a given depth in [cm]."""
        return -100 * self.get_layer(depth).matrix_potential
    @property
    def matrix_potential(self):
        """Returns the capillary suction in m for each layer (including the bedrock layer)"""
        return [l.matrix_potential for l in self.cmf_cell.layers]
    def get_nitrogen(self,depth):
        """ Depth in cm; Returns the nitrogen concentration in the soil solution in [g l-1]"""
        l=self.get_layer(depth)
        if isinstance(self.N,cmf.solute):
            return max(l.conc(self.N) * 1e-3,0.0)
        else:
            return self.default_Nconc
    def soilprofile(self):
        """ Returns a list with the lower limits of the layers in the whole soilprofile in [cm]. """
        return [l.lower_boundary*100 for l in self.cmf_cell.layers]
    def get_fc(self,depth):
        """ soil water content at field capacity [m3 m-3] """
        return self.cmf_cell.layers[0].soil.Wetness_pF(2.5)
    def get_wp(self,depth):
        """ soil water content at wilting point [m3 m-3] """
        return self.cmf_cell.layers[0].soil.Wetness_pF(4.2)
    def get_wetness(self,depth):
        """ wetness in the top soil layer in [m3 m-3] """
        return self.get_layer(depth).wetness
    def get_layer(self,depth):
        if depth<0:
            return self.cmf_cell.layers[0]
        for l in self.cmf_cell.layers:
            if (l.upper_boundary<=depth*0.01 and l.lower_boundary>depth*0.01):
                return l
        return self.cmf_cell.layers[-1]
    def get_tmean(self,time):
        return self.cmf_cell.get_weather(time).T
    def get_tmin(self,time):
        """ Time as datetime instance: datetime(JJJJ,MM,DD); Returns minimal temperature in Celsius """
        return self.cmf_cell.get_weather(time).Tmin
    def get_tmax(self,time):
        """ Time as datetime instance: datetime(JJJJ,MM,DD); Returns maximal temperature in Celsius """
        return self.cmf_cell.get_weather(time).Tmax
    def get_Rs(self,time):
        """ Time as datetime instance: datetime(JJJJ,MM,DD); Returns total solar radiation in [MJ m-2]"""
        return self.cmf_cell.get_weather(time).Rs
    def get_Rn(self,time,albedo,daily=True):
        """ Time as datetime instance: datetime(JJJJ,MM,DD); Returns total solar radiation in [MJ m-2]"""
        return self.cmf_cell.get_weather(time).Rn(albedo,daily)

    def get_ea(self,time):
        """ Time as datetime instance: datetime(JJJJ,MM,DD); Returns actual vapor pressure in [kPa]"""
        return self.cmf_cell.get_weather(time).e_a
    def get_es(self,time):
        """ Time as datetime instance: datetime(JJJJ,MM,DD); Returns saturated vapor pressure in [kPa] """
        return self.cmf_cell.get_weather(time).e_s
    def get_windspeed(self,time):
        """ Time as datetime instance: datetime(JJJJ,MM,DD); Returns windspeed in [m s-1]"""
        return self.cmf_cell.get_weather(time).Windspeed
    def Kr(self):
        cell=self.cmf_cell
        # Get top layer
        layer=cell.layers[0]
        # get field Capacity and wilting point
        fc=layer.soil.Wetness_pF(1.8)
        wp=layer.soil.Wetness_pF(4.2)
        kr = cmf.piecewise_linear(layer.wetness,0.5*wp,0.5*(fc+wp))
        return kr
 