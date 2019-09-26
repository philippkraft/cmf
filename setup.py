#!/usr/bin/env python

# Copyright 2010 by Philipp Kraft
# This file is part of cmf.
#
#   cmf is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
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

import sys
import os
import io
import re
import time

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from distutils.sysconfig import customize_compiler
from distutils.command.build_py import build_py

version = '2.0.0b4'

branchversion = version
try:
    from pygit2 import Repository
    head = Repository('.').head.shorthand
    if head != 'master':
        branchversion = version + '.' + head
except:
    Repository = None

print('cmf', branchversion)



swig = False
openmp = False
debug = False

class StaticLibrary:
    """
    A wrapper to build and link static libraries to an extension
    """
    def __init__(self, includepath, libpath, *libs, build_script=None, build_always=False):
        self.includepath = includepath
        self.libpath = libpath
        self.libs = libs
        self.to_lists = self.as_win32 if sys.platform == 'win32' else self.as_posix
        self.build_script_name = build_script
        self.build_always = build_always

    def __str__(self):
        return self.libs[0] + ' - library'

    def as_win32(self):
        checked_libs = []
        for lib in self.libs:
            if os.path.exists(os.path.join(self.libpath, lib + '.lib')):
                checked_libs.append(lib)
            elif os.path.exists(os.path.join(self.libpath, 'lib' + lib + '.lib')):
                checked_libs.append('lib' + lib)
            else:
                raise FileNotFoundError("Can't find static library " + os.path.join(self.libpath, lib))
        return [self.libpath], reversed(checked_libs), []

    def as_posix(self):
        # Move static libraries to extra_objects (with path) to ensure static linking in posix systems
        if os.path.exists(self.libpath):
            libpath = self.libpath
        elif os.path.exists(self.libpath + '64'):
            libpath = self.libpath + '64'
        else:
            raise FileNotFoundError("Can't find static library directory" + self.libpath)

        libfiles = ['{}/lib{}.a'.format(libpath, l) for l in self.libs]
        for lf in libfiles:
            if not os.path.exists(lf):
                raise FileNotFoundError("Can't find static library " + lf)
        return [], [], libfiles

    def exists(self):
        try:
            self.to_lists()
        except FileNotFoundError:
            return False
        else:
            return True

    @staticmethod
    def extend_if_not_exists(items, target):
        for item in items:
            if item and item not in target:
                target.append(item)

    def extend(self, include_dirs, library_dirs, libraries, extra_objects):
        self.extend_if_not_exists([self.includepath], include_dirs)
        path, libs, extra_obj = self.to_lists()
        self.extend_if_not_exists(path, library_dirs)
        libraries += libs
        extra_objects += extra_obj

    def build(self):
        import subprocess as sp
        cwd = os.path.dirname(__file__)
        if os.name == 'nt':
            script_ext = '.bat'
        elif os.name == 'posix':
            script_ext = '.sh'
        else:
            raise RuntimeError('OS must be nt or posix')
        script = os.path.join(cwd, 'tools', self.build_script_name + script_ext)
        if not os.path.exists(script):
            raise FileNotFoundError('{} not found, cannot build {} from source'
                                    .format(os.path.relpath(script, cwd), self))
        sp.run(script, check=True)


static_libraries = [

    StaticLibrary('lib/include/suitesparse', 'lib/lib',
                  'klu', 'amd', 'btf', 'colamd', 'suitesparseconfig',
                  build_script='install_solvers'),
    StaticLibrary('lib/include', 'lib/lib',
                  'sundials_cvode', 'sundials_sunlinsolklu',
                  build_script='install_solvers'),
    StaticLibrary('cmf/cmf_core_src', 'lib/lib',
                  'cmf_core',
                  build_script='install_cmf_core', build_always=True),
]


