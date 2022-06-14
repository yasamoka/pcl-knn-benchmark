#include <benchmark/benchmark.h>

#include <benchmarks/kdtree/cuda/kdtree_flann.hpp>

using Range = std::vector<int64_t>;
using ArgList = std::vector<Range>;

benchmark::IterationCount num_iterations = 10;
benchmark::TimeUnit time_unit = benchmark::TimeUnit::kMillisecond;

Range num_search_points_range = benchmark::CreateRange(100000, 10000000, 10);
Range max_leaf_size_range = benchmark::CreateRange(8, 1024, 2);

ArgList arglist {
    num_search_points_range,
    max_leaf_size_range
};

BENCHMARK(BM_CUDA_KdTreeFLANN_Build_Tree)->ArgsProduct(arglist)->Iterations(num_iterations)->Unit(time_unit);

BENCHMARK_MAIN();
