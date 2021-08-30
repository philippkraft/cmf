#!/usr/bin/env bash

export CLAGS="-fPIC"
export CXXFLAGS="-fPIC"

CWD=$PWD
TOOLDIR=$(dirname $0)
CMFDIR=$TOOLDIR/..

BUILDDIR=$CMFDIR/build/cmf_core
echo "CMF build directory: "$BUILDDIR
CMF_CORE_SRC=$CMFDIR/cmf/cmf_core_src
echo "CMF core source directory: "$CMF_CORE_SRC
cmake -S $CMF_CORE_SRC -B $BUILDDIR -DCMAKE_BUILD_TYPE=Release
make -C $BUILDDIR
make install -C $BUILDDIR
cd $CWD
