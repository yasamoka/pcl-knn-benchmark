#pragma once

#include <benchmarks/base.hpp>

template <class Tree, class... Ts>
void BM_KdTree_Build_Tree(benchmark::State& state, Ts... args)
{
    size_t num_search_points = state.range(0);
    int max_leaf_size = state.range(1);

    CloudConstPtr search_cloud = CloudManager::get_search_cloud(num_search_points);
    
    for (auto _ : state)
    {
        state.PauseTiming();
        Tree tree;
        tree.setMaxLeafSize(max_leaf_size);
        state.ResumeTiming();
        tree.setInputCloud(search_cloud);
    }
}

template <typename TreeManager>
static void BM_KdTree_Search(benchmark::State& state)
{
    int leaf_max_size = state.range(4);
    BM_Search<TreeManager>(state, leaf_max_size);
}
