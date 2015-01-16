#!/usr/bin/python
from __future__ import print_function
import os
import sys
def get_revision():
    pipe = os.popen('svnversion')
    res=pipe.read().strip()
    if ':' in res:
        res=res.split(':')[-1]
    return res.strip('M')
def updateversion(revision):
    if revision:
        module_code = open('cmf/__init__.py').readlines()
        fout = open('cmf/__init__.py','w')
        for line in module_code:
            if line.startswith('__version__'):
                fout.write("__version__ = '%s'\n" % revision)
            else:
                fout.write(line)
        doxycode = open('Doxyfile').readlines()
        fout = open('Doxyfile','w')
        for line in doxycode:
            if line.strip().startswith('PROJECT_NUMBER'):
                fout.write("PROJECT_NUMBER         = %s\n" % revision)
            else:
                fout.write(line)

if __name__=='__main__':
    rev = get_revision()
    print("cmf.__version__ =",rev)
    updateversion(rev)