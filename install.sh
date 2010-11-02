svn up
python setup.py build_ext noopenmp swig
mv cmf/cmf_core_src/cmf_core.py cmf/cmf_core.py
python setup.py build_py
python setup.py install --user
