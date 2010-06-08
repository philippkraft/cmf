#!/usr/bin/python

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
import sys

# Change these variables to match your compiler. (gcc or 
# For Visual Studio 2008 no action is needed
gcc = sys.platform == 'linux2'
msvc = sys.platform == 'win32'

# Change these pathes to your sundials 2.4+ installation
sundials_lib_path = [r"..\sundials-2.4.0\inst_msvc\lib"] if msvc else []
sundials_include_path = [r"..\sundials-2.4.0\inst_msvc\include"] if msvc else []

# Change this path to your boost installation (not needed for gcc)
boost_path = r"..\boost_1_41_0" if msvc else ""



# No user action required beyond this point
import os
import datetime
import shutil
from distutils.core import setup,Extension
if "swig" in sys.argv:
    #os.chdir("cmf/cmf_core_src")
    os.execvp("swig",["-Wextra","-w512","-python","-castmode","-O","-c++","-o cmf_wrap.cxx","-outdir ..","cmf/cmf_core_src/cmf.i"])
    #exit()
    sys.argv.pop("swig")
def count_lines(files):
    lcount=0
    for fn in files:
        lines = file(fn).readlines()
        lcount+=len(lines)
    return lcount
def make_cmf_core():
    library_dirs=sundials_lib_path
    include_dirs=sundials_include_path
    if msvc:
        include_dirs += [boost_path,boost_path+r"\boost\tr1"]
    if msvc: 
        compile_args = ["/openmp","/EHsc",r'/Fd"build\vc90.pdb"']
        link_args=["/DEBUG"]
    if gcc: 
        compile_args = ["-std=gnu++98","-fopenmp"]
        link_args=["-fopenmp"]
    libraries=["sundials_cvode","sundials_nvecserial"]
    if gcc:
        libraries += ['gomp']
    cmf_files=[]
    cmf_headers=[]
    for root,dirs,files in os.walk('cmf/cmf_core_src'):
        cmf_files.extend(os.path.join(root,f) for f in files if f.endswith('.cpp') and f!='cmf_wrap.cpp')
        cmf_headers.extend(os.path.join(root,f) for f in files if f.endswith('.h'))
    print "Compiling %i source files" % (len(cmf_files)+1)
    cmf_files.append("cmf/cmf_core_src/cmf.i")
    print "Total number of C++ loc:", count_lines(cmf_files + cmf_headers)
    cmf_core = Extension('cmf._cmf_core',
                            sources=cmf_files,
                            library_dirs=library_dirs,
                            libraries = libraries,
                            include_dirs=include_dirs,
                            extra_compile_args=compile_args,
                            extra_link_args=link_args,
                            swig_opts=['-c++','-Wextra','-w512','-w511','-keyword','-castmode','-O']
                        )
    return cmf_core
def make_raster():
    files=['cmf/raster/raster_src/raster.i']
    if msvc: 
        compile_args = ["/openmp","/EHsc",r'/Fd"build\vc90.pdb"']
        link_args=["/DEBUG"]
    if gcc: 
        compile_args = ["-std=gnu++98","-fopenmp"]
        link_args=["-fopenmp"]
    libraries=[]
    if gcc:
        libraries += ['gomp']
    raster = Extension('cmf.raster._raster',
                        sources=files,
                        libraries = libraries,
                        extra_compile_args=compile_args,
                        extra_link_args=link_args,
                        swig_opts=['-c++','-Wextra','-w512','-w511','-keyword','-castmode','-O']
                    )
    return raster
if __name__=='__main__':
    ext = [make_raster(),make_cmf_core()]
    #ext = [make_cmf_core()]
    author = "Philipp Kraft"
    author_email = "philipp.kraft@umwelt.uni-giessen.de"
    url = "www.uni-giessen.de/ilr/frede/cmf"
    py=[]
    for root,dirs,files in os.walk('cmf'):
        for d in dirs:
            if d.endswith('_src'):
                dirs.remove(d)
        py_found=[os.path.join(root,f[:-3]).replace('\\','.') for f in files if f.endswith('.py')]
        if py_found:
            print 'In %s %i modules found' % (root,len(py_found))
        py.extend(py_found)
    
    now = datetime.datetime.now()
    setup(name='cmf',
          version='0.1',
          license='GPL',
          ext_modules=ext,
          py_modules=py, 
          requires=['shapely (>=1.0)'],author=author,
          url=url,author_email=author_email)
    if msvc: shutil.copy('build/lib.win32-2.6/cmf/raster/_raster.pyd','cmf/raster/')
    #setup(name='cmf_setups',version='0.1',license='GPL',
    #      py_modules=['cmf_setups.'+f[:-3] for f in os.listdir('cmf_setups') if f.endswith('.py')])
    print "build ok"
