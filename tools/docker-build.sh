#!/bin/bash

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
export MAKEFLAGS="-j30"
CMFDIR=/io/cmf2
TOOLDIR=$CMFDIR/tools
pushd $CMFDIR
# Install solvers
SOLVERBUILDDIR=$CMFDIR/build/extern
rm -rf $SOLVERBUILDDIR
cmake -S ${TOOLDIR} -B ${SOLVERBUILDDIR} -DCMAKE_BUILD_TYPE=Release
make -C ${SOLVERBUILDDIR}

# build cmf_core
CMFBUILDDIR=$CMFDIR/build/cmf_core
rm -rf $CMFBUILDDIR
CMFSOURCEDIR=$CMFDIR/cmf/cmf_core_src
cmake -S $CMFSOURCEDIR -B $CMFBUILDDIR -DCMAKE_BUILD_TYPE=Release
make -C $CMFBUILDDIR
make install -C $CMFBUILDDIR

# Compile wheels
for PYBIN in /opt/python/cp31*/bin; do
    "${PYBIN}/python" -m pip install -r $CMFDIR/requirements.txt
    "${PYBIN}/python" setup.py bdist_wheel
done

# Bundle external shared libraries into the wheels
for whl in dist/*.whl; do
    repair_wheel "$whl"
done
popd
# Install packages and test
# for PYBIN in /opt/python/*/bin/; do
#    "${PYBIN}/pip" install python-manylinux-demo --no-index -f /io/wheelhouse
#    (cd "$HOME"; "${PYBIN}/nosetests" pymanylinuxdemo)
#done
