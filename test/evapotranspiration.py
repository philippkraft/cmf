import cmf
import unittest



class Model:
    def __init__(self):
        self.project = cmf.project()
        self.cell: cmf.Cell = self.project.NewCell(0, 0, 0, 1000)
        self.layer = self.cell.add_layer(0.5)
        self.layer.volume = 15
        self.et = self.cell.transpiration

    def __call__(self, test):
        self.cell.set_uptakestress(cmf.VolumeStress(10, 0))
        solver = cmf.HeunIntegrator(self.project)
        solver.t = cmf.Time(1, 6, 2019)
        # print()
        for t in solver.run(solver.t, solver.t + cmf.day * 5, cmf.day):
            v = self.layer.volume
            test.assertGreater(v, 0)
            # print(f'{t!s:<20} {v:0.3f} mm')


class TestET(unittest.TestCase):

    def test_HargreaveET(self):
        m = Model()
        cmf.HargreaveET(m.layer, m.et)
        m(self)

    def test_OudinET(self):
        m = Model()
        cmf.OudinET(m.layer, m.et)
        m(self)

    def test_OudinET_JensenHaise1963(self):
        m = Model()
        cmf.OudinET.JensenHaise1963(m.layer, m.et)
        m(self)

    def test_OudinET_McGuinessBordne1972(self):
        m = Model()
        cmf.OudinET.McGuinessBordne1972(m.layer, m.et)
        m(self)

    def test_PenmanMonteithET(self):
        m = Model()
        cmf.PenmanMonteithET(m.layer, m.et)
        m(self)

    def test_PriestleyTaylorET(self):
        m = Model()
        cmf.PriestleyTaylorET(m.layer, m.et)
        m(self)

    def test_TurcET(self):
        m = Model()
        cmf.TurcET(m.layer, m.et)
        m(self)

    def test_constantETpot(self):
        m = Model()
        cmf.constantETpot(m.layer, m.et, 5)
        m(self)

    def test_stressedET(self):
        m = Model()
        with self.assertRaises(AttributeError):
            cmf.stressedET(m.layer, m.et)

    def test_timeseriesETpot(self):
        m = Model()
        cmf.timeseriesETpot(m.layer, m.et, cmf.timeseries.from_scalar(5))
        m(self)
