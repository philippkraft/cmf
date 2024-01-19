import cmf
import unittest

t = cmf.Time()

class CustomConnection(cmf.BaseConnection):
    def __init__(self, left, right):
        super().__init__(left, right, type(self).__name__)
        self.left = left
        self.right = right

    def calc_q(self, t):
        return self.left.volume - self.right.volume


class ConnectionDirectorTest(unittest.TestCase):
    def test_connection(self):
        """Test a Python connection"""
        p = cmf.project()
        s1 = p.NewStorage('S1')
        s1.volume = 1.0
        s2 = p.NewStorage('S2')

        con = CustomConnection(s1, s2)
        self.assertEqual(s1.flux_to(s2, t), con.calc_q(t), 'Flux S1->S2 does not equal difference')
        s2.volume = 1.5
        self.assertEqual(s1.flux_to(s2, t), con.calc_q(t), 'Flux S1<-S2 does not equal difference')


if __name__ == '__main__':
    unittest.main(verbosity=2)