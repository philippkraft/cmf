from __future__ import division
from numpy import exp,cos,pi
'''
Created on 17.09.2009

@author: philkraf
'''

class SOMcomponent(object):
    '''
    classdocs
    '''


    def __init__(self,Id,Name,k_pot,E_a,K_w,n_w,K_pH,m_pH=1):
        '''
        Constructor
        '''
        self.Id=Id
        self.Name=Name
        self.k_pot=k_pot
        self.E_a=E_a
        self.K_w=K_w
        self.n_w=n_w
        self.K_pH=K_pH
        self.m_pH=m_pH
        self.product_fractions={}
    def __hash__(self):
        return hash((self.Id,self.Name))
    def __repr__(self):
        return self.Name
    def temp_factor(self,T):
        R=8.314*0.001
        T_R=5.0
        arr_gamma=self.E_a/(R*(T_R+273.16))-self.E_a/(R*(T+273.16))
        return exp(arr_gamma)
    def water_factor(self,theta):
        return self.K_w*theta**self.n_w/(1+self.K_w*theta**self.n_w)
    def pH_factor(self,pH):
        H_conc=10**(-pH)
        return 1.0/(1.0+self.K_pH * H_conc ** self.m_pH)
    def decomp_rate(self,T,theta,pH):
        f_T=self.temp_factor(T)
        f_w=self.water_factor(theta)
        f_pH=self.pH_factor(pH)
        return self.k_pot * f_T * f_w * f_pH / 365
         
        
EDC=SOMcomponent(0,"EDC",240,18,9.4,3.4,65600)
CELL=SOMcomponent(1,"Cellulose",11,33,9.4,3.4,20500.)
LIGN=SOMcomponent(2,"Lignin",1.7,50,9.4,3.4,1050.)
RC=SOMcomponent(3,"Recalc.",0.025,53,9.4,3.4,1050)
DOC=SOMcomponent(4,"DOC",0.025,50,110,2.454,20500)
CO2=SOMcomponent(5,"CO_2",0,0,0,0,0)
# alpha Sources of CO2
EDC.product_fractions[CO2]=0.45
CELL.product_fractions[CO2]=0.45
LIGN.product_fractions[CO2]=0.4
RC.product_fractions[CO2]=0.5
DOC.product_fractions[CO2]=0.75
# beta Sources of DOC
EDC.product_fractions[DOC]=0.45
CELL.product_fractions[DOC]=0.45
LIGN.product_fractions[DOC]=0.4
RC.product_fractions[DOC]=0.5
#gamma No sources of CELL
#delta Sources of LIGN
LIGN.product_fractions[LIGN]=0.1
#epsilon Sources of RC
EDC.product_fractions[RC]=0.1
CELL.product_fractions[RC]=0.1
LIGN.product_fractions[RC]=0.1
RC.product_fractions[RC]=0.1
DOC.product_fractions[RC]=0.25
SOMcomponents = [EDC,CELL,LIGN,RC,DOC]

