@echo off
echo run doxygen
doxygen tools\Doxyfile
echo make docstring-doxy.i
python tools\doxy2swig.py -q doxy\xml\index.xml doxy\docstrings-doxy.i
echo make docstrings.i
python tools\docstrings2rest.py doxy\docstrings-doxy.i >cmf\cmf_core_src\docstrings.i
