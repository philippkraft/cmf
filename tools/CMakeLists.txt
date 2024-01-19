cmake_minimum_required(VERSION 3.15)

project(cmf_sundials_lib)

set(default_build_type "Release")

# Super necessary! Without this cannot link to cmf_wrap.cxx to create a shared object!
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(HOME ${CMAKE_CURRENT_SOURCE_DIR}/..)
message(STATUS "HOME=${HOME}")
set(LIB_DIR ${HOME}/lib)
message(STATUS "LIB_DIR=${LIB_DIR}")

include(ExternalProject)

set(KLU_DIR ${HOME}/build/extern/suitesparse)
message(STATUS "KLU_DIR=${KLU_DIR}")
set(KLU_URL "https://github.com/philippkraft/suitesparse-metis-for-windows")

ExternalProject_Add(SuiteSparseProject
    GIT_REPOSITORY ${KLU_URL}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIB_DIR} -DBUILD_METIS=OFF -DCMAKE_BUILD_TYPE=RELEASE
    PREFIX ${KLU_DIR}
)

set(SND_DIR ${HOME}/build/extern/sundials)
set(SND_URL "https://github.com/philippkraft/sundials")
message(STATUS "SND_DIR=${SND_DIR}")

ExternalProject_Add(sundials
    GIT_REPOSITORY ${SND_URL}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIB_DIR}
               -DBUILD_ARKODE=OFF -DBUILD_CVODES=OFF -DBUILD_IDA=OFF -DBUILD_IDAS=OFF -DBUILD_KINSOL=OFF
               -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=${LIB_DIR}
               -DEXAMPLES_INSTALL=OFF -DEXAMPLES_ENABLE_CXX=OFF -DEXAMPLES_ENABLE_C=OFF -DEXAMPLES_INSTALL=OFF
               -DKLU_ENABLE=ON -DKLU_LIBRARY_DIR=${LIB_DIR}/lib -DKLU_INCLUDE_DIR=${LIB_DIR}/include/suitesparse
               -DOPENMP_ENABLE=ON -DCMAKE_BUILD_TYPE=Release

    PREFIX ${SND_DIR}
)
add_dependencies(sundials SuiteSparseProject)




