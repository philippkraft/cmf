import cmf
import unittest

def get_setup():
    p = cmf.project()
    c = p.NewCell(0, 0, 0, 1000, True)
    c2 = p.NewCell(0, 0, 0, 1000, True)
    c.topology.AddNeighbor(c2, 1)
    return p

class TopologyTest(unittest.TestCase):
    """
    Test for regression on #14
    """
    def test_neighbor_iterator(self):
        p = get_setup()
        for n, w in p[0].neighbors:
            self.assertEqual(n, p[1])
            self.assertEqual(w, 1)
