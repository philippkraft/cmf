#!/usr/bin/env bash

# Downloads and builds suitesparse, including the sparse linear solver KLU

CWD=$PWD

CMFDIR=$CWD/$(dirname $0)/..

export CFLAGS="-fPIC"
export CXXFLAGS="-fPIC"

LIB_DIR=$CMFDIR/lib

KLU_SRC="$LIB_DIR/src/klu"
KLU_URL="https://github.com/philippkraft/suitesparse-metis-for-windows"

if [ ! -d "$KLU_SRC" ]; then
    mkdir -p $KLU_SRC
    git clone $KLU_URL $KLU_SRC
    rm -rf $KLU_SRC/.git
fi

rm -rf $KLU_SRC/build
mkdir -p $KLU_SRC/build
cd $KLU_SRC/build
cmake .. -DCMAKE_INSTALL_PREFIX=$LIB_DIR -DBUILD_METIS=OFF

make $MAKE_OPTIONS
make install

cd $CWD

