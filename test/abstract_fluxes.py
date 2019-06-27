"""
Tests the functionality of the rather abstract connection types

"""
import cmf
import numpy as np
import unittest


def get_project():
    p = cmf.project()
    w = p.NewStorage('source', 0, 0, 1)
    o = p.NewOutlet('out', 1, 0, 0)
    return p, w, o


class TestAbstractFluxes(unittest.TestCase):

    def test_constant_flux(self):
        p, w, o = get_project()
        cmf.ConstantFlux(w, o, 1.0, 1.0, cmf.day)
        t0 = cmf.Time()

        def assert_flux(vol, flux):
            w.volume = vol
            self.assertAlmostEqual(w.flux_to(o, t0), flux)

        for vol, flux in [(3, 1), (2, 1), (1.5, 0.5), (1.0, 0.0), (0.0, 0.0)]:
            assert_flux(vol, flux)

    def test_constant_state_flux(self):
        p, w, o = get_project()
        o.is_source = True
        cmf.ConstantStateFlux(w, o, 1.0, cmf.day)
        t0 = cmf.Time()

        def assert_flux(vol, flux):
            w.volume = vol
            self.assertAlmostEqual(w.flux_to(o, t0), flux)

        for vol, flux in [(2, 1), (1, 0), (1.5, 0.5), (0.0, -1.0)]:
            assert_flux(vol, flux)

    def test_balance_flux(self):
        p, w, o = get_project()
        i = p.NewNeumannBoundary('input', w)
        o.is_source = True
        cmf.WaterbalanceFlux(w, o)
        t0 = cmf.Time()

        def assert_flux(influx, outflux):
            i.flux = influx
            self.assertAlmostEqual(w.flux_to(o, t0), outflux)

        for flux in np.arange(0, 10, 0.1):
            assert_flux(flux, flux)

    @unittest.skip('Issue #71')
    def test_multi_balance_fails(self):
        p, w, o = get_project()
        i = p.NewNeumannBoundary('input', w)
        i.flux = 1.0
        cmf.WaterbalanceFlux(w, o)
        self.assertEqual(w.flux_to(o, cmf.h * 0), 1)
        o2 = p.NewOutlet('o2')
        with self.assertRaises(RuntimeError):
            cmf.WaterbalanceFlux(w, o2)


class TestLinearGradientFlux(unittest.TestCase):

    def test_math(self):
        """ Math is correct"""
        p, w, o = get_project()
        cmf.LinearGradientFlux(w, o, 1, 1)
        w.volume = 1
        self.assertAlmostEqual(w.flux_to(o, cmf.h * 0), 1)
        o.potential = 0.5
        self.assertAlmostEqual(w.flux_to(o, cmf.h * 0), 0.5)

    def test_auto_distance(self):
        p, w, o = get_project()
        lgf = cmf.LinearGradientFlux(w, o, 1)
        self.assertEqual(lgf.d, 1)

    def test_boundary_storage(self):
        p, w, o = get_project()
        cmf.LinearGradientFlux(o, w, 1, 1)
        w.volume = 1
        self.assertAlmostEqual(w.flux_to(o, cmf.h * 0), 1)
        o.potential = 0.5
        self.assertAlmostEqual(w.flux_to(o, cmf.h * 0), 0.5)

    @unittest.skip('Issue #71')
    def test_boundary_boundary(self):
        """ Test if c'tor fails for connections between boundaries"""
        p, w, o = get_project()
        o2 = p.NewOutlet('o2', 2, 0, -1)
        cmf.LinearGradientFlux(o, w, 1, 1)
        with self.assertRaises(RuntimeError):
            cmf.LinearGradientFlux(o, o2, 1, 1)


    @unittest.skip('Issue #71')
    def test_negative_distance(self):
        """ Test if LinearGradientConnection fails with negative distance """
        p, w, o = get_project()
        o.position = w.position
        with self.assertRaises(RuntimeError):
            cmf.LinearGradientFlux(o, w, 1, -1)

    @unittest.skip('Issue #71')
    def test_zero_auto_distance(self):
        """ Test if c'tor fails for automatic distance at the same point"""
        p, w, o = get_project()
        o.position = w.position
        with self.assertRaises(RuntimeError):
            cmf.LinearGradientFlux(o, w, 1)


    @unittest.skip('Issue #71')
    def test_negative_q1(self):
        """ Test if c'tor fails with negative q1 """
        p, w, o = get_project()
        with self.assertRaises(RuntimeError):
            cmf.LinearGradientFlux(o, w, -1)

    def test_is_source(self):
        p, w, o = get_project()
        lgf = cmf.LinearGradientFlux(w, o, 1, 1)
        o.potential = 2
        self.assertAlmostEqual(o.flux_to(w, cmf.h * 0), 0)
        o.is_source = True
        self.assertAlmostEqual(o.flux_to(w, cmf.h * 0), 1)

    def test_respect_empty_storage(self):
        p, w, o = get_project()
        w.volume = 0.0
        cmf.LinearGradientFlux(o, w, 1, 1)
        self.assertAlmostEqual(w.flux_to(o, cmf.h * 0), 0)




#  Can handle emptying of source (using a solver)


if __name__ == '__main__':
    unittest.main()
