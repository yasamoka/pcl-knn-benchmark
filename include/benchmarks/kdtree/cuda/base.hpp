#pragma once

#include <benchmarks/base.hpp>
#include <benchmarks/kdtree/base.hpp>

template <typename Tree>
static void BM_CUDA_KdTree_Build_Tree(benchmark::State& state)
{
    BM_KdTree_Build_Tree<Tree>(state);
}

template <typename TreeManager>
static void BM_CUDA_KdTree_Search(benchmark::State& state)
{
    int leaf_max_size = state.range(4);
    BM_CUDA_Search<TreeManager>(state, leaf_max_size);
}
