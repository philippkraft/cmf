import os
## from numpy.distutils.core import setup
## from numpy.distutils.misc_util import Configuration
## def make_cmf_core(parent_package='',top_path='.'):
##     cmf_core = Configuration('cmf_core', parent_package,top_path)
##     library_dirs=["../../OpenSource/cvode-2.5.0/gcc"]
##     include_dirs=["../../OpenSource/cvode-2.5.0/include"]
##     libraries=["cvode"]
##     cmf_files=[]
##     for root,dirs,files in os.walk(top_path):
##         cmf_files.extend(os.path.join(root,f) for f in files if f.endswith('.cpp'))
##     print "Compiling %i source files" % (len(cmf_files)+1)
##     cmf_files.append("cmf_wrap.cxx")
##     cmf_core.add_extension("_cmf_core",
##         sources=cmf_files,
##         library_dirs=library_dirs,
##         libraries = libraries,
##         include_dirs=include_dirs,
##         extra_compile_args=["-fopenmp"],
##         extra_link_args=["-fopenmp"],
##     )
##     return cmf_core
## if __name__ == '__main__':
##     cmf_core = make_cmf_core()
##     setup(**cmf_core.todict())

from distutils.core import setup,Extension
    
def make_cmf_core():
    library_dirs=["../../OpenSource/cvode-2.5.0/gcc"]
    include_dirs=["../../OpenSource/cvode-2.5.0/include"]
    libraries=["cvode"]
    cmf_files=[]
    for root,dirs,files in os.walk('.'):
        cmf_files.extend(os.path.join(root,f) for f in files if f.endswith('.cpp'))
    print "Compiling %i source files" % (len(cmf_files)+1)
    cmf_files.append("cmf_wrap.cxx")
    cmf_core = Extension('cmf_core',
                            sources=cmf_files,
                            library_dirs=library_dirs,
                            libraries = libraries,
                            include_dirs=include_dirs,
                            extra_compile_args=["/openmp"],
                        )
    return cmf_core
if __name__=='__main__':
    cmf_core = make_cmf_core()
    setup(name='cmf_core',version='0.8',ext_modules=[cmf_core])
