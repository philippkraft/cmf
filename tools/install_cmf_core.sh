#!/usr/bin/env bash


CWD=$PWD

CMFDIR=$CWD/$(dirname $0)/..

# Gets and makes the dependencies for the new sparese CVodeIntegrator
export CFLAGS="-fPIC"

mkdir -p $CMFDIR/build
cd $CMFDIR/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make $MAKE_OPTIONS
make install
cd $CWD
