#!/bin/bash
doxygen Doxyfile
python doxy2swig.py -q doxy/xml/index.xml doxy/docstrings-doxy.i
python docstrings2rest.py doxy/docstrings-doxy.i > cmf/cmf_core_src/docstrings.i
