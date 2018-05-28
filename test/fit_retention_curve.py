import unittest

from cmf import fit_retention_curve as frc

class TestFitRetentionCurve(unittest.TestCase):
    def setUp(self):
        self.theta = [[0.324615604527165, 0.4482, 0.4708, 0.4801, 0.4806, 0.4864, 0.582851086403064],
                      [0.342471569196674, 0.4394, 0.4676, 0.4826, 0.4856, 0.4907, 0.566714937861957],
                      [0.364151497261831, 0.4624, 0.4945, 0.5089, 0.5101, 0.512, 0.539286123841295]
                      ]
        self.pF = [4.2, 3, 2.5, 2, 1.8, 1.4, -1]

    def test_fit_vgm(self):
        for theta in self.theta:
            vgm = frc.fit_vgm(self.pF, theta)
            # vgm_var_m  = frc.fit_vgm(self.pF, theta, variable_m=True)

    def test_fit_bc(self):
        for theta in self.theta:
            rc = frc.fit_bc(self.pF, theta)

if __name__ == '__main__':
    unittest.main()