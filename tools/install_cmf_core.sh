#!/usr/bin/env bash

export CLAGS="-fPIC"
export CXXFLAGS="-fPIC"

CWD=$PWD

CMFDIR=$CWD/$(dirname $0)/..
CMF_CORE_SRC=$CMFDIR/cmf/cmf_core_src
BUILDDIR=$CMFDIR/build/cmf_core

mkdir -p $BUILDDIR
cd $BUILDDIR
cmake $CMF_CORE_SRC -DCMAKE_BUILD_TYPE=Release
make $MAKE_OPTIONS
make install
cd $CWD
