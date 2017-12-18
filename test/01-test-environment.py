#!/bin/env python

import sys
import os
import unittest


class CheckEnvironment(unittest.TestCase):
    def test_sys_path_local_entry(self):
        path = sys.path
        print('\n'.join(path))
        self.assertNotIn('.', path, 'Local directory is part of the path')

if __name__ == '__main__':
    unittest.main()
