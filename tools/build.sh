#!/usr/bin/env bash

# This script should be called inside the manylinux docker
# docker run -it -v $(pwd):/io quay.io/pypa/manylinux1_x86_64


set -e -x

# Compile libraries
. /io/tools/install_solvers.sh
# Compile wheels
for PYBIN in /opt/python/*/bin; do
    "${PYBIN}/pip" install -r /io/requirements.txt
    "${PYBIN}/pip" wheel /io/ -w wheelhouse/
done

# Bundle external shared libraries into the wheels
for whl in wheelhouse/*.whl; do
    auditwheel repair "$whl" -w /io/wheelhouse/
done

# Install packages and test
for PYBIN in /opt/python/*/bin/; do
    "${PYBIN}/pip" install python-manylinux-demo --no-index -f /io/wheelhouse
    (cd "/io/test"; "${PYBIN}/nosetests -vv")
done