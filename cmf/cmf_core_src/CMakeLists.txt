cmake_minimum_required(VERSION 3.13)
project(cmf)

set(CMAKE_CXX_STANDARD 11)
set(CMF_EXT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)
include_directories(
        ${CMF_EXT_LIB_DIR}/include
        ${CMF_EXT_LIB_DIR}/include/suitesparse
        ${CMAKE_CURRENT_SOURCE_DIR}
)
link_directories(${CMF_EXT_LIB_DIR}/lib ${CMF_EXT_LIB_DIR}/lib64)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()

add_library(libcmf STATIC
        atmosphere/meteorology.cpp
        atmosphere/precipitation.cpp
        atmosphere/Weather.cpp
        geometry/geometry.cpp
        math/integrators/bdf2.cpp
        math/integrators/cvode.cpp
        math/integrators/implicit_euler.cpp
        math/integrators/Integrator.cpp
        math/integrators/multiintegrator.cpp
        math/integrators/RKFintegrator.cpp
        math/integrators/WaterSoluteIntegrator.cpp
        math/num_array.cpp
        math/real.cpp
        math/root_finding.cpp
        math/spline.cpp
        math/statevariable.cpp
        math/time.cpp
        math/timeseries.cpp
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
        water/adsorption.cpp
        water/boundary_condition.cpp
        water/collections.cpp
        water/flux_connection.cpp
        water/flux_node.cpp
        water/simple_connections.cpp
        water/Solute.cpp
        water/SoluteStorage.cpp
        water/system_bridge.cpp
        water/WaterStorage.cpp
        project.cpp
)

# Use a lib prefix if MSVC compiler, because suitesparse does it that way
if(MSVC)
	set(SP_LIB_PREFIX "lib")  # Libs are: "libXXX"
endif(MSVC)

target_link_libraries(libcmf 
   sundials_cvode sundials_sunlinsolklu 
   ${SP_LIB_PREFIX}klu ${SP_LIB_PREFIX}amd ${SP_LIB_PREFIX}btf ${SP_LIB_PREFIX}colamd suitesparseconfig
)

add_executable(nash_cascade apps/nash_cascade.cpp)
target_link_libraries(nash_cascade libcmf)

add_executable(subsurface apps/subsurface.cpp)
target_link_libraries(subsurface libcmf)

add_executable(reach_network apps/reach_network.cpp)
target_link_libraries(reach_network libcmf)
