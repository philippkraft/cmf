# coding=utf-8


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
""" This module provides a soil class for the retrieval of relevant data for hydrologic modeling
The basic concept is a soilType->Layer relationship. Most of the data is stored in the layer class
If you'd like to use mean values for the whole soil profile, depth weighted means for any layer method are available
You can calculate the field capacity, wilting point and porosity from texture and bulk density using the KA4 (AG Boden)
"""
from math import *
class Pedotransfer:

    def __init__(self) :
        #self.data contains the porosity variables for each texture type
        # Usage:
        # porosity,fieldcapacity,wiltpoint = self.data[textureID,bulkDensityID]
        # where textureID=0..27 (defined below) and bulkDensityID=0..4
        # use getTextureID(self,clay,silt,sand) and getBulkDensID(bulkdens) to get the indices
        self.__data = [
                        ((0.41,0.185,0.065),(0.41,0.185,0.065),(0.36,0.165,0.06),(0.345,0.15,0.06),(0.345,0.15,0.06)),
                        ((0.41,0.24,0.065),(0.41,0.24,0.065),(0.35,0.215,0.055),(0.295,0.185,0.055),(0.295,0.185,0.055)),
                        ((0.425,0.26,0.07),(0.425,0.26,0.07),(0.365,0.24,0.065),(0.295,0.205,0.045),(0.295,0.205,0.045)),
                        ((0.43,0.305,0.09),(0.43,0.305,0.09),(0.37,0.26,0.08),(0.305,0.235,0.08),(0.305,0.235,0.08)),
                        ((0.445,0.26,0.075),(0.445,0.26,0.075),(0.385,0.23,0.09),(0.325,0.215,0.08),(0.325,0.215,0.08)),
                        ((0.435,0.32,0.09),(0.435,0.32,0.09),(0.37,0.28,0.075),(0.31,0.255,0.075),(0.31,0.255,0.075)),
                        ((0.445,0.35,0.085),(0.445,0.35,0.085),(0.365,0.29,0.07),(0.315,0.27,0.07),(0.315,0.27,0.07)),
                        ((0.46,0.375,0.1),(0.46,0.375,0.1),(0.38,0.315,0.105),(0.34,0.285,0.115),(0.34,0.285,0.115)),
                        ((0.45,0.33,0.125),(0.45,0.33,0.125),(0.385,0.285,0.115),(0.31,0.255,0.11),(0.31,0.255,0.11)),
                        ((0.44,0.35,0.18),(0.44,0.35,0.18),(0.385,0.295,0.14),(0.33,0.26,0.14),(0.33,0.26,0.14)),
                        ((0.5,0.405,0.195),(0.5,0.405,0.195),(0.405,0.335,0.18),(0.36,0.3,0.165),(0.36,0.3,0.165)),
                        ((0.51,0.42,0.22),(0.51,0.42,0.22),(0.4,0.335,0.185),(0.335,0.29,0.16),(0.335,0.29,0.16)),
                        ((0.505,0.4,0.19),(0.505,0.4,0.19),(0.395,0.315,0.155),(0.325,0.28,0.155),(0.325,0.28,0.155)),
                        ((0.555,0.475,0.3),(0.555,0.475,0.3),(0.44,0.39,0.24),(0.37,0.335,0.225),(0.37,0.335,0.225)),
                        ((0.535,0.485,0.31),(0.535,0.485,0.31),(0.465,0.42,0.285),(0.405,0.37,0.265),(0.405,0.37,0.265)),
                        ((0.47,0.4,0.23),(0.47,0.4,0.23),(0.405,0.35,0.21),(0.36,0.32,0.21),(0.36,0.32,0.21)),
                        ((0.51,0.45,0.26),(0.51,0.45,0.26),(0.415,0.37,0.23),(0.37,0.34,0.23),(0.37,0.34,0.23)),
                        ((0.46,0.395,0.11),(0.46,0.395,0.11),(0.395,0.365,0.105),(0.36,0.33,0.09),(0.36,0.33,0.09)),
                        ((0.47,0.39,0.12),(0.47,0.39,0.12),(0.38,0.33,0.075),(0.33,0.28,0.06),(0.33,0.28,0.06)),
                        ((0.48,0.39,0.12),(0.48,0.39,0.12),(0.4,0.36,0.11),(0.34,0.32,0.11),(0.34,0.32,0.11)),
                        ((0.31,0.215,0.0),(0.31,0.215,0.0),(0.405,0.36,0.125),(0.35,0.33,0.115),(0.35,0.33,0.115)),
                        ((0.48,0.395,0.135),(0.48,0.395,0.135),(0.395,0.33,0.11),(0.34,0.305,0.11),(0.34,0.305,0.11)),
                        ((0.48,0.39,0.17),(0.48,0.39,0.17),(0.415,0.36,0.155),(0.36,0.335,0.155),(0.36,0.335,0.155)),
                        ((0.51,0.435,0.225),(0.51,0.435,0.225),(0.425,0.365,0.195),(0.375,0.335,0.19),(0.375,0.335,0.19)),
                        ((0.575,0.505,0.33),(0.575,0.505,0.33),(0.47,0.435,0.295),(0.4,0.375,0.275),(0.4,0.375,0.275)),
                        ((0.58,0.52,0.31),(0.58,0.52,0.31),(0.46,0.415,0.26),(0.395,0.36,0.245),(0.395,0.36,0.245)),
                        ((0.525,0.455,0.26),(0.525,0.455,0.26),(0.425,0.37,0.21),(0.365,0.325,0.205),(0.365,0.325,0.205)),
                        ((0.6,0.56,0.36),(0.6,0.56,0.36),(0.53,0.5,0.36),(0.45,0.43,0.32),(0.45,0.43,0.32)),
                        ((0.615,0.565,0.38),(0.615,0.565,0.38),(0.54,0.505,0.36),(0.455,0.435,0.325),(0.455,0.435,0.325)),
                        ((0.62,0.58,0.365),(0.62,0.58,0.365),(0.52,0.49,0.345),(0.445,0.425,0.315),(0.445,0.425,0.315)),
                        ((0.625,0.6,0.43),(0.625,0.6,0.43),(0.585,0.56,0.41),(0.49,0.475,0.365),(0.49,0.475,0.365))
                    ]

        #self.texture contains the boundaries for the texture classes
        # Usage:
        # Lower boundary:
        #   clay,silt,sand = self.texture[textureID][0]
        # Upper boundary:
        #   clay,silt,sand = self.texture[textureID][0]
        self.__texture = [
                        ((0,0,85),(5,10,100)),((0,10,70),(5,25,90)),((5,10,67),(8,25,85)),((8,10,48),(12,40,82)),
                        ((5,0,73),(17,10,95)),((0,25,52),(8,40,75)),((0,40,42),(8,50,60)),((8,40,33),(17,50,52)),
                        ((12,10,43),(17,40,78)),((17,0,60),(25,15,83)),((17,40,25),(25,50,43)),((17,30,35),(25,40,53)),
                        ((17,15,45),(25,30,68)),((25,30,15),(35,50,45)),((25,15,25),(45,30,60)),((25,0,50),(35,15,75)),
                        ((35,0,40),(45,15,65)),((0,80,0),(8,100,20)),((0,50,12),(8,80,50)),((8,65,0),(12,92,27)),
                        ((12,65,0),(17,88,23)),((8,50,18),(17,65,42)),((17,65,0),(25,83,18)),((17,50,5),(30,65,33)),
                        ((35,30,5),(45,50,35)),((30,50,0),(45,65,20)),((25,65,0),(35,75,10)),((45,0,20),(65,15,55)),
                        ((45,15,5),(65,30,40)),((45,30,0),(65,55,25)),((65,0,0),(100,35,35))
                        ]



        self.__textNames = ("Ss","Su2","Sl2","Sl3","St2","Su3","Su4","Slu","Sl4","St3","Ls2","Ls3","Ls4","Lt2","Lts","Ts4","Ts3","Uu","Us","Ut2","Ut3","Uls","Ut4","Lu","Lt3","Tu3","Tu4","Ts2","Tl","Tu2","Tt")
        self.__Ksat = [
                        (335.,335.,230.,200.,200.),
                        (155.,155.,126.,64.,64.),
                        (126.,126.,66.,43.,43.),
                        (51.,51.,42.,17.,17.),
                        (106.,106.,65.,49.,49.),
                        (92.,92.,51.,19.,19.),
                        (39.,39.,34.,17.,17.),
                        (17.,17.,15.,8.,8.),
                        (46.,46.,21.,12.,12.),
                        (106.,106.,16.,12.,12.),
                        (22.,22.,19.,11.,11.),
                        (11.,11.,7.,6.,6.),
                        (13.,13.,12.,6.,6.),
                        (9.,9.,7.,4.,4.),
                        (8.,8.,5.,5.,5.),
                        (4.,4.,2.,1.,1.),
                        (4.,4.,2.,1.,1.),
                        (17.,17.,10.,6.,6.),
                        (27.,27.,13.,6.,6.),
                        (44.,44.,9.,3.,3.),
                        (45.,45.,8.,3.,3.),
                        (49.,49.,17.,6.,6.),
                        (32.,32.,8.,5.,5.),
                        (25.,25.,12.,5.,5.),
                        (9.,9.,9.,3.,3.),
                        (15.,15.,10.,2.,2.),
                        (35.,35.,16.,1.,1.),
                        (4.,4.,2.,1.,1.),
                        (2.,2.,1.,1.,1.),
                        (5.,5.,4.,1.,1.),
                        (4.,4.,2.,1.,1.)
                        ]


    def __inbetween(self,texture,mintext,maxtext) :
        for i in range(3):
            if texture[i]<mintext[i] or texture[i]>maxtext[i] :
                return False
        return True

    def __getTextureID(self,texture) :
        for id,bounds in enumerate(self.__texture) :
            if (self.__inbetween(texture,bounds[0],bounds[1])):
                return id
        return -1
    def __getBulkDensID(self,bulkdens,clay) :
        ld=bulkdens+0.009 * clay
        if ld<1.4 :
            return 0
        elif ld<1.6 :
            return 1
        elif ld<1.8 :
            return 2
        elif ld<2.0 :
            return 3
        else :
            return 4


    def GetPorosity(self,clay,silt,sand,bulkdensity=1440) :
        """Returns (porosity, field capacity, wilting point,KSat, name)
        clay,silt,sand : Soil content in mass % (will be normalized to clay+silt+sand=100)
        bulkdensity    : Bulkdensity in kg/m3
        KSat           : Saturated conductivity in m/day
        name           : String representing the german texture class
        """
        sum=clay+silt+sand
        t=(clay*100.0/sum,silt*100.0/sum,sand*100.0/sum)
        bdID=self.__getBulkDensID(bulkdensity*0.001,t[0])
        tID=self.__getTextureID(t)
        return self.__data[tID][bdID][0],self.__data[tID][bdID][1],self.__data[tID][bdID][2],self.__Ksat[tID][bdID]*0.01,self.__textNames[tID]

    def GetTextureName(self,clay,silt,sand):
        sum=clay+silt+sand
        if sum<=0: return 'Invalid data'
        t=(clay*100.0/sum,silt*100.0/sum,sand*100.0/sum)
        return self.__textNames[self.__getTextureID(t)]

