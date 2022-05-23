#pragma once

#include <managers/cloud.hpp>

template <class TreeManager, class... Ts>
void BM_Search(benchmark::State& state, Ts... args)
{
    size_t num_search_points = state.range(0);
    size_t num_query_points = state.range(1);
    int k = state.range(2);
    bool multithreaded = static_cast<bool>(state.range(3));
    
    auto tree = TreeManager::get_tree(num_search_points, args...);
    CloudConstPtr query_cloud = CloudManager::get_query_cloud(num_query_points);

    pcl::Indices indices(k);
    std::vector<float> sqr_distances(k);
    
    if (multithreaded)
    {
        for (auto _ : state)
        {
            std::for_each(
                std::execution::par_unseq,
                query_cloud->points.cbegin(),
                query_cloud->points.cend(),
                [&k, &tree, &indices, &sqr_distances]
                (const Point& point)
                { tree.nearestKSearch(point, k, indices, sqr_distances); }
            );
        }
    }
    else
    {
        for (auto _ : state)
        {
            std::for_each(
                std::execution::seq,
                query_cloud->points.cbegin(),
                query_cloud->points.cend(),
                [&k, &tree, &indices, &sqr_distances]
                (const Point& point)
                { tree.nearestKSearch(point, k, indices, sqr_distances); }
            );
        }
    }
}

template <class Tree, class... Ts>
void BM_CUDA_Search(benchmark::State& state, Ts... args)
{
    size_t num_search_points = state.range(0);
    size_t num_query_points = state.range(1);
    int k = state.range(2);
    
    auto search_cloud = CloudManager::get_search_cloud(num_search_points);
    Tree tree { true, args... };
    tree.setInputCloud(search_cloud);
    
    CloudConstPtr query_cloud = CloudManager::get_query_cloud(num_query_points);

    Eigen::Matrix<pcl::index_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> indices_mat;
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> sqr_distances_mat;
    
    for (auto _ : state)
    {
        tree.nearestKSearch(query_cloud->points, k, indices_mat, sqr_distances_mat);
    }
}
