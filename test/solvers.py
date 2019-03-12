"""
Tests the functionality of the solvers

"""
import cmf
import numpy as np
import unittest


def get_project():
    p = cmf.project('X')
    X, = p.solutes
    stores = [p.NewStorage('source{}'.format(i)) for i in range(10)]
    for l, r in zip(stores[:-1],stores[1:]):
        cmf.LinearStorageConnection(l, r, 1)
    stores[0].volume = 1
    stores[0][X].source = 1
    return p, stores


solver_types = [cmf.ExplicitEuler_fixed, cmf.BDF2, cmf.ImplicitEuler, cmf.RKFIntegrator, cmf.CVodeIntegrator]


class TestSolver(unittest.TestCase):

    def test_solver_size(self):
        p, stores = get_project()
        for st in solver_types:
            solver = st(p)
            self.assertEqual(len(solver), 20)

    def test_solver_access(self):
        p, stores = get_project()
        for st in solver_types:
            solver = st(p)
            self.assertIsInstance(solver[0], cmf.WaterStorage)
            self.assertIsInstance(solver[-1], cmf.SoluteStorage)
            with self.assertRaises(IndexError):
                _ = solver[len(solver)]

# Check exactness
# Check jacobian (CVodeDirect, CVodeKLU)

