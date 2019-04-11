#!/usr/bin/env bash

CWD=$PWD
TOOLDIR=$(dirname $0)
TOOLDIR=$(python3 $TOOLDIR/abspath.py $TOOLDIR)
echo "TOOLDIR="$TOOLDIR
CMFDIR=$(python3 $TOOLDIR/abspath.py $TOOLDIR/..)
echo "CMFDIR="$CMFDIR
export CFLAGS="-fPIC"
export CXXFLAGS="-fPIC"

BUILDDIR=$CMFDIR/build/extern
echo "BUILDDIR="$BUILDDIR

mkdir -p $BUILDDIR
cd $BUILDDIR
cmake ${TOOLDIR} -DCMAKE_BUILD_TYPE=Release
make $MAKE_OPTIONS

cd $CWD
