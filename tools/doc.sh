#!/bin/bash
doxygen tools/Doxyfile
python tools/doxy2swig.py -q doxy/xml/index.xml doxy/docstrings-doxy.i
python tools/docstrings2rest.py doxy/docstrings-doxy.i > cmf/cmf_core_src/docstrings.i
