#!/usr/bin/env bash

CWD=$PWD

CMFDIR=$CWD/$(dirname $0)/..

export CFLAGS="-fPIC"
export CXXFLAGS="-fPIC"

BUILDDIR=$CMFDIR/build/extern

mkdir -p $BUILDDIR
cd $BUILDDIR
cmake $CMFDIR/tools -DCMAKE_BUILD_TYPE=Release
make $MAKE_OPTIONS
make install
cd $CWD
