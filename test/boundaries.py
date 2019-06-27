import sys

import unittest
import cmf

class TestDirichletBoundaries(unittest.TestCase):

    @staticmethod
    def create_project(outlet_potential):
        p = cmf.project()
        c: cmf.Cell = p.NewCell(0, 0, 0, 1, False)
        l: cmf.SoilLayer = c.add_layer(1.0)
        l.volume = 1.0
        o: cmf.DirichletBoundary = p.NewOutlet('o', 1, 0, 0)
        d = cmf.LinearGradientFlux(l, o, 1, 1)
        if type(outlet_potential) is cmf.timeseries:
            o.set_dynamic_potential(outlet_potential)
        else:
            o.potential = outlet_potential

        return p, l, o

    def test_kinematic_outlet(self):
        p, l, o = self.create_project(0)
        cmf.LinearStorageConnection(l, o, 1)
        self.assertEqual(l.flux_to(o, cmf.Time()), 1.0)

    def test_darcy_outlet(self):
        p, l, o = self.create_project(-1)
        self.assertEqual(l.flux_to(o, cmf.Time()), 1)

    def test_no_backflow_outlet(self):
        p, l, o = self.create_project(1)
        self.assertEqual(l.flux_to(o, cmf.Time()), 0.0)
        o.is_source = True
        self.assertEqual(o.flux_to(l, cmf.Time()), 1)

    def test_timeseries_boundary(self):
        ts_pot = cmf.timeseries.from_sequence(cmf.Time(), cmf.day, [1, 0, -1])
        p, l, o = self.create_project(ts_pot)
        self.assertEqual(l.flux_to(o, cmf.day * 0), 0.0)
        o.is_source = True
        self.assertEqual(l.flux_to(o, cmf.day * 0), -1)
        self.assertEqual(l.flux_to(o, cmf.day * 0.5), -0.5)
        self.assertEqual(l.flux_to(o, cmf.day * 1), 0)
        self.assertEqual(l.flux_to(o, cmf.day * 1.5), 0.5)
        self.assertEqual(l.flux_to(o, cmf.day * 2), 1)
        self.assertEqual(l.flux_to(o, cmf.day * 3), 1)







