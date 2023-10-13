#!/bin/bash

# This file should be used in a manylinux container to build several binaries
set -e -u -x

function repair_wheel {
    wheel="$1"
    if ! auditwheel show "$wheel"; then
        echo "Skipping non-platform wheel $wheel"
    else
        auditwheel repair "$wheel" --plat "$PLAT" -w /io/wheelhouse/
    fi
}


export CFLAGS="-fPIC"
export CXXFLAGS="-fPIC"
export MAKEFLAGS="-j$(nproc)"

CMFDIR=/io/cmf1.6
cd $CMFDIR
TOOLDIR=$CMFDIR/tools

# Compile wheels
for PYBIN in /opt/python/cp*/bin; do
    "${PYBIN}/pip" install numpy
    "${PYBIN}/python" setup.py bdist_wheel
done

# Bundle external shared libraries into the wheels
for whl in dist/*.whl; do
    repair_wheel "$whl"
done
