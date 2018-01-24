#!/usr/bin/env bash

# Gets and makes the dependencies for the new sparese CVodeIntegrator


# Test for BLAS
if [[ $(ldconfig -p | grep blas) ]]; then

    echo "Needs OpenBLAS, install on Ubuntu with:"
    echo "sudo apt install libopenblas-dev"
    echo "on HPC it is often there, load with module openblas"

fi
# Get KLU
git clone https://github.com/PetterS/SuiteSparse suitesparse

# Get sundials
git clone https://github.com/LLNL/sundials sundials



# Make KLU
export KLUINSTALL_DIR=~/.local/lib/suiteparse
cd suitesparse
mkdir ${KLUINSTALL_DIR}
mkdir ${KLUINSTALL_DIR}/lib
mkdir ${KLUINSTALL_DIR}/include
mkdir ${KLUINSTALL_DIR}/doc

make
make install INSTALL_LIB=${KLUINSTALL_DIR}/lib INSTALL_INCLUDE=${KLUINSTALL_DIR}/include INSTALL_DOC=${KLUINSTALL_DIR}/doc
cd ..

# Make sundials
cd sundials
mkdir build
cd build
cmake .. \
    -D BLAS_ENABLE=ON \
    -D CMAKE_INSTALL_PREFIX=~/.local/ \
    -D EXAMPLES_INSTALL=OFF \
    -D KLU_ENABLE=ON \
    -D KLU_LIBRARY_DIR=${KLUINSTALL_DIR}/lib \
    -D KLU_INCLUDE_DIR=${KLUINSTALL_DIR}/include \
    -D OPENMP_ENABLE=ON \
    -D LAPACK_ENABLE=ON

