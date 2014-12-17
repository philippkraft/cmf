

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
from __future__ import division

'''
Fits cmf retention curves objects into a measured pF curve

Created on 07.10.2009

@author: philkraf
'''
import cmf
import numpy as np
import scipy.optimize as opt
import random
def get_error_bc(params, pF,theta):
    bc=cmf.BrooksCoreyRetentionCurve(1,params[0],params[1],params[2])
    model_wetness=bc.Wetness_pF(pF)
    err = np.sum((theta - params[0] * model_wetness)**2)
    return err
            

def fit_bc(pF,theta,count=20):
    """Fits a Brooks-Corey retention curve into data
    pF: a sequence of pF values
    theta: an array of water contents
    """
    theta_mean=np.mean(theta)
    ns_denom=np.sum((theta-theta_mean)**2)
    best_f=1e12
    for i in range(count):
        x0=np.array(( random.uniform(0.01,1.0), random.uniform(1.0,20),random.uniform(0.01,1.0)))
        x_opt, f_opt,n_iter,n_eval, warn = opt.fmin(get_error_bc, x0 = x0, args=(pF,theta),full_output=1, disp=0)
        if f_opt<best_f:
            print "%i: x=%s f=%0.12g iter=%i, eval=%i" % (i,x_opt,1-f_opt/ns_denom,n_iter,n_eval) 
            bc=cmf.BrooksCoreyRetentionCurve(1.,x_opt[0],x_opt[1],x_opt[2])
            best_f=f_opt

    return bc, 1-best_f/ns_denom

def get_error_vgm(params,pF,theta):
    vgm=cmf.VanGenuchtenMualem(1.0, *params)
    model_wetness=vgm.Wetness_pF(pF)
    err = np.sum((theta - params[0] * model_wetness )**2)
    return err
    
def fit_vgm(pF,theta,variable_m=False, count=20):
    """Fits a Van Genuchten / Mualem retention curve into data
    pF: a sequence of pF values
    theta: an array of water contents
    alpha_range: a 2-sequence holding the range for alpha values
    n_range: a 2-sequence holding the range for n values
    """
    theta_mean=np.mean(theta)
    ns_denom=np.sum((theta-theta_mean)**2)

    best_f=1e12
    for i in range(count):
        x0=[ random.uniform(0.01,1.0), random.uniform(0.0001,10),random.uniform(1.000001,5)]
        if variable_m: x0.append(1-1/random.uniform(1.000001,5))
        x_opt, f_opt,n_iter,n_eval, warn = opt.fmin(get_error_vgm, x0 = np.array(x0), args=(pF,theta),full_output=1, disp=0)
        if f_opt<best_f:
            print "%i: x=%s f=%0.12g iter=%i, eval=%i" % (i,x_opt,1-f_opt/ns_denom,n_iter,n_eval) 
            vgm=cmf.VanGenuchtenMualem(1,*x_opt)
            best_f=f_opt
    theta_mean=np.mean(theta)
    return vgm, 1-best_f/ns_denom
    
