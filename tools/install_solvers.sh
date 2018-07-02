#!/usr/bin/env bash

CWD=$PWD

TOOLSDIR=$CWD/$(dirname $0)

# Gets and makes the dependencies for the new sparese CVodeIntegrator
export CFLAGS="-fPIC"
export KLUINSTALL_DIR=$TOOLSDIR/suitesparse-lib
export SUNDIALS_DIR=$TOOLSDIR/sundials-lib

function klu {
    # Get KLU
    git clone https://github.com/PetterS/SuiteSparse $TOOLSDIR/suitesparse

    # Make KLU

    mkdir -p ${KLUINSTALL_DIR}/lib
    mkdir -p ${KLUINSTALL_DIR}/include
    mkdir -p ${KLUINSTALL_DIR}/doc

    cd $TOOLSDIR/suitesparse


    make
    make install \
        INSTALL_LIB=${KLUINSTALL_DIR}/lib \
        INSTALL_INCLUDE=${KLUINSTALL_DIR}/include \
        INSTALL_DOC=${KLUINSTALL_DIR}/doc

    cd $TOOLSDIR
}

function sundials {
    # Get sundials
    git clone https://github.com/philippkraft/sundials $TOOLSDIR/sundials

    # Make sundials
    mkdir -p ${SUNDIALS_DIR}

    cd $TOOLSDIR/sundials

    mkdir -p build
    cd build

    cmake .. \
        -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true \
        -DBLAS_ENABLE=ON \
        -DBUILD_SHARED_LIBS=OFF \
        -DCMAKE_INSTALL_PREFIX=${SUNDIALS_DIR} \
        -DEXAMPLES_INSTALL=ON \
        -DKLU_ENABLE=ON \
        -DKLU_LIBRARY_DIR=${KLUINSTALL_DIR}/lib \
        -DKLU_INCLUDE_DIR=${KLUINSTALL_DIR}/include \
        -DOPENMP_ENABLE=ON \
        -DEXAMPLES_ENABLE_CXX=ON -DMPI_ENABLE=ON -DMPI_CC=mpicc

    make
    make install
    
    cd $TOOLSDIR
}

echo "Calling from: " $CWD
echo "Running in: " $TOOLSDIR
echo "Installing KLU into: " $KLUINSTALL_DIR
echo "Installing SUNDIALS into: " $SUNDIALS_DIR

if "$1" == "help"; then
    exit 0
fi
    
if "$1" != "sundials"; then
    klu
fi

if "$1" != "klu"; then
    sundials
fi
    