def rounded_linear(x,s,ymax,r):
    K=ymax*r/s
    y0=ymax-K*s
    x0=y0/s
    return ((x>x0) * (y0+(x-x0)*K*s/(K+(x-x0)))) + ((x<=x0) * x * s)    


class layer :
    """Class to hold a layer

    Public properties:
    Texture: (clay + silt + sand = 100) + rock
        clay     : Clay content in mass % (default=10)
        silt     : Silt content in mass % (default=30)
        sand     : Sand content in mass % (default=60)
        rock     : Rock content in mass % (default=0)
    Other soil physics parameters
        bulkdens : Bulk density in kg/m3  (default=1440)
        ksat     : Saturated water conductivity in m/day (default=100)
        thickness: Thickness of soil layer in mm
        CECpot   : Potential cation exchnage capacity in molc/kg
    """

    def vanGenuchtenParameter(self) :
        """Returns the parameters of the van Genuchten model of water content / matrix potential model
            (theta_r,theta_s,alpha,n)
        clay,silt,sand : Soil content in mass % (will be normalized to clay+silt+sand=100)
        Corg           : Soil content of organic carbon in mass %
        bulkdensity    : Bulkdensity in kg/m3
        Source: http://www.bgr.de/saf_boden/AdhocAG/Ergaenzungsregel_1_18.pdf
        """
        tr=0.015+0.005*self.clay+0.014*self.Corg
        ts=0.81-0.283e-3*self.bulkdens+0.001*self.clay
        a=10**(-2.486+0.025*self.sand-0.351*self.Corg-2.617e-3*self.bulkdens-0.023*self.clay)
        n=10**(0.053-0.009*self.sand-0.013*self.clay+0.00015*self.sand**2)
        return (tr,ts,a,n)
    def __init__(self,thickness=0.1,clay=10.0,silt=30.0,sand=60.0,bulkdensity=1440.0,Corg=1.5,rock=0.0):
        sumt=clay+silt+sand
        self.thickness=thickness
        if sumt>0 :
            self.__porosity=Pedotransfer()
            self.clay=clay*100.0/sumt
            self.silt=silt*100.0/sumt
            self.sand=sand*100.0/sumt
            self.Corg=Corg
            self.depth=thickness
            if bulkdensity<20 : # if bulkdensity is < 20 then assume bulk density in kg/l, instead of kg/m3
                self.bulkdens=bulkdensity*1000.0
            else:
                self.bulkdens=bulkdensity
            self.rock=rock
            pResult=self.__porosity.GetPorosity(self.clay,self.silt,self.sand,self.bulkdens)
            self.porosity,self.fieldcap,self.wiltpoint,self.KSat,self.texture=pResult
            self.CECpot=(0.5*self.clay + 0.05 * self.silt + self.Corg)*0.01
            
            # Calculate shape parameter of pF curve
            pF_fieldcap=2.5
            pF_WiltPoint=4.2
            self.b=(pF_fieldcap-pF_WiltPoint)/(log10(self.wiltpoint/self.fieldcap))
        else :
            self.clay=0
            self.sand=0
            self.silt=0
            self.Corg=0
            self.rock=100
            self.KSat=0
            self.b=1
            self.fieldcap=0
            self.porosity=0.000001
            self.wiltpoint=0
            self.texture="Solid"
            self.bulkdens=2750.0
    def Wetness(self,theta):
        return theta/self.porosity
    def K(self,theta):
        return self.KSat*(theta/self.porosity)**(2+3*self.b)
         
    def MatrixPotential(self,theta):
        psi_f=-100*10**1.8
        W_f=self.Wetness(self.fieldcap)
        W=self.Wetness(theta)
        b=self.b
        W_i=0.9+0.005*b
        if (W<W_i):
            return psi_f*(W/W_f)**(-b)
        else:
            psi_i=psi_f*((W_i/W_f)**(-b))
            m1=-psi_i/((1-W_i)**2)
            m2=b*(-psi_i)/(W_i*(1-W_i))
            m=m1-m2
            n=2*W_i-1+b*psi_i/(m*W_i)
            if W<=1:
                return m*(W-n)*(W-1)
            else:
                slope=m*(1-n)
                return (exp(10*(W-1))*slope-slope)/10

