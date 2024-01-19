cmake_minimum_required(VERSION 3.15)
project(cmf_core)

# Super necessary! Without this cannot link to cmf_wrap.cxx to create a shared object!
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 11)

set(CMF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(CMF_LIB_DIR ${CMF_DIR}/lib/lib)
set(CMF_LIB64_DIR ${CMF_DIR}/lib/lib64)
set(CMF_INCLUDE_EXT ${CMF_DIR}/lib/include)
set(CMF_BIN_DIR ${CMF_DIR}/bin)


include_directories(
        ${CMF_INCLUDE_EXT}
        ${CMF_INCLUDE_EXT}/suitesparse
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PYTHON_INCLUDE}
)
link_directories(${CMF_LIB_DIR} ${CMF_LIB64_DIR})

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()


add_library(cmf_core STATIC
        math/num_array.cpp
        math/real.cpp
        math/root_finding.cpp
        math/spline.cpp
        math/statevariable.cpp
        math/time.cpp
        math/timeseries.cpp
        math/geometry.cpp
        math/odesystem.cpp
        math/sparse_struct.cpp
        water/adsorption.cpp
        # water/freundlich_adsorption.cpp
        water/boundary_condition.cpp
        water/collections.cpp
        water/flux_connection.cpp
        water/flux_node.cpp
        water/simple_connections.cpp
        water/Solute.cpp
        water/reaction.cpp
        water/SoluteStorage.cpp
        water/WaterStorage.cpp
        math/integrators/explicit_euler.cpp
        math/integrators/bdf2.cpp
        math/integrators/cvode.cpp
        math/integrators/implicit_euler.cpp
        math/integrators/Integrator.cpp
        math/integrators/RKFintegrator.cpp
        math/integrators/multiintegrator.cpp
        atmosphere/meteorology.cpp
        atmosphere/precipitation.cpp
        atmosphere/Weather.cpp
        reach/cross_section_reach.cpp
        reach/ManningConnection.cpp
        reach/OpenWaterStorage.cpp
        reach/Reach.cpp
        reach/ReachType.cpp
        upslope/connections/AtmosphericFluxes.cpp
        upslope/connections/infiltration.cpp
        upslope/connections/Percolation.cpp
        upslope/connections/subsurfacefluxes.cpp
        upslope/connections/surfacefluxes.cpp
        upslope/Soil/RetentionCurve.cpp
        upslope/vegetation/ET.cpp
        upslope/vegetation/ShuttleworthWallace.cpp
        upslope/vegetation/waterstress.cpp
        upslope/algorithm.cpp
        upslope/cell.cpp
        upslope/cell_vector.cpp
        upslope/groundwater.cpp
        upslope/layer_list.cpp
        upslope/macropore.cpp
        upslope/SoilLayer.cpp
        upslope/surfacewater.cpp
        upslope/Topology.cpp
        math/integrators/WaterSoluteIntegrator.cpp
        project.cpp
        )

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(cmf_core PUBLIC OpenMP::OpenMP_CXX)
    message(STATUS "Found OpenMP")
endif()

# Use a lib prefix if MSVC compiler, because suitesparse does it that way
if(WIN32)  #  Changed from MSVC
    set(SP_LIB_PREFIX "lib")  # Libs are: "libXXX"
endif(WIN32)

target_link_libraries(cmf_core PUBLIC
        sundials_cvode sundials_sunlinsolklu
        ${SP_LIB_PREFIX}klu ${SP_LIB_PREFIX}amd
        ${SP_LIB_PREFIX}btf ${SP_LIB_PREFIX}colamd suitesparseconfig
        )

install(TARGETS cmf_core
        ARCHIVE DESTINATION ${CMF_LIB_DIR}
        )

file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/apps/*.cpp")

foreach(filename ${files})
    get_filename_component(target_name ${filename} NAME_WE)
    message(STATUS "Build ${target_name} from ${filename}")
    add_executable(${target_name} ${filename})
#    if(MSVC)
#        target_compile_options(${target_name} PRIVATE /W3)
#    else()
#        target_compile_options(${target_name} PRIVATE -Wall -Wextra)
#    endif()
    target_link_libraries(${target_name} cmf_core)
    install(TARGETS ${target_name} RUNTIME DESTINATION ${CMF_BIN_DIR})
endforeach()


# set(PYTHON_INCLUDE $ENV{PYTHON_INCLUDE})
# add_library(_cmf_core SHARED cmf_wrap.cpp)
# target_link_libraries(_cmf_core cmf_core)

# if(MSVC)
#     set_target_properties(_cmf_core PROPERTIES SUFFIX ".pyd")
# endif(MSVC)
