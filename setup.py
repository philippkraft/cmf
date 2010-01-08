import os

from distutils.core import setup,Extension

def make_cmf_core():
    library_dirs=["../sundials-2.4.0/instdir/lib"]
    include_dirs=["../sundials-2.4.0/instdir/include",
                  "../boost_1_41_0","../boost_1_41_0/boost/tr1"]
    libraries=["sundials_cvode","sundials_nvecserial"]
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
