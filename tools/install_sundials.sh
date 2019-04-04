#!/usr/bin/env bash

# Downloads and builds the CVode solver library

CWD=$PWD

CMFDIR=$CWD/$(dirname $0)/..

export CFLAGS="-fPIC"
LIB_DIR=$CMFDIR/lib

# Get sundials
SND_SRC="$LIB_DIR/src/sundials"
SND_URL="https://github.com/philippkraft/sundials"

if [ ! -d "$SND_SRC" ]; then
    mkdir -p $SND_SRC
    echo "Create $SND_SRC directory"
    git clone $SND_URL $SND_SRC
    rm -rf $SND_SRC/.git
fi

mkdir -p $SND_SRC/build
cd $SND_SRC/build

cmake .. \
    -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true \
    -DBLAS_ENABLE=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_INSTALL_PREFIX=${LIB_DIR} \
    -DEXAMPLES_INSTALL=OFF \
    -DKLU_ENABLE=ON \
    -DKLU_LIBRARY_DIR=${LIB_DIR}/lib \
    -DKLU_INCLUDE_DIR=${LIB_DIR}/include/suitesparse \
    -DOPENMP_ENABLE=ON \
    -DEXAMPLES_ENABLE_CXX=OFF -DEXAMPLES_ENABLE_C=OFF -DEXAMPLES_INSTALL=OFF \
    -DBUILD_ARKODE=OFF -DBUILD_CVODES=OFF -DBUILD_IDA=OFF -DBUILD_IDAS=OFF -DBUILD_KINSOL=OFF


make $MAKE_OPTIONS
make install

cd $CWD