class CmfBuildExt(build_ext):
    """
    Custom build class to get rid of the -Wstrict-prototypes warning
    source: https://stackoverflow.com/a/36293331/3032680

    Removes also CLASS_swigregister clutter in cmf_core.py
    """
    @staticmethod
    def clean_swigregister(cmf_core_py):
        """
        SWIG creates ugly CLASS_swigregister functions. We remove them.
        """
        rp_call = re.compile(r'^(\w*?)_swigregister\((\w*?)\)', re.MULTILINE)
        cmf_core_py, n = rp_call.subn('# \\1 end', cmf_core_py)
        print(n, 'CLASS_swigregister(CLASS) lines deleted')
        rp_def = re.compile(r'^(\w*?)_swigregister = _cmf_core\.(\w*?)_swigregister', re.MULTILINE)
        cmf_core_py, n = rp_def.subn('_cmf_core.\\1_swigregister(\\1)', cmf_core_py)
        print(n, 'CLASS_swigregister = _cmf_core... -> _cmf_core.CLASS_swigregister(CLASS)')
        return cmf_core_py

    @staticmethod
    def clean_static_methods(cmf_core_py):
        """SWIG creates still static methods as free functions (extra) to ensure Py2.2 compatibility
        We don't want that in 2018
        """
        # Find class names and free functions
        classes = re.findall(r'class\s(\w*?)\(.*?\):', cmf_core_py, re.MULTILINE)
        funcs = re.findall(r'^def (\w*?)\(\*args, \*\*kwargs\):$', cmf_core_py, flags=re.MULTILINE)

        # Find old style static methods (def CLASS_method():)
        static_methods = [f for f in funcs if [c for c in classes if f.startswith(c)]]

        count = 0
        for sm in static_methods:
            cmf_core_py, n = re.subn(r'^def {}.*?:.*?return.*?$'.format(sm),
                                     '\n\n', cmf_core_py, flags=re.MULTILINE + re.DOTALL)
            count += n
        print(count, 'old style static methods removed from', len(classes), 'classes')
        return cmf_core_py

    def add_numpy_include(self):
        # Try to import numpy, if it fails we have a problem
        try:
            # Import a function to get a path to the include directories of numpy
            # noinspection PyPackageRequirements
            from numpy import get_include as get_numpy_include
        except ImportError:
            raise RuntimeError("For building and running of cmf an installation of numpy is needed")

        for ext in self.extensions:
            ext.include_dirs += [get_numpy_include()]

    def build_libraries(self):
        for sl in static_libraries:
            if not sl.exists():
                print(sl, 'get downloaded and installed')

        for sl in static_libraries:
            if not sl.exists() or sl.build_always:
                sl.build()

        cmf_core = self.extensions[-1]

        for sl in reversed(static_libraries):
            sl.extend(cmf_core.include_dirs, cmf_core.library_dirs,
                      cmf_core.libraries, cmf_core.extra_objects)

        print('libraries:', ' '.join(cmf_core.libraries))
        print('library_dirs:', ' '.join(cmf_core.library_dirs))
        print('include_dirs:', ' '.join(cmf_core.include_dirs))
        print('extra_objects:', ' '.join(cmf_core.extra_objects))


    def build_extensions(self):
        customize_compiler(self.compiler)

        try:
            self.compiler.compiler_so.remove("-Wstrict-prototypes")
        except (AttributeError, ValueError):
            pass

        self.add_numpy_include()
        self.build_libraries()

        build_ext.build_extensions(self)

        if swig:
            if os.path.exists('cmf/cmf_core_src/cmf_core.py'):
                fn = 'cmf/cmf_core_src/cmf_core.py'
            elif os.path.exists('cmf/cmf_core.py'):
                fn = 'cmf/cmf_core.py'
            else:
                raise RuntimeError('cmf_core.py not found, run "python setup.py build_ext swig" to create it')
            cmf_core_py = open(fn).read()
            cmf_core_py = self.clean_swigregister(cmf_core_py)
            cmf_core_py = self.clean_static_methods(cmf_core_py)

            open('cmf/cmf_core.py', 'w').write(cmf_core_py)

            if os.path.exists('cmf/cmf_core_src/cmf_core.py'):
                os.unlink('cmf/cmf_core_src/cmf_core.py')


def updateversion():
    """
    Writes the actual revision number into the relevant files:
        cmf/__init__.py: set __version__ constant
        Doxyfile: set PROJECT_NUMBER
    """
    try:
        module_code = open('cmf/__init__.py').readlines()
    except IOError:
        pass
    else:
        fout = open('cmf/__init__.py', 'w')
        for line in module_code:
            if line.startswith('__version__'):
                fout.write("__version__ = '{}'\n".format(branchversion))
            elif line.startswith('__compiletime__'):
                fout.write("__compiletime__ = '{}'\n".format(time.ctime()))
            else:
                fout.write(line)
    try:
        doxycode = open('tools/Doxyfile').readlines()
    except IOError:
        pass
    else:
        fout = open('tools/Doxyfile', 'w')
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
    lcount = 0
    for fn in files:
        if not fn.endswith('.c'):
            lines = open(fn, encoding='utf-8').readlines()
            lcount += len(lines)
    return lcount


