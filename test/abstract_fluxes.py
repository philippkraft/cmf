"""
Tests the functionality of the rather abstract connection types

"""
import cmf
import numpy as np
import unittest

def get_project():
    p = cmf.project()
    w = p.NewStorage('source')
    o = p.NewOutlet('out')

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
        cmf.WaterBalanceFlux(w, o)
        t0 = cmf.Time()

        def assert_flux(influx, outflux):
            i.flux = influx
            self.assertAlmostEqual(w.flux_to(o, t0), outflux)

        for flux in np.arange(0, 10, 0.1):
            assert_flux(flux, flux)


if __name__ == '__main__':
    unittest.main()


