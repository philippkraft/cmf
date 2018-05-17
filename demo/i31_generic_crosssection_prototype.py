"""
This file is a Python prototype for a C++ implementation
of issue #31, to create a generic reach cross section.

When working, this file will be deleted or transfered into
a test for the new generic reach cross section
"""

from typing import Sequence
from math import sqrt


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
        """
        Iterates over the x and depth sequence, returning (x, y1, y2) tuple
        where:
         - x is the horizontal wetted width of a section
         - y1 is the left depth of the section below h
         - y2 is the right depth of the section below h

        x=y1=y2=0 for dry sections

        How to implement a generator in C++:
         - As a functor, with the generator state as attribute and the operator() as next method
        :param h: Current depth of the river above deepest point
        :return: x, y1, y2
        """
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
                yield 0, 0, 0

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
        :param area: The actual wetted area in m², can be obtained by V/l,
                     where V is the stored volume and l is the reach length
        :return: the actual depth in m
        """
        ...

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