def is_source_file(fn, include_headerfiles=False):
    """
    Returns True if fn is the path of a source file
    """
    fn = fn.lower()
    res = (
            fn.endswith('.cpp') or
            (include_headerfiles and fn[-2:] == '.h')
    )
    return res


def get_source_files(include_headerfiles=False, path='cmf/cmf_core_src'):

    result = []

    def ignore(fn):
        return ('cmake-build' in fn or
                fn.startswith('.') or
                fn == 'apps' or
                fn == 'cmf_wrap.cpp')

    for fn in os.listdir(path):
        fullname = os.path.normpath(os.path.join(path, fn))
        if is_source_file(fullname, include_headerfiles) and not ignore(fn):
            result.append(fullname)
        if os.path.isdir(fullname) and not ignore(fn):
            result.extend(get_source_files(include_headerfiles, fullname))
    return result


def make_cmf_core():
    """
    Puts all information needed for the Python extension object together
     - source files
     - include dirs
     - extra compiler flags
    """

    # Include numpy
    include_dirs = []
    library_dirs = []
    libraries = []
    extra_objects = []
    link_args = []

    # Platform specific stuff, alternative is to subclass build_ext command as in:
    # https://stackoverflow.com/a/5192738/3032680
    if sys.platform == 'win32':

        compile_args = ["/EHsc",
                        r'/Fd"build\vc90.pdb"',
                        "/D_SCL_SECURE_NO_WARNINGS",
                        "/D_CRT_SECURE_NO_WARNINGS",
                        "/wd4244",
                        "/MP"
                        ]
        if openmp:
            compile_args.append("/openmp")

        if debug:
            link_args = ["/DEBUG"]


    else:

        compile_args = ['-Wno-comment', '-Wno-reorder', '-Wno-unused','-fPIC',
                        '-Wno-sign-compare', '-std=c++11', '-march=native', '-mtune=native', '-pipe']
        if debug:
            compile_args += ['-ggdb']

        if sys.platform == 'darwin':
            compile_args += ['-stdlib=libc++']

        link_args = []
        if debug:
            link_args += ['-ggdb']


        # Add OpenMP support
        # Disable OpenMP on Mac see https://github.com/alejandrobll/py-sphviewer/issues/3
        if openmp and not sys.platform == 'darwin':
            compile_args.append('-fopenmp')
            link_args.append("-fopenmp")
            libraries.append('gomp')

    # Get the source files
    cmf_files = [] #  get_source_files()
    if not any('cmf_core' in sl.libs for sl in static_libraries):
        print('search source files')
        cmf_files += get_source_files()

    if swig:
        # Adding cmf.i when build_ext should perform the swig call
        cmf_files.append("cmf/cmf_core_src/cmf.i")
        swig_opts = ['-c++', '-w512', '-w511', '-O', '-keyword', '-castmode', '-modern']

    else:
        # Else use what we have there
        cmf_files.append("cmf/cmf_core_src/cmf_wrap.cpp")
        swig_opts = []

    cmf_core = Extension('cmf._cmf_core',
                         sources=cmf_files,
                         libraries=libraries,
                         library_dirs=library_dirs,
                         include_dirs=include_dirs,
                         extra_objects=extra_objects,
                         extra_compile_args=compile_args,
                         extra_link_args=link_args,
                         swig_opts=swig_opts
                         )
    return cmf_core


if __name__ == '__main__':
    updateversion()
    openmp = not pop_arg('noopenmp')
    swig = pop_arg('swig')
    debug = not pop_arg('nodebug')
    ext = [make_cmf_core()]
    description = 'Catchment Modelling Framework - A hydrological modelling toolkit'
    long_description = io.open('README.rst', encoding='utf-8').read()
    classifiers = [
        'Development Status :: 4 - Beta',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: GNU General Public License v3 or later (GPLv3+)',
        'Programming Language :: C++',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Topic :: Scientific/Engineering',
        'Topic :: Software Development :: Libraries :: Python Modules',
    ]

    setup(name='cmf',
          version=version,
          license='GPLv3+',
          ext_modules=ext,
          packages=['cmf', 'cmf.draw', 'cmf.geometry'],
          python_requires='>=3.5',
          install_requires='numpy>=1.11.1',
          keywords='hydrology catchment simulation toolbox',
          author='Philipp Kraft',
          author_email="philipp.kraft@umwelt.uni-giessen.de",
          url="https://www.uni-giessen.de/hydro/download",
          description=description,
          long_description=long_description,
          classifiers=classifiers,
          cmdclass=dict(build_py=build_py,
                        build_ext=CmfBuildExt),
          )

