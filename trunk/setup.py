import os

from distutils.core import setup,Extension

def make_cmf_core():
    library_dirs=[r"D:\Code\sundials-2.4.0\instdir\lib"]
    include_dirs=[r"D:\Code\sundials-2.4.0\instdir\include",
                  r"D:\Code\boost_1_41_0",r"D:\Code\boost_1_41_0\boost\tr1"]
    libraries=["sundials_cvode","sundials_nvecserial"]
    cmf_files=[]
    for root,dirs,files in os.walk('.'):
        cmf_files.extend(os.path.join(root,f) for f in files if f.endswith('.cpp'))
    print "Compiling %i source files" % (len(cmf_files)+1)
    cmf_files.append("cmf/cmf_core_src/cmf_wrap.cxx")
    cmf_core = Extension('cmf._cmf_core',
                            sources=cmf_files,
                            library_dirs=library_dirs,
                            libraries = libraries,
                            include_dirs=include_dirs,
                            extra_compile_args=["/openmp","/EHsc"],
                        )
    return cmf_core
def make_raster():
    files=[r'cmf\raster\raster_src\raster_wrap.cxx']
    raster = Extension('cmf.raster._raster',
                        sources=files,
                        extra_compile_args=["/openmp"],
                    )
    return raster
if __name__=='__main__':
    ext = [make_cmf_core(),make_raster()]
    author = "Philipp Kraft"
    author_email = "philipp.kraft@umwelt.uni-giessen.de"
    url = "www.uni-giessen.de/ilr/frede/cmf"
    py=[]
    for root,dirs,files in os.walk('cmf'):
        py.extend(os.path.join(root,f[:-3]).replace('\\','.') for f in files if f.endswith('.py'))
    setup(name='cmf',version='0.8',ext_modules=ext,py_modules=py)
    print "build ok"
