cmake_minimum_required(VERSION 3.1)
project(cmf)
set(default_build_type "Release")

# Super necessary! Without this cannot link to cmf_wrap.cxx to create a shared object!
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(HOME ${CMAKE_CURRENT_SOURCE_DIR})

set(LIB_DIR ${HOME}/lib)

set(CMF_CORE_DIR ${HOME}/build/cmf)

add_subdirectory(cmf/cmf_core_src ${CMF_CORE_DIR})


