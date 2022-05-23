#include <benchmark/benchmark.h>

#include <benchmarks/brute_force.hpp>
#include <benchmarks/kdtree/cuda/kdtree_flann.hpp>
#include <benchmarks/kdtree/kdtree_flann.hpp>
#include <benchmarks/kdtree/kdtree_nanoflann.hpp>
#include <benchmarks/octree/gpu/octree.hpp>
#include <benchmarks/octree/octree.hpp>

using Range = std::vector<int64_t>;
using ArgList = std::vector<Range>;

benchmark::IterationCount num_iterations = 10;
benchmark::TimeUnit time_unit = benchmark::TimeUnit::kMillisecond;

Range num_search_points_range = benchmark::CreateRange(10000, 10000000, 10);
Range num_query_points_range = { 100000 };
Range k_range = { 1 };
Range threading_range = { 0, 1 };
Range max_leaf_size_range = benchmark::CreateRange(1, 256, 2);

/*ArgList brute_force_arglist {
    benchmark::CreateRange(10000, 1000000, 10),
    { 1000 },
    { 1 },
    { 0, 1 }
};*/

ArgList kdtree_arglist {
    num_search_points_range,
    num_query_points_range,
    k_range,
    threading_range,
    max_leaf_size_range
};

/*ArgList cpu_octree_arglist {
    benchmark::CreateRange(10000, 1000000, 10),
    { 1000 },
    { 1 },
    { 0, 1 },
    { 128 }
};

ArgList gpu_octree_arglist {
    benchmark::CreateRange(10000, 1000000, 10),
    benchmark::CreateRange(1000, 1000000, 10)
};

ArgList cuda_kdtree_flann_arglist {
    benchmark::CreateRange(1000000, 10000000, 10),
    benchmark::CreateRange(100000, 1000000, 10),
    { 1 },
    { 64 }
};*/

//BENCHMARK(BM_BruteForce_Search)->ArgsProduct(brute_force_arglist)->Iterations(num_iterations)->Unit(time_unit);

BENCHMARK(BM_KdTreeFLANN_Search)->ArgsProduct(kdtree_arglist)->Iterations(num_iterations)->Unit(time_unit);
BENCHMARK(BM_KdTreeNanoflann_Search)->ArgsProduct(kdtree_arglist)->Iterations(num_iterations)->Unit(time_unit);

/*BENCHMARK(BM_CPU_Octree_Search)->ArgsProduct(cpu_octree_arglist)->Iterations(num_iterations)->Unit(time_unit);

BENCHMARK(BM_GPU_Octree_Search)->ArgsProduct(gpu_octree_arglist)->Iterations(num_iterations)->Unit(time_unit);

BENCHMARK(BM_CUDA_KdTreeFLANN_Search)->ArgsProduct(cuda_kdtree_flann_arglist)->Iterations(num_iterations)->Unit(time_unit);*/

BENCHMARK_MAIN();
