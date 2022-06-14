project(pcl-knn-benchmark)
cmake_minimum_required(VERSION 3.23)

find_package(PCL REQUIRED)
find_package(benchmark REQUIRED)
find_package(fmt REQUIRED)

add_definitions("-DPCL_NO_PRECOMPILE")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -mavx")

add_library(benchmarks STATIC
    src/cpp/managers/cloud.cpp
    src/cpp/benchmarks/brute_force.cpp
    src/cpp/benchmarks/kdtree/cuda/kdtree_flann.cpp
    src/cpp/benchmarks/kdtree/kdtree_flann.cpp
    src/cpp/benchmarks/kdtree/kdtree_nanoflann.cpp
    src/cpp/benchmarks/octree/gpu/octree.cpp
    src/cpp/benchmarks/octree/octree.cpp
)

target_include_directories(benchmarks PRIVATE
    $<BUILD_INTERFACE:
        ${PCL_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    >
)

target_link_directories(benchmarks PRIVATE ${PCL_LIBRARY_DIR})
target_link_libraries(benchmarks PRIVATE ${PCL_LIBRARIES} PUBLIC benchmark::benchmark)

foreach(run build_tree nn_search)
    add_executable(${run} src/cpp/runs/${run}.cpp)
    target_include_directories(${run} PRIVATE
        $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
    )
    target_link_libraries(${run} PRIVATE benchmarks)

    add_executable(cuda_${run} src/cpp/runs/cuda/${run}.cpp)
    target_include_directories(cuda_${run} PRIVATE
        $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
    )
    target_link_libraries(cuda_${run} PRIVATE benchmarks)
endforeach()
