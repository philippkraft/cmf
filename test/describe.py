import cmf
import unittest


class DescribeFunctionTest(unittest.TestCase):
    def test_describe(self):
        p = cmf.project()
        c = p.NewCell(0, 0, 0, 1000, True)
        p.meteo_stations.add_station('Giessen', (0,0,0))
        c.add_layer(1.0)
        text = cmf.describe(p).split('\n')
        print('cmf.describe(project) returns {} lines'.format(len(text)))
        self.assertGreaterEqual(len(text), 5)


if __name__ == '__main__':
    unittest.main()
