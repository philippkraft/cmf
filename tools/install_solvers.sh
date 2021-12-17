#!/usr/bin/env bash
export CFLAGS="-fPIC"
export CXXFLAGS="-fPIC"

CWD=$PWD
TOOLDIR=$(dirname $0)

BUILDDIR=$TOOLDIR/../build/extern
echo "BUILDDIR="$BUILDDIR

cmake -S ${TOOLDIR} -B ${BUILDDIR} -DCMAKE_BUILD_TYPE=Release
make -C ${BUILDDIR}
