project(pcl-knn-benchmark)
cmake_minimum_required(VERSION 3.23)

find_package(PCL REQUIRED)
find_package(benchmark REQUIRED)

add_executable(knn_benchmark
    src/main.cpp
    src/base.cpp
    src/brute_force.cpp
    src/kdtree_flann.cpp
    src/octree.cpp
    src/octree_gpu.cpp
)

target_include_directories(knn_benchmark PRIVATE
    $<BUILD_INTERFACE:
        ${PCL_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    >
)

target_link_directories(knn_benchmark PRIVATE ${PCL_LIBRARY_DIR})
target_link_libraries(knn_benchmark PRIVATE ${PCL_LIBRARIES} benchmark::benchmark)
