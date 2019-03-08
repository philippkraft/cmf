import cmf
import unittest


class TestSoluteAccess(unittest.TestCase):
    def testConc(self):
        p = cmf.project('X Y Z')
        X, Y, Z = p.solutes
        ws = p.NewStorage('storage')
        self.assertEqual(repr(ws[X]), '{storage}[X]')
        self.assertEqual(ws.conc(X), ws[X].conc)

    def testState(self):
        p = cmf.project('X Y Z')
        X, Y, Z = p.solutes
        ws = p.NewStorage('storage')
        ws.volume = 1.0
        ws[X].state = 1.0
        ws.volume = 2.0
        self.assertAlmostEqual(ws[X].conc, 0.5)
        self.assertAlmostEqual(ws[X].state, 1.0)
        self.assertAlmostEqual(ws[Y].conc, 0.0)