class soil:
    def __init__(self,name) :
        self.name=name
        self.layers=[]
    def __evalute(self,function,layer) :
        if '$layer$' in function :
            return eval(function.replace('$layer$','layer'))
        else :
            return eval('layer.' + function)

    def Average(self,layerProperty='porosity',depth=1000) :
        """Returns the depth weigted average of a soil property
        layerProperty  : a string representing one of the properties of the layer class, like
                        clay, silt, sand, KSat, Corg,rock,fieldcap,porosity,wiltpoint,bulkdens
                        If you need a calculation to be performed, you can use $layer$ as a proxy for the layer
                        e.g. you need the average quotient of fieldcap/porosity, then layerProperty='$layer$.fieldcap/$layer$.porosity'
        depth          : Maximum depth for the calculation in m (depth>soil.depth will be ignored)
        """
        totDepth=0.
        sumVal=0.
        for l in self.layers :
            if totDepth<depth :
                if totDepth+l.thickness < depth:
                    d=l.thickness
                else :
                    d=depth-totDepth
                totDepth+=d
                sumVal+= d * self.__evalute(layerProperty,l)
        return sumVal/totDepth



    def depth(self) :
        """Returns the depth of the soil in m"""
        depth=0.0
        for l in self.layers:
            depth+=l.thickness
        return depth

def BrooksCoreyParams(clay,silt,sand,bulkdensity=1440.0,Corg=1.5):
    l=layer(0.1,clay,silt,sand,bulkdensity,Corg)
    return l.KSat, l.porosity,l.b,l.fieldcap, -0.65
