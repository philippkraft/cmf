python setup.py build_ext
mv cmf_core.py cmf/
mv raster.py cmf/raster/
python setup.py build_py -c -O2 -f
python setup.py install
