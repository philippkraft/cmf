"""
Test loader from https://stackoverflow.com/a/40437679/3032680
"""

from __future__ import absolute_import, print_function, division

import unittest
import os
import sys

import cmf
print('CMF v' + cmf.__version__)

sys.path = [p for p in sys.path if p not in ('', '.', 'cmf')]

loader = unittest.TestLoader()

start_dir = os.path.normpath(os.path.dirname(__file__) + '/../test')
print(start_dir)

suite = loader.discover(start_dir, '*.py')

runner = unittest.TextTestRunner(verbosity=5)
runner.run(suite)

