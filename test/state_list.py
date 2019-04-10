import cmf

import unittest


class TestStateList(unittest.TestCase):

    def test_project_to_state_list(self):
        p = cmf.project('X')
        p.NewStorage('bla')

        sl = cmf.state_list(p)
        self.assertEqual(len(sl), 2, 'Project should have 2 states')

    def test_cell_to_state_list(self):
        p = cmf.project('X')
        c = p.NewCell(0, 0, 0, 1000, True)
        sl = cmf.state_list(c)
        self.assertEqual(len(sl), 2, 'Cell should have 2 states')

    def test_state_list_to_state_list(self):
        p = cmf.project('X')
        c = p.NewCell(0, 0, 0, 1000, True)
        sl = cmf.state_list(c)
        sl2 = cmf.state_list(sl)
        self.assertEqual(len(sl2), 2, 'Cell should have 2 states')

    def test_list_to_state_list(self):
        p = cmf.project('X')
        c = p.NewCell(0, 0, 0, 1000, True)
        sl = list(cmf.state_list(c))
        sl2 = cmf.state_list(sl)
        self.assertEqual(len(sl2), 2, 'Cell should have 2 states')

    def test_slice(self):
        p = cmf.project('X')
        for i in range(100):
            p.NewStorage('bla{:02d}'.format(i))
        sl = cmf.state_list(p)
        sl_slice = sl[:5]
        self.assertEqual(type(sl_slice), cmf.state_list)
        self.assertEqual(len(sl_slice), 5)

    def test_getitem(self):
        p = cmf.project('X')
        p.NewStorage('bla')
        sl = cmf.state_list(p)
        self.assertEqual(type(sl[0]), cmf.WaterStorage)
        self.assertEqual(type(sl[1]), cmf.SoluteStorage)


if __name__ == '__main__':
    unittest.main(verbosity=100)