import numpy
class SOM(object):
    def __init__(self,N=0.0,C=[0.0]*5):
        if len(C)!=5:
            raise ValueError("Carbon pool must be a sequence of lenght 5!")
        self.C=numpy.array(C,dtype=float)
        self.N=N
        self.__internal_inputs=numpy.zeros(5,dtype=float)
        self.__decomp=numpy.zeros(5,dtype=float)
        self.CNmin=15.0
        self.CNmax=40.0
    @property
    def Cpool(self):
        return sum(self.C[:-1])
    @property
    def CNratio(self):
        return self.Cpool/self.N
    @property
    def DOC(self):
        return self.C[-1]
    @DOC.setter
    def DOC(self,value):
        self.C[-1]=value
    def __repr__(self):
        return 'SOM(C/N=%5.2f,EDC=%0.6g,CELL=%0.6g,LIGN=%0.6g,RC=%0.6g,DOC=%0.6g)' % ((self.CNratio,) + tuple(self.C))
    
    def __getitem__(self,index):
        if isinstance(index, SOMcomponent):
            return self.C[index.Id]
        else:
            return self.C[index]
    def __mul__(self,factor):
        return SOM(self.N*factor,self.C*factor)
    def __rmul__(self,factor):
        return self * factor
    def __add__(self,som):
        assert(isinstance(som, SOM))
        return SOM(self.N + som.N, self.C + som.C)
    def __iadd__(self,som):
        assert(isinstance(som, SOM))
        self.N += som.N
        self.C += som.C
    def __imul__(self,factor):
        self.N *= factor
        self.C *= factor
    def copy(self):
        return SOM(self.N,self.C)
        
    def dCdt(self,litter_in, T,theta,pH):           
        self.__internal_inputs=numpy.zeros(shape=6,dtype=float)
        for comp in SOMcomponents:
            self.__decomp[comp.Id]=max(self.C[comp.Id],0) * comp.decomp_rate(T,theta,pH)
            for prod,frac in comp.product_fractions.items():
                self.__internal_inputs[prod.Id] += self.__decomp[comp.Id] * frac
        return litter_in.C + self.__internal_inputs[:-1] - self.__decomp
    def dNdt(self,net_min,N_in):
        C_pool=self.Cpool
        if C_pool<=0.0 or self.N<=0:
            return N_in
        else:
            gross_Nmin = net_min/self.Cpool * self.N
            Nimmob = (self.CNratio - self.CNmin)/(self.CNmax - self.CNmin) * gross_Nmin
            return Nimmob - gross_Nmin + N_in
        
         
    abs_tol=1e-6
    rel_tol=1e-6    
    def integrate(self,dt,litter_in,T,theta,pH,implicit=1):
        last_step=self.copy()
        self.iterations=0
        C_in=litter_in.Cpool
        C_last=last_step.Cpool
        if implicit:
            while self.iterations<100:
                compare=self.copy()
                dCdt=self.dCdt(litter_in, T, theta, pH)
                dNdt=self.dNdt(C_in - sum(dCdt[:-1]), litter_in.N)
                self.C = last_step.C + dCdt * dt
                self.N = last_step.N + dNdt * dt
                self.iterations+=1
                abs_error=max(abs(self.C-compare.C))
                rel_error=max(abs(self.C-compare.C)/(self.C+compare.C))
                if  abs_error< self.abs_tol or rel_error < self.rel_tol:
                    break
        else:
            dCdt=self.dCdt(litter_in, T, theta, pH)
            dNdt=self.dNdt(C_in - sum(dCdt[:-1]), litter_in.N)
            self.C = last_step.C + dCdt * dt
            self.N = last_step.N + dNdt * dt
            
            
foliar_litter=SOM(N=1/20,C=(0.1,.5,.32,.08,.0))
root_litter=SOM(N=1/20,C=(.21,.4,.33,.05,.0))
wood_litter=SOM(N=1/50,C=(.04,.6,.27,.09,.0))
EDC_som=SOM(N=1/20,C=(1.0,0,0,0,0.0))
 
if __name__=='__main__':
    import pylab
    layer = SOM()
    layer.rel_tol=1e-3
    T = lambda t: 8 - 9 * cos( t * 2 * pi)
    pH = 8.0
    theta = 1.0
    res_C=[]
    res_N=[]
    min_rate=[]
    dt=1
    for i in range(20000):
        t = i/dt / 365
        litter_in = 5 * root_litter + 10 * wood_litter
        if (i//dt) % 365 == 9 * 30:
            litter_in = litter_in + (50 * 365 * foliar_litter)
        layer.DOC=0.0
        Cpool=sum(litter_in.C)+layer.Cpool
        layer.integrate(dt, litter_in, T(t), theta, pH,1)
        if i% 365 == 0: print 't=%i, iter=%i  %s' % (i , layer.iterations, layer)
        res_C.append(layer.C.copy())
        res_N.append(layer.CNratio)
        min_rate.append(((-(layer.Cpool - Cpool)*2/(layer.Cpool + Cpool)), sum(layer.dCdt(SOM(), T(t), theta, pH))))
    res_C=numpy.array(res_C)
    pylab.subplot(211)
    pylab.plot(numpy.linspace(0,(i+1)*dt,len(res_C)),res_C)
    #pylab.semilogy(res_C)
    pylab.legend(SOMcomponents) 
    pylab.subplot(212)
    #min_rate=numpy.array(min_rate)
    #pylab.plot(numpy.linspace(0,(i+1)/365,len(res_C)),min_rate)
    pylab.plot(numpy.linspace(0,(i+1)*dt,len(res_N)),res_N)
    
    pylab.show()
    
