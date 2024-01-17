
"""
Implementation of BROOK 90 by pass macropore infiltration to subsurface
using PartitionFluxRoute

    see: http://www.ecoshift.net/brook/wat.html#INFPAR

"""

import cmf
import numpy as np

def inf_fraction(layer: cmf.SoilLayer, infexp: float, max_depth=None):
    """
    Calculates the infiltration fraction for soil layer as in BROOK90

    see: http://www.ecoshift.net/brook/wat.html#INFPAR


    Parameters
    ----------
    layer : cmf.SoilLayer
        The cmf layer to calculate the infiltration fraction.
    infexp : float
        The infiltration exponent.

    max_depth : float, optional
        The maximum depth for infiltration (IDEPTH in BROOK90). If None,
        use the depth of the deepest soil layer in cmf
        The default is None.

    """
    l = layer.lower_boundary
    u = layer.upper_boundary
    t = max_depth or layer.cell.soildepth
    if infexp == 0:  # if infexp is 0, all water goes to the first layer
        return float(u == 0)
    elif l > t:
        return 0.0
    return (l / t) ** infexp - (u / t) ** infexp


# %%
def use_brook90_infiltration(c: cmf.Cell, infexp: float):
    """
    Setup the multilayer infiltration from BROOK90

    This is not done in a straight forward way, because cmf is limited to
    one connection between to nodes. Hence all infiltration goes in the first layer
    and is partitioned from there to an extra storage (could be a node) 'macropore'.
    All water that should infiltrate to deeper layers is routed to the macropore
    storage and distributed from there to the deeper layers.
    """
    if infexp > 0:
        # Get the flux fraction that should go to the deeper layers (=1-fraction for layer 0)
        infiltration_to_deeper = 1 - inf_fraction(c.layers[0], infexp)
        mp = c.add_storage('macropores')
        pfr0 = cmf.PartitionFluxRoute(c.surfacewater, c.layers[0], mp, infiltration_to_deeper)
        pfr = [
            cmf.PartitionFluxRoute(c.layers[0], mp, l, inf_fraction(l, infexp) / pfr0.fraction)
            for l in c.layers[1:]
        ]
        return [pfr0] + pfr
    else:
        return []


# %%
def setup():
    p: cmf.project = cmf.project()
    c: cmf.Cell = p.NewCell(0, 0, 0, 1000)

    for i in range(100):
        l = c.add_layer((i + 1) * 0.01, cmf.VanGenuchtenMualem())
        l.wetness = 0.1

    cmf.Richards.use_for_cell(c)
    out = p.NewOutlet('out', c.x, c.y, c.z - c.soildepth)
    cmf.FreeDrainagePercolation(c.layers[-1], out)

    return p, c


p, c = setup()
c.set_rainfall(10)

use_brook90_infiltration(c, 0)
solver = cmf.CVodeDense(p, 1e-9)

res = np.array([c.layers.volume] + [
    c.layers.volume for t in solver.run(cmf.Time(), cmf.day * 60, cmf.h)
]).T


