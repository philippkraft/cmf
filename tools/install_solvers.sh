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



if ($SUPERLUMT); then
   wget http://crd-legacy.lbl.gov/~xiaoye/SuperLU/superlu_mt_3.1.tar.gz
   tar -xzf superlu_mt_3.1.tar.gz
   cd SuperLU_MT_3.1/
   cp MAKE_INC/make.linux.openmp make.inc
fi


# Get KLU
git clone https://github.com/PetterS/SuiteSparse suitesparse

# Get sundials
git clone https://github.com/LLNL/sundials sundials



# Make KLU

# For of KLU & sundials build in Windows see:
# http://my-it-notes.com/2013/01/how-to-build-suitesparse-under-windows-using-visual-studio/
# https://github.com/jlblancoc/suitesparse-metis-for-windows
# https://dmerej.info/blog/post/cmake-visual-studio-and-the-command-line/
# https://ninja-build.org/



cd suitesparse
mkdir -p ${KLUINSTALL_DIR}/lib
mkdir -p ${KLUINSTALL_DIR}/include
mkdir -p ${KLUINSTALL_DIR}/doc



make
make install INSTALL_LIB=${KLUINSTALL_DIR}/lib INSTALL_INCLUDE=${KLUINSTALL_DIR}/include INSTALL_DOC=${KLUINSTALL_DIR}/doc
cd ..

# Make sundials
mkdir -p ${SUNDIALS_DIR}

cd sundials

mkdir -p build
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
