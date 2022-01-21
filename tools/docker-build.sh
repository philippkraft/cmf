#!/bin/bash

# Explain usage if no /io directory exists (which mounts on proper docker usage the current directory)
[[ -d "/io" ]] || echo "Run in manylinux container:\ndocker run -ti -v $(pwd):/io quay.io/pypa/manylinux2014_x86_64 /bin/bash /io/cmf2/tools/docker-build.sh"

# This file should be used in a manylinux container to build several binaries
set -e -u -x

function repair_wheel {
    wheel="$1"
    if ! auditwheel show "$wheel"; then
        echo "Skipping non-platform wheel $wheel"
    else
        auditwheel repair "$wheel" -w /io/wheelhouse/
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
