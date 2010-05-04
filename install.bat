python setup.py build_ext
move /Y cmf_core.py cmf\
move /Y raster.py cmf\raster\
python setup.py build_py -c -O2 -f
python setup.py install
