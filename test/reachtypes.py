import cmf
import numpy as np
import unittest

class TestReachCreation(unittest.TestCase):
    def test_reach_creation(self):
        p = cmf.project()
        r_shape = cmf.TriangularReach(100)
        r1 = p.NewReach(0, 0, 0, r_shape, True)
        r2 = p.NewReach(100, 0, 0, r_shape, True)
        r2.set_downstream(r1)

class TestCrossectionReachType(unittest.TestCase):
    def test_area(self):
        # make triangular reach
        cr = cmf.CrossSectionReach(1, [0, 2, 4], [0, 1, 0])
        tr = cmf.TriangularReach(1)

        for d in np.arange(0., 1., 0.01):
            self.assertAlmostEqual(cr.get_flux_crossection(d), tr.get_flux_crossection(d))

    def test_width(self):
        # make triangular reach
        cr = cmf.CrossSectionReach(1, [0, 2, 4], [0, 1, 0])
        tr = cmf.TriangularReach(1)

        for d in np.arange(0., 1., 0.01):
            self.assertAlmostEqual(cr.get_channel_width(d), tr.get_channel_width(d))

    def test_perimeter(self):
        # make triangular reach
        cr = cmf.CrossSectionReach(1, [0, 2, 4], [0, 1, 0])
        tr = cmf.TriangularReach(1)

        for d in np.arange(0., 1., 0.01):
            self.assertAlmostEqual(cr.get_wetted_perimeter(d), tr.get_wetted_perimeter(d))

    def test_depth(self):
        # make triangular reach
        cr = cmf.CrossSectionReach(1, [0, 2, 4], [0, 1, 0])
        tr = cmf.TriangularReach(1)

        for a in np.arange(0., 2., 0.01):
            self.assertAlmostEqual(cr.get_depth(a), tr.get_depth(a))


if __name__ == '__main__':
    unittest.main()
