"""
This file demonstrates the features of the cmf.ConceptualInfiltration connection
and produces the figures in the wiki
"""
from __future__ import print_function, division, absolute_import
import cmf
import numpy as np
import pylab as plt


def create_project(W0=0.9):
    """
    Creates the cmf project with a single cell and one layer
    :return:
    """
    p = cmf.project()
    c = p.NewCell(0, 0, 0, 1000, with_surfacewater=True)
    l = c.add_layer(0.1)
    c.set_rainfall(l.get_capacity())
    si_con = cmf.ConceptualInfiltration(l, c.surfacewater, W0)
    return p, si_con


def storage_runoff(p, si_con, W0=None):
    c = p[0]
    l = c.layers[0]
    if W0 is not None:
        si_con.W0 = W0
    t = cmf.Time()

    def get_inf(W):
        l.volume = W * l.get_capacity()
        I = c.get_rainfall(t)
        q_inf = c.surfacewater.flux_to(l, t) / I
        inf_ex = c.surfacewater.waterbalance(t) / I
        return q_inf, inf_ex

    wetness = np.linspace(0, 1, 1001)
    q_inf, inf_ex = np.transpose([get_inf(W) for W in wetness])

    return wetness, q_inf, inf_ex


def plot_storage_runoff(p, si_con, W0=None, c='blue'):
    w, q_inf, inf_ex = storage_runoff(p, si_con, W0)
    W0_str = '{:0.4g}'.format(si_con.W0)
    plt.plot(w, q_inf, '-', color=c, label='$Q_{inf}(W_{1/2}=' + W0_str + ')$')
    plt.plot(w, inf_ex, '--', color=c, label='$1-Q_{inf}(W_{1/2}=' + W0_str + ')$')
    plt.xlabel('Storage fill $W=V/C$')
    plt.ylabel('Flux fraction of input')
    plt.legend(loc=0)


def infiltration_per_time(p, si_con, W0=None):
    c = p[0]
    l = c.layers[0]
    if W0 is not None:
        si_con.W0 = W0
    t = cmf.Time()
    solver = cmf.RKFIntegrator(p, 1e-9)
    solver.t = t
    l.volume = 0.0
    c.surfacewater.volume = 0.0
    I = c.get_rainfall(t)
    sw_vol = [c.surfacewater.volume]
    l_vol = [l.volume]
    q_inf = [c.surfacewater.flux_to(l, t) / I]
    inf_ex = [c.surfacewater.waterbalance(t) / I]
    t_list = [t / cmf.day + 1]
    for t in solver.run(solver.t, solver.t + cmf.day * 2, cmf.min * 10):
        sw_vol.append(c.surfacewater.volume / l.get_capacity())
        l_vol.append(l.volume / l.get_capacity())
        q_inf.append(c.surfacewater.flux_to(l, t) / I)
        inf_ex.append(c.surfacewater.waterbalance(t) / I)
        t_list.append(t / cmf.h)

    return t_list, sw_vol, l_vol, q_inf, inf_ex


def plot_infiltration(p, si_con, W0=None, c='blue'):
    t_list, sw_vol, l_vol, q_inf, inf_ex = infiltration_per_time(p, si_con, W0)
    W0_str = '{:0.4g}'.format(si_con.W0)
    plt.plot(t_list, q_inf, '-', color=c, label='$Q_{inf}(W_{1/2}=' + W0_str + ')$')
    plt.plot(t_list, inf_ex, '--', color=c, label='$1-Q_{inf}(W_{1/2}=' + W0_str + ')$')
    plt.xticks(np.arange(0, t_list[-1], 6))
    plt.xlabel('Time in h')
    plt.ylabel('Flux fraction of input')


def plot_state(p, si_con, W0=None, c='blue'):
    t_list, sw_vol, l_vol, q_inf, inf_ex = infiltration_per_time(p, si_con, W0)
    W0_str = '{:0.4g}'.format(si_con.W0)
    plt.plot(t_list, l_vol, '-', color=c, label='Soil storage $W_{1/2}=' + W0_str + '$')
    plt.plot(t_list, sw_vol, '--', color=c, label='Surface storage $W_{1/2}=' + W0_str + '$')
    plt.xticks(np.arange(0, t_list[-1] + 1, 6))
    plt.xlabel('Time in h')
    plt.ylabel('Volume as fraction of Capacity')


if __name__ == '__main__':
    p, si_con = create_project()
    plt.figure()
    plt.subplot(1, 2, 1)
    plot_storage_runoff(p, si_con, 0.95, 'blue')
    plot_storage_runoff(p, si_con, 0.75, 'orange')
    plt.subplot(2, 2, 2)
    plot_infiltration(p, si_con, 0.95, 'blue')
    plot_infiltration(p, si_con, 0.75, 'orange')
    plt.subplot(2, 2, 4)
    plot_state(p, si_con, 0.95, 'blue')
    plot_state(p, si_con, 0.75, 'orange')
    plt.show()
