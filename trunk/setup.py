#!/usr/bin/env python

# Copyright 2010 by Philipp Kraft
# This file is part of cmf.
#
#   cmf is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 2 of the License, or
#   (at your option) any later version.
#
#   cmf is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
#   

# This file can build and install cmf
from __future__ import print_function, division
import sys
import os
import io
import datetime

version = '1.0.2'

# Try to import numpy, if it fails we have a problem 
try:
    # Import a function to get a path to the include directories of numpy
    from numpy import get_include as get_numpy_include
except ImportError:
    raise RuntimeError("For building and running of cmf an installation of numpy is needed")


# Try to import build_py_2to3 as "Python builder", if this fails, we are on Python 2
# and the automatic translation is not necessary
try:
    from distutils.command.build_py import build_py_2to3 as build_py
except ImportError:
    from distutils.command.build_py import build_py

from setuptools import setup, Extension
from distutils.sysconfig import get_config_var


def get_revision():
    """
    Gets the actual revision from subversion
    """
    pipe = os.popen('svnversion')
    res=pipe.read().strip()
    if ':' in res:
        res=res.split(':')[-1]
    return res.strip('M')

def updateversion(revision):
    """
    Writes the actual revision number into the relevant files:
        cmf/__init__.py: set __version__ constant
        Doxyfile: set PROJECT_NUMBER
    """
    if revision:
        module_code = open('cmf/__init__.py').readlines()
        fout = open('cmf/__init__.py','w')
        for line in module_code:
            if line.startswith('__version__'):
                fout.write("__version__ = '{}'\n".format(version))
            elif line.startswith('__revision__'):
                fout.write("__revision__ = {}\n".format(revision))
            else:
                fout.write(line)
        doxycode = open('Doxyfile').readlines()
        fout = open('Doxyfile','w')
        for line in doxycode:
            if line.strip().startswith('PROJECT_NUMBER'):
                fout.write("PROJECT_NUMBER         = {}\n".format(version))
            else:
                fout.write(line)

def pop_arg(arg):
    """
    Looks for a commandline argument and removes it from syws.argv
    returns: True if the argument is present, False when it is missing
    """
    try:
        sys.argv.remove(arg)
        return True
    except ValueError:
        return False
        
def count_lines(files):
    """
    Python version of the bash command wc -l
    """
    lcount=0
    for fn in files:
        if not fn.endswith('.c'):
            lines = open(fn, encoding='utf-8').readlines()
            lcount+=len(lines)
    return lcount

def is_source_file(fn,include_headerfiles=False):
    """
    Returns True if fn is the path of a source file
    """
    fn=fn.lower()
    res = False
    res = res or fn.endswith('.cpp')
    res = res or fn.endswith('.c')
    res = res or fn.endswith('.cc')
    res = res or (include_headerfiles and fn.endswith('.h'))
    res = res or (include_headerfiles and fn.endswith('.hpp'))
    return res


    
def make_cmf_core(swig, openmp):
    """
    Puts all information needed for the Python extension object together
     - source files
     - include dirs
     - extra compiler flags
    """
    libraries=None
    # Include CVODE
    include_dirs=[os.path.join(*'cmf/cmf_core_src/math/integrators/sundials_cvode/include'.split('/'))]
    # Include numpy
    include_dirs += [get_numpy_include()]
    
    # Platform specific stuff, alternative is to subclass build_ext command as in:
    # https://stackoverflow.com/a/5192738/3032680
    if sys.platform == 'win32':
        # Only include boost if VS2008 compiler is used, else we use C++ 11
        if sys.version_info.major == 2:
            boost_path = os.environ.get('BOOSTDIR',r"..\boost_1_41_0")
            include_dirs += [boost_path,boost_path+r"\boost\tr1"]
        compile_args = ["/EHsc",r'/Fd"build\vc90.pdb"',"/D_SCL_SECURE_NO_WARNINGS", "/D_CRT_SECURE_NO_WARNINGS","/MP"]
        if openmp: compile_args.append("/openmp")
        link_args=["/DEBUG"]
    else: 
        if os.sys.platform == 'darwin':
            # TODO: Benjamin, this is to specific!
            os.environ["CC"] = "gcc-7"
            os.environ["CXX"] = "g++-7"
            os.environ["ARCHFLAGS"]="-arch x86_64"

            include_dirs += ["/usr/local/Cellar/gcc/7.1.0/include/c++/7.1.0/"]
            include_dirs += ["/usr/include/"]
        # Remove the annoying warning because of "-Wstrict-prototypes" deprecated
        # by https://stackoverflow.com/a/9740721/5885054
        opt = get_config_var('OPT')
        os.environ['OPT'] = " ".join(
                                     flag for flag in opt.split() if flag != '-Wstrict-prototypes'
        )
        compile_args=['-Wno-comment','-Wno-reorder','-Wno-unused','-Wno-sign-compare','-ggdb','-std=c++11']
        if openmp: compile_args.append('-fopenmp')
        link_args=["-fopenmp"] if openmp else []
        link_args.append('-ggdb')
        libraries = ['gomp'] if openmp else None
    
    # Get the source files
    cmf_files=[]
    for root, _dirs, files in os.walk(os.path.join('cmf','cmf_core_src')):
        if os.path.basename(root)!='debug_scripts':
            cmf_files.extend(os.path.join(root,f) for f in files if is_source_file(f) and f!='cmf_wrap.cpp')
            
    if swig:
        # Adding cmf.i when build_ext should perform the swig call
        cmf_files.append("cmf/cmf_core_src/cmf.i")
    else:
        # Else use what we have there
        cmf_files.append("cmf/cmf_core_src/cmf_wrap.cpp")

    cmf_core = Extension('cmf._cmf_core',
                            sources=cmf_files,
                            libraries = libraries,
                            include_dirs=include_dirs,
                            extra_compile_args=compile_args,
                            extra_link_args=link_args,
                            swig_opts=['-c++','-Wextra','-w512','-w511','-O','-keyword','-castmode'] # +extraswig
                        )
    return cmf_core
    
    
if __name__=='__main__':
    revision = get_revision()
    if pop_arg('revision'):
        updateversion(revision)
        exit()
    if 'build' in sys.argv or 'build_py' in sys.argv:
        updateversion(revision)

    ext = [make_cmf_core(swig=pop_arg('swig'), openmp=not pop_arg('noopenmp'))]
    description = 'Catchment Modelling Framework - A hydrological modelling toolkit'
    long_description = io.open('README.rst', encoding='utf-8').read()
    classifiers = [
        'Development Status :: 4 - Beta',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: GNU General Public License v2 or later (GPLv2+)',
        'Programming Language :: C++',
        'Programming Language :: C',
        'Programming Language :: Python',
        'Topic :: Scientific/Engineering',
        'Topic :: Software Development :: Libraries :: Python Modules',
    ]
    now = datetime.datetime.now()
    # log.set_verbosity(5)
    setup(name='cmf',
          version=version,
          license='GPL',
          ext_modules=ext,
          packages=['cmf', 'cmf.draw'],
          install_requires=['numpy>=1.8'],
          python_requires='>=2.7',
          keywords='hydrology catchment simulation toolbox',
          author = 'Philipp Kraft',
          author_email = "philipp.kraft@umwelt.uni-giessen.de",
          url = "https://www.uni-giessen.de/hydro/download",
          description=description,
          long_description=long_description,
          classifiers=classifiers,
          cmdclass={'build_py':build_py},
          )
    print("build ok")
