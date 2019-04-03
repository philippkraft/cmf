"""
Tests the functionality of the solvers

"""
import cmf

import unittest
import sys
import glob

def get_project(with_solute=False):
    if with_solute:
        p = cmf.project('X')
        X, = p.solutes
    else:
        p = cmf.project()
        X = None
    stores = [p.NewStorage('source{}'.format(i)) for i in range(10)]

    for l, r in zip(stores[:-1],stores[1:]):
        cmf.LinearStorageConnection(l, r, 1)
    stores[0].volume = 1
    if with_solute:
        stores[0][X].source = 1
    return p, stores, X


solver_types = [
    cmf.ExplicitEuler_fixed, 
    cmf.RKFIntegrator, 
    cmf.HeunIntegrator,
    cmf.BDF2, 
    cmf.ImplicitEuler,
    cmf.CVodeDense, 
    cmf.CVodeBanded, 
    cmf.CVodeKrylov, 
    cmf.CVodeDiag, 
    cmf.CVodeKLU,
    # cmf.CVodeAdams
] 

# solver_types = solver_types[0:3]
class TestSolver(unittest.TestCase):

    def test_1_solver_size_nosolute(self):
        p, stores, X = get_project()
        for st in solver_types:
            with self.subTest(solver_type=st.__name__):
                 solver = st(p)
                 self.assertEqual(len(solver), 10)

    def test_2_solver_size_solute(self):
        p, stores, X = get_project(True)
        for st in solver_types:
            solver = st(p)
            print(solver.to_string())
            self.assertEqual(len(solver), 20)


    def test_3_solver_access_solute(self):
        p, stores, X = get_project(True)
        for st in solver_types:
            with self.subTest(solver_type=st.__name__):
                 solver = st(p)
                 self.assertIsInstance(solver[0], cmf.WaterStorage)
                 self.assertIsInstance(solver[-1], cmf.SoluteStorage)
                 with self.assertRaises(IndexError):
                     _ = solver[len(solver)]

    def test_4_solver_access_nosolute(self):
        p, stores, X = get_project()
        for st in solver_types:
            with self.subTest(solver_type=st.__name__):
                 solver = st(p)
                 self.assertIsInstance(solver[0], cmf.WaterStorage)
                 self.assertIsInstance(solver[-1], cmf.WaterStorage)
                 with self.assertRaises(IndexError):
                     _ = solver[len(solver)]

    def test_solver_results_solute(self):

        vol_ref = [0.04978707, 0.1493612, 0.2240418, 0.2240418, 0.1680314,
                   0.1008188, 0.05040941, 0.02160403, 0.008101512, 0.003802992]
        smass_ref = [0.9502129, 0.8008517, 0.5768099, 0.3527681, 0.1847368,
                     0.08391794, 0.03350854, 0.0119045, 0.003802992, 0.001486583]

        for st in solver_types:
            with self.subTest(solver_type=st.__name__):
                 p, stores, X = get_project(True)
                 solver = st(p)

                 solver.integrate_until(cmf.day * 3, cmf.h)

                 vol = [s.volume for s in stores]
                 smass = [s[X].state for s in stores]

                 mse_v = sum((vr - v)**2 for v, vr in zip(vol, vol_ref)) / len(vol)
                 mse_s = sum((sr - s)**2 for s, sr in zip(smass, smass_ref)) / len(smass)

                 self.assertAlmostEqual(1 - mse_v, 1, 2, "RMSE between reference volume and {} too large".format(st.__name__))
                 self.assertAlmostEqual(1 - mse_s, 1, 2, "RMSE between reference solute and {} too large".format(st.__name__))

    def test_solver_results_nosolute(self):

        vol_ref = [0.04978707, 0.1493612, 0.2240418, 0.2240418, 0.1680314,
                   0.1008188, 0.05040941, 0.02160403, 0.008101512, 0.003802992]

        for st in solver_types:
            with self.subTest(solver_type=st.__name__):
                p, stores, X = get_project(True)
                solver = st(p)

                solver.integrate_until(cmf.day * 3, cmf.h)
                vol = [s.volume for s in stores]
                mse_v = sum((vr - v)**2 for v, vr in zip(vol, vol_ref)) / len(vol)
                self.assertAlmostEqual(1 - mse_v, 1, 2, "MSE between reference volume and {} too large".format(st.__name__))

    def test_solver_run(self):

        for st in solver_types:
            with self.subTest(solver_type=st.__name__):
                p, stores, X = get_project(True)
                solver = st(p)

                # Test all run parameters
                for t in solver.run(cmf.Time(), cmf.day, cmf.h, reset=False, max_errors=2):
                    pass
                self.assertEqual(solver.t, cmf.day)

                # Test for few parameters in run
                for t in solver.run(step=cmf.h):
                    pass

                self.assertEqual(solver.t, cmf.day + 100 * cmf.h)


if __name__ == '__main__':
    unittest.main(verbosity=100)
