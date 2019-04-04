#!/usr/bin/env bash

CWD=$PWD

CMFDIR=$CWD/$(dirname $0)/..

# Gets and makes the dependencies for the new sparese CVodeIntegrator
export CFLAGS="-fPIC"
LIB_DIR=$CMFDIR/lib

function klu {
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

}

function sundials {
    # Get sundials
    SND_SRC="$LIB_DIR/src/sundials"
    SND_URL="https://github.com/philippkraft/sundials"

    if [ ! -d "$KLU_SRC" ]; then
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
}

echo "Calling from: " $CWD
echo "Installing KLU into: " $LIB_DIR
echo "Installing SUNDIALS into: " $LIB_DIR

if [[ "$1" == "help" ]]; then
    exit 0
fi

if [[ "$1" != "sundials" ]]; then
    klu
fi

if [[ "$1" != "klu" ]]; then
    sundials
fi
