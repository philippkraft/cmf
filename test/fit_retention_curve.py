import unittest

from cmf import fit_retention_curve as frc


class TestFitRetentionCurve(unittest.TestCase):
    def setUp(self):
        self.theta = [[0.324615604527165, 0.4482, 0.4708, 0.4801, 0.4806, 0.4864, 0.582851086403064],
                      [0.342471569196674, 0.4394, 0.4676, 0.4826, 0.4856, 0.4907, 0.566714937861957],
                      [0.089264, 0.15987, 0.20368, 0.2589, 0.28448, 0.34087, 0.49898],
                      [0.00018569, 0.0029429, 0.0093021, 0.029289, 0.04609, 0.10914, 0.29985],
                      ]

        self.pF = [4.2, 3, 2.5, 2, 1.8, 1.4, -1]

    def test_fit_vgm(self):
        for i, theta in enumerate(self.theta):
            vgm, fit = frc.fit_vgm(self.pF, theta, count=5)
            vgm.fit_w0()
            self.assertGreater(0.1, fit, 'Bad RMSE for VanGenuchten(tr=F,m=F) for theta[{}]'.format(i))
            vgm, fit = frc.fit_vgm(self.pF, theta, fit_m=True, count=5)
            vgm.fit_w0()
            self.assertGreater(0.1, fit, 'Bad RMSE for VanGenuchten(tr=F,m=T) for theta[{}]'.format(i))
            vgm, fit = frc.fit_vgm(self.pF, theta, fit_theta_r=True, count=5)
            vgm.fit_w0()
            self.assertGreater(0.1, fit, 'Bad RMSE for VanGenuchten(tr=T,m=F) for theta[{}]'.format(i))
            vgm, fit = frc.fit_vgm(self.pF, theta, fit_m=True, fit_theta_r=True, count=5)
            vgm.fit_w0()
            self.assertGreater(0.1, fit, 'Bad RMSE for VanGenuchten(tr=T,m=F) for theta[{}]'.format(i))


    def test_fit_bc(self):
        for theta in self.theta:
            rc, fit = frc.fit_bc(self.pF, theta, count=5)
            # self.assertGreater(0.1, fit, 'Bad RMSE for VanGenuchten(tr=T,m=F) for theta[{}]'.format(i))

class W0fit:
    def __init__(self, Psi1=1.0, w1=1.01):
        self.Psi1 = Psi1
        self.w1 = w1

    def __call__(self, vgm, value):
        vgm.w0 = value
        return (vgm.Wetness(self.Psi1)[0] - self.w1) / (self.w1 - 1)

if __name__ == '__main__':
    unittest.main()
