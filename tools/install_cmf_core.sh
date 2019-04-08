#!/usr/bin/env bash

export CLAGS="-fPIC"
export CXXFLAGS="-fPIC"

CWD=$PWD

CMFDIR=$CWD/$(dirname $0)/..

BUILDDIR=$CMFDIR/build/cmf_core

mkdir -p $BUILDDIR
cd $BUILDDIR
cmake $CMFDIR -DCMAKE_BUILD_TYPE=Release
make $MAKE_OPTIONS
make install
cd $CWD
