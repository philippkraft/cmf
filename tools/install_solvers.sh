#!/usr/bin/env bash

# Gets and makes the dependencies for the new sparese CVodeIntegrator
export KLUINSTALL_DIR=~/.local/lib/suitesparse
export SUNDIALS_DIR=~/.local/lib/sundials

# Test for BLAS
if [[ $(ldconfig -p | grep blas) ]]; then
    echo "BLAS ok"
else
    echo "Needs OpenBLAS, install on Ubuntu with:"
    echo "sudo apt install libopenblas-dev"
    echo "on HPC it is often there, load with module openblas"
    exit 1
fi


# Get KLU
git clone https://github.com/PetterS/SuiteSparse suitesparse

# Get sundials
git clone https://github.com/LLNL/sundials sundials



# Make KLU
cd suitesparse
mkdir ${KLUINSTALL_DIR}
mkdir ${KLUINSTALL_DIR}/lib
mkdir ${KLUINSTALL_DIR}/include
mkdir ${KLUINSTALL_DIR}/doc

make
make install INSTALL_LIB=${KLUINSTALL_DIR}/lib INSTALL_INCLUDE=${KLUINSTALL_DIR}/include INSTALL_DOC=${KLUINSTALL_DIR}/doc
cd ..

# Make sundials
mkdir ${SUNDIALS_DIR}

cd sundials

mkdir build
cd build

cmake .. \
    -DBLAS_ENABLE=ON \
    -DCMAKE_INSTALL_PREFIX=${SUNDIALS_DIR} \
    -DEXAMPLES_INSTALL=OFF \
    -DKLU_ENABLE=ON \
    -DKLU_LIBRARY_DIR=${KLUINSTALL_DIR}/lib \
    -DKLU_INCLUDE_DIR=${KLUINSTALL_DIR}/include \
    -DOPENMP_ENABLE=ON \
    -DLAPACK_ENABLE=ON \
    -DSUNDIALS_INDEX_TYPE=int32_t

make
make install

cd ../..
