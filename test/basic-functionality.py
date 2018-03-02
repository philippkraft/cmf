# -*- coding: utf-8 -*-
"""
Created on Wed Dec 13 15:42:17 2017

@author: gh1961
"""

import cmf
import numpy as np
from itertools import product
import unittest


class Test_Basic(unittest.TestCase):
    def test_version(self):
        print()
        print('CMF v' + cmf.__version__)
        print('CMF in:' + cmf.__file__)

    def test_make_project(self):
        p = cmf.project()
        c = p.NewCell(0, 0, 0, 1000, True)
        self.assertEqual(len(p), 1, 'Should create only one cell')
        self.assertEqual(len(p.get_storages()), 1)

    def test_storage(self):
        p = cmf.project()
        w1 = p.NewStorage('W1')
        self.assertEqual(w1.to_string(), '{W1}')
        self.assertEqual(w1.position, cmf.point(0, 0, 0))


def create_project_with_2_storages():
    p = cmf.project()
    W1 = p.NewStorage(name="W1")
    W2 = p.NewStorage(name="W2")

    return p, W1, W2


class Test_PowerLawConnection(unittest.TestCase):

    def test_Q_S(self):
        """
        Tests, if the power law connection behaves as programmed
        :return:
        """
        p, W1, W2 = create_project_with_2_storages()
        def get_runoff(v, beta=1, residual=0):
            """
            Calculates runoff from W1 to W2 when volume of W1 is v with beta and residual
            """
            cmf.PowerLawConnection(W1, W2, Q0=1, V0=1, beta=beta, residual=residual)
            W1.volume = v
            return W1.flux_to(W2, cmf.Time())

        def Q(S, beta=1, residual=0):
            return np.array([get_runoff(v, beta, residual) for v in S])

        Beta = [0.25, 0.5, 1., 2., 4]
        R = [0, 0.5]
        S = np.arange(0, 3, 0.1)

        for b, r in product(Beta, R):
            Qcmf = Q(S, b, r)
            Qanal = np.maximum(S - r, 0) ** b
            rmse = np.sqrt(np.mean((Qcmf - Qanal) ** 2))
            self.assertAlmostEqual(rmse, 0, 6,
                                   'RMSE between CMF and analytical solution is {:0.5g} for b={:0.5g} and r={:0.5g}'.format(
                                       rmse, b, r))


class Test_LinearStorageConnection(unittest.TestCase):

    def test_Q_t(self):
        """
        Tests if the linear storage connection behaves similar to the analytical solution
        """
        p, w1, w2 = create_project_with_2_storages()

        q = cmf.LinearStorageConnection(w1, w2, 1)

        # Loop over several scales for residence time
        for res_t in 10**np.arange(-3, 3, 0.5):

            q.residencetime = res_t
            w1.volume = 1.0
            w2.volume = 0.0
            solver = cmf.CVodeDense(p, 1e-9)
            solver.integrate_until(cmf.day)
            self.assertAlmostEqual(w1.volume, np.exp(-1/res_t), 5,
                                   'LinearStorage fails to reproduce analytical solution for t_r={:0.5g}'
                                   .format(res_t))



if __name__ == '__main__':
    unittest.main()
