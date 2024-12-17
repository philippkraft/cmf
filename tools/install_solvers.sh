#!/usr/bin/env bash

CWD=$PWD
TOOLDIR=$( cd "$(dirname "$0")"; pwd -P )

echo "TOOLDIR="$TOOLDIR
CMFDIR=$(realpath $TOOLDIR/..)
echo "CMFDIR="$CMFDIR
export CFLAGS="-fPIC"
export CXXFLAGS="-fPIC"

BUILDDIR=$CMFDIR/build/extern-linux
echo "BUILDDIR="$BUILDDIR

cmake -S ${TOOLDIR} -B ${BUILDDIR} -DCMAKE_BUILD_TYPE=Release
make -C ${BUILDDIR}
