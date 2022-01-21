#!/bin/bash

# Explain usage if no /io directory exists (which mounts on proper docker usage the current directory)
[[ -d "/io" ]] || echo "Run in manylinux container:\ndocker run -ti -v $(pwd):/io quay.io/pypa/manylinux2014_x86_64 /bin/bash /io/cmf2/tools/docker-build.sh"

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

CMFDIR=/io/cmf2
cd $CMFDIR
TOOLDIR=$CMFDIR/tools

# Install solvers
SOLVERBUILDDIR=$CMFDIR/build/extern
# rm -rf $SOLVERBUILDDIR
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
for PYBIN in /opt/python/cp*/bin; do
    "${PYBIN}/pip" install -r $CMFDIR/requirements.txt
    "${PYBIN}/python" setup.py bdist_wheel
done

# Bundle external shared libraries into the wheels
for whl in dist/*.whl; do
    repair_wheel "$whl"
done
