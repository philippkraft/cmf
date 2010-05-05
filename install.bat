if not "x%1x"=="xdoxyx" goto build
doxygen Doxyfile
python doxy2swig.py doxy\xml\index.xml cmf\cmf_core_src\docstrings.i
ren cmf\cmf_core_src\cmf.i cmf_1.i
ren cmf\cmf_core_src\cmf_1.i cmf.i
:build
python setup.py build_ext 
move /Y cmf_core.py cmf\
move /Y raster.py cmf\raster\
python setup.py build_py -c -O2 -f
python setup.py install
