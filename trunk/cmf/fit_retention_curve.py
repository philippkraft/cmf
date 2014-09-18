

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
            

def fit_bc(pF,theta,count=20,verbose=False):
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
            if verbose: print "%i: x=%s f=%0.12g iter=%i, eval=%i" % (i,x_opt,1-f_opt/ns_denom,n_iter,n_eval) 
            bc=cmf.BrooksCoreyRetentionCurve(1.,x_opt[0],x_opt[1],x_opt[2])
            best_f=f_opt

    return bc, 1-best_f/ns_denom

def get_error_vgm(params,pF,theta):
    try:
        vgm=cmf.VanGenuchtenMualem(1.0, *params)
    except RuntimeError:
        return 2 # worst case would be 1.0 and 2.0 indicates a impossible parameterization (NaN causes errors during the optimization)
    
    model_wetness=vgm.Wetness_pF(pF)
    err = np.sum((theta - params[0] * model_wetness )**2)
    return err
    
def fit_vgm(pF,theta,variable_m=False, count=20, fitlevel=None,verbose=False):
    """Fits a Van Genuchten / Mualem retention curve into data
    pF: a sequence of pF values
    theta: an array of water contents
    alpha_range: a 2-sequence holding the range for alpha values
    n_range: a 2-sequence holding the range for n values
    """
    theta_mean=np.mean(theta)
    ns_denom=np.sum((theta-theta_mean)**2)

    best_f=1e120
    for i in range(count):
        x0=[ random.uniform(0.01,0.99), random.uniform(0.0001,1.9),random.uniform(1.01,3.9)]
        if variable_m: x0.append(1-1/random.uniform(1.01,5))
        x_opt, f_opt,n_iter,n_eval, warn = opt.fmin(get_error_vgm, x0 = np.array(x0), args=(pF,theta),full_output=1, disp=0)
        if f_opt<best_f:
            if verbose: print "%i: x=%s f=%0.12g iter=%i, eval=%i" % (i,x_opt,1-f_opt/ns_denom,n_iter,n_eval) 
            vgm=cmf.VanGenuchtenMualem(1,*x_opt)
            best_f=f_opt
            if 1-best_f/ns_denom>=fitlevel:
                break
    theta_mean=np.mean(theta)
    return vgm, 1-best_f/ns_denom

def narrowparameters_vgm(pF,theta,phi,alpha,n,count=10000,perc=10.):
    """
    Narrows the parameter space down to the best perc (default=10) percent of the results
    pF: A sequence of pF values matching theta
    theta: A sequence of water content values for the corresponding pF values
    phi: A distribution of porosity values, only the range is used
    alpha: A distribution of VanGenuchten alpha values, only the range is used
    n: A distribution of VanGenuchten n values, only the range is used
    count: Number of random values to be drawn in the range of the parameters
    perc: percentile of best fitting parameters
    
    Returns:
    phi,alpha,n,vgm_err: New distribution of phi, alpha, n and the corrseponding errors
                         for the best perc% of the results
                         
    Usage:
        # Create a priori range
        phi, n, alpha = 
    """
    phi,alpha,n = [np.random.uniform(arg.min(),arg.max(),count) for arg in [phi,alpha,n]]
    vgm_err = np.array([get_error_vgm((phi_,alpha_,n_),pF,theta) for phi_,alpha_,n_ in zip(phi,alpha,n)]) 
    res_perc = np.percentile(vgm_err[np.isfinite(vgm_err)],perc)
    take = vgm_err<res_perc
    minmax = lambda x: (x.min(),x.max())
    print "Best %g%% has an error value < %g" % (perc,res_perc)
    for arg,narg in zip([phi,alpha,n,vgm_err],'phi alpha n vgm_error'.split()):
        print narg, ': %g - %g' % minmax(arg[take])
    print "Error for mean value: %g" % get_error_vgm((phi.mean(),alpha.mean(),n.mean()),pF,theta)
    return [arg[take] for arg in [phi,alpha,n,vgm_err]]
    
def narrowparameters_bc(pF,theta,phi,b,w_x,count=10000,perc=10.):
    """
    Narrows the parameter space down to the best perc (default=10) percent of the results
    pF: A sequence of pF values matching theta
    theta: A sequence of water content values for the corresponding pF values
    phi: A distribution of porosity values, only the range is used
    alpha: A distribution of VanGenuchten alpha values, only the range is used
    n: A distribution of VanGenuchten n values, only the range is used
    count: Number of random values to be drawn in the range of the parameters
    perc: percentile of best fitting parameters
    
    Returns:
    phi,alpha,n,vgm_err: New distribution of phi, alpha, n and the corrseponding errors
                         for the best perc% of the results
    """
    phi,b,w_x= [np.random.uniform(arg.min(),arg.max(),count) for arg in [phi,b,w_x]]
    vgm_err = np.array([get_error_bc((phi_,b_,w_x_ * phi_),pF,theta) for phi_,b_,w_x_ in zip(phi,b,w_x)]) 
    res_perc = np.percentile(vgm_err[np.isfinite(vgm_err)],perc)
    take = vgm_err<res_perc
    minmax = lambda x: (x.min(),x.max())
    print "Best %g%% has an error value < %g" % (perc,res_perc)
    for arg,narg in zip([phi,b,w_x,vgm_err],'phi b w_x vgm_error'.split()):
        print narg, ': %g - %g' % minmax(arg[take])
    print "Error for mean value: %g" % get_error_bc((phi.mean(),b.mean(),(w_x*phi).mean()),pF,theta)
    return [arg[take] for arg in [phi,b,w_x,vgm_err]]
      
