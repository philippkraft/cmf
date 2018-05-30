"""
This file is a Python prototype for a C++ implementation
of issue #31, to create a generic reach cross section.

When working, this file will be deleted or transfered into
a test for the new generic reach cross section
"""

from typing import Sequence
from math import sqrt
from enum import Enum

def regula_falsi(f, x1, x2, *, target=0.0, verbose=False, tolerance=1e-12):
    """
    Finds the root of function f in the interval x1..x2
    using the Andersen/Björk algorithm
    (see https://en.wikipedia.org/wiki/False_position_method)

    :param f: The function to solve
    :param x1: left side of the root
    :param x2: right side of the root
    :param target: Target value of f, for normal root finding = 0.0
    :param verbose: Prints intermediate output
    :return: returns the root
    """


    def singlestep(_x1, _x2, _f1, _f2):
        """
        Performs a single regula falsi step with Andersen Björk shorting
        and parameter switch
        :return: The new best value first, followed by the input
        """

        _xz = _x1 - _f1 * (_x2 - _x1) / (_f2 - _f1)
        _fz = f(_xz) - target
        if _fz * _f1 < 0:
            # Parameter switch
            return _xz, _x2, _xz, _f2, _fz
        else:
            m = 1 - _fz / _f2
            m = m if m > 0 else 0.5
            return _xz, _x1, _xz, m * _f1, _fz

    i = 0
    f2 = f(x2) - target
    f1 = f(x1) - target
    xz = x1
    while abs(x2 - x1) > tolerance and min(abs(f1), abs(f2)) > tolerance:
        xz, x1, x2, f1, f2 = singlestep(x1, x2, f1, f2)
        i += 1
        if verbose:
            print(f'#{i}: x={x1:0.5g}, {x2:0.5g}    f={f1:0.5g},{f2:0.5g}')
    return xz



class CrossSectionReach:
    """
    Calculates the states of the Manning equation A and P from the water depth
    for a river geometry defined by a list of coordinates
    """
    def __init__(self, x: Sequence[float], depth: Sequence[float], n_manning=0.035):
        """
        Creates the new geometry
        :param x: Sequence of distance from left bank
        :param depth: Sequence of depth below bank [m]
        :param n_manning: Manning's roughness (n)
        """
        self.x = x
        self.depth = depth
        self.nManning = n_manning

    def set_nManning(self, val: float) -> None:
        self.nManning = val

    def get_nManning(self) -> float:
        return self.nManning

    def typecode(self) -> str:
        return 'C'

    def iter_trapezoid(self, h: float):
        d_max = max(self.depth)
        y = [d_max - d for d in self.depth]
        for xl, xr, yl, yr in zip(self.x[:-1], self.x[1:], y[:-1], y[1:]):
            if yl >= h > yr:
                yield (xr - xl) * (h - yr) / (yl - yr), 0, (h - yr)
            elif yr >= h > yl:
                yield (xr - xl) * (h - yl) / (yr - yl), (h - yl), 0
            elif h > yr and h > yl:
                yield xr - xl, h - yl, h - yr
            elif yl >= h and yr >= h:
                yield 0.0, 0.0, 0.0

    def get_channel_width(self, depth: float) -> float:
        """
        Calculates the flow width from a given actual depth [m] using the actual geometry
        :param depth: Actual water level above deepest part of river in [m]
        :return: Actual channel width in m
        """
        return sum(x for x, _, _ in self.iter_trapezoid(depth))

    def get_wetted_perimeter(self, depth: float) -> float:
        """
        Calculates the wetted perimeter from a given actual depth [m] using the actual geometry
        :return:
        """
        return sum(sqrt(x**2 + (y2-y1)**2) for x, y1, y2 in self.iter_trapezoid(depth))

    def get_depth(self, area: float) -> float:
        """
        Calculates the actual depth of the reach from the wetted area using the geometry
        Uses a numerical approximation to calculate the depth. Hence, this
        reach type is slower than others.

        :param area: The actual wetted area in m², can be obtained by V/l,
                     where V is the stored volume and l is the reach length
        :return: the actual depth in m
        """
        return regula_falsi(self.get_flux_cross_section,
                            x1=0, x2=max(self.depth),
                            target=area,
                            verbose=False)

    def get_flux_cross_section(self, depth: float) -> float:
        """
        Calculates the wetted area from a given depth using the IChannel geometry.
        :param depth: depth of the reach m
        :return: Wetted area of a river cross section in m²
        """
        return sum(
            (x * (y2 - y1)) / 2  # Triangle part of trapezoid
            + x * y1             # Rectangle part of trapezoid
            for x, y1, y2 in self.iter_trapezoid(depth))





