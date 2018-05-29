
# Copyright 2010 by Philipp Kraft
# This file is part of cmf.
#
#   cmf is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
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

'''
Fits cmf retention curves objects into a measured pF curve

Created on 07.10.2009

@author: philkraf
'''
from __future__ import division, print_function, absolute_import
import cmf
import numpy as np
import scipy.optimize as opt
import random


class FitRetentionCurve:

    def __init__(self, rc_class, theta, pF, verbose=False):
        assert len(theta) == len(pF), "theta and pF vectors do not have the same length"
        self.theta = theta
        self.pF = pF
        self.bounds = [[]]
        self.parameternames = []
        self.verbose = verbose
        self.cls = rc_class

    def create_rc(self, params):
        kwargs = dict(zip(self.parameternames, params))
        try:
            return self.cls(1.0, **kwargs)
        except RuntimeError:
            return None

    def get_error(self, params):
        rc = self.create_rc(params)
        if self.verbose:
            print(rc)
        if rc:
            theta_model = rc.Wetness_pF(self.pF) * rc.Porosity()
        else:
            theta_model = np.zeros(len(self.theta))
        err = np.sqrt(np.sum((self.theta - theta_model) ** 2))
        return err

    def __call__(self, count=1):
        final = None
        for i in range(count):
            x0 = [random.uniform(*b) for b in self.bounds]
            optres = opt.minimize(self.get_error, x0, bounds=self.bounds)
            if (not final) or optres.fun < final.fun:
                final = optres
        rc = self.create_rc(final.x)
        return final, rc


class FitVanGenuchtenMualem(FitRetentionCurve):

    def __init__(self, theta, pF, fit_theta_r=False, fit_m=False, verbose=False):
        super(FitVanGenuchtenMualem, self).__init__(cmf.VanGenuchtenMualem, theta, pF,
                                                    verbose=verbose)
        self.parameternames = 'phi', 'alpha', 'n'
        self.bounds = [[1e-5, 0.999999], [1e-9, 1.999999], [1.001, 3.99999]]
        if fit_m:
            self.parameternames += 'm',
            self.bounds += [[1e-4, 0.74999999]]
        if fit_theta_r:
            self.parameternames += 'theta_r',
            self.bounds += [[0.0, np.min(theta)]]


class FitBrooksCorey(FitRetentionCurve):

    def __init__(self, theta, pF,verbose=False):
        super(FitBrooksCorey, self).__init__(cmf.BrooksCoreyRetentionCurve,
                                             theta, pF, verbose=verbose)
        self.parameternames = 'porosity', '_b', 'theta_x'
        self.bounds = [[1e-5, 0.999999], [1, 12], [np.min(self.theta), np.max(self.theta)]]





def fit_vgm(pF, theta, fit_m=False, fit_theta_r=False, count=1, verbose=False):
    """
    Fits the vanGenuchten Mualem retention curve into measured soilphysics values
    :param pF: A sequence of pF values
    :param theta: A sequence of the volumetric water content in l water/l soil at the respective pF value
    :param fit_m: If True, the retention curve is fitted with a variable m (deprecated by vanGenuchten)
    :param fit_theta_r:  If True, the retention curve is fitted using a theta_r (kind of deprecated by vanGenuchten)
    :param count: Times to repeat the minimization to avoid local minima
    :param verbose: If True: Print the values in between
    :return: (Retentioncurve, RMSE)
    """
    fvgm = FitVanGenuchtenMualem(pF, theta, fit_m=fit_m,
                                 fit_theta_r=fit_theta_r, verbose=verbose)
    optres, rc = fvgm(count=count)
    return rc, optres.fun

def fit_bc(pF, theta, count=1, verbose=False):
    """
    Fits the vanGenuchten Mualem retention curve into measured soilphysics values
    :param pF: A sequence of pF values
    :param theta: A sequence of the volumetric water content in l water/l soil at the respective pF value
    :param count: Times to repeat the minimization to avoid local minima
    :param verbose: If True: Print the values in between
    :return: (Retentioncurve, RMSE)
    """
    fbc = FitBrooksCorey(pF, theta, verbose=verbose)
    optres, rc = fbc()
    return rc, optres.fun



if __name__ == '__main__':
    theta5 = [0.324615604527165, 0.4482, 0.4708, 0.4801, 0.4806, 0.4864, 0.582851086403064]
    theta15 = [0.342471569196674, 0.4394, 0.4676, 0.4826, 0.4856, 0.4907, 0.566714937861957]
    theta25 = [0.364151497261831, 0.4624, 0.4945, 0.5089, 0.5101, 0.512, 0.539286123841295]

    pF = [4.2, 3, 2.5, 2, 1.8, 1.4, -1]
    phi = [0.4, 0.6]
    alpha = [0.0001, 1.0]
    n = [1.001, 3]
    theta_r = [0.0, 0.3]


# %%
def plot_vgms(pF, theta, phi, alpha, n, theta_r):
    from pylab import plot, ioff, ion, draw
    ioff()
    pF_dense = np.arange(-1, 7, 0.01)
    for params in zip(phi, alpha, n, theta_r):
        vgm = make_vgm(params)
        plot(pF_dense, vgm.theta(vgm.Wetness_pF(pF_dense)), 'k-', alpha=0.1)
    plot(pF, theta, 'ro')
    ion()
    draw()
