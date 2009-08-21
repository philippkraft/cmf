from __future__ import division
import soil
import math
import pylab
def rounded_linear(x,s,ymax,r):
    K=ymax*r/s
    y0=ymax-K*s
    x0=y0/s
    return ((x>x0) * (y0+(x-x0)*K*s/(K+(x-x0)))) + ((x<=x0) * x * s)    
class VarLayers:
    @property
    def soildepth(self):
        return self.soil.thickness
    def __sat_thick(self,sat_state):
        return rounded_linear(sat_state/self.soil.porosity, 1, self.soildepth-0.1, 0.01)
    @property
    def thickness(self):
        satthick=self.__sat_thick(self.states[1])
        return (self.soildepth-satthick,satthick)
    @property
    def theta(self):
        return (self.states[0]/self.thickness[0] if self.states[0]>0 else self.soil.porosity, 
                self.states[1]/self.thickness[1] if self.states[1]>0 else self.soil.porosity)  
    @property
    def rel_fill(self):
        return self.states[0]/self.thickness[0]/self.soil.porosity,self.thickness[1]/self.soildepth
    @property
    def potential(self):
        theta_u,theta_s=self.theta
        return (self.soil.MatrixPotential(theta_u)*1e-4,
                #self.states[1]/self.soil.porosity - self.soildepth) 
                self.soil.MatrixPotential(theta_s)*1e-4 - self.thickness[0]) 
    @property
    def q_perc(self):
        pot_u,pot_s=self.potential
        theta_u,theta_s=self.theta   
        gradient=(pot_u-pot_s)/(self.thickness[0]*0.5)
        K=math.sqrt(self.soil.K(theta_u) * self.soil.KSat)
        if K*gradient>self.soil.KSat: return self.soil.KSat
        if K*gradient<-self.soil.KSat: return -self.soil.KSat
        return K*gradient
    @property
    def Exw(self):
        f_Exw=1/(1-min(self.theta[0]/self.soil.porosity,0.9))-1
        waterrise=(self.q_perc+self.q_lat) * 1e6 * (self.__sat_thick(self.states[1])-self.__sat_thick(self.states[1]-1e-6))
        return waterrise*self.soil.porosity * f_Exw
    @property
    def Infiltration(self):
        pot=min(self.potential[0],0.05)
        return min(-self.soil.KSat * pot/(0.05),self.rain) 
    @property
    def water_balance(self):
        q_perc, Exw=self.q_perc, self.Exw
        inf=self.Infiltration
        return inf - q_perc - Exw , q_perc + Exw + self.q_lat 
    
    def get_error(self,compare_states):
        return [abs(self.states[i]-compare_states[i])+1e-20 for i in [0,1]]
    def iterate(self,t,dt,iterations=50,abstol=1e-12):
        self.old_states=list(self.states)
        error=[]
        flux=[]
        states=[]
        iter=0
        for i in range(iterations):
            compare_states=list(self.states)
            wb=self.water_balance
            self.states=list(self.old_states)
            self.states[0]+=wb[0]*dt
            self.states[1]+=wb[1]*dt

            states.append(self.potential)
            flux.append((self.Infiltration,self.q_perc,self.Exw))
            error.append(self.get_error(compare_states))
            if max(error[-1])>1e-12: 
                iter+=1
            else:
                break 
        #if max(error[-1])>1e-12: print "log(Error), %i it: %5.2f at t=%0.3f, sat_d=%0.4f" % (iterations,math.log10(max(error[-1])),t,self.thickness[0])
        t+=dt 
        return t,states,flux,error,iter
    def reset(self):
        self.states=list(self.old_states)
    def __init__(self,layer):
        self.soil=layer
        self.states=[0.35*layer.porosity*layer.thickness,0.5*layer.porosity*layer.thickness]
        self.q_lat=0.0
        self.rain=0.0
        self.f_Exw=1.25


def run_model(sd,theta_u,layer):
    varlayers=VarLayers(layer)
    varlayers.states[1]=sd*layer.porosity
    varlayers.states[0]=varlayers.thickness[0]*theta_u
    #print varlayers.potential
    #varlayers.f_Exw=f_Exw
    varlayers.q_lat=0.1
    #varlayers.rain=0.1
    h=1./24.
    t=0
    pot=[]
    fluxts=[]
    iterations=[]
    state=[]
    abstol=1e-9
    while t<3.5:
        t,states,flux,error,iter=varlayers.iterate(t,h,100,abstol)
        pot.append(states[-1])
        fluxts.append(flux[-1])
        state.append(varlayers.states)
        iterations.append(iter)
    return t,pot,fluxts,iterations,state


def pot_u_diff(f_Exw,sd,theta_u,layer):
    t,pot,fluxts,iterations=run_model(sd, theta_u, layer)
    return pot[-1][0]-pot[0][0]
def find_fExw(sd,theta_u,layer):
    fExw=[0.0,5.0]
    pot=[pot_u_diff(f_Exw,sd,theta_u,layer) for f_Exw in fExw]
    i=0
    while abs(pot[-1])>1e-12 and i<1000:
        fExw.append(fExw[-1] - (fExw[-1]-fExw[-2])/(pot[-1]-pot[-2])*pot[-1])
        pot.append(pot_u_diff(fExw[-1],sd,theta_u,layer))
        i+=1
    return fExw,pot
if __name__=='__main__':
    layer=soil.layer(1.0, 20, 20, 80)
    layer.KSat*1
    sd=.0
    w=0.5
    theta_u=w*layer.porosity
    varlayers=VarLayers(layer)
    varlayers.states[1]=sd*layer.porosity
    varlayers.states[0]=varlayers.thickness[0]*theta_u
    varlayers.q_lat=0.1
    varlayers.rain=0.0
    h=1./24.
    t=0
    ts=[]
    pot=[]
    fluxts=[]
    iterations=[]
    state=[]
    theta=[]
    i=0
    abstol=1e-9
    step=0.001
    failed=False
    maxiter=50
    while t<3.5:
        t,states,flux,error,iter=varlayers.iterate(t,0.0005,maxiter,abstol)
        pot.append(states[-1])
        fluxts.append(flux[-1])
        state.append(varlayers.states)
        iterations.append(iter)
        theta.append(varlayers.theta)
        ts.append(t)
        i+=1
        failed=failed or iter==maxiter
        if i % int(1/step)==0 : 
            print "." if not failed else '!',
            failed=False 
    pot=pylab.array(pot)
    f=pylab.array(fluxts)
#    pylab.plot(pot[:,0]-pot[:,1],f[:,1])
    pylab.subplot(411)
    pylab.plot(ts,pot,'-')
    pylab.plot(ts,pylab.array(theta)/layer.porosity)
    pylab.legend(r'$\Psi_{u}$ $\Psi_s$ $\Theta_u$ $\Theta_s$'.split(),loc=0)
    pylab.grid()
    pylab.ylim(-5,5)
    pylab.subplot(412)
    pylab.plot(ts,state)
    pylab.legend(r'$V_{u}$ $V_s$'.split(),loc=0)
    pylab.grid()    
    pylab.subplot(413)
    pylab.plot(ts,fluxts)
    pylab.legend(r'$q_{in}$ $q_{perc}$ $q_{Ex_w}$'.split(),loc=0)
    pylab.grid()
    pylab.subplot(414)
    pylab.plot(ts,iterations)
    pylab.legend(r'#iterations'.split(),loc=0)
    pylab.grid()
    pylab.show()