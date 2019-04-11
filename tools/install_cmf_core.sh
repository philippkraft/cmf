#!/usr/bin/env bash

export CLAGS="-fPIC"
export CXXFLAGS="-fPIC"

CWD=$PWD
TOOLDIR=$(dirname $0)
TOOLDIR=$(python3 $TOOLDIR/abspath.py $TOOLDIR)
echo "tools: "$TOOLDIR
CMFDIR=$(python3 $TOOLDIR/abspath.py $TOOLDIR/..)
echo "home: "$CMFDIR


BUILDDIR=$CMFDIR/build/cmf_core
echo "CMF build directory: "$BUILDDIR
CMF_CORE_SRC=$CMFDIR/cmf/cmf_core_src
echo "CMF core source directory: "$CMF_CORE_SRC
mkdir -p $BUILDDIR
cd $BUILDDIR
cmake $CMF_CORE_SRC -DCMAKE_BUILD_TYPE=Release
make $MAKE_OPTIONS
make install
cd $CWD
