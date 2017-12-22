# -*- coding: utf-8 -*-
"""
This code is for the visualization of different
connection types in cmf.
"""

import cmf
import pylab as plt


class Project: 
    """
    Creates a cmf project and 2 waterstorages and connects them
    with a connection defined by type and paramters
    """
    def __init__(self, con_type, *args, **kwargs):
        self.p = cmf.project()
        self.w1 = self.p.NewStorage('W1')
        self.w2 = self.p.NewStorage('W2')
        self.con = con_type(self.w1, self.w2, *args, **kwargs)

    def plot_Vt(self, label=None):
        """
        Plots an integration of the Volume over 1 week
        starting with a volume of 2 m³ in W1 and an empty W2
        """
        self.w1.volume = 2.0
        self.w2.volume = 0.0
        label = label or str(self.con)
        solver = cmf.CVodeIntegrator(self.p, 1e-9)
        V0 = self.w1.volume
        t, vol = zip(*[(0, V0)] + [(t/cmf.day, self.w1.volume) 
                        for t in solver.run(cmf.Time(), cmf.week, cmf.h)])
        
        plt.plot(t, vol,label=label)
        plt.xlabel(r'$t\ in\ days$')
        plt.ylabel(r'$V\ in\ m^3$')
        plt.yticks([0,1,2],['0','$V_0$','$V(t_0)$'])
        plt.grid(True)

    def plot_VQ(self, label=None):
        """
        Plots the Q(V) relation for the connection
        for V_w1 = [0..2], and V_w2 = 0.5
        """
        label = label or str(self.con)
        def Q(V):
            self.w1.volume = V
            self.w2.volume = 0.5
            return self.w1.flux_to(self.w2, cmf.Time())
        vol = plt.arange(0, 2, 0.01)
        plt.plot(vol, [Q(V) for V in vol],label=label)
        plt.xticks([0,1],['0','$V_0$'])
        plt.yticks([0,0.5],['0','$Q_0$'])
        plt.ylabel(r'$Q\ in\ \frac{m^3}{day}$')
        plt.xlabel(r'$V\ in\ m^3$')
        plt.grid(True)
    
    def plot_all(self, ax, label):
        """
        Plots all plots defined in project
        """
        plt.sca(ax[0])
        p.plot_VQ(label)
        plt.legend(loc=0)
        plt.sca(ax[1])
        p.plot_Vt(label)
        plt.legend(loc=0)

            
        

fig, ax = plt.subplots(1, 2)

# Linear storage
p = Project(cmf.LinearStorageConnection, 2)
p.plot_all(ax, r'$Q(V)=\frac{V}{2 days}$')

# Power law beta=2
p = Project(cmf.PowerLawConnection, Q0=.5, V0=1, beta=2)
p.plot_all(ax, r'$Q(V)=Q_0 \cdot  (V/V_0)^2$')

# Power law beta = 0.5
p = Project(cmf.PowerLawConnection, Q0=.5, V0=1, beta=0.5)
p.plot_all(ax, r'$Q(V)=Q_0 \cdot  (V/V_0)^{1/2}$')

# Constraint kinematic
p = Project(cmf.constraint_kinematic_wave, residencetime=2, Vrmax=1.0)
p.plot_all(ax, r'$Q(V)=Q_0 \cdot \frac{V_{l}}{V_0} \cdot \frac{V_{r,max}-V_{r}}{V_{r,max}}$')

# Exponential decline
p = Project(cmf.ExponentialDeclineConnection, Q0=.5, V0=1, m=1)
p.plot_all(ax, r'$Q(V)=Q_0 \cdot  e^{(V-V_0)/m}$')

plt.show()
